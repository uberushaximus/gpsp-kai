/* unofficial gameplaySP kai
 *
 * Copyright (C) 2006 Exophase <exophase@gmail.com>
 * Copyright (C) 2007 takka <takka@tfact.net>
 * Copyright (C) 2007 ????? <?????>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "common.h"

static u32 __attribute__((aligned(32))) display_list[2048];

s32 load_video_config();
void set_resolution_parameter(video_scale_type scale);
void set_video_out();

#define MENU_LINE_SIZE  512
#define GBA_LINE_SIZE   240
#define FRAME_LINE_SIZE 768

#define GPSP_CONFIG_FILENAME "cfg/video.cfg"

// VRAM メモリマップ(VRAM割当てのAPIを使用しないで全て自前で管理)
// 0x000000～0x0BFFFF(0xC0000 768.0kb) 表示用フレームバッファ
// 0x0C0000～0x0D2DFF(0x12E00  75.5kb) GBA用フレームバッファ 1
// 0x0D2E00～0x0E5BFF(0x12E00  75.5kb) GBA用フレームバッファ 2
// 0x0E5C00～0x125BFF(0x40000 256.0kb) MENU用フレームバッファ 1
// 0x125C00～0x165BFF(0x40000 256.0kb) MENU用フレームバッファ 2
// 0x165C00～0x1A5BFF(0x40000 256.0kb) MENU用スキンバッファ(将来用)
// 0x1A5C00～0x1FBFFF(0x56400 345.0kb) 汎用データバッファ
// 0x1FC000～0x1FC0FF(0x00100 256byte) フレームバッファ転送用GUコマンドバッファ
// 0x1FC100～0x1FC1FF(0x00100 256byte) フレームバッファ転送用頂点データバッファ
// 0x1FC200～0x1FFFFF(0x03E00  15.5kb) 汎用頂点データバッファ
static float *screen_vertex = (float *)0x441FC100;
static u32 *ge_cmd = (u32 *)0x441FC000;
static u16 *psp_gu_vram_base = (u16 *)(0x44000000);
static u32 *ge_cmd_ptr = (u32 *)0x441FC000;
static u32 gecbid;
float *temp_vertex = (float *)0x441FC200;
u16 *vram_data = (u16 *)0x441A5C00;

u32 video_draw_frame = FRAME_GAME;

#define GBA_BUFFER_SIZE (240*161*2)  /* インタレース用に1ライン多く確保 */
#define MENU_BUFFER_SIZE (480*273*2) /* インタレース用に1ライン多く確保 */
#define PSP_DISPLAY_BUFFER_SIZE (768 * 512 * 2)
u16 *screen_address = (u16 *)(0x04000000 + PSP_DISPLAY_BUFFER_SIZE);
u16 *screen_offset_address = (u16 *)PSP_DISPLAY_BUFFER_SIZE;
u32 screen_pitch = 240;
u32 screen_width = 240;
u32 screen_height = 160;
u32 screen_width2 = 240 / 2;
u32 screen_height2 = 160 / 2;

u32 flip_address[2][2] =
{
  { 0x04000000 + PSP_DISPLAY_BUFFER_SIZE, 0x04000000 + PSP_DISPLAY_BUFFER_SIZE + GBA_BUFFER_SIZE },
  { 0x04000000 + PSP_DISPLAY_BUFFER_SIZE + GBA_BUFFER_SIZE * 2, 0x04000000 + PSP_DISPLAY_BUFFER_SIZE + GBA_BUFFER_SIZE * 2 + MENU_BUFFER_SIZE }
};

u32 flip_offset_address[2][2] =
{
  { PSP_DISPLAY_BUFFER_SIZE, PSP_DISPLAY_BUFFER_SIZE + GBA_BUFFER_SIZE },
  { PSP_DISPLAY_BUFFER_SIZE + GBA_BUFFER_SIZE * 2, PSP_DISPLAY_BUFFER_SIZE + GBA_BUFFER_SIZE * 2 + MENU_BUFFER_SIZE }
};

#define GE_CMD_NOP    0x00
#define GE_CMD_VADDR  0x01
#define GE_CMD_IADDR  0x02
#define GE_CMD_PRIM   0x04
#define GE_CMD_SIGNAL 0x0C
#define GE_CMD_FINISH 0x0F
#define GE_CMD_BASE   0x10
#define GE_CMD_VTYPE  0x12
#define GE_CMD_WMS    0x3A
#define GE_CMD_WORLD  0x3B
#define GE_CMD_VMS    0x3C
#define GE_CMD_VIEW   0x3D
#define GE_CMD_PMS    0x3E
#define GE_CMD_PROJ   0x3F
#define GE_CMD_FBP    0x9C
#define GE_CMD_FBW    0x9D
#define GE_CMD_TBP0   0xA0
#define GE_CMD_TBW0   0xA8
#define GE_CMD_TSIZE0 0xB8
#define GE_CMD_TFLUSH 0xCB
#define GE_CMD_CLEAR  0xD3

#define GE_CMD(cmd, operand)                                                \
  *ge_cmd_ptr = (((GE_CMD_##cmd) << 24) | (operand));                       \
  ge_cmd_ptr++                                                              \

static void Ge_Finish_Callback(int id, void *arg)
{
}


u32 screen_flip = 0;

u32 video_out_mode;

u32 use_video_out;

#define PSP_OUT     0
#define ANALOG_OUT  1
#define DIGITAL_OUT 2

#define SCREEN_INTERLACE 2  /* on/off */
#define SCREEN_RATIO     2  /* 4:3/16:9 */
#define SCREEN_SCALE     5  /* non-scale/scale/full/etc 1/etc 2 */

SCREEN_PARAMETER screen_parameter_psp_game[SCREEN_SCALE];
SCREEN_PARAMETER screen_parameter_psp_menu;

SCREEN_PARAMETER screen_parameter_composite_game[SCREEN_RATIO][SCREEN_INTERLACE][SCREEN_SCALE];
SCREEN_PARAMETER screen_parameter_composite_menu[SCREEN_RATIO][SCREEN_INTERLACE];

SCREEN_PARAMETER screen_parameter_component_game[SCREEN_RATIO][SCREEN_INTERLACE][SCREEN_SCALE];
SCREEN_PARAMETER screen_parameter_component_menu[SCREEN_RATIO][SCREEN_INTERLACE];

SCREEN_PARAMETER *current_parameter;

typedef union
{
  float f;
  u32   i;
}IFLOAT;

void init_video()
{
  use_video_out = 0;
  video_out_mode = 0;
  video_draw_frame = FRAME_GAME;
  static u32 first_load = 0;

  // パラメータの初期化
  if(first_load == 0)
  {
    if(load_video_config() == 0)
      use_video_out = 1;
    first_load = 1;
  }

  sceDisplaySetMode(0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT);

  sceDisplayWaitVblankStart();
  // パレットを5551(0BBBBBGGGGGRRRRR)にする GBAも0BBBBBGGGGGRRRRRなので変換の必要がない
  sceDisplaySetFrameBuf((void*)psp_gu_vram_base, FRAME_LINE_SIZE, PSP_DISPLAY_PIXEL_FORMAT_5551, PSP_DISPLAY_SETBUF_NEXTFRAME);

  sceGuInit();

  sceGuStart(GU_DIRECT, display_list);
  sceGuDrawBuffer(GU_PSM_5551, (void*)0, FRAME_LINE_SIZE);
  sceGuDispBuffer(PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, (void*)0, FRAME_LINE_SIZE);
  sceGuClear(GU_COLOR_BUFFER_BIT);

  sceGuOffset(2048 - (PSP_SCREEN_WIDTH / 2), 2048 - (PSP_SCREEN_HEIGHT / 2));
  sceGuViewport(2048, 2048, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT);

  sceGuScissor(0, 0, PSP_SCREEN_WIDTH + 0, PSP_SCREEN_HEIGHT + 0);
  sceGuEnable(GU_SCISSOR_TEST);
  sceGuTexMode(GU_PSM_5551, 0, 0, GU_FALSE);
  sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
  sceGuTexFilter(GU_LINEAR, GU_LINEAR);
  sceGuEnable(GU_TEXTURE_2D);

  sceGuFrontFace(GU_CW);
  sceGuDisable(GU_BLEND);

  sceGuFinish();
  sceGuSync(0, 0);

  sceDisplayWaitVblankStart();
  sceGuDisplay(GU_TRUE);

  PspGeCallbackData gecb;
  gecb.signal_func = NULL;
  gecb.signal_arg = NULL;
  gecb.finish_func = Ge_Finish_Callback;
  gecb.finish_arg = NULL;
  gecbid = sceGeSetCallback(&gecb);

  memcpy(screen_vertex, &screen_parameter_psp_game_init, sizeof(float) * 20);
  // Set framebuffer to PSP VRAM
  GE_CMD(FBP, ((u32)psp_gu_vram_base & 0x00FFFFFF));
  GE_CMD(FBW, (((u32)psp_gu_vram_base & 0xFF000000) >> 8) | 768);
  // Set texture 0 to the screen texture
  GE_CMD(TBP0, ((u32)flip_address[0][0] & 0x00FFFFFF));
  GE_CMD(TBW0, (((u32)flip_address[0][0] & 0xFF000000) >> 8) | GBA_SCREEN_WIDTH);
  // Set the texture size to 256 by 256 (2^8 by 2^8)
  GE_CMD(TSIZE0, (9 << 8) | 9);
  // Flush the texture cache
  GE_CMD(TFLUSH, 0);
  // Use 2D coordinates, no indeces, no weights, 32bit float positions,
  // 32bit float texture coordinates
  GE_CMD(VTYPE, (1 << 23) | (0 << 11) | (0 << 9) | (3 << 7) | (0 << 5) | (0 << 2) | 3);
  // Set the base of the index list pointer to 0
  GE_CMD(BASE, 0);
  // Set the rest of index list pointer to 0 (not being used)
  GE_CMD(IADDR, 0);
  // Set the base of the screen vertex list pointer
  GE_CMD(BASE, ((u32)screen_vertex & 0xFF000000) >> 8);
  // Set the rest of the screen vertex list pointer
  GE_CMD(VADDR, ((u32)screen_vertex & 0x00FFFFFF));
  // Primitive kick: render sprite (primitive 6), 2 vertices
  GE_CMD(PRIM, (6 << 16) | 2);
  GE_CMD(PRIM, (6 << 16) | 2);
  // Done with commands
  GE_CMD(FINISH, 0);
  // Raise signal interrupt
  GE_CMD(SIGNAL, 0);
  GE_CMD(NOP, 0);
  GE_CMD(NOP, 0);
}

u32 current_scale = scaled_aspect;

u32 TBP0_temp[2];
u32 TBW0_temp[2];
u32 TSIZE0_temp;
u32 flip_address_temp[2];
u32 flip_offset_address_temp[2];

void flip_screen()
{
  u32 *old_ge_cmd_ptr = ge_cmd_ptr;

  sceKernelDcacheWritebackAll();

  // Render the current screen
  ge_cmd_ptr = ge_cmd + 2;
  *ge_cmd_ptr = TBP0_temp[screen_flip];
  ge_cmd_ptr++;
  *ge_cmd_ptr = TBW0_temp[screen_flip];
  ge_cmd_ptr++;
  *ge_cmd_ptr = TSIZE0_temp;
  ge_cmd_ptr++;
  ge_cmd_ptr = old_ge_cmd_ptr;

  // Flip to the next screen
  screen_flip ^= 1;
  screen_address = (u16 *)flip_address_temp[screen_flip];
  screen_offset_address = (u16 *)flip_offset_address_temp[screen_flip];

  sceKernelDcacheWritebackAll();

  sceGeListEnQueue(ge_cmd, ge_cmd_ptr, gecbid, NULL);
}

void video_resolution(u32 frame)
{
  static u32 first_load = 0;
  static SCREEN_PARAMETER *old_parameter = NULL;

  if(first_load == 0)
  {
    first_load = 1;
    current_parameter = &screen_parameter_psp_menu;
  }
  else
  {
    flip_screen();
    clear_screen(0);
    flip_screen();
    clear_screen(0);
    flip_screen();
  }

  if(video_draw_frame != frame)
  {
    if((psp_model == psp_2000) && (use_video_out == 1))
      video_out_mode = pspDveMgrCheckVideoOut();
    else
      video_out_mode = 0;

    switch(((video_out_mode << 1) | frame))
    {
      case ((PSP_OUT << 1) | FRAME_GAME):
        current_parameter = &screen_parameter_psp_game[gpsp_config.screen_scale];
        break;
      case ((PSP_OUT << 1) | FRAME_MENU):
        current_parameter = &screen_parameter_psp_menu;
        break;
      case ((ANALOG_OUT << 1) | FRAME_GAME):
        current_parameter = &screen_parameter_composite_game[gpsp_config.screen_ratio][gpsp_config.screen_interlace][gpsp_config.screen_scale];
        break;
      case ((ANALOG_OUT << 1) | FRAME_MENU):
        current_parameter = &screen_parameter_composite_menu[gpsp_config.screen_ratio][gpsp_config.screen_interlace];
        break;
      case ((DIGITAL_OUT << 1) | FRAME_GAME):
        current_parameter = &screen_parameter_component_game[gpsp_config.screen_ratio][gpsp_config.screen_interlace][gpsp_config.screen_scale];
        break;
      case ((DIGITAL_OUT << 1) | FRAME_MENU):
        current_parameter = &screen_parameter_component_menu[gpsp_config.screen_ratio][gpsp_config.screen_interlace];
        break;
    }

    if(old_parameter != current_parameter)
    {
      old_parameter = current_parameter;
      if((psp_model == psp_2000) && (use_video_out == 1))
      {
        pspDveMgrSetVideoOut(current_parameter->video_out.u, current_parameter->video_out.displaymode, current_parameter->video_out.width,
            current_parameter->video_out.height, current_parameter->video_out.x, current_parameter->video_out.y, current_parameter->video_out.z);
      }
      else
      {
        sceDisplaySetMode(0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT);
      }
    }

    memcpy(screen_vertex, &current_parameter->screen_setting_1, sizeof(float) * 20);

    video_draw_frame = frame;
    screen_address = (u16 *)flip_address[frame][0];
    screen_flip = 0;
    screen_pitch = current_parameter->texture_size.pitch;
    screen_width = current_parameter->texture_size.width;
    screen_height = current_parameter->texture_size.height;
    screen_width2 = screen_width / 2;
    screen_height2 = screen_height / 2;

    flip_address_temp[0] = flip_address[frame][0];
    flip_address_temp[1] = flip_address[frame][1];
    flip_offset_address_temp[0] = flip_offset_address[frame][0];
    flip_offset_address_temp[1] = flip_offset_address[frame][1];
    TBP0_temp[0] = ((GE_CMD_TBP0) << 24) | (flip_address_temp[0] & 0x00FFFFFF);
    TBP0_temp[1] = ((GE_CMD_TBP0) << 24) | (flip_address_temp[1] & 0x00FFFFFF);
    TBW0_temp[0] = ((GE_CMD_TBW0) << 24) | (((flip_address_temp[0] & 0xFF000000) >> 8) | screen_pitch);
    TBW0_temp[1] = ((GE_CMD_TBW0) << 24) | (((flip_address_temp[1] & 0xFF000000) >> 8) | screen_pitch);
    TSIZE0_temp  = ((GE_CMD_TSIZE0) << 24) | ((current_parameter->texture_bit.x << 8) | current_parameter->texture_bit.y);

    sceGuStart(GU_DIRECT, display_list);
    sceGuDispBuffer(current_parameter->screen_size.width, current_parameter->screen_size.height, (void*)0, FRAME_LINE_SIZE);
    sceGuOffset(2048 - (current_parameter->screen_size.width / 2), 2048 - (current_parameter->screen_size.height / 2));
    sceGuViewport(2048, 2048, current_parameter->screen_size.width, current_parameter->screen_size.height);
    sceGuScissor(current_parameter->view.x, current_parameter->view.y, current_parameter->view.width, current_parameter->view.height);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuTexFilter(current_parameter->filter[gpsp_config.screen_filter], current_parameter->filter[gpsp_config.screen_filter]);
    sceGuTexWrap(GU_CLAMP, GU_CLAMP);
    sceGuEnable(GU_TEXTURE_2D);
    sceGuFinish();
    sceGuSync(0, 0);

    flip_screen();
    clear_screen(0);
    flip_screen();
    clear_screen(0);
    flip_screen();
  }
}

void clear_screen(u16 color)
{
  u32 color32;
  color32 = (((color & 0x1F) * 8) << 0) | ((((color >> 5) & 0x1F) * 8) << 8) | ((((color >> 10) & 0x1F) * 8) << 16) | (0xFF << 24);

  sceGuStart(GU_DIRECT, display_list);
  sceGuDrawBufferList(GU_PSM_5551, (void *)screen_offset_address, screen_pitch);
  sceGuScissor(0, 0, current_parameter->texture_size.width, current_parameter->texture_size.height);
  // クリアする色の設定 ※パレット設定にかかわらず32bitで指定する
  sceGuClearColor(color32);
  sceGuClear(GU_COLOR_BUFFER_BIT);
  sceGuDrawBufferList(GU_PSM_5551, (void *)0, FRAME_LINE_SIZE);
  sceGuScissor(current_parameter->view.x, current_parameter->view.y, current_parameter->view.width, current_parameter->view.height);
  sceGuFinish();
  sceGuSync(0, 0);
}

u16 *copy_screen()
{
  u16 *copy = malloc(240 * 160 * 2);
  memcpy(copy, screen_address, 240 * 160 * 2);
  return copy;
}

void blit_to_screen(u16 *src, u32 w, u32 h, u32 dest_x, u32 dest_y)
{
  u32 pitch = screen_pitch;
  u16 *dest_ptr = screen_address + dest_x + (dest_y * pitch);

  u16 *src_ptr = src;
  u32 line_skip = pitch - w;
  u32 x, y;

  for(y = 0; y < h; y++)
  {
    for(x = 0; x < w; x++, src_ptr++, dest_ptr++)
    {
      *dest_ptr = *src_ptr;
    }
    dest_ptr += line_skip;
  }
}

#define video_savestate_body(type)                                            \
{                                                                             \
  FILE_##type##_ARRAY(g_state_buffer_ptr, affine_reference_x);            \
  FILE_##type##_ARRAY(g_state_buffer_ptr, affine_reference_y);            \
}                                                                             \

void video_read_mem_savestate()
video_savestate_body(READ_MEM);

void video_write_mem_savestate()
video_savestate_body(WRITE_MEM);

#define LOAD_PARAMETER_NUMBER (SCREEN_SCALE + 1 + (SCREEN_RATIO*SCREEN_INTERLACE*SCREEN_SCALE + SCREEN_RATIO*SCREEN_INTERLACE) * 2)

s32 load_video_config()
{
  FILE *video_file;
  char current_line[256];
  u32 i,loop;
  int p[6][8];
  float f[2][10];
  SCREEN_PARAMETER parameter[LOAD_PARAMETER_NUMBER];

  // 設定の初期化
  memcpy(&screen_parameter_psp_menu,  &screen_parameter_psp_menu_init,     sizeof(SCREEN_PARAMETER));
  memcpy(&screen_parameter_psp_game,  &screen_parameter_psp_game_init,     sizeof(SCREEN_PARAMETER)*5);

  // video configファイルのオープン
  video_file = fopen(GPSP_CONFIG_FILENAME, "r");

  if(video_file)
  {
    i = 0;
    loop = 0;
    while(fgets(current_line, 256, video_file))
    {
      if((current_line[0] !='#') && (current_line[0] !='\n'))
      {
        switch(i)
        {
          case 0:
            sscanf(current_line, " %d, %i, %d, %d, %d, %d, %d, %d",
                &p[i][0],&p[i][1],&p[i][2],&p[i][3],&p[i][4],&p[i][5],&p[i][6],&p[i][7]);
            break;

          case 1 ... 5:
            sscanf(current_line, " %d, %d, %d, %d",
                &p[i][0],&p[i][1],&p[i][2],&p[i][3]);
            break;

          case 6 ... 7:
            sscanf(current_line, " %f, %f, %f, %f, %f, %f, %f, %f, %f, %f",
                &f[i-6][0],&f[i-6][1],&f[i-6][2],&f[i-6][3],&f[i-6][4],&f[i-6][5],&f[i-6][6],&f[i-6][7],&f[i-6][8],&f[i-6][9]);
            break;
        }
        i++;
      }
      if(i == 8)
      {
        // パラメータの保存
        memcpy(&parameter[loop].video_out.u,         &p[0][0], sizeof(VIDEO_OUT_PARAMETER));
        memcpy(&parameter[loop].filter[0],           &p[1][0], sizeof(int) * 2);
        memcpy(&parameter[loop].texture_size.pitch,  &p[2][0], sizeof(TEXTURE_SIZE));
        memcpy(&parameter[loop].texture_bit.x,       &p[3][0], sizeof(TEXTURE_BIT));
        memcpy(&parameter[loop].screen_size.width,   &p[4][0], sizeof(SCREEN_SIZE));
        memcpy(&parameter[loop].view.x,              &p[5][0], sizeof(VIEW_PORT));
        memcpy(&parameter[loop].screen_setting_1.u1, &f[0][0], sizeof(SPRITE));
        memcpy(&parameter[loop].screen_setting_2.u1, &f[1][0], sizeof(SPRITE));
        i = 0;
        loop++;
      }
      if(loop == LOAD_PARAMETER_NUMBER)
      {
        // パラメータのセット
        memcpy(&screen_parameter_psp_menu,       &parameter[0],  sizeof(SCREEN_PARAMETER));
        memcpy(&screen_parameter_composite_menu, &parameter[1],  sizeof(SCREEN_PARAMETER)*2*2);
        memcpy(&screen_parameter_component_menu, &parameter[5],  sizeof(SCREEN_PARAMETER)*2*2);
        memcpy(&screen_parameter_psp_game,       &parameter[9],  sizeof(SCREEN_PARAMETER)*5);
        memcpy(&screen_parameter_composite_game, &parameter[14], sizeof(SCREEN_PARAMETER)*2*2*5);
        memcpy(&screen_parameter_component_game, &parameter[34], sizeof(SCREEN_PARAMETER)*2*2*5);
        return 0;
      }
    }
  }

  return -1;
}
