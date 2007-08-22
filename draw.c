/* unofficial gameplaySP kai
 *
 * Copyright (C) 2007 NJ
 * Copyright (C) 2007 takka <takka@tfact.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licens e as
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

/******************************************************************************
 * draw.c
 * 基本描画の処理
 ******************************************************************************/

/******************************************************************************
 * ヘッダファイルの読込み
 ******************************************************************************/
#include "common.h"

/******************************************************************************
 * マクロ等の定義
 ******************************************************************************/
#define progress_sx (240-150)
#define progress_ex (240+150)
#define progress_sy (138+3)
#define progress_ey (138+13)
#define progress_color COLOR16(15,15,15)
#define progress_wait (1 * 1000 * 1000)

#define VRAM_POS(x, y)  (screen_address + (x + y * screen_pitch));

/******************************************************************************
 * グローバル変数の定義
 ******************************************************************************/
static int progress_total;
static int progress_current;
static char progress_message[128];

/******************************************************************************
 * ローカル変数の定義
 ******************************************************************************/

/******************************************************************************
 * ローカル関数の宣言
 ******************************************************************************/

/******************************************************************************
 * グローバル関数の定義
 ******************************************************************************/
/*------------------------------------------------------
  文字列を描画 / センタリング処理
------------------------------------------------------*/
void print_string_center(int sy, u32 color, u32 bg_color, const char *str)
{
  int width = fbm_getwidth(str);
  int sx = (screen_width - width) / 2;

  PRINT_STRING_BG(str, color, bg_color, sx, sy);
}

/*------------------------------------------------------
  文字列を描画 / 影付き / センタリング処理
------------------------------------------------------*/
void print_string_shadow_center(int sy, u32 color, const char *str)
{
  int width = fbm_getwidth(str);
  int sx = (screen_width - width) / 2;

  PRINT_STRING_SHADOW(str, color, sx, sy);
}

/*------------------------------------------------------
  水平線描画
------------------------------------------------------*/
void hline(u32 sx, u32 ex, u32 y, u32 color)
{
  u32 x;
  u32 width  = (ex - sx) + 1;
  u16 *dst = VRAM_POS(sx, y);

  for (x = 0; x < width; x++)
    *dst++ = (u16)color;
}

/*------------------------------------------------------
  水平線描画 (アルファブレンド) alpha = 0 - 15
------------------------------------------------------*/
void hline_alpha(u32 sx, u32 ex, u32 y, u32 color, u32 alpha)
{
  u32 x;
  u32 dst_r, dst_g, dst_b;
  int width  = (ex - sx) + 1;
  u16 *dst = VRAM_POS(sx, y);

  u32 r = GET_R16(color);
  u32 g = GET_G16(color);
  u32 b = GET_B16(color);

  for (x = 0; x < width; x++)
  {
    color = dst[x];
    dst_r = GET_R16(color);
    dst_g = GET_G16(color);
    dst_b = GET_B16(color);

    dst_r = alpha_blend[alpha][r][dst_r];
    dst_g = alpha_blend[alpha][g][dst_g];
    dst_b = alpha_blend[alpha][b][dst_b];

    dst[x] = (u16)COLOR16(dst_r, dst_g, dst_b);
  }
}

/*------------------------------------------------------
  垂直線描画
------------------------------------------------------*/
void vline(u32 x, u32 sy, u32 ey, u32 color)
{
  int y;
  int height = (ey - sy) + 1;
  u16 *dst = VRAM_POS(x, sy);

  for (y = 0; y < height; y++)
  {
    *dst = (u16)color;
    dst += screen_pitch;
  }
}

/*------------------------------------------------------
  垂直線描画 (アルファブレンド) alpha = 0 - 15
------------------------------------------------------*/
void vline_alpha(u32 x, u32 sy, u32 ey, u32 color, u32 alpha)
{
  u32 y;
  u32 dst_r, dst_g, dst_b;
  u32 height = (ey - sy) + 1;
  u16 *dst = VRAM_POS(x, sy);

  u32 r = GET_R16(color);
  u32 g = GET_G16(color);
  u32 b = GET_B16(color);

  for (y = 0; y < height; y++)
  {
    dst_r = GET_R16(*dst);
    dst_g = GET_G16(*dst);
    dst_b = GET_B16(*dst);

    dst_r = alpha_blend[alpha][r][dst_r];
    dst_g = alpha_blend[alpha][g][dst_g];
    dst_b = alpha_blend[alpha][b][dst_b];

    *dst = (u16)COLOR16(dst_r, dst_g, dst_b);

    dst += screen_pitch;
  }
}

/*------------------------------------------------------
  矩形描画 (16bit)
------------------------------------------------------*/
void box(u32 sx, u32 sy, u32 ex, u32 ey, u32 color)
{
  hline(sx, ex - 1, sy, color);
  vline(ex, sy, ey - 1, color);
  hline(sx + 1, ex, ey, color);
  vline(sx, sy + 1, ey, color);
}

/*------------------------------------------------------
  矩形塗りつぶし
------------------------------------------------------*/
void boxfill(u32 sx, u32 sy, u32 ex, u32 ey, u32 color)
{
  u32 x, y;
  u32 width  = (ex - sx) + 1;
  u32 height = (ey - sy) + 1;
  u16 *dst = (u16 *)(screen_address + (sx + sy * screen_pitch));

  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x++)
    {
      dst[x + y * screen_pitch] = (u16)color;
    }
  }
}

/*------------------------------------------------------
  矩形塗りつぶし (アルファブレンド) alpha = 0 - 15
------------------------------------------------------*/
void boxfill_alpha(u32 sx, u32 sy, u32 ex, u32 ey, u32 color, u32 alpha)
{
  u32 x, y;
  u32 dst_r, dst_g, dst_b;
  u32 r = GET_R16(color);
  u32 g = GET_G16(color);
  u32 b = GET_B16(color);
  u32 width  = (ex - sx) + 1;
  u32 height = (ey - sy) + 1;
  u16 *dst = (u16 *)(screen_address + (sx + sy * screen_pitch));

  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x++)
    {
      color = dst[x];

      dst_r = GET_R16(color);
      dst_g = GET_G16(color);
      dst_b = GET_B16(color);

      dst_r = alpha_blend[alpha][r][dst_r];
      dst_g = alpha_blend[alpha][g][dst_g];
      dst_b = alpha_blend[alpha][b][dst_b];

      dst[x + y * screen_pitch] = (u16)COLOR16(dst_r, dst_g, dst_b);
    }
  }
}

/*------------------------------------------------------
  ダイアログボックス表示
------------------------------------------------------*/
void draw_dialog(u32 sx, u32 sy, u32 ex, u32 ey)
{
//  draw_box_shadow(sx, sy, ex, ey);
  boxfill(sx + 5, sy + 5, ex + 5, ey + 5, COLOR_DIALOG_SHADOW);

  hline(sx, ex - 1, sy, COLOR_FRAME);
  vline(ex, sy, ey - 1, COLOR_FRAME);
  hline(sx + 1, ex, ey, COLOR_FRAME);
  vline(sx, sy + 1, ey, COLOR_FRAME);

  sx++;
  ex--;
  sy++;
  ey--;

  hline(sx, ex - 1, sy, COLOR_FRAME);
  vline(ex, sy, ey - 1, COLOR_FRAME);
  hline(sx + 1, ex, ey, COLOR_FRAME);
  vline(sx, sy + 1, ey, COLOR_FRAME);

  sx++;
  ex--;
  sy++;
  ey--;

  boxfill(sx, sy, ex, ey, COLOR_DIALOG);
}

/*--------------------------------------------------------
  プログレスバー
--------------------------------------------------------*/
/*--------------------------------------------------------
  プログレスバー初期化
--------------------------------------------------------*/
void init_progress(int total, const char *text)
{
  progress_current = 0;
  progress_total   = total;
  strcpy(progress_message, text);

  draw_dialog(240-158, 136-26, 240+158, 136+26);

  boxfill(progress_sx - 1, progress_sy - 1, progress_ex + 1, progress_ey + 1, 0);

  if (text[0] != '\0')
    print_string_center(118, COLOR_PROGRESS_TEXT, COLOR_DIALOG, text);
}

/*--------------------------------------------------------
  プログレスバー更新
--------------------------------------------------------*/
void update_progress(void)
{
  int width = (++progress_current * 100 / progress_total) * 3;

  draw_dialog(240-158, 136-26, 240+158, 136+26);

  boxfill(progress_sx - 1, progress_sy - 1, progress_ex + 1, progress_ey + 1, COLOR_BLACK);
  if (progress_message[0] != '\0')
    print_string_center(118, COLOR_PROGRESS_TEXT, COLOR_DIALOG, progress_message);
  boxfill(progress_sx, progress_sy, progress_sx+width, progress_ey, COLOR_PROGRESS_BAR);

  flip_screen();
}

/*--------------------------------------------------------
  プログレスバー結果表示
--------------------------------------------------------*/
void show_progress(const char *text)
{
  draw_dialog(240-158, 136-26, 240+158, 136+26);

  boxfill(progress_sx - 1, progress_sy - 1, progress_ex + 1, progress_ey + 1, COLOR_BLACK);
    
  if (progress_current)
  {
    int width = (progress_current * 100 / progress_total) * 3;
    boxfill(progress_sx, progress_sy, progress_sx+width, progress_ey, COLOR_PROGRESS_BAR);
  }

  if (text[0] != '\0')
    print_string_center(118, COLOR_PROGRESS_TEXT, COLOR_DIALOG,text);

  flip_screen();
  sceKernelDelayThread(progress_wait);
}

/******************************************************************************
 * ローカル関数の定義
 ******************************************************************************/
