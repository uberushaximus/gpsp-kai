/****************************************************************************

  FBM fonts print string source
                                                                      by mok
****************************************************************************/

// gpSP用に不必要な部分を削除(32bit modeなど)

//#include <pspdisplay.h>
//#include <stdlib.h>
//#include <string.h>
#include "common.h"

#define FBM_PSP_WIDTH			(480)
#define FBM_PSP_HEIGHT			(272)
#define FBM_PSP_LINE_SIZE		(512)
#define FBM_PSP_PIXEL_FORMAT	(3)

#define FBM_SIZE_CONTROL		(9)
#define FBM_SIZE_MAP			(6)


fbm_control_t fbmControl[2];
fbm_map_t     *fbmFontMap[2];
fbm_font_t    *fbmFont[2];
static u8     *font_buf[2];

fbm_font_t    fbmFontbyFile;

int           fbmMaxCol;
int           fbmMaxRow;
static int    nextx;
static int    nexty;

static char   use_double;	// 0=single only, other=single+double
static char   read_mode;	// 0=file, 1=on memory

static char   *fbm_path[2];
static int    fbm_whence[2];
static int    fbm_fd[2] = {-1, -1};

u32 (*fbmColMix)(void *vr, u32 color, int rate);

void *fbm_malloc(size_t size);
void fbm_free(void **ptr);
int fbm_fopen(char *path);
void fbm_fclose(int *fd);
int fbm_readfct(int fd, fbm_control_t *control, fbm_map_t **map, int *fbm_whence);
int fbm_readfbm(int fd, fbm_control_t *control, fbm_font_t **font, u8 **buf, int fbm_whence, int index, int fontcnt);
int fbm_issingle(u16 c);
int fbm_isdouble(u16 c);
fbm_font_t * fbm_getfont(u16 index, u8 isdouble);

/*------------------------------------------------------
  フォントの初期設定
  s_path: Single Byte Font (ex.ASCII)
  d_path: Double Byte Font (ex.SJIS)
  mode: Font Read Mode 1=On Memory(fast)
                       0=Disk Access(slow)
------------------------------------------------------*/
int fbm_init(char *s_path, char *d_path, int mode)
{
  int  result;


  read_mode = mode;
  use_double = (d_path) ? (d_path[0]) ? 1: 0: 0;
  fbmMaxCol = fbmMaxRow = 0;
  nextx = nexty = 0;

  fbm_freeall();

  fbm_fd[0] = fbm_fopen(s_path);

  if (fbm_fd[0] < 0)
  {
    result = -1;
    goto err_label;
  }

  result = fbm_readfct(fbm_fd[0], &fbmControl[0], &fbmFontMap[0], &fbm_whence[0]);

  if (result)
  {
    result -= 1;
    goto err_label;
  }

  if (use_double)
  {
    fbm_fd[1] = fbm_fopen(d_path);

    if (fbm_fd[1] < 0)
    {
      result = -5;
      goto err_label;
    }

    result = fbm_readfct(fbm_fd[1], &fbmControl[1], &fbmFontMap[1], &fbm_whence[1]);

		if (result)
		{
			result -= 5;
			goto err_label;
		}
	}

	if (read_mode > 0)
	{
		result = fbm_readfbm(fbm_fd[0], &fbmControl[0], &fbmFont[0], &font_buf[0], fbm_whence[0], 0, fbmControl[0].fontcnt);

		if (result)
		{
			result -= 8;
			goto err_label;
		}

		if (use_double)
		{
			result = fbm_readfbm(fbm_fd[1], &fbmControl[1], &fbmFont[1], &font_buf[1], fbm_whence[1], 0, fbmControl[1].fontcnt);

			if (result)
			{
				result -= 12;
				goto err_label;
			}
		}
	}
	else
	{
		fbm_path[0] = fbm_malloc(strlen(s_path) + 1);
		strcpy(fbm_path[0], s_path);

		if (use_double)
		{
			fbm_path[1] = fbm_malloc(strlen(d_path) + 1);
			strcpy(fbm_path[1], d_path);

			font_buf[0] = fbm_malloc((1 + fbmControl[1].byteperchar));

			if (font_buf[0] == NULL)
			{
				result = -16;
				goto err_label;
			}
		}
		else
		{
			font_buf[0] = fbm_malloc((1 + fbmControl[0].byteperchar));

			if (font_buf[0] == NULL)
			{
				result = -17;
				goto err_label;
			}
		}

		fbmFont[0] = &fbmFontbyFile;
		fbmFontbyFile.width = font_buf[0];
		fbmFontbyFile.font = &font_buf[0][1];
	}

	fbmMaxCol = FBM_PSP_WIDTH / fbmControl[0].width;
	fbmMaxRow = FBM_PSP_HEIGHT / fbmControl[0].height;

	fbm_fclose(&fbm_fd[0]);
	fbm_fclose(&fbm_fd[1]);

	return 0;

err_label:
	fbm_fclose(&fbm_fd[0]);
	fbm_fclose(&fbm_fd[1]);
	fbm_freeall();

	return result;
}

/*------------------------------------------------------
  フォントの解放
------------------------------------------------------*/
void fbm_freeall()
{
	fbm_free((void *)&fbm_path[0]);
	fbm_free((void *)&fbm_path[1]);
	fbm_free((void *)&fbmFontMap[0]);
	fbm_free((void *)&fbmFontMap[1]);
	fbm_free((void *)&fbmFont[0]);
	fbm_free((void *)&fbmFont[1]);
	fbm_free((void *)&font_buf[0]);
	fbm_free((void *)&font_buf[1]);

	fbm_whence[0] = 0;
	fbm_whence[1] = 0;

	if (fbm_fd[0] >= 0) fbm_fclose(&fbm_fd[0]);
	if (fbm_fd[1] >= 0) fbm_fclose(&fbm_fd[1]);

	fbmMaxCol = fbmMaxRow = 0;
}


/*------------------------------------------------------
  文字列の横幅を計算
  str: Draw String
  RET: Draw Width
------------------------------------------------------*/
int fbm_getwidth(char *str)
{
	int           i;
	int           len;
	int           index;
	int           width;
	fbm_font_t    *font;


	width = 0;
	len = strlen(str);

	if (!read_mode)
	{
		fbm_fd[0] = fbm_fopen(fbm_path[0]);
		if (fbm_fd[0] < 0) goto err_label;

		if (use_double)
		{
			fbm_fd[1] = fbm_fopen(fbm_path[1]);
			if (fbm_fd[1] < 0) goto err_label;
		}
	}

	for (i = 0; i < len; i++)
	{
		index = (i < len - 1) ? fbm_isdouble((u16)(((u8)str[i] << 8) | (u8)str[i + 1])): -1;

		if (index >= 0)
		{
			font = fbm_getfont(index, 1);
			width += *(font->width);
			i++;
		}
		else
		{
			index = fbm_issingle((u16)((u8)str[i]));

			if (index < 0)
				index = fbmControl[0].defaultchar;

			font = fbm_getfont(index, 0);
			width += *(font->width);
		}
	}

	if (!read_mode)
	{
		fbm_fclose(&fbm_fd[0]);
		fbm_fclose(&fbm_fd[1]);
	}

	return width;

err_label:
	if (!read_mode)
	{
		fbm_fclose(&fbm_fd[0]);
		fbm_fclose(&fbm_fd[1]);
	}

	return -1;
}
/*------------------------------------------------------
  指定したフレームバッファに文字列を書込む
  Print String: Base VRAM Addr + XY Pixel
  vram: Base VRAM Addr
  bufferwidth: buffer-width per line,
  x: X (0-479), y: Y (0-271), str: Print String,
  color: Font Color, back: Back Grand Color,
  fill: Fill Mode Flag (ex.FBM_FONT_FILL | FBM_BACK_FILL),
  rate: Mix Rate (0-100 or -1--101)
------------------------------------------------------*/
int fbm_printVRAM(void *vram, int bufferwidth, int x, int y, char *str, u32 color, u32 back, u8 fill, int rate, u32 pad)
{
	int i;
	int len;
	int index;
	int isdouble;
    int loop;

	if (bufferwidth == 0) return -1;

	if (x >= 0) nextx = x % FBM_PSP_WIDTH;
	if (y >= 0) nexty = y % FBM_PSP_HEIGHT;

	fbmColMix = (rate < 0) ? fbm_colmixrev: fbm_colmix;

	if (rate < 0) rate = rate * -1 - 1;
	if (rate > 100) rate = 100;

	if (!read_mode)
	{
		fbm_fd[0] = fbm_fopen(fbm_path[0]);
		if (fbm_fd[0] < 0) goto err_label;

		if (use_double)
		{
			fbm_fd[1] = fbm_fopen(fbm_path[1]);
			if (fbm_fd[1] < 0) goto err_label;
		}
	}

	len = strlen(str);

    if ( pad > len ) {
      loop = pad;
    } else {
      loop = len;
    }

	for (i = 0; i < loop; i++)
	{
        if (i < len)
        {
            if ((u8)str[i] == '\n')
        	{
       			nextx = x;
     			nexty += fbmControl[0].height;
      		}
    		else
        	{
                index = (i < len - 1) ? fbm_isdouble((u16)(((u8)str[i] << 8) | (u8)str[i + 1])): -1;
        		if (index >= 0)
        		{
        			isdouble = 1;
      				i++;
     			}
        		else
     			{
       				index = fbm_issingle((u16)((u8)str[i]));
     				isdouble = 0;
    				if (index < 0)
        				index = fbmControl[0].defaultchar;
     			}
        		fbm_printSUB(vram, bufferwidth, index, isdouble,
        			fbmControl[isdouble].height,
        			fbmControl[isdouble].byteperchar / fbmControl[isdouble].height,
        			color, back, fill, rate);
            }
	    }
        else
        {
		    isdouble = 0;
    		index = fbmControl[0].defaultchar;
    		fbm_printSUB(vram, bufferwidth, index, isdouble,
	    		fbmControl[isdouble].height,
	    		fbmControl[isdouble].byteperchar / fbmControl[isdouble].height,
	    		color, back, fill, rate);
	    }
	}

	if (!read_mode)
	{
		fbm_fclose(&fbm_fd[0]);
		fbm_fclose(&fbm_fd[1]);
	}

	return 0;

err_label:
	if (!read_mode)
	{
		fbm_fclose(&fbm_fd[0]);
		fbm_fclose(&fbm_fd[1]);
	}

	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// Print String Subroutine (Draw VRAM)
// vram: Base VRAM Addr, bufferwidth: buffer-width per line,
// index: Font Index, isdouble: Is Double Byte Font? (0=Single, 1=Double),
// height: Font Height, byteperline: Used 1 Line Bytes,
// color: Font Color, back: Back Grand Color,
// fill: Fill Mode Flag (ex.FBM_FONT_FILL | FBM_BACK_FILL),
// rate: Mix Rate (0-100 or -1--101)
/////////////////////////////////////////////////////////////////////////////
void fbm_printSUB(void *vram, int bufferwidth, int index, int isdouble, int height, int byteperline, u32 color, u32 back, u8 fill, int rate)
{
	int           i;
	int           j;
	int           shift;
	u8            pt;
	u16           *vptr;
	fbm_font_t    *font;


	if (index < 0) return;

	font = fbm_getfont(index, isdouble);

	if (nextx + *(font->width) > FBM_PSP_WIDTH)
	{
		nextx = 0;
		nexty += fbmControl[0].height;
	}

	if (nexty + height > FBM_PSP_HEIGHT)
	{
		nexty = 0;
	}

	vram = (u16 *)vram + nextx + nexty * bufferwidth;

	for (i = 0; i < height; i++)
	{
		vptr = (u16 *)vram;
		shift = 0;

		index = i * byteperline;
		pt = font->font[index++];

		for (j = 0; j < *(font->width); j++)
		{
			if (shift >= 8)
			{
				shift = 0;
				pt = font->font[index++];
			}

			if (pt & 0x80)
			{
			  // 文字描画時
				if (fill & 0x01 && rate > 0)
					*vptr = (rate < 100) ? fbmColMix(vptr, color, rate) : color;
			}
			// 背景描画時
			else
			{
			  if (fill & 0x10 && rate > 0)
			    *vptr = (rate < 100) ? fbmColMix(vptr, back, rate) : back;
			}

			vptr++;

			shift++;
			pt <<= 1;
		}

		vram = (u16 *)vram + bufferwidth;
	}

	nextx = nextx + *(font->width);
}

/////////////////////////////////////////////////////////////////////////////
// VRAM Color Mix
// vr: VRAM Address, color: Mix Color, rate: Mix Rate (0-100)
/////////////////////////////////////////////////////////////////////////////
u32 fbm_colmix(void *vr, u32 color, int rate)
{
  if (rate !=100 )
  {
    u32 r1, g1, b1;
    u32 r2, g2, b2;

    r1 = color & 0x1f;
    g1 = (color >> 5) & 0x1f;
    b1 = (color >> 10)& 0x1f;

    r2 = (*(u16 *)vr >> 10)& 0x1f;
    g2 = (*(u16 *)vr >> 5) & 0x1f;
    b2 = *(u16 *)vr & 0x1f;

    r1 = ((r1 * rate) + (r2 * (100 - rate)) + 50) / 100;
    g1 = ((g1 * rate) + (g2 * (100 - rate)) + 50) / 100;
    b1 = ((b1 * rate) + (b2 * (100 - rate)) + 50) / 100;

    color = (b1 << 10) | (g1 << 5) | r1;
  }

  return color;
}

u32 fbm_colmixrev(void *vr, u32 color, int rate)
{
  if (rate !=100 )
  {
    u32 r1, g1, b1;
    u32 r2, g2, b2;

    r1 = (color >>10)& 0x1f;
    g1 = (color >> 5) & 0x1f;
    b1 = color & 0x1f;

    r2 = (*(u16 *)vr >> 10)& 0x1f;
    g2 = (*(u16 *)vr >> 5) & 0x1f;
    b2 = *(u16 *)vr & 0x1f;

    r1 = ((r1 * rate) + (r2 * (100 - rate)) + 50) / 100;
    g1 = ((g1 * rate) + (g2 * (100 - rate)) + 50) / 100;
    b1 = ((b1 * rate) + (b2 * (100 - rate)) + 50) / 100;

    color = (r1 << 10) | (g1 << 5) | b1;
  }
  return color;
}

void *fbm_malloc(size_t size)
{
	int *p;
	int h_block;


	if (size == 0) return NULL;

	h_block = sceKernelAllocPartitionMemory(2, "block", 0, size + sizeof(h_block), NULL);

	if (h_block < 0) return NULL;

	p = (int *)sceKernelGetBlockHeadAddr(h_block);
	*p = h_block;

	return (void *)(p + 1);
}


void fbm_free(void **ptr)
{
	int *p;
	int h_block;


	if (*ptr != NULL)
	{
		p = (int *)*ptr;
		h_block = *(p - 1);
		sceKernelFreePartitionMemory(h_block);
		*ptr = NULL;
	}
}


int fbm_fopen(char *path)
{
	int result;


	result = sceIoOpen(path, PSP_O_RDONLY, 0777);

	return result;
}


void fbm_fclose(int *fd)
{
	if (*fd < 0) return;
	sceIoClose(*fd);
	*fd = -1;
}


int fbm_readfct(int fd, fbm_control_t *control, fbm_map_t **map, int *fbm_whence)
{
	int result;


	result = sceIoRead(fd, control, FBM_SIZE_CONTROL);

	if (result != FBM_SIZE_CONTROL)
	{
		sceIoClose(fd);
		return -1;
	}

	*map = (fbm_map_t *)fbm_malloc(6 * control->mapcnt);

	if (*map == NULL)
	{
		sceIoClose(fd);
		return -2;
	}

	result = sceIoRead(fd, *map, FBM_SIZE_MAP * control->mapcnt);

	if (result != FBM_SIZE_MAP * control->mapcnt)
	{
		sceIoClose(fd);
		return -3;
	}

	*fbm_whence = FBM_SIZE_CONTROL + FBM_SIZE_MAP * control->mapcnt;

	return 0;
}


int fbm_readfbm(int fd, fbm_control_t *control, fbm_font_t **font, u8 **buf, int fbm_whence, int index, int fontcnt)
{
	int result;
	int offset;
	int rebuild;
	u16 i;


	rebuild = (*font == NULL || *buf == NULL) ? 1: 0;

	if (rebuild)
	{
		fbm_free((void **)font);
		fbm_free((void **)buf);

		*font = (fbm_font_t *)fbm_malloc(sizeof(fbm_font_t) * fontcnt);

		if (*font == NULL)
		{
			return -1;
		}

		*buf = (u8 *)fbm_malloc((1 + control->byteperchar) * fontcnt);

		if (*buf == NULL)
		{
			return -2;
		}
	}

	offset = (1 + control->byteperchar) * index;
	result = sceIoLseek32(fd, offset + fbm_whence, 0);

	if (result != offset + fbm_whence)
	{
		sceIoClose(fd);
		return -3;
	}

	result = sceIoRead(fd, *buf, (1 + control->byteperchar) * fontcnt);

	if (result != (1 + control->byteperchar) * fontcnt)
	{
		sceIoClose(fd);
		return -4;
	}

	if (rebuild)
	{
		for (i = 0; i < fontcnt; i++)
		{
			(*font)[i].width = *buf + (1 + control->byteperchar) * i;
			(*font)[i].font = *buf + (1 + control->byteperchar) * i + 1;
		}
	}

	return 0;
}


int fbm_issingle(u16 c)
{
	int i;


	for (i = 0; i < fbmControl[0].mapcnt && c >= fbmFontMap[0][i].start; i++)
	{
		if (c >= fbmFontMap[0][i].start && c <= fbmFontMap[0][i].end)
			return c - fbmFontMap[0][i].distance;
	}

	return -1;
}


int fbm_isdouble(u16 c)
{
	int i;


	if (!use_double) return -1;

	for (i = 0; i < fbmControl[1].mapcnt && c >= fbmFontMap[1][i].start; i++)
	{
		if (c >= fbmFontMap[1][i].start && c <= fbmFontMap[1][i].end)
			return c - fbmFontMap[1][i].distance;
	}

	return -1;
}


fbm_font_t * fbm_getfont(u16 index, u8 isdouble)
{
	int result;


	if (read_mode)
	{
		return &fbmFont[isdouble][index];
	}
	else
	{
		result = fbm_readfbm(fbm_fd[isdouble], &fbmControl[isdouble],
							 &fbmFont[0], &font_buf[0], fbm_whence[isdouble], index, 1);

		if (result < 0) font_buf[0][0] = 0;
		return &fbmFontbyFile;
	}
}
