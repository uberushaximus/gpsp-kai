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

#define MAX_GAMEPAD_CONFIG_MAP 16

typedef struct
{
  u32 screen_scale;
  u32 screen_filter;
  u32 screen_ratio;
  u32 screen_interlace;
  u32 enable_audio;
  u32 enable_analog;
  u32 analog_sensitivity_level;
  u32 enable_home;
  u32 gamepad_config_map[MAX_GAMEPAD_CONFIG_MAP];
  u32 language;
} GPSP_CONFIG_V10;

typedef struct
{
  u32 frameskip_type;
  u32 frameskip_value;
  u32 random_skip;
  u32 clock_speed_number;
  u32 audio_buffer_size_number;
  u32 update_backup_flag;
  CHEAT_TYPE cheats_flag[MAX_CHEATS];
  u32 gamepad_config_map[MAX_GAMEPAD_CONFIG_MAP];
  u32 use_default_gamepad_map;
  u32 allocate_sensor;
} GAME_CONFIG_V10;

/******************************************************************************
 * グローバル変数の宣言
 ******************************************************************************/
extern u32 SAVESTATE_SLOT;
extern char g_default_rom_dir[MAX_PATH];
extern char DEFAULT_SAVE_DIR[MAX_PATH];
extern char DEFAULT_CFG_DIR[MAX_PATH];
extern char DEFAULT_SS_DIR[MAX_PATH];
extern char DEFAULT_CHEAT_DIR[MAX_PATH];

extern GPSP_CONFIG_V10 gpsp_config;
extern GAME_CONFIG_V10 game_config;

/******************************************************************************
 * グローバル関数の宣言
 ******************************************************************************/
s32 load_file(char **wildcards, char *result, char *default_dir_name);
s32 load_game_config_file();
s32 load_config_file();
s32 save_game_config_file();
s32 save_config_file();

u32 menu(u16 *original_screen);

u32 load_dircfg(char *file_name);
u32 load_fontcfg(char *file_name);
u32 load_msgcfg(char *file_name);
u32 load_font();
void get_savestate_filename_noshot(u32 slot, char *name_buffer);
void init_gpsp_config();
void init_game_config();

#endif

