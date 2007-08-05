/****************************************************************************

  FBM fonts print string headder
                                                                      by mok
****************************************************************************/
#ifndef __FBM_PRINT__
#define __FBM_PRINT__


#define FBM_FONT_FILL	(0x01)		// Fill Font(normal)
#define FBM_BACK_FILL	(0x10)		// Fill BackGrand


typedef struct
{
	u16 fontcnt;
	u16 mapcnt;
	u16 defaultchar;
	u8  width;
	u8  height;
	u8  byteperchar;
} fbm_control_t;

typedef struct
{
	u16 start;
	u16 end;
	u16 distance;
} fbm_map_t;

typedef struct
{
	u8  *width;
	u8  *font;
} fbm_font_t;


/////////////////////////////////////////////////////////////////////////////
// Fbm Font Initialize
// s_path: Single Byte Font (ex.ASCII), d_path: Double Byte Font (ex.SJIS),
// mode: Font Read Mode (1=On Memory(fast), 0=Disk Access(slow))
/////////////////////////////////////////////////////////////////////////////
int fbm_init(char *s_path, char *d_path, int mode);

/////////////////////////////////////////////////////////////////////////////
// Fbm Font Termination
/////////////////////////////////////////////////////////////////////////////
void fbm_freeall();

/////////////////////////////////////////////////////////////////////////////
// Get Draw Width-Pixcels
// str: Draw String
/////////////////////////////////////////////////////////////////////////////
int fbm_getwidth(char *str);

/////////////////////////////////////////////////////////////////////////////
// Print String: Columns & Rows
// col: Columns (0-fbmMaxCol), row: Rows (0-fbmMaxRow), str: Print String,
// color: Font Color, back: Back Grand Color,
// fill: Fill Mode Flag (ex.FBM_FONT_FILL | FBM_BACK_FILL),
// rate: Mix Rate (0-100 or -1--101)
/////////////////////////////////////////////////////////////////////////////
int fbm_printCR(int col, int row, char *str, u32 color, u32 back, u8 fill, int rate);

/////////////////////////////////////////////////////////////////////////////
// Print String: XY Pixel
// x: X (0-479), y: Y (0-271), str: Print String,
// color: Font Color, back: Back Grand Color,
// fill: Fill Mode Flag (ex.FBM_FONT_FILL | FBM_BACK_FILL),
// rate: Mix Rate (0-100 or -1--101)
/////////////////////////////////////////////////////////////////////////////
int fbm_printXY(int x, int y, char *str, u32 color, u32 back, u8 fill, int rate);

/////////////////////////////////////////////////////////////////////////////
// Print String: Base VRAM Addr + XY Pixel
// vram: Base VRAM Addr, bufferwidth: buffer-width per line,
// pixelformat: pixel color format (0=16bit, 1=15bit, 2=12bit, 3=32bit)
// x: X (0-479), y: Y (0-271), str: Print String,
// color: Font Color, back: Back Grand Color,
// fill: Fill Mode Flag (ex.FBM_FONT_FILL | FBM_BACK_FILL),
// rate: Mix Rate (0-100 or -1--101)
/////////////////////////////////////////////////////////////////////////////
int fbm_printVRAM(void *vram, int bufferwidth, int pixelformat, int x, int y, char *str, u32 color, u32 back, u8 fill, int rate, u32 pad);

/////////////////////////////////////////////////////////////////////////////
// Print String Subroutine (Draw VRAM)
// vram: Base VRAM Addr, bufferwidth: buffer-width per line,
// index: Font Index, isdouble: Is Double Byte Font? (0=Single, 1=Double),
// height: Font Height, byteperline: Used 1 Line Bytes,
// color: Font Color, back: Back Grand Color,
// fill: Fill Mode Flag (ex.FBM_FONT_FILL | FBM_BACK_FILL),
// rate: Mix Rate (0-100 or -1--101)
/////////////////////////////////////////////////////////////////////////////
void fbm_printSUB16(void *vram, int bufferwidth, int index, int isdouble, int height, int byteperline, u32 color, u32 back, u8 fill, int rate);
void fbm_printSUB32(void *vram, int bufferwidth, int index, int isdouble, int height, int byteperline, u32 color, u32 back, u8 fill, int rate);

/////////////////////////////////////////////////////////////////////////////
// VRAM Color Mix
// vr: VRAM Address, color: Mix Color, rate: Mix Rate (0-100)
/////////////////////////////////////////////////////////////////////////////
u32 fbm_colmix0(void *vr, u32 color, int rate);
u32 fbm_colmix1(void *vr, u32 color, int rate);
u32 fbm_colmix2(void *vr, u32 color, int rate);
u32 fbm_colmix3(void *vr, u32 color, int rate);
u32 fbm_colmixrev0(void *vr, u32 color, int rate);
u32 fbm_colmixrev1(void *vr, u32 color, int rate);
u32 fbm_colmixrev2(void *vr, u32 color, int rate);
u32 fbm_colmixrev3(void *vr, u32 color, int rate);

#endif
