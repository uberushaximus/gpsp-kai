/* unofficial gameplaySP kai
 *
 * Copyright (C) 2006 Exophase <exophase@gmail.com>
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

/******************************************************************************
 * グローバル変数の定義
 ******************************************************************************/

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
  矩形塗りつぶし
------------------------------------------------------*/
void boxfill(u32 sx, u32 sy, u32 ex, u32 ey, u32 color)
{
  u32 x, y;
  u32 width  = (ex - sx) + 1;
  u32 height = (ey - sy) + 1;
  u16 *dst = screen_address + (sx + sy * screen_pitch);

  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x++)
    {
      dst[x] = color;
    }
    dst += screen_pitch;
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
  u16 *dst = screen_address + (sx + sy * screen_pitch);

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

      dst[x] = COLOR16(dst_r, dst_g, dst_b);
    }
    dst += screen_pitch;
  }
}

/*--------------------------------------------------------
  プログレスバー
--------------------------------------------------------*/

static int progress_total;
static int progress_current;
static char progress_message[128];

/*--------------------------------------------------------
  プログレスバー初期化
--------------------------------------------------------*/
void init_progress(int total, const char *text)
{
  progress_current = 0;
  progress_total   = total;
  strcpy(progress_message, text);

//  draw_dialog(240-158, 136-26, 240+158, 136+26);
  boxfill(240-151, 138+2, 240+151, 138+14, 0);

//  uifont_print_shadow_center(118, 255,255,255, text);

}

/*--------------------------------------------------------
  プログレスバー更新
--------------------------------------------------------*/
void update_progress(void)
{
  int width = (++progress_current * 100 / progress_total) * 3;

//  show_background();

//  draw_dialog(240-158, 136-26, 240+158, 136+26);
  boxfill(240-151, 138+3, 240+151, 138+13, COLOR_ERROR);

//  uifont_print_shadow_center(118, 255,255,255, progress_message);
//  draw_battery_status(1);

  boxfill(240-150, 138+3, 240-150+width-1, 138+13, COLOR16(15, 15, 15));

//  video_flip_screen(1);
}

/*--------------------------------------------------------
  プログレスバー結果表示
--------------------------------------------------------*/
void show_progress(const char *text)
{
//  show_background();

//  draw_dialog(240-158, 136-26, 240+158, 136+26);
  boxfill(240-151, 138+2, 240+151, 138+14, 0);

//  uifont_print_shadow_center(118, 255,255,255, text);
//  draw_battery_status(1);

  if (progress_current)
  {
    int width = (progress_current * 100 / progress_total) * 3;
    boxfill(240-150, 138+3, 240-150+width-1, 138+13, COLOR16(15, 15, 15));
  }

//  video_flip_screen(1);

  sceKernelDelayThread(1000000);
}

/******************************************************************************
 * ローカル関数の定義
 ******************************************************************************/
