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
  u32 x;
  u32 y;
  u32 dst_r;
  u32 dst_g;
  u32 dst_b;
  u32 r = GET_R16(color);
  u32 g = GET_G16(color);
  u32 b = GET_B16(color);
  u32 bg_color;
  u32 width  = (ex - sx) + 1;
  u32 height = (ey - sy) + 1;
  u16 *dst = screen_address + (sx + sy * screen_pitch);

  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x++)
    {
      bg_color = dst[x];

      dst_r = GET_R16(bg_color);
      dst_g = GET_G16(bg_color);
      dst_b = GET_B16(bg_color);

      dst_r = alpha_blend[alpha][r][dst_r];
      dst_g = alpha_blend[alpha][g][dst_g];
      dst_b = alpha_blend[alpha][b][dst_b];

      dst[x] = COLOR16(dst_r, dst_g, dst_b);
    }
    dst += screen_pitch;
  }
}

/******************************************************************************
 * ローカル関数の定義
 ******************************************************************************/
