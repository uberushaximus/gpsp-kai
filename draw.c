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
#define progress_sx (screen_width2 - screen_width / 3)  // 中心から -160/-80
#define progress_ex (screen_width2 + screen_width / 3)  // 中心から +160/+80
#define progress_sy (screen_height2 + 3)                // 中心から+3
#define progress_ey (screen_height2 + 13)               // 中心から+13
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
void draw_dialog(u32 sx, u32 sy, u32 ex, u32 ey);

/******************************************************************************
 * グローバル関数の定義
 ******************************************************************************/
/*------------------------------------------------------
  文字列を描画 / センタリング処理
------------------------------------------------------*/
void print_string_center(u32 sy, u32 color, u32 bg_color, char *str)
{
  int width = fbm_getwidth(str);
  int sx = (screen_width - width) / 2;

  PRINT_STRING_BG(str, color, bg_color, sx, sy);
}

/*------------------------------------------------------
  文字列を描画 / 影付き / センタリング処理
------------------------------------------------------*/
void print_string_shadow_center(u32 sy, u32 color, char *str)
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
  ダイアログボックス表示
------------------------------------------------------*/
void draw_dialog(u32 sx, u32 sy, u32 ex, u32 ey)
{
// 影の表示
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
  yes/no ダイヤログボックス
--------------------------------------------------------*/

/*--------------------------------------------------------
  プログレスバー
--------------------------------------------------------*/
/*--------------------------------------------------------
  プログレスバー初期化
--------------------------------------------------------*/
void init_progress(u32 total, char *text)
{
  progress_current = 0;
  progress_total   = total;
  strcpy(progress_message, text);

  draw_dialog(progress_sx - 8, progress_sy -29, progress_ex + 8, progress_ey + 13);

  boxfill(progress_sx - 1, progress_sy - 1, progress_ex + 1, progress_ey + 1, 0);

  if (text[0] != '\0')
    print_string_center(progress_sy - 21, COLOR_PROGRESS_TEXT, COLOR_DIALOG, text);
}

/*--------------------------------------------------------
  プログレスバー更新
--------------------------------------------------------*/
void update_progress(void)
{
  int width = (int)( ((float)++progress_current / (float)progress_total) * ((float)screen_width / 3.0 * 2.0) );

  draw_dialog(progress_sx - 8, progress_sy -29, progress_ex + 8, progress_ey + 13);

  boxfill(progress_sx - 1, progress_sy - 1, progress_ex + 1, progress_ey + 1, COLOR_BLACK);
  if (progress_message[0] != '\0')
    print_string_center(progress_sy - 21, COLOR_PROGRESS_TEXT, COLOR_DIALOG, progress_message);
  boxfill(progress_sx, progress_sy, progress_sx+width, progress_ey, COLOR_PROGRESS_BAR);

  flip_screen();
}

/*--------------------------------------------------------
  プログレスバー結果表示
--------------------------------------------------------*/
void show_progress(char *text)
{
  draw_dialog(progress_sx - 8, progress_sy -29, progress_ex + 8, progress_ey + 13);

  boxfill(progress_sx - 1, progress_sy - 1, progress_ex + 1, progress_ey + 1, COLOR_BLACK);
    
  if (progress_current)
  {
    int width = (int)( (float)(++progress_current / progress_total) * (float)(screen_width / 3.0 * 2.0) );
    boxfill(progress_sx, progress_sy, progress_sx+width, progress_ey, COLOR_PROGRESS_BAR);
  }

  if (text[0] != '\0')
    print_string_center(progress_sy - 21, COLOR_PROGRESS_TEXT, COLOR_DIALOG,text);

  flip_screen();
  sceKernelDelayThread(progress_wait);
}

/*--------------------------------------------------------
  スクロールバー表示(メニュー画面のみ対応)
--------------------------------------------------------*/
#define SCROLLBAR_SX   3
#define SCROLLBAR_EX   6
#define SCROLLBAR_SY  23
#define SCROLLBAR_EY 256
#define SCROLLBAR_LEN (SCROLLBAR_EY - SCROLLBAR_SY)
#define SCROLLBAR_COLOR1 COLOR16( 0, 2, 8)
#define SCROLLBAR_COLOR2 COLOR16(15,15,15)
u32 old_scrollbar_sy = 0;
u32 old_scrollbar_ey = 0;

void scrollbar(u32 all,u32 view,u32 now)
{
  u32 scrollbar_sy;
  u32 scrollbar_ey;
  
  if ((all != 0) && (all > now))
    scrollbar_sy = (u32)((float)SCROLLBAR_LEN * (float)now / (float)all);
  else
    scrollbar_sy = 0;

  if ((all > (now + view)) && (all != 0))
    scrollbar_ey = (u32)((float)SCROLLBAR_LEN * (float)(now + view) / (float)all );
  else
    scrollbar_ey = SCROLLBAR_LEN;

  if ((scrollbar_sy != old_scrollbar_sy) || (scrollbar_ey != old_scrollbar_ey))
  {
    box(SCROLLBAR_SX - 1, SCROLLBAR_SY - 1, SCROLLBAR_EX + 1, SCROLLBAR_EY + 1, COLOR_BLACK);
    boxfill(SCROLLBAR_SX, SCROLLBAR_SY, SCROLLBAR_EX, SCROLLBAR_EY, SCROLLBAR_COLOR1);
    boxfill(SCROLLBAR_SX, SCROLLBAR_SY + scrollbar_sy, SCROLLBAR_EX, SCROLLBAR_SY + scrollbar_ey, SCROLLBAR_COLOR2);
  }

  old_scrollbar_sy = scrollbar_sy;
  old_scrollbar_ey = scrollbar_ey;
}

/******************************************************************************
 * ローカル関数の定義
 ******************************************************************************/
