/* unofficial gameplaySP kai
 *
 * Copyright (C) 2006 Exophase <exophase@gmail.com>
 * Copyright (C) 2007 takka <takka@tfact.net>
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

/******************************************************************************
 * draw.h
 * 基本描画の処理
 ******************************************************************************/
#ifndef DRAW_H
#define DRAW_H

/******************************************************************************
 * マクロ等の定義
 ******************************************************************************/
#define COLOR16(red, green, blue) ((blue << 10) | (green << 5) | red)
#define GET_R16(color) (color & 0x1f)
#define GET_G16(color) ((color >> 5) & 0x1f)
#define GET_B16(color) ((color >> 10)& 0x1f)

/******************************************************************************
 * グローバル変数の宣言
 ******************************************************************************/

/******************************************************************************
 * グローバル関数の宣言
 ******************************************************************************/
extern void boxfill(u32 sx, u32 sy, u32 ex, u32 ey, u32 color);
extern void boxfill_alpha(u32 sx, u32 sy, u32 ex, u32 ey, u32 color, u32 alpha);
extern void init_progress(int total, const char *text);
extern void update_progress(void);
extern void show_progress(const char *text);

#endif

