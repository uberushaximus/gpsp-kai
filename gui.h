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
 * gui.h
 * gui周りの処理
 ******************************************************************************/
#ifndef GUI_H
#define GUI_H

// 基本カラーの設定
#define COLOR_TEXT          COLOR16(31, 31, 31)
#define COLOR_ERROR         COLOR16(31,  0,  0)
#define COLOR_BG            COLOR16(2,  4,  10)
#define COLOR_ROM_INFO      COLOR16(22, 18, 26)
#define COLOR_ACTIVE_ITEM   COLOR16(31, 31, 31)
#define COLOR_INACTIVE_ITEM COLOR16(13, 20, 18)
#define COLOR_HELP_TEXT     COLOR16(16, 20, 24)

/******************************************************************************
 * グローバル変数の宣言
 ******************************************************************************/
extern u32 SAVESTATE_SLOT;
extern char DEFAULT_ROM_DIR[MAX_PATH];
extern char DEFAULT_SAVE_DIR[MAX_PATH];
extern char DEFAULT_CFG_DIR[MAX_PATH];
extern char DEFAULT_SS_DIR[MAX_PATH];
extern char DEFAULT_CHEAT_DIR[MAX_PATH];

/******************************************************************************
 * グローバル関数の宣言
 ******************************************************************************/
s32 load_file(char **wildcards, char *result, char *default_dir_name);
s32 load_game_config_file();
s32 load_config_file();
u32 menu(u16 *original_screen);

u32 load_dircfg(char *file_name);
u32 load_fontcfg(char *file_name);
u32 load_msgcfg(char *file_name);
u32 load_font();
void get_savestate_filename_noshot(u32 slot, char *name_buffer);

#endif

