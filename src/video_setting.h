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

#ifndef VIDEO_SETTINGH
#define VIDEO_SETTINGH

#define GBA_SCREEN_WIDTH 240
#define GBA_SCREEN_HEIGHT 160

#define PSP_SCREEN_WIDTH 480
#define PSP_SCREEN_HEIGHT 272

// VIDEO OUTの有効画素は674x450(GBAの2.85倍)?
// インタレース時の有効画素は674x220?

const SCREEN_PARAMATER screen_paramater_psp_menu_init =
{
    /* 本体出力 / MENU画面 */
    { 0, 0x000, 480, 272, 1, 15, 0 },
    { 0, 1 },
    { 480, 480, 271 },
    { 9, 9 },
    { 480, 272 },
    { 0, 0, 480, 272 },
    { 0, 0, 0, 0, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

const SCREEN_PARAMATER screen_paramater_analog_menu_init[] =
{
  {
    /* アナログ出力 / MENU画面 / 4:3 / ノンインタレース*/
    { 2, 0x1D2, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 480, 480, 271 },
    { 9, 9 },
    { 720, 503 },
    { 0, 0, 480, 272 },
    { 0, 0, 0, 0, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* アナログ出力 / MENU画面 / 4:3 / インタレース*/
    { 2, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 480, 480, 271 },
    { 9, 9 },
    { 720, 503 },
    { 0, 0, 480, 272 },
    { 0, 0, 0, 0, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 0 },
    { 0, 0, 0, 262, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT + 262, 0 }
  },

  {
    /* アナログ出力 / MENU画面 / 16:9 / ノンインタレース*/
    { 2, 0x1D2, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 480, 480, 271 },
    { 9, 9 },
    { 720, 503 },
    { 0, 0, 480, 272 },
    { 0, 0, 0, 0, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* アナログ出力 / MENU画面 / 16:9 / インタレース*/
    { 2, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 480, 480, 271 },
    { 9, 9 },
    { 720, 503 },
    { 0, 0, 480, 272 },
    { 0, 0, 0, 0, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 0 },
    { 0, 0, 0, 262, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT + 262, 0 }
  },
};

const SCREEN_PARAMATER screen_paramater_digital_menu_init[] =
{
  {
    /* デジタル出力 / MENU画面 / 4:3 / ノンインタレース*/
    { 0, 0x1D2, 720, 480, 1, 15, 0 },
    { 0, 1 },
    { 480, 480, 271 },
    { 9, 9 },
    { 720, 503 },
    { 0, 0, 480, 272 },
    { 0, 0, 0, 0, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },
  {
    /* デジタル出力 / MENU画面 / 4:3 / インタレース*/
    { 0, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 480, 480, 271 },
    { 9, 9 },
    { 720, 503 },
    { 0, 0, 480, 272 },
    { 0, 0, 0, 0, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 0 },
    { 0, 0, 0, 262, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT + 262, 0 }
  },

  {
    /* デジタル出力 / MENU画面 / 16:9 / ノンインタレース*/
    { 0, 0x1D2, 720, 480, 1, 15, 0 },
    { 0, 1 },
    { 480, 480, 271 },
    { 9, 9 },
    { 720, 503 },
    { 0, 0, 480, 272 },
    { 0, 0, 0, 0, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* デジタル出力 / MENU画面 / 16:9 / インタレース*/
    { 0, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 480, 480, 271 },
    { 9, 9 },
    { 720, 503 },
    { 0, 0, 480, 272 },
    { 0, 0, 0, 0, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 0 },
    { 0, 0, 0, 262, 0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT + 262, 0 }
  },
};


const SCREEN_PARAMATER screen_paramater_psp_game_init[] =
{
  {
    /* 本体出力 / GAME画面 / unscaled */
    { 0, 0x000, 480, 272, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 480, 272 },
    { 0, 0, 480, 272 },
    { 0, 0, 120, 56, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH + 120, GBA_SCREEN_HEIGHT + 56, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* 本体出力 / GAME画面 / aspect */
    { 0, 0x000, 480, 272, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 480, 272 },
    { 0, 0, 480, 272 },
    { 0.25, 0.25, 36, 0, 0, GBA_SCREEN_WIDTH - 0.25, GBA_SCREEN_HEIGHT - 0.25, 408+36, PSP_SCREEN_HEIGHT, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* 本体出力 / GAME画面 / fullscreen */
    { 0, 0x000, 480, 272, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 480, 272 },
    { 0, 0, 480, 272 },
    { 0.25, 0.25, 0, 0, 0, GBA_SCREEN_WIDTH - 0.25, GBA_SCREEN_HEIGHT - 0.25, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* 本体出力 / GAME画面 / etc 1 */
    { 0, 0x000, 480, 272, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 480, 272 },
    { 0, 0, 480, 272 },
    { 0, 0, 120, 56, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH + 120, GBA_SCREEN_HEIGHT + 56, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* 本体出力 / GAME画面 / etc 2 */
    { 0, 0x000, 480, 272, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 480, 272 },
    { 0, 0, 480, 272 },
    { 0, 0, 120, 56, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH + 120, GBA_SCREEN_HEIGHT + 56, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },
};

const SCREEN_PARAMATER screen_paramater_analog_game_init[] =
{
  {
    /* アナログ出力 / GAME画面 / 4:3 / ノンインタレース / unscaled */
    { 2, 0x1D2, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 120, 40, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT + 40, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 4:3 / ノンインタレース / aspect */
    { 2, 0x1D2, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 20, 10, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 674 + 20, 220 + 10, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 4:3 / ノンインタレース / fullscreen */
    { 2, 0x1D2, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 0, 0, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 720, 240, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 4:3 / ノンインタレース / etc 1 */
    { 2, 0x1D2, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 120, 40, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT+40, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 4:3 / ノンインタレース / etc 2 */
    { 2, 0x1D2, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 120, 40, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT + 40, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 4:3 / インタレース / unscaled */
    { 2, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 120, 40, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT + 40, 0 },
    { 0, 0, 120, 40 + 262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT + 40 + 262, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 4:3 / インタレース / aspect */
    { 2, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 20, 10, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 674 + 20, 220 + 10, 0 },
    { 0, 0, 20, 10 + 262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 674 + 20, 220 + 10 + 262, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 4:3 / インタレース / fullscreen */
    { 2, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 0, 0, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 720, 240, 0 },
    { 0, 0, 0, 0+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 720, 240 + 262, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 4:3 / インタレース / etc 1 */
    { 2, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 120, 40, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT + 40, 0 },
    { 0, 0, 120, 40+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT + 40 + 262, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 4:3 / インタレース / etc 2 */
    { 2, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 120, 40, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT + 40, 0 },
    { 0, 0, 120, 40+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT + 40 + 262, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 16:9 / ノンインタレース / unscaled */
    { 2, 0x1D2, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 120, 40, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT+40, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 16:9 / ノンインタレース / aspect */
    { 2, 0x1D2, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 20, 10, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 674 + 20, 220+10, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 16:9 / ノンインタレース / fullscreen */
    { 2, 0x1D2, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 0, 0, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 720, 240, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 16:9 / ノンインタレース / etc 1 */
    { 2, 0x1D2, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 120, 40, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 16:9 / ノンインタレース / etc 2 */
    { 2, 0x1D2, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 120, 40, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* アナログ出力 / GAME画面 / 16:9 / インタレース / unscaled */
    { 2, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 503 },
    { 0, 0, 120, 40, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40, 0 },
    { 0, 0, 120, 40+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40+262, 0 },
  },

  {
    /* アナログ出力 / GAME画面 / 16:9 / インタレース / aspect */
    { 2, 0x1D1, 720, 503, 1, 15, 0},
    { 0, 1},
    { 240, 240, 160},
    { 8, 8},
    { 720, 503 },
    { 0, 0, 720, 503},
    { 0, 0, 20, 10, 0, GBA_SCREEN_WIDTH , GBA_SCREEN_HEIGHT , 674+20 , 220+10 , 0},
    { 0, 0, 20, 10+262, 0, GBA_SCREEN_WIDTH , GBA_SCREEN_HEIGHT , 674+20 , 220+10+262 , 0},
  },

  {
    /* アナログ出力 / GAME画面 / 16:9 / インタレース / fullscreen */
    { 2, 0x1D1, 720, 503, 1, 15, 0},
    { 0, 1},
    { 240, 240, 160},
    { 8, 8},
    { 720, 503 },
    { 0, 0, 720, 503},
    { 0, 0, 0, 0, 0, GBA_SCREEN_WIDTH , GBA_SCREEN_HEIGHT , 720 , 240 , 0},
    { 0, 0, 0, 0+262, 0, GBA_SCREEN_WIDTH , GBA_SCREEN_HEIGHT , 720 , 240+262 , 0},
  },

  {
    /* アナログ出力 / GAME画面 / 16:9 / インタレース / etc 1 */
    { 2, 0x1D1, 720, 503, 1, 15, 0},
    { 0, 1},
    { 240, 240, 160},
    { 8, 8},
    { 720, 503 },
    { 0, 0, 720, 503},
    { 0, 0, 120, 40, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT + 40, 0},
    { 0, 0, 120, 40 + 262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT + 40 + 262, 0},
  },

  {
    /* アナログ出力 / GAME画面 / 16:9 / インタレース / etc 2 */
    { 2, 0x1D1, 720, 503, 1, 15, 0},
    { 0, 1},
    { 240, 240, 160},
    { 8, 8},
    { 720, 503 },
    { 0, 0, 720, 503},
    { 0, 0, 120, 40, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT + 40, 0},
    { 0, 0, 120, 40 + 262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH * 2 + 120, GBA_SCREEN_HEIGHT + 40 + 262, 0},
  },
};

const SCREEN_PARAMATER screen_paramater_digital_game_init[] =
{
  {
    /* デジタル出力 / GAME画面 / 4:3 / ノンインタレース / unscaled */
    { 0, 0x1D2, 720, 480, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 240, 160, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* デジタル出力 / GAME画面 / 4:3 / ノンインタレース / aspect */
    { 0, 0x1D2, 720, 480, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 15, 10, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 675+15, 450+10, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* デジタル出力 / GAME画面 / 4:3 / ノンインタレース / fullscreen */
    { 0, 0x1D2, 720, 480, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 0, 0, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 720, 480, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* デジタル出力 / GAME画面 / 4:3 / ノンインタレース / etc 1 */
    { 0, 0x1D2, 720, 480, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 240, 160, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* デジタル出力 / GAME画面 / 4:3 / ノンインタレース / etc 2 */
    { 0, 0x1D2, 720, 480, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 240, 160, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* デジタル出力 / GAME画面 / 4:3 / インタレース / unscaled */
    { 0, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 240, 160, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160, 0 },
    { 0, 0, 240, 160+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160+262, 0 },
  },

  {
    /* デジタル出力 / GAME画面 / 4:3 / インタレース / aspect */
    { 0, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 15, 10, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 675+15, 450+10, 0 },
    { 0, 0, 15, 10+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 675+15, 450+10+262, 0 },
  },

  {
    /* デジタル出力 / GAME画面 / 4:3 / インタレース / fullscreen */
    { 0, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 0, 0, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 720, 480, 0 },
    { 0, 0, 0, 0+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 720, 480+262, 0 },
  },

  {
    /* デジタル出力 / GAME画面 / 4:3 / インタレース / etc 1 */
    { 0, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 240, 160, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160, 0 },
    { 0, 0, 240, 160+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160+262, 0 },
  },

  {
    /* デジタル出力 / GAME画面 / 4:3 / インタレース / etc 2 */
    { 0, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 240, 160, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160, 0 },
    { 0, 0, 240, 160+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160+262, 0 },
  },

  {
    /* デジタル出力 / GAME画面 / 16:9 / ノンインタレース / unscaled */
    { 0, 0x1D2, 720, 480, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 240, 160, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* デジタル出力 / GAME画面 / 16:9 / ノンインタレース / aspect */
    { 0, 0x1D2, 720, 480, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 15, 10, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 675+15, 450+10, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* デジタル出力 / GAME画面 / 16:9 / ノンインタレース / fullscreen */
    { 0, 0x1D2, 720, 480, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 0, 0, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 720, 480, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* デジタル出力 / GAME画面 / 16:9 / ノンインタレース / etc 1 */
    { 0, 0x1D2, 720, 480, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 240, 160, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* デジタル出力 / GAME画面 / 16:9 / ノンインタレース / etc 2 */
    { 0, 0x1D2, 720, 480, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 240, 160, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  },

  {
    /* デジタル出力 / GAME画面 / 16:9 / インタレース / unscaled */
    { 0, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1 },
    { 240, 240, 160 },
    { 8, 8 },
    { 720, 503 },
    { 0, 0, 720, 480 },
    { 0, 0, 240, 160, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160, 0 },
    { 0, 0, 240, 160+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160+262, 0 },
  },

  {
    /* デジタル出力 / GAME画面 / 16:9 / インタレース / aspect */
    { 0, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1},
    { 240, 240, 160},
    { 8, 8},
    { 720, 503 },
    { 0, 0, 720, 480},
    { 0, 0, 15, 10, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 675+15, 450+10, 0 },
    { 0, 0, 15, 10+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 675+15, 450+10+262, 0 },
  },

  {
    /* デジタル出力 / GAME画面 / 16:9 / インタレース / fullscreen */
    { 0, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1},
    { 240, 240, 160},
    { 8, 8},
    { 720, 503 },
    { 0, 0, 720, 480},
    { 0, 0, 0, 0, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 720, 480, 0 },
    { 0, 0, 0, 0+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, 720, 480+262, 0 },
  },

  {
    /* デジタル出力 / GAME画面 / 16:9 / インタレース / etc 1 */
    { 0, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1},
    { 240, 240, 160},
    { 8, 8},
    { 720, 503 },
    { 0, 0, 720, 480},
    { 0, 0, 240, 160, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160, 0 },
    { 0, 0, 240, 160+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160+262, 0 },
  },

  {
    /* デジタル出力 / GAME画面 / 16:9 / インタレース / etc 2 */
    { 0, 0x1D1, 720, 503, 1, 15, 0 },
    { 0, 1},
    { 240, 240, 160},
    { 8, 8},
    { 720, 503 },
    { 0, 0, 720, 480},
    { 0, 0, 240, 160, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160, 0 },
    { 0, 0, 240, 160+262, 0, GBA_SCREEN_WIDTH, GBA_SCREEN_HEIGHT, GBA_SCREEN_WIDTH+240, GBA_SCREEN_HEIGHT+160+262, 0 },
  },
};

#endif
