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
 * gui.c
 * GUI周りの処理
 ******************************************************************************/

/******************************************************************************
 * ヘッダファイルの読込み
 ******************************************************************************/
#include "common.h"

/******************************************************************************
 * マクロ等の定義
 ******************************************************************************/
#define STATUS_ROWS 0
#define CURRENT_DIR_ROWS 1
#define FILE_LIST_ROWS 25
#define FILE_LIST_POSITION 10
#define DIR_LIST_POSITION 360
#define PAGE_SCROLL_NUM 5

#define GPSP_CONFIG_FILENAME "gpsp.cfg"

#define GAMEPAD_MENU_WIDTH 15

// gpSPの設定ファイルのヘッダー
// ヘッダーは4byteまで
#define GPSP_CONFIG_HEADER     "gpSP"
#define GPSP_CONFIG_HEADER_U32 0x50537067
const u32 gpsp_config_ver = 0x00010000;
gpsp_config_t g_gpsp_config;

// GAMEの設定ファイルのヘッダー
// ヘッダーは4byteまで
#define GAME_CONFIG_HEADER     "gcfg"
#define GAME_CONFIG_HEADER_U32 0x67666367
const u32 game_config_ver = 0x00010000;
game_config_t g_game_config;

#ifdef TEST_MODE
#define VER_RELEASE "test"
#else
#define VER_RELEASE "release"
#endif

#define MAKE_MENU(name, init_function, passive_function)                      \
  MENU_TYPE name##_menu =                                                     \
  {                                                                           \
    init_function,                                                            \
    passive_function,                                                         \
    name##_options,                                                           \
    sizeof(name##_options) / sizeof(MENU_OPTION_TYPE)                         \
  }                                                                           \

#define GAMEPAD_CONFIG_OPTION(display_string, number)                         \
{                                                                             \
  NULL,                                                                       \
  menu_fix_gamepad_help,                                                      \
  NULL,                                                                       \
  display_string,                                                             \
  gamepad_config_buttons,                                                     \
  gamepad_config_map + gamepad_config_line_to_button[number],                 \
  sizeof(gamepad_config_buttons) / sizeof(gamepad_config_buttons[0]),         \
  gamepad_help[gamepad_config_map[                                            \
   gamepad_config_line_to_button[number]]],                                   \
  number,                                                                     \
  STRING_SELECTION_TYPE                                                       \
}                                                                             \

#define ANALOG_CONFIG_OPTION(display_string, number)                          \
{                                                                             \
  NULL,                                                                       \
  menu_fix_gamepad_help,                                                      \
  NULL,                                                                       \
  display_string,                                                             \
  gamepad_config_buttons,                                                     \
  gamepad_config_map + number + 12,                                           \
  sizeof(gamepad_config_buttons) / sizeof(gamepad_config_buttons[0]),         \
  gamepad_help[gamepad_config_map[number + 12]],                              \
  number + 2,                                                                 \
  STRING_SELECTION_TYPE                                                     \
}                                                                             \

#define CHEAT_OPTION(number)                                                  \
{                                                                             \
  NULL,                                                                       \
  NULL,                                                                       \
  NULL,                                                                       \
  cheat_format_str[number],                                                   \
  enable_disable_options,                                                     \
  &(g_game_config.cheats_flag[number].cheat_active),                                 \
  2,                                                                          \
  msg[MSG_CHEAT_MENU_HELP_0],                                                 \
  (number) % 10,                                                              \
  STRING_SELECTION_TYPE                                                     \
}                                                                             \

#define ACTION_OPTION(action_function, passive_function, display_string,      \
 help_string, line_number)                                                    \
{                                                                             \
  action_function,                                                            \
  passive_function,                                                           \
  NULL,                                                                       \
  display_string,                                                             \
  NULL,                                                                       \
  NULL,                                                                       \
  0,                                                                          \
  help_string,                                                                \
  line_number,                                                                \
  ACTION_TYPE                                                               \
}                                                                             \

#define SUBMENU_OPTION(sub_menu, display_string, help_string, line_number)    \
{                                                                             \
  NULL,                                                                       \
  NULL,                                                                       \
  sub_menu,                                                                   \
  display_string,                                                             \
  NULL,                                                                       \
  NULL,                                                                       \
  sizeof(sub_menu) / sizeof(MENU_OPTION_TYPE),                                \
  help_string,                                                                \
  line_number,                                                                \
  SUBMENU_TYPE                                                              \
}                                                                             \

#define SELECTION_OPTION(passive_function, display_string, options,           \
 option_ptr, num_options, help_string, line_number, type)                     \
{                                                                             \
  NULL,                                                                       \
  passive_function,                                                           \
  NULL,                                                                       \
  display_string,                                                             \
  options,                                                                    \
  option_ptr,                                                                 \
  num_options,                                                                \
  help_string,                                                                \
  line_number,                                                                \
  type                                                                        \
}                                                                             \

#define ACTION_SELECTION_OPTION(action_function, passive_function,            \
 display_string, options, option_ptr, num_options, help_string, line_number,  \
 type)                                                                        \
{                                                                             \
  action_function,                                                            \
  passive_function,                                                           \
  NULL,                                                                       \
  display_string,                                                             \
  options,                                                                    \
  option_ptr,                                                                 \
  num_options,                                                                \
  help_string,                                                                \
  line_number,                                                                \
  type | ACTION_TYPE                                                        \
}                                                                             \

#define STRING_SELECTION_OPTION(passive_function, display_string, options,    \
 option_ptr, num_options, help_string, line_number)                           \
  SELECTION_OPTION(passive_function, display_string, options,                 \
   option_ptr, num_options, help_string, line_number, STRING_SELECTION_TYPE)\

#define NUMERIC_SELECTION_OPTION(passive_function, display_string,            \
 option_ptr, num_options, help_string, line_number)                           \
  SELECTION_OPTION(passive_function, display_string, NULL, option_ptr,        \
   num_options, help_string, line_number, NUMBER_SELECTION_TYPE)            \

#define STRING_SELECTION_ACTION_OPTION(action_function, passive_function,     \
 display_string, options, option_ptr, num_options, help_string, line_number)  \
  ACTION_SELECTION_OPTION(action_function, passive_function,                  \
   display_string,  options, option_ptr, num_options, help_string,            \
   line_number, STRING_SELECTION_TYPE)                                      \

#define NUMERIC_SELECTION_ACTION_OPTION(action_function, passive_function,    \
 display_string, option_ptr, num_options, help_string, line_number)           \
  ACTION_SELECTION_OPTION(action_function, passive_function,                  \
   display_string,  NULL, option_ptr, num_options, help_string,               \
   line_number, NUMBER_SELECTION_TYPE)                                      \

#define NUMERIC_SELECTION_HIDE_OPTION(action_function,                 \
 passive_function, display_string, option_ptr, num_options, help_string,      \
 line_number)                                                                 \
  ACTION_SELECTION_OPTION(action_function, passive_function,                  \
   display_string, NULL, option_ptr, num_options, help_string,                \
   line_number, NUMBER_SELECTION_TYPE)                                      \

typedef enum
{
  NUMBER_SELECTION_TYPE = 0x01,
  STRING_SELECTION_TYPE = 0x02,
  SUBMENU_TYPE          = 0x04,
  ACTION_TYPE           = 0x08
} MENU_OPTION_TYPE_ENUM;

struct _MENU_OPTION_TYPE
{
  void (* action_function)();
  void (* passive_function)();
  struct _MENU_TYPE *sub_menu;
  char *display_string;
  void *options;
  u32 *current_option;
  u32 num_options;
  char *help_string;
  u32 line_number;
  MENU_OPTION_TYPE_ENUM option_type;
};

struct _MENU_TYPE
{
  void (* init_function)();
  void (* passive_function)();
  struct _MENU_OPTION_TYPE *options;
  u32 num_options;
};

typedef struct _MENU_OPTION_TYPE MENU_OPTION_TYPE;
typedef struct _MENU_TYPE MENU_TYPE;


typedef enum
{
  MAIN_MENU,
  GAMEPAD_MENU,
  SAVESTATE_MENU,
  FRAMESKIP_MENU,
  CHEAT_MENU,
  ADHOC_MENU,
  MISC_MENU
} MENU_ENUM;

/******************************************************************************
 * グローバル変数の定義
 ******************************************************************************/
char g_default_rom_dir[MAX_PATH];
char g_default_save_dir[MAX_PATH];
char g_default_cfg_dir[MAX_PATH];
char g_default_ss_dir[MAX_PATH];
char g_default_cheat_dir[MAX_PATH];
u32 g_savestate_slot_num = 0;

/******************************************************************************
 * ローカル変数の定義
 ******************************************************************************/
static char m_font[MAX_PATH];
static char s_font[MAX_PATH];
static u32 menu_cheat_page = 0;
static u32 gamepad_config_line_to_button[] = { 8, 6, 7, 9, 1, 2, 3, 0, 4, 5, 11, 10 };

/******************************************************************************
 * ローカル関数の宣言
 ******************************************************************************/
static void get_savestate_snapshot(char *savestate_filename, u32 slot_num);
static void get_savestate_filename(u32 slot, char *name_buffer);
static int sort_function(const void *dest_str_ptr, const void *src_str_ptr);
static u32 parse_line(char *current_line, char *current_str);
static void print_status(u32 mode);
static void get_timestamp_string(char *buffer, u16 msg_id, u16 year, u16 mon, u16 day, u16 wday, u16 hour, u16 min, u16 sec, u32 msec);
static void save_ss_bmp(u16 *image);
void _flush_cache();
void button_up_wait();
void init_gpsp_config();

static int sort_function(const void *dest_str_ptr, const void *src_str_ptr)
{
  char *dest_str = *((char **)dest_str_ptr);
  char *src_str = *((char **)src_str_ptr);

  if(src_str[0] == '.')
    return 1;

  if(dest_str[0] == '.')
    return -1;

  return strcasecmp(dest_str, src_str);
}

/******************************************************************************
 * グローバル関数の定義
 ******************************************************************************/
/*--------------------------------------------------------
  汎用ファイル読込み
--------------------------------------------------------*/
s32 load_file(char **wildcards, char *result,char *default_dir_name)
{
  DIR *current_dir;
  struct dirent *current_file;
  struct stat file_info;
  char current_dir_name[MAX_PATH];
  char current_dir_short[81];
  u32 current_dir_length;
  u32 total_filenames_allocated;
  u32 total_dirnames_allocated;
  char **file_list;
  char **dir_list;
  u32 num_files;                      // カレントディレクトリのファイル数
  u32 num_dirs;                       // カレントディレクトリのフォルダ数
  char *file_name;
  u32 file_name_length;
  s32 ext_pos = -1;
  u32 chosen_file, chosen_dir;
  s32 return_value = 1;
  u32 current_file_selection;         // 選択しているファイル
  u32 current_file_scroll_value;      // スクロールの位置
  u32 current_dir_selection;
  u32 current_dir_scroll_value;
  u32 current_file_in_scroll;
  u32 current_dir_in_scroll;
  u32 current_file_number, current_dir_number;
  u32 current_column = 0;
  u32 repeat;
  u32 i;
  gui_action_type gui_action;
  char utf8[2048];

  if (default_dir_name != NULL)
    chdir(default_dir_name);

  while(return_value == 1)
  {
    current_file_selection = 0;
    current_file_scroll_value = 0;
    current_dir_selection = 0;
    current_dir_scroll_value = 0;
    current_file_in_scroll = 0;
    current_dir_in_scroll = 0;

    total_filenames_allocated = 32;
    total_dirnames_allocated = 32;
    file_list = (char **)malloc(sizeof(char *) * 32);
    dir_list = (char **)malloc(sizeof(char *) * 32);
    memset(file_list, 0, sizeof(char *) * 32);
    memset(dir_list, 0, sizeof(char *) * 32);

    num_files = 0;
    num_dirs = 0;
    chosen_file = 0;
    chosen_dir = 0;

    getcwd(current_dir_name, MAX_PATH);
    current_dir = opendir(current_dir_name);

    do
    {
      if(current_dir)
        current_file = readdir(current_dir);
      else
        current_file = NULL;

      if(current_file)
      {
        file_name = current_file->d_name;
        file_name_length = strlen(file_name);

        if((stat(file_name, &file_info) >= 0) &&
         ((file_name[0] != '.') || (file_name[1] == '.')))
        {
          if(S_ISDIR(file_info.st_mode))
          {
            dir_list[num_dirs] =
             (char *)malloc(file_name_length + 1);
             sprintf(dir_list[num_dirs], "%s", file_name);

            num_dirs++;
          }
          else
          {
            // Must match one of the wildcards, also ignore the .
            if(file_name_length >= 4)
            {
              if(file_name[file_name_length - 4] == '.')
                ext_pos = file_name_length - 4;
              else

              if(file_name[file_name_length - 3] == '.')
                ext_pos = file_name_length - 3;

              else
                ext_pos = 0;

              for(i = 0; wildcards[i] != NULL; i++)
              {
                if(!strcasecmp((file_name + ext_pos),
                 wildcards[i]))
                {
                  file_list[num_files] =
                   (char *)malloc(file_name_length + 1);

                  sprintf(file_list[num_files], "%s", file_name);

                  num_files++;
                  break;
                }
              }
            }
          }
        }

        if(num_files == total_filenames_allocated)
        {
          file_list = (char **)realloc(file_list, sizeof(char *) *
           total_filenames_allocated * 2);
          memset(file_list + total_filenames_allocated, 0,
           sizeof(u8 *) * total_filenames_allocated);
          total_filenames_allocated *= 2;
        }

        if(num_dirs == total_dirnames_allocated)
        {
          dir_list = (char **)realloc(dir_list, sizeof(char *) *
           total_dirnames_allocated * 2);
          memset(dir_list + total_dirnames_allocated, 0,
           sizeof(char *) * total_dirnames_allocated);
          total_dirnames_allocated *= 2;
        }
      }
    } while(current_file);

    qsort((void *)file_list, num_files, sizeof(u8 *), sort_function);
    qsort((void *)dir_list, num_dirs, sizeof(u8 *), sort_function);

    closedir(current_dir);

    current_dir_length = strlen(current_dir_name);

    if(current_dir_length > 80)
    {
      memcpy(current_dir_short, "...", 3);
      memcpy(current_dir_short + 3,
       current_dir_name + current_dir_length - 77, 77);
      current_dir_short[80] = 0;
    }
    else
    {
      memcpy(current_dir_short, current_dir_name,
       current_dir_length + 1);
    }

    repeat = 1;

    if(num_files == 0)
      current_column = 1;

  clear_screen(COLOR_BG);
  flip_screen();
  clear_screen(COLOR_BG);
  {
    while(repeat)
    {
      gui_action = get_gui_input();

      switch(gui_action)
      {
        case CURSOR_DOWN:
          if(current_column == 0)
          {
            if(current_file_selection < (num_files - 1))
            {
              current_file_selection++;
              if(current_file_in_scroll == (FILE_LIST_ROWS - CURRENT_DIR_ROWS - 1))
              {
                clear_screen(COLOR_BG);
                current_file_scroll_value++;
              }
              else
              {
                current_file_in_scroll++;
              }
            }
          }
          else
          {
            if(current_dir_selection < (num_dirs - 1))
            {
              current_dir_selection++;
              if(current_dir_in_scroll == (FILE_LIST_ROWS - CURRENT_DIR_ROWS - 1))
              {
                clear_screen(COLOR_BG);
                current_dir_scroll_value++;
              }
              else
              {
                current_dir_in_scroll++;
              }
            }
          }

          break;

        case CURSOR_RTRIGGER:
          if(current_column == 0)
          {
            if(num_files > PAGE_SCROLL_NUM)
            {
              if(current_file_selection < (num_files - PAGE_SCROLL_NUM))
              {
                current_file_selection += PAGE_SCROLL_NUM;
                if(current_file_in_scroll >= (FILE_LIST_ROWS - CURRENT_DIR_ROWS - PAGE_SCROLL_NUM))
                {
                  clear_screen(COLOR_BG);
                  current_file_scroll_value += PAGE_SCROLL_NUM;
                }
                else
                {
                  current_file_in_scroll += PAGE_SCROLL_NUM;
                }
              }
            }
          }
          else
          {
            if(num_dirs > PAGE_SCROLL_NUM)
            {
              if(current_dir_selection < (num_dirs - PAGE_SCROLL_NUM))
              {
                current_dir_selection += PAGE_SCROLL_NUM;
                if(current_dir_in_scroll >= (FILE_LIST_ROWS - CURRENT_DIR_ROWS - PAGE_SCROLL_NUM))
                {
                  clear_screen(COLOR_BG);
                  current_dir_scroll_value += PAGE_SCROLL_NUM;
                }
                else
                {
                  current_dir_in_scroll += PAGE_SCROLL_NUM;
                }
              }
            }
          }
          break;

        case CURSOR_UP:
          if(current_column == 0)
          {
            if(current_file_selection)
            {
              current_file_selection--;
              if(current_file_in_scroll == 0)
              {
                clear_screen(COLOR_BG);
                current_file_scroll_value--;
              }
              else
              {
                current_file_in_scroll--;
              }
            }
          }
          else
          {
            if(current_dir_selection)
            {
              current_dir_selection--;
              if(current_dir_in_scroll == 0)
              {
                clear_screen(COLOR_BG);
                current_dir_scroll_value--;
              }
              else
              {
                current_dir_in_scroll--;
              }
            }
          }
          break;

        case CURSOR_LTRIGGER:
          if(current_column == 0)
          {
            if(current_file_selection >= PAGE_SCROLL_NUM)
            {
              current_file_selection -= PAGE_SCROLL_NUM;
              if(current_file_in_scroll < PAGE_SCROLL_NUM)
              {
                clear_screen(COLOR_BG);
                current_file_scroll_value -= PAGE_SCROLL_NUM;
              }
              else
              {
                current_file_in_scroll -= PAGE_SCROLL_NUM;
              }
            }
          }
          else
          {
            if(current_dir_selection >= PAGE_SCROLL_NUM)
            {
              current_dir_selection -= PAGE_SCROLL_NUM;
              if(current_dir_in_scroll < PAGE_SCROLL_NUM)
              {
                clear_screen(COLOR_BG);
                current_dir_scroll_value -= PAGE_SCROLL_NUM;
              }
              else
              {
                current_dir_in_scroll -= PAGE_SCROLL_NUM;
              }
            }
          }
          break;

        case CURSOR_RIGHT:
          if(current_column == 0)
          {
            if(num_dirs != 0)
              current_column = 1;
          }
          break;

        case CURSOR_LEFT:
          if(current_column == 1)
          {
            if(num_files != 0)
              current_column = 0;
          }
          break;

        case CURSOR_SELECT:
          if(current_column == 1)
          {
            repeat = 0;
            chdir(dir_list[current_dir_selection]);
          }
          else
          {
            if(num_files != 0)
            {
              repeat = 0;
              return_value = 0;
              strcpy(result, file_list[current_file_selection]);
              // ROMのフルパスを取得
              getcwd(rom_path, MAX_PATH);
            }
          }
          break;

        case CURSOR_BACK:

          if(!strcmp(current_dir_name, "ms0:/PSP"))
            break;

          repeat = 0;
          chdir("..");
          break;

        case CURSOR_EXIT:
          return_value = -1;
          repeat = 0;
          break;

        default:
          ;
          break;
      }
      print_status(1);
      PRINT_STRING_BG_SJIS(utf8, current_dir_short, COLOR_ACTIVE_ITEM, COLOR_BG, 0, (CURRENT_DIR_ROWS * 10));
      PRINT_STRING_BG(msg[MSG_RETURN_MENU], COLOR_HELP_TEXT, COLOR_BG, 20, 260);

      for(i = 0, current_file_number = i + current_file_scroll_value;
       i < (FILE_LIST_ROWS - CURRENT_DIR_ROWS); i++, current_file_number++)
      {
        if(current_file_number < num_files)
        {
          if((current_file_number == current_file_selection) &&
           (current_column == 0))
          {
            PRINT_STRING_BG_SJIS(utf8, file_list[current_file_number], COLOR_ACTIVE_ITEM,
             COLOR_BG, FILE_LIST_POSITION, ((i + CURRENT_DIR_ROWS + 1) * 10));
          }
          else
          {
            PRINT_STRING_BG_SJIS(utf8, file_list[current_file_number], COLOR_INACTIVE_ITEM,
             COLOR_BG, FILE_LIST_POSITION, ((i + CURRENT_DIR_ROWS + 1) * 10));
          }
        }
      }

      for(i = 0, current_dir_number = i + current_dir_scroll_value;
       i < (FILE_LIST_ROWS - CURRENT_DIR_ROWS); i++, current_dir_number++)
      {
        if(current_dir_number < num_dirs)
        {
          if((current_dir_number == current_dir_selection) &&
           (current_column == 1))
          {
            PRINT_STRING_BG_SJIS(utf8, dir_list[current_dir_number], COLOR_ACTIVE_ITEM,
             COLOR_BG, DIR_LIST_POSITION, ((i + CURRENT_DIR_ROWS + 1) * 10));
          }
          else
          {
            PRINT_STRING_BG_SJIS(utf8, dir_list[current_dir_number], COLOR_INACTIVE_ITEM,
             COLOR_BG, DIR_LIST_POSITION, ((i + CURRENT_DIR_ROWS + 1) * 10));
          }
        }
      }
      scrollbar(2, 22, 7, 257, num_files, FILE_LIST_ROWS, current_file_scroll_value);

      flip_screen();
      clear_screen(COLOR_BG);
    }
  }
    for(i = 0; i < num_files; i++)
    {
      free(file_list[i]);
    }
    free(file_list);

    for(i = 0; i < num_dirs; i++)
    {
      free(dir_list[i]);
    }
    free(dir_list);
  }

  chdir(main_path);

  clear_screen(COLOR_BG);
  flip_screen();
  return return_value;
}

//標準のキーマップ
const u32 gamepad_config_map_init[MAX_GAMEPAD_CONFIG_MAP] =
{
    BUTTON_ID_MENU,     /* △ */
    BUTTON_ID_A,        /* ○ */
    BUTTON_ID_B,        /* × */
    BUTTON_ID_FPS,      /* □ */
    BUTTON_ID_L,        /* [L] */
    BUTTON_ID_R,        /* [R] */
    BUTTON_ID_DOWN,     /* ↓ */
    BUTTON_ID_LEFT,     /* ← */
    BUTTON_ID_UP,       /* ↑ */
    BUTTON_ID_RIGHT,    /* → */
    BUTTON_ID_SELECT,   /* [SELECT] */
    BUTTON_ID_START,    /* [START] */
    BUTTON_ID_UP,       /* (↑) */
    BUTTON_ID_DOWN,     /* (↓) */
    BUTTON_ID_LEFT,     /* (←) */
    BUTTON_ID_RIGHT     /* (→) */
};

/*--------------------------------------------------------
  game cfgの初期化
--------------------------------------------------------*/
void init_game_config()
{
  u32 i;
  g_game_config.frameskip_type = auto_frameskip;
  g_game_config.frameskip_value = 9;
  g_game_config.random_skip = NO;
  g_game_config.clock_speed_number = 9;
  g_game_config.audio_buffer_size_number = 2;
  g_game_config.update_backup_flag = NO;
  for(i = 0; i < MAX_CHEATS; i++)
  {
    g_game_config.cheats_flag[i].cheat_active = NO;
    g_game_config.cheats_flag[i].cheat_name[0] = '\0';
  }
  memcpy(g_game_config.gamepad_config_map, gamepad_config_map_init, sizeof(gamepad_config_map_init));
  g_game_config.use_default_gamepad_map = YES;
  g_game_config.allocate_sensor = NO;
}

/*--------------------------------------------------------
  gpSP cfgの初期化
--------------------------------------------------------*/
void init_gpsp_config()
{
  int temp;
  g_gpsp_config.screen_mode = scaled_aspect;
  g_gpsp_config.screen_scale = 100;
  g_gpsp_config.screen_filter = filter_bilinear;
  g_gpsp_config.screen_ratio = R4_3;
  g_gpsp_config.screen_interlace = PROGRESSIVE;
  g_gpsp_config.enable_audio = YES;
  g_gpsp_config.enable_analog = YES;
  g_gpsp_config.analog_sensitivity_level = 4;
  g_gpsp_config.enable_home = NO;
  memcpy(g_gpsp_config.gamepad_config_map, gamepad_config_map_init, sizeof(gamepad_config_map_init));
  memcpy(gamepad_config_map, g_gpsp_config.gamepad_config_map, sizeof(g_gpsp_config.gamepad_config_map));
  temp = g_gpsp_config.language;
  sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &temp);
  g_gpsp_config.language = temp;
  g_gpsp_config.emulate_core = ASM_CORE;
  g_gpsp_config.debug_flag = NO;
  g_gpsp_config.fake_fat = NO;
  g_gpsp_config.boot_mode = GAME_CART;
#if 0
  g_gpsp_config.solar_level = 0;
#endif
}

/*--------------------------------------------------------
  game cfgファイルの読込み
  3/4修正
--------------------------------------------------------*/
void load_game_config_file(void)
{
  char game_config_filename[MAX_FILE];
  char game_config_path[MAX_PATH];
  FILE_ID game_config_file;
  u32 header, ver;

  // 初期値の設定
  init_game_config();

  change_ext(gamepak_filename, game_config_filename, ".cfg");

  if (g_default_cfg_dir != NULL)
    sprintf(game_config_path, "%s/%s", g_default_cfg_dir, game_config_filename);
  else
    strcpy(game_config_path, game_config_filename);

  FILE_OPEN(game_config_file, game_config_path, READ);

  if(FILE_CHECK_VALID(game_config_file))
  {
    // ヘッダーのチェック
    FILE_READ_VARIABLE(game_config_file, header);
    if(header == GAME_CONFIG_HEADER_U32)
    {
      // バージョンのチェック
      FILE_READ_VARIABLE(game_config_file, ver);
      switch(ver)
      {
        case 0x10000: /* 1.0 */
          FILE_READ_VARIABLE(game_config_file, g_game_config);
          if(g_game_config.use_default_gamepad_map == YES)
            memcpy(gamepad_config_map, g_gpsp_config.gamepad_config_map, sizeof(g_gpsp_config.gamepad_config_map));
          else
            memcpy(gamepad_config_map, g_game_config.gamepad_config_map, sizeof(g_game_config.gamepad_config_map));
          break;
      }
    }
  }
  FILE_CLOSE(game_config_file);
}

/*--------------------------------------------------------
  gpSP cfgファイルの読込み
--------------------------------------------------------*/
s32 load_config_file()
{
  char gpsp_config_path[MAX_PATH];
  FILE_ID gpsp_config_file;
  u32 header, ver;

  // 初期値の設定
  init_gpsp_config();
  sprintf(gpsp_config_path, "%s/%s", main_path, GPSP_CONFIG_FILENAME);

  FILE_OPEN(gpsp_config_file, gpsp_config_path, READ);

  if(FILE_CHECK_VALID(gpsp_config_file))
  {
    // ヘッダーのチェック
    FILE_READ_VARIABLE(gpsp_config_file, header);
    if(header != GPSP_CONFIG_HEADER_U32)
    {
      FILE_CLOSE(gpsp_config_file);
      return -1;
    }
    FILE_READ_VARIABLE(gpsp_config_file, ver);
    switch(ver)
    {
      case 0x10000: /* 1.0 */
        FILE_READ_VARIABLE(gpsp_config_file, g_gpsp_config);
        break;
    }
    FILE_CLOSE(gpsp_config_file);
    memcpy(gamepad_config_map, g_gpsp_config.gamepad_config_map, sizeof(g_gpsp_config.gamepad_config_map));
    return 0;
  }
  FILE_CLOSE(gpsp_config_file);
  return -1;
}

/*--------------------------------------------------------
  メニューの表示
--------------------------------------------------------*/
u32 menu(u16 *original_screen)
{
  gui_action_type gui_action;
  u32 i;
  u32 repeat = 1;
  u32 return_value = 0;
  u32 first_load = 0;
  char current_savestate_filename[MAX_FILE];
  char line_buffer[256];
  char cheat_format_str[MAX_CHEATS][41*4];

  MENU_TYPE *current_menu;
  MENU_OPTION_TYPE *current_option;
  MENU_OPTION_TYPE *display_option;
  u32 current_option_num;

  auto void choose_menu();
  auto void menu_exit();
  auto void menu_quit();
  auto void menu_load();
  auto void menu_restart();
  auto void menu_save_ss();
  auto void menu_change_state();
  auto void menu_save_state();
  auto void menu_load_state();
  auto void menu_load_state_file();
  auto void menu_load_cheat_file();
  auto void menu_fix_gamepad_help();
  auto void submenu_graphics_sound();
  auto void submenu_cheats();
  auto void submenu_misc();
  auto void submenu_gamepad();
  auto void submenu_analog();
  auto void submenu_savestate();
  auto void submenu_main();
  auto void reload_cheats_page();
  auto void home_mode();
  auto void set_gamepad();
#ifdef USE_ADHOC
  auto void adhoc_connect_menu();
  auto void adhoc_disconnect_menu();
#endif

  char *gamepad_help[] =
  {
    msg[MSG_PAD_MENU_CFG_HELP_0],
    msg[MSG_PAD_MENU_CFG_HELP_1],
    msg[MSG_PAD_MENU_CFG_HELP_2],
    msg[MSG_PAD_MENU_CFG_HELP_3],
    msg[MSG_PAD_MENU_CFG_HELP_4],
    msg[MSG_PAD_MENU_CFG_HELP_5],
    msg[MSG_PAD_MENU_CFG_HELP_6],
    msg[MSG_PAD_MENU_CFG_HELP_7],
    msg[MSG_PAD_MENU_CFG_HELP_8],
    msg[MSG_PAD_MENU_CFG_HELP_9],
    msg[MSG_PAD_MENU_CFG_HELP_10],
    msg[MSG_PAD_MENU_CFG_HELP_11],
    msg[MSG_PAD_MENU_CFG_HELP_12],
    msg[MSG_PAD_MENU_CFG_HELP_13],
    msg[MSG_PAD_MENU_CFG_HELP_14],
    msg[MSG_PAD_MENU_CFG_HELP_15],
    msg[MSG_PAD_MENU_CFG_HELP_16],
    msg[MSG_PAD_MENU_CFG_HELP_17],
    msg[MSG_PAD_MENU_CFG_HELP_18],
    msg[MSG_PAD_MENU_CFG_HELP_19],
    msg[MSG_PAD_MENU_CFG_HELP_20],
    msg[MSG_PAD_MENU_CFG_HELP_21]
  };

  //ローカル関数の定義
  void menu_exit()
  {
    if(!first_load)
      repeat = 0;
  }

  void menu_quit()
  {
    quit(0);
  }

  void menu_load()
  {
    char *file_ext[] = { ".gba", ".bin", ".zip", NULL };
    char load_filename[MAX_FILE];

    save_config_file();
    save_game_config_file();

//    if(!g_gpsp_config.update_backup_flag) // TODO タイミングを検討
      update_backup_force();

    if(load_file(file_ext, load_filename, g_default_rom_dir) != -1)
    {
       if(load_gamepak(load_filename) == -1)
       {
         quit(0);
       }
       reset_gba();
       return_value = 1;
       repeat = 0;
       reg[CHANGED_PC_STATUS] = 1;
    }
    else
    {
      choose_menu(current_menu);
    }
  }

  void menu_restart()
  {
    if(!first_load)
    {
      reset_gba();
      reg[CHANGED_PC_STATUS] = 1;
      return_value = 1;
      repeat = 0;
    }
  }

  void menu_save_ss()
  {
    if(!first_load)
      save_ss_bmp(original_screen);
  }

  void menu_change_state()
  {
    get_savestate_filename(g_savestate_slot_num, current_savestate_filename);
  }

  void menu_save_state()
  {
    menu_change_state();
    if(!first_load)
    {
      get_savestate_filename_noshot(g_savestate_slot_num, current_savestate_filename);
      save_state(current_savestate_filename, original_screen, g_savestate_slot_num);
      pause_sound(1);
      clear_screen(COLOR_BG);
      blit_to_screen(original_screen, 240, 160, 230, 40);
    }
  }

  void menu_load_state()
  {
    menu_change_state();
    if(!first_load)
    {
      if (load_state(current_savestate_filename, g_savestate_slot_num) == 1)
      {
        return_value = 1;
        repeat = 0;
      }
      else
      {
        clear_screen(COLOR_BG);
        blit_to_screen(original_screen, 240, 160, 230, 40);
      }
    }
  }

  void menu_load_state_file()
  {
    char *file_ext[] = { ".svs", NULL };
    char load_filename[512];
    if(load_file(file_ext, load_filename, g_default_save_dir) != -1)
    {
      if (load_state(load_filename, g_savestate_slot_num) == 1)
      {
        return_value = 1;
        repeat = 0;
      }
      else
      {
        clear_screen(COLOR_BG);
        choose_menu(current_menu);
      }
    }
    else
    {
      choose_menu(current_menu);
    }
  }

  // メニュー:チートファイルのロード
  void menu_load_cheat_file()
  {
    char *file_ext[] = { ".cht", NULL };
    char load_filename[MAX_FILE];
    u32 i;

    if(load_file(file_ext, load_filename, g_default_cheat_dir) != -1)
    {
      add_cheats(load_filename);
      for(i = 0; i < MAX_CHEATS; i++)
      {
        if(i >= g_cheats_num)
        {
          sprintf(cheat_format_str[i], msg[MSG_CHEAT_MENU_NON_LOAD], i);
        }
        else
        {
          sprintf(cheat_format_str[i], msg[MSG_CHEAT_MENU_0], i, g_game_config.cheats_flag[i].cheat_name);
        }
      }
      choose_menu(current_menu);

    }
    else
    {
      choose_menu(current_menu);
    }
  }

  void menu_fix_gamepad_help()
  {
    current_option->help_string =
     gamepad_help[gamepad_config_map[gamepad_config_line_to_button[current_option_num]]];
  }

  void submenu_graphics_sound()
  {

  }

  void submenu_cheats()
  {

  }

  void submenu_misc()
  {

  }

  void submenu_gamepad()
  {

  }

#ifdef USE_ADHOC
  void submenu_adhoc()
  {

  }

  void adhoc_connect_menu()
  {
    set_cpu_clock(g_game_config.clock_speed_number);
    adhoc_init(gamepak_title);
    adhoc_select();
  }

  void adhoc_disconnect_menu()
  {
    adhoc_exit();
  }

#endif

  void submenu_analog()
  {

  }

  void submenu_savestate()
  {
    PRINT_STRING_BG(msg[MSG_STATE_MENU_TITLE], COLOR_ACTIVE_ITEM, COLOR_BG, 10, 70);
    menu_change_state();
  }

  void submenu_main()
  {
    get_savestate_filename_noshot(g_savestate_slot_num, current_savestate_filename);
  }

  char *yes_no_options[] =
  {
    msg[MSG_NO],
    msg[MSG_YES]
  };

  char *enable_disable_options[] =
  {
    msg[MSG_DISABLED],
    msg[MSG_ENABLED]
  };

  char *scale_options[] =
  {
    msg[MSG_SCALE_UNSCALED],
    msg[MSG_SCALE_SCALED],
    msg[MSG_SCALE_FULL],
    msg[MSG_SCN_OPT1],
    msg[MSG_SCN_OPT2]
  };

  char *frameskip_options[] = { msg[MSG_FS_AUTO], msg[MSG_FS_MANUAL], msg[MSG_FS_OFF] };
  char *frameskip_variation_options[] = { msg[MSG_FS_UNIFORM], msg[MSG_FS_RANDOM] };

  char *audio_buffer_options[] =
  {
    msg[MSG_SB_2048], msg[MSG_SB_3072], msg[MSG_SB_4096], msg[MSG_SB_5120], msg[MSG_SB_6144],
    msg[MSG_SB_7168], msg[MSG_SB_8192], msg[MSG_SB_9216], msg[MSG_SB_10240], msg[MSG_SB_11264], msg[MSG_SB_12288]
  };

  char *update_backup_options[] = { msg[MSG_BK_EXITONLY], msg[MSG_BK_AUTO] };

  char *clock_speed_options[] =
  {
    msg[MSG_CLK_33], msg[MSG_CLK_66], msg[MSG_CLK_100], msg[MSG_CLK_133], msg[MSG_CLK_166],
    msg[MSG_CLK_200], msg[MSG_CLK_233], msg[MSG_CLK_266], msg[MSG_CLK_300], msg[MSG_CLK_333]
  };

  char *gamepad_config_buttons[] =
  {
    msg[MSG_PAD_MENU_CFG_0],
    msg[MSG_PAD_MENU_CFG_1],
    msg[MSG_PAD_MENU_CFG_2],
    msg[MSG_PAD_MENU_CFG_3],
    msg[MSG_PAD_MENU_CFG_4],
    msg[MSG_PAD_MENU_CFG_5],
    msg[MSG_PAD_MENU_CFG_6],
    msg[MSG_PAD_MENU_CFG_7],
    msg[MSG_PAD_MENU_CFG_8],
    msg[MSG_PAD_MENU_CFG_9],
    msg[MSG_PAD_MENU_CFG_10],
    msg[MSG_PAD_MENU_CFG_11],
    msg[MSG_PAD_MENU_CFG_12],
    msg[MSG_PAD_MENU_CFG_13],
    msg[MSG_PAD_MENU_CFG_14],
    msg[MSG_PAD_MENU_CFG_15],
    msg[MSG_PAD_MENU_CFG_16],
    msg[MSG_PAD_MENU_CFG_17],
    msg[MSG_PAD_MENU_CFG_18],
    msg[MSG_PAD_MENU_CFG_19],
    msg[MSG_PAD_MENU_CFG_20],
    msg[MSG_PAD_MENU_CFG_21],
  };

  char *ratio_options[] = { msg[MSG_R_4_3], msg[MSG_R_16_9] };

  char *interlace_options[] = { msg[MSG_I_NON], msg[MSG_I_ON] };

  char *language_options[] =
  {
    lang[0],
    lang[1],
    lang[2],
    lang[3],
    lang[4],
    lang[5],
    lang[6],
    lang[7],
    lang[8],
    lang[9],
    lang[10],
    lang[11]
  };

  char *boot_mode_options[] =
  {
      "GAME CART",
      "GBA BIOS"
  };

#ifdef USE_C_CORE
  char *emulate_core_options[] =
  {
    "ASM CORE",
    "C CORE"
  };
#endif

  /*--------------------------------------------------------
     グラフィック・サウンド オプション
  --------------------------------------------------------*/
  MENU_OPTION_TYPE graphics_sound_options[] =
  {
    /* 00 */ STRING_SELECTION_OPTION(NULL, msg[MSG_SCALE], scale_options, &g_gpsp_config.screen_mode, 5, msg[MSG_G_S_MENU_HELP_0], 0),
    /* 01 */ STRING_SELECTION_OPTION(NULL, msg[MSG_FILTER], yes_no_options, &g_gpsp_config.screen_filter, 2, msg[MSG_G_S_MENU_HELP_1], 1),
    /* 02 */ STRING_SELECTION_OPTION(NULL, msg[MSG_RATIO], ratio_options, &g_gpsp_config.screen_ratio, 2, msg[MSG_G_S_MENU_HELP_9], 2),
    /* 03 */ STRING_SELECTION_OPTION(NULL, msg[MSG_INTERLACE], interlace_options, &g_gpsp_config.screen_interlace, 2, msg[MSG_G_S_MENU_HELP_10], 3),
    /* 04 */
    /* 05 */ STRING_SELECTION_OPTION(NULL, msg[MSG_SKIP_TYPE], frameskip_options, &g_game_config.frameskip_type, 3, msg[MSG_G_S_MENU_HELP_2], 5),
    /* 06 */ NUMERIC_SELECTION_OPTION(NULL, msg[MSG_SKIP_VALUE], &g_game_config.frameskip_value, 100, msg[MSG_G_S_MENU_HELP_3], 6),
    /* 07 */ STRING_SELECTION_OPTION(NULL, msg[MSG_SKIP_RANDOM], frameskip_variation_options, &g_game_config.random_skip, 2, msg[MSG_G_S_MENU_HELP_4], 7),
    /* 08 */
    /* 09 */ STRING_SELECTION_OPTION(NULL, msg[MSG_AUDIO_ENABLE], yes_no_options, &g_gpsp_config.enable_audio, 2, msg[MSG_G_S_MENU_HELP_5], 9),
    /* 10 */ STRING_SELECTION_OPTION(NULL, msg[MSG_AUDIO_BUFFER], audio_buffer_options, &g_game_config.audio_buffer_size_number, 11, msg[MSG_G_S_MENU_HELP_6], 11),
    /* 11 */
    /* 12 */ ACTION_OPTION(menu_save_ss, NULL, msg[MSG_SCREEN_SHOT], msg[MSG_G_S_MENU_HELP_7], 12),
    /* 13 */
    /* 14 */NUMERIC_SELECTION_OPTION(NULL, "ratio %d", &g_gpsp_config.screen_scale, 201, "ratio", 14),
    /* 15 */
    /* 16 */ SUBMENU_OPTION(NULL, msg[MSG_MENU_RETURN_MAIN], msg[MSG_G_S_MENU_HELP_8], 16)
  };

  MAKE_MENU(graphics_sound, submenu_graphics_sound, NULL);

  /*--------------------------------------------------------
     チート オプション
  --------------------------------------------------------*/
  MENU_OPTION_TYPE cheats_options[] =
  {
    /* 00 */ CHEAT_OPTION((10 * menu_cheat_page) + 0),
    /* 01 */ CHEAT_OPTION((10 * menu_cheat_page) + 1),
    /* 02 */ CHEAT_OPTION((10 * menu_cheat_page) + 2),
    /* 03 */ CHEAT_OPTION((10 * menu_cheat_page) + 3),
    /* 04 */ CHEAT_OPTION((10 * menu_cheat_page) + 4),
    /* 05 */ CHEAT_OPTION((10 * menu_cheat_page) + 5),
    /* 06 */ CHEAT_OPTION((10 * menu_cheat_page) + 6),
    /* 07 */ CHEAT_OPTION((10 * menu_cheat_page) + 7),
    /* 08 */ CHEAT_OPTION((10 * menu_cheat_page) + 8),
    /* 09 */ CHEAT_OPTION((10 * menu_cheat_page) + 9),
    /* 10 */ NUMERIC_SELECTION_OPTION(reload_cheats_page, msg[MSG_CHEAT_MENU_5], &menu_cheat_page, MAX_CHEATS_PAGE, msg[MSG_CHEAT_MENU_HELP_5], 10),
    /* 11 */ ACTION_OPTION(menu_load_cheat_file, NULL, msg[MSG_CHEAT_MENU_1], msg[MSG_CHEAT_MENU_HELP_1], 11),
    /* 12 */
    /* 13 */
    /* 14 */
    /* 15 */
    /* 16 */ SUBMENU_OPTION(NULL, msg[MSG_CHEAT_MENU_4], msg[MSG_CHEAT_MENU_HELP_4], 16)
  };

  MAKE_MENU(cheats, submenu_cheats, NULL);

  /*--------------------------------------------------------
     その他 オプション
  --------------------------------------------------------*/
  MENU_OPTION_TYPE misc_options[] =
  {
    STRING_SELECTION_OPTION(NULL, msg[MSG_CHEAT_MENU_2], clock_speed_options, &g_game_config.clock_speed_number, 10, msg[MSG_CHEAT_MENU_HELP_2], 0),
    STRING_SELECTION_OPTION(NULL, msg[MSG_CHEAT_MENU_3], update_backup_options, &g_game_config.update_backup_flag, 2, msg[MSG_CHEAT_MENU_HELP_3], 1),
    STRING_SELECTION_OPTION(home_mode, msg[MSG_CHEAT_MENU_6], yes_no_options, &g_gpsp_config.enable_home, 2, msg[MSG_CHEAT_MENU_HELP_6], 2),

#ifdef USE_C_CORE
    STRING_SELECTION_OPTION(NULL, "Emulate Core : %s", emulate_core_options, &g_gpsp_config.emulate_core , 2, "Emulate Core", 3),
#endif

#ifdef USE_DEBUG
    STRING_SELECTION_OPTION(NULL, "Debug mode : %s", yes_no_options, &g_gpsp_config.debug_flag, 2, "Debug mode", 4),
#endif

    STRING_SELECTION_OPTION(NULL, "言語 : %s", language_options, &g_gpsp_config.language, 12, "言語", 5), /* TODO */
    STRING_SELECTION_OPTION(NULL, "Fake Fat : %s", yes_no_options, &g_gpsp_config.fake_fat, 2, "Fake Fat", 6), /* TODO */
    STRING_SELECTION_OPTION(NULL, "Boot Mode : %s", boot_mode_options, &g_gpsp_config.boot_mode, 2, "Boot Mode", 8), /* TODO */
#if 0
    NUMERIC_SELECTION_OPTION(NULL, "周囲の明るさ : %d", &g_gpsp_config.solar_level, 25, "周囲の明るさ", 8), /* TODO */
#endif
    SUBMENU_OPTION(NULL, msg[MSG_CHEAT_MENU_4], msg[MSG_CHEAT_MENU_HELP_4], 16)
  };

  MAKE_MENU(misc, submenu_misc, NULL);

  /*--------------------------------------------------------
     セーブステート オプション
  --------------------------------------------------------*/
  MENU_OPTION_TYPE savestate_options[] =
  {
    NUMERIC_SELECTION_HIDE_OPTION(menu_load_state, menu_change_state, msg[MSG_STATE_MENU_0], &g_savestate_slot_num, 11, msg[MSG_STATE_MENU_HELP_0], 6),
    NUMERIC_SELECTION_HIDE_OPTION(menu_save_state, menu_change_state, msg[MSG_STATE_MENU_1], &g_savestate_slot_num, 11, msg[MSG_STATE_MENU_HELP_1], 7),

    NUMERIC_SELECTION_HIDE_OPTION(menu_load_state_file, menu_change_state, msg[MSG_STATE_MENU_2], &g_savestate_slot_num, 11, msg[MSG_STATE_MENU_HELP_2], 9),

    NUMERIC_SELECTION_OPTION(menu_change_state, msg[MSG_STATE_MENU_3], &g_savestate_slot_num, 11, msg[MSG_STATE_MENU_HELP_3], 11),



    SUBMENU_OPTION(NULL, msg[MSG_STATE_MENU_4], msg[MSG_STATE_MENU_HELP_4], 15)
  };

  MAKE_MENU(savestate, submenu_savestate, NULL);

  /*--------------------------------------------------------
     ゲームパッド オプション
  --------------------------------------------------------*/
  MENU_OPTION_TYPE gamepad_config_options[] =
  {
    GAMEPAD_CONFIG_OPTION(msg[MSG_PAD_MENU_0], 0),
    GAMEPAD_CONFIG_OPTION(msg[MSG_PAD_MENU_1], 1),
    GAMEPAD_CONFIG_OPTION(msg[MSG_PAD_MENU_2], 2),
    GAMEPAD_CONFIG_OPTION(msg[MSG_PAD_MENU_3], 3),
    GAMEPAD_CONFIG_OPTION(msg[MSG_PAD_MENU_4], 4),
    GAMEPAD_CONFIG_OPTION(msg[MSG_PAD_MENU_5], 5),
    GAMEPAD_CONFIG_OPTION(msg[MSG_PAD_MENU_6], 6),
    GAMEPAD_CONFIG_OPTION(msg[MSG_PAD_MENU_7], 7),
    GAMEPAD_CONFIG_OPTION(msg[MSG_PAD_MENU_8], 8),
    GAMEPAD_CONFIG_OPTION(msg[MSG_PAD_MENU_9], 9),
    GAMEPAD_CONFIG_OPTION(msg[MSG_PAD_MENU_10], 10),
    GAMEPAD_CONFIG_OPTION(msg[MSG_PAD_MENU_11], 11),

    STRING_SELECTION_OPTION(set_gamepad, msg[MSG_PAD_MENU_13], yes_no_options, &g_game_config.use_default_gamepad_map, 2, msg[MSG_PAD_MENU_HELP_13], 13),

    SUBMENU_OPTION(NULL, msg[MSG_PAD_MENU_12], msg[MSG_PAD_MENU_HELP_12], 15)
  };

  MAKE_MENU(gamepad_config, submenu_gamepad, NULL);

  /*--------------------------------------------------------
     アナログパッド オプション
  --------------------------------------------------------*/
  MENU_OPTION_TYPE analog_config_options[] =
  {
    ANALOG_CONFIG_OPTION(msg[MSG_A_PAD_MENU_0], 0),
    ANALOG_CONFIG_OPTION(msg[MSG_A_PAD_MENU_1], 1),
    ANALOG_CONFIG_OPTION(msg[MSG_A_PAD_MENU_2], 2),
    ANALOG_CONFIG_OPTION(msg[MSG_A_PAD_MENU_3], 3),



    STRING_SELECTION_OPTION(NULL, msg[MSG_A_PAD_MENU_4], yes_no_options, &g_gpsp_config.enable_analog, 2, msg[MSG_A_PAD_MENU_HELP_0], 7),
    NUMERIC_SELECTION_OPTION(NULL, msg[MSG_A_PAD_MENU_5], &g_gpsp_config.analog_sensitivity_level, 10, msg[MSG_A_PAD_MENU_HELP_1], 8),
    STRING_SELECTION_OPTION(NULL, "tilt sensor : %s", yes_no_options, &g_game_config.allocate_sensor, 2, "tilt sensor", 9), /* TODO */
    STRING_SELECTION_OPTION(set_gamepad, msg[MSG_PAD_MENU_13], yes_no_options, &g_game_config.use_default_gamepad_map, 2, msg[MSG_PAD_MENU_HELP_13], 10),



    SUBMENU_OPTION(NULL, msg[MSG_A_PAD_MENU_6], msg[MSG_A_PAD_MENU_HELP_2], 15)
  };

  MAKE_MENU(analog_config, submenu_analog, NULL);

#ifdef USE_ADHOC
  /*--------------------------------------------------------
     ADHOC オプション
  --------------------------------------------------------*/
  MENU_OPTION_TYPE adhoc_options[] =
  {
    ACTION_OPTION(adhoc_connect_menu, NULL, "adhoc connect", "adhoc connect", 0),                                                      /*  0行目 */
    ACTION_OPTION(adhoc_disconnect_menu, NULL, "adhoc disconnect", "adhoc disconnect", 1),                                                      /*  0行目 */














    SUBMENU_OPTION(NULL, "exit", "exit", 15)
  };

  MAKE_MENU(adhoc, submenu_adhoc, NULL);
#endif

  /*--------------------------------------------------------
     メイン オプション
  --------------------------------------------------------*/
  MENU_OPTION_TYPE main_options[] =
  {
    SUBMENU_OPTION(&graphics_sound_menu, msg[MSG_MAIN_MENU_0], msg[MSG_MAIN_MENU_HELP_0], 0),                                         /*  0行目 */

    NUMERIC_SELECTION_ACTION_OPTION(menu_load_state, NULL, msg[MSG_MAIN_MENU_1], &g_savestate_slot_num, 11, msg[MSG_MAIN_MENU_HELP_1], 2),  /*  2行目 */
    NUMERIC_SELECTION_ACTION_OPTION(menu_save_state, NULL, msg[MSG_MAIN_MENU_2], &g_savestate_slot_num, 11, msg[MSG_MAIN_MENU_HELP_2], 3),  /*  3行目 */
    SUBMENU_OPTION(&savestate_menu, msg[MSG_MAIN_MENU_3], msg[MSG_MAIN_MENU_HELP_3], 4),                                              /*  4行目 */

    SUBMENU_OPTION(&gamepad_config_menu, msg[MSG_MAIN_MENU_4], msg[MSG_MAIN_MENU_HELP_4], 6),                                         /*  6行目 */
    SUBMENU_OPTION(&analog_config_menu, msg[MSG_MAIN_MENU_5], msg[MSG_MAIN_MENU_HELP_5], 7),                                          /*  7行目 */
#ifdef USE_ADHOC
    SUBMENU_OPTION(&adhoc_menu, "adhoc", "adhoc", 8),                                                                                 /*  8行目 */
#endif
    SUBMENU_OPTION(&cheats_menu, msg[MSG_MAIN_MENU_6], msg[MSG_MAIN_MENU_HELP_6], 9),                                            /*  9行目 */
    SUBMENU_OPTION(&misc_menu, "misc", "misc", 10),                                                                                   /* 10行目 */
    ACTION_OPTION(menu_load, NULL, msg[MSG_MAIN_MENU_7], msg[MSG_MAIN_MENU_HELP_7], 11),                                              /* 11行目 */
    ACTION_OPTION(menu_restart, NULL, msg[MSG_MAIN_MENU_8], msg[MSG_MAIN_MENU_HELP_8], 12),                                           /* 12行目 */
    ACTION_OPTION(menu_exit, NULL, msg[MSG_MAIN_MENU_9], msg[MSG_MAIN_MENU_HELP_9], 13),                                              /* 13行目 */

    ACTION_OPTION(menu_quit, NULL, msg[MSG_MAIN_MENU_10], msg[MSG_MAIN_MENU_HELP_10], 15)                                             /* 15行目 */
  };

  MAKE_MENU(main, submenu_main, NULL);

  void choose_menu(MENU_TYPE *new_menu)
  {
    if(new_menu == NULL)
      new_menu = &main_menu;

    clear_screen(COLOR_BG);
    blit_to_screen(original_screen, 240, 160, 230, 40);

    current_menu = new_menu;

    current_option = new_menu->options;
    current_option_num = 0;
    if(current_menu->init_function)
     current_menu->init_function();
  }

  void reload_cheats_page()
  {
    for(i = 0; i<10; i++)
    {
      cheats_options[i].display_string = cheat_format_str[(10 * menu_cheat_page) + i];
      cheats_options[i].current_option = &(g_game_config.cheats_flag[(10 * menu_cheat_page) + i].cheat_active);
    }
  }

  void home_mode()
  {
    sceImposeSetHomePopup(g_gpsp_config.enable_home);
  }

  void set_gamepad()
  {
    if(g_game_config.use_default_gamepad_map == 1)
    {
      memcpy(g_game_config.gamepad_config_map, gamepad_config_map, sizeof(g_game_config.gamepad_config_map));
      memcpy(gamepad_config_map, g_gpsp_config.gamepad_config_map, sizeof(g_gpsp_config.gamepad_config_map));
    }
    else
    {
      memcpy(g_gpsp_config.gamepad_config_map, gamepad_config_map, sizeof(g_gpsp_config.gamepad_config_map));
      memcpy(gamepad_config_map, g_game_config.gamepad_config_map, sizeof(g_game_config.gamepad_config_map));
    }
  }

  // ローカル関数の定義 終了

  // ここからメニューの処理

  pause_sound(1);

  button_up_wait();

  video_resolution(FRAME_MENU);

  clear_screen(COLOR_BG);

  // MENU時は222MHz
  set_cpu_clock(10);

  if(gamepak_filename[0] == 0)
  {
    first_load = 1;
    memset(original_screen, 0x00, 240 * 160 * 2);
    PRINT_STRING_EXT_BG(msg[MSG_NON_LOAD_GAME], 0xFFFF, 0x0000, 60, 75, original_screen, 240);
  }

  choose_menu(&main_menu);
  current_menu = &main_menu;

  for(i = 0; i < MAX_CHEATS; i++)
  {
    if(i >= g_cheats_num)
    {
      sprintf(cheat_format_str[i], msg[MSG_CHEAT_MENU_NON_LOAD], i);
    }
    else
    {
      sprintf(cheat_format_str[i], msg[MSG_CHEAT_MENU_0], i, g_game_config.cheats_flag[i].cheat_name);
    }
  }

  reload_cheats_page();

  current_menu->init_function();

  // メニューのメインループ
  while(repeat)
  {

    print_status(0);
    blit_to_screen(original_screen, 240, 160, 230, 40);

    display_option = current_menu->options;

    for(i = 0; i < current_menu->num_options; i++, display_option++)
    {
      if(display_option->option_type & NUMBER_SELECTION_TYPE)
      {
        sprintf(line_buffer, display_option->display_string,
         *(display_option->current_option));
      }
      else

      if(display_option->option_type & STRING_SELECTION_TYPE)
      {
        sprintf(line_buffer, display_option->display_string,
         ((u32 *)display_option->options)[*(display_option->current_option)]);
      }
      else
      {
        strcpy(line_buffer, display_option->display_string);
      }

      if(display_option == current_option)
      {
        PRINT_STRING_BG(line_buffer, COLOR_ACTIVE_ITEM, COLOR_BG, 10,
         (display_option->line_number * 10) + 40);
      }
      else
      {
        PRINT_STRING_BG(line_buffer, COLOR_INACTIVE_ITEM, COLOR_BG, 10,
         (display_option->line_number * 10) + 40);
      }
    }

    PRINT_STRING_BG(current_option->help_string, COLOR_HELP_TEXT, COLOR_BG, 30, 210);

    gui_action = get_gui_input();

//    if (quit_flag == 1)
//      menu_quit();

    switch(gui_action)
    {
      case CURSOR_DOWN:
        current_option_num = (current_option_num + 1) %
          current_menu->num_options;

        current_option = current_menu->options + current_option_num;
//        clear_help();
        break;

      case CURSOR_UP:
        if(current_option_num)
          current_option_num--;
        else
          current_option_num = current_menu->num_options - 1;

        current_option = current_menu->options + current_option_num;
//        clear_help();
        break;

      case CURSOR_RIGHT:
        if(current_option->option_type & (NUMBER_SELECTION_TYPE |
         STRING_SELECTION_TYPE))
        {
          *(current_option->current_option) =
           (*current_option->current_option + 1) %
           current_option->num_options;

          if(current_option->passive_function)
            current_option->passive_function();
        }
        break;

      case CURSOR_LEFT:
        if(current_option->option_type & (NUMBER_SELECTION_TYPE |
         STRING_SELECTION_TYPE))
        {
          u32 current_option_val = *(current_option->current_option);

          if(current_option_val)
            current_option_val--;
          else
            current_option_val = current_option->num_options - 1;

          *(current_option->current_option) = current_option_val;

          if(current_option->passive_function)
            current_option->passive_function();
        }
        break;

      case CURSOR_EXIT:
        if(current_menu == &main_menu)
          menu_exit();

        choose_menu(&main_menu);
        break;

      case CURSOR_SELECT:
        if(current_option->option_type & ACTION_TYPE)
          current_option->action_function();

        else if(current_option->option_type & SUBMENU_TYPE)
          choose_menu(current_option->sub_menu);
        break;

      case KEY_SELECT:
        break;

      default:
        ;
        break;
    }  // end switch
    if(current_menu->init_function)
     current_menu->init_function();
    flip_screen();
    clear_screen(COLOR_BG);
  }  // end while

// menu終了時の処理
  button_up_wait();

  clear_screen(0);
  flip_screen();
  clear_screen(0);
  flip_screen();

  video_resolution(FRAME_GAME);

  set_cpu_clock(g_game_config.clock_speed_number);

  pause_sound(0);
  real_frame_count = 0;
  virtual_frame_count = 0;
  return return_value;
}

void button_up_wait()
{
  SceCtrlData ctrl_data;
  sceKernelDelayThread(10);
  while(sceCtrlPeekBufferPositive(&ctrl_data, 1), ((ctrl_data.Buttons | readHomeButton()) & 0x1F3F9) != 0)
  {
    sceKernelDelayThread(10);
  }
}


u32 load_dircfg(char *file_name)  // TODO:スマートな実装に書き直す
{
  int loop;
  int next_line;
  char current_line[256];
  char current_str[256];
  FILE *msg_file;
  char msg_path[MAX_PATH];

  sprintf(msg_path, "%s/%s", main_path, file_name);

  msg_file = fopen(msg_path, "r");

  next_line = 0;
  if(msg_file)
  {
    loop = 0;
    while(fgets(current_line, 256, msg_file))
    {
      if(parse_line(current_line, current_str) != -1)
      {
        switch(loop)
        {
          case 0:
            if(opendir(current_str) != NULL)
              strcpy(g_default_rom_dir,current_str);
            else
            {
              g_default_rom_dir[0] = '\0';
              pspDebugScreenInit();
              printf("not open rom dir : %s\n",current_str);
              sceKernelDelayThread(500000*2);
              init_video();
              video_resolution(FRAME_MENU);
            }
            loop++;
            break;

          case 1:
            if(opendir(current_str) != NULL)
              strcpy(g_default_save_dir,current_str);
            else
            {
              g_default_save_dir[0] = '\0';
              pspDebugScreenInit();
              printf("not open save dir : %s\n",current_str);
              sceKernelDelayThread(500000*2);
              init_video();
              video_resolution(FRAME_MENU);
            }
            loop++;
            break;

          case 2:
            if(opendir(current_str) != NULL)
              strcpy(g_default_cfg_dir,current_str);
            else
            {
              g_default_cfg_dir[0] = '\0';
              pspDebugScreenInit();
              printf("not open cfg dir : %s\n",current_str);
              sceKernelDelayThread(500000*2);
              init_video();
              video_resolution(FRAME_MENU);
            }
            loop++;
            break;

          case 3:
            if(opendir(current_str) != NULL)
              strcpy(g_default_ss_dir,current_str);
            else
            {
              g_default_ss_dir[0] = '\0';
              pspDebugScreenInit();
              printf("not open screen shot dir : %s\n",current_str);
              sceKernelDelayThread(500000*2);
              init_video();
              video_resolution(FRAME_MENU);
            }
            loop++;
            break;

          case 4:
            if(opendir(current_str) != NULL)
              strcpy(g_default_cheat_dir,current_str);
            else
            {
              g_default_cheat_dir[0] = '\0';
              pspDebugScreenInit();
              printf("not open cheat dir : %s\n",current_str);
              sceKernelDelayThread(500000*2);
              init_video();
              video_resolution(FRAME_MENU);
            }
            loop++;
            break;
        }
      }
    }

    fclose(msg_file);
    if (loop == 5)
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }
  fclose(msg_file);
  return -1;
}

u32 load_fontcfg(char *file_name)  // TODO:スマートな実装に書き直す
{
  int loop;
  int next_line;
  char current_line[256];
  char current_str[256];
  FILE *msg_file;
  char msg_path[MAX_PATH];

  sprintf(msg_path, "%s/%s", main_path, file_name);

  m_font[0] = '\0';
  s_font[0] = '\0';

  msg_file = fopen(msg_path, "rb");

  next_line = 0;
  if(msg_file)
  {
    loop = 0;
    while(fgets(current_line, 256, msg_file))
    {
      if(parse_line(current_line, current_str) != -1)
      {
        switch(loop)
        {
          case 0:
            strcpy(m_font, current_str);
            loop++;
            break;
          case 1:
            strcpy(s_font, current_str);
            loop++;
            break;
        }
      }
    }

    fclose(msg_file);
    if (loop > 0)
      return 0;
    else
      return -1;
  }
  fclose(msg_file);
  return -1;
}

u32 load_msgcfg(char *file_name)
{
  int loop;
  int next_line;
  char current_line[256];
  char current_str[256];
  FILE *msg_file;
  char msg_path[MAX_PATH];
  u32 offset;

  sprintf(msg_path, "%s/%s", main_path, file_name);

  msg_file = fopen(msg_path, "rb");

  next_line = 0;
  offset = 0;
  if(msg_file)
  {
    loop = 0;
    while(fgets(current_line, 256, msg_file))
    {
      if(parse_line(current_line, current_str) != -1)
      {
        if (loop <= (MSG_END + 1 + next_line)) {
          if (next_line == 0)
          {
            // 新しい行の場合
            msg[loop] = &msg_data[offset];  // 新しい行
            next_line = 1;
            loop++;
          }
          strcpy(&msg_data[offset], current_str);
          offset = offset + strlen(current_str);  // offset はNULLの位置を示す
        }
      }
      else
      {
        next_line = 0;
        offset++;
      }
    }

    fclose(msg_file);
    if (loop == (MSG_END))
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }
  fclose(msg_file);
  return -1;
}

/*--------------------------------------------------------
  フォントの読込
--------------------------------------------------------*/
u32 load_font()
{
    return fbm_init(m_font,s_font);
}

void get_savestate_filename_noshot(u32 slot, char *name_buffer)
{
  char savestate_ext[16];

  sprintf(savestate_ext, "_%d.svs", (int)slot);
  change_ext(gamepak_filename, name_buffer, savestate_ext);
}

/*--------------------------------------------------------
  game cfgファイルの書込
--------------------------------------------------------*/
s32 save_game_config_file()
{
  char game_config_filename[MAX_FILE];
  char game_config_path[MAX_PATH];
  FILE_ID game_config_file;

  if(gamepak_filename[0] == 0) return -1;

  change_ext(gamepak_filename, game_config_filename, ".cfg");

  if (g_default_cfg_dir != NULL)
    sprintf(game_config_path, "%s/%s", g_default_cfg_dir, game_config_filename);
  else
    strcpy(game_config_path, game_config_filename);

  if(g_game_config.use_default_gamepad_map == 0)
    memcpy(g_game_config.gamepad_config_map, gamepad_config_map, sizeof(g_game_config.gamepad_config_map));

  FILE_OPEN(game_config_file, game_config_path, WRITE);
  if(FILE_CHECK_VALID(game_config_file))
  {
    FILE_WRITE(game_config_file, (int *)GAME_CONFIG_HEADER, sizeof(u32));
    FILE_WRITE_VARIABLE(game_config_file, game_config_ver);
    FILE_WRITE_VARIABLE(game_config_file, g_game_config);
    FILE_CLOSE(game_config_file);
    return 0;
  }
  FILE_CLOSE(game_config_file);
  return -1;
}

/*--------------------------------------------------------
  gpSP cfgファイルの書込
--------------------------------------------------------*/
s32 save_config_file()
{
  char gpsp_config_path[MAX_PATH];
  FILE_ID gpsp_config_file;

  sprintf(gpsp_config_path, "%s/%s", main_path, GPSP_CONFIG_FILENAME);

  save_game_config_file();

  if(g_game_config.use_default_gamepad_map == 1)
    memcpy(g_gpsp_config.gamepad_config_map, gamepad_config_map, sizeof(g_gpsp_config.gamepad_config_map));

  FILE_OPEN(gpsp_config_file, gpsp_config_path, WRITE);
  if(FILE_CHECK_VALID(gpsp_config_file))
  {
    FILE_WRITE(gpsp_config_file, (int *)GPSP_CONFIG_HEADER, sizeof(u32));
    FILE_WRITE_VARIABLE(gpsp_config_file, gpsp_config_ver);
    FILE_WRITE_VARIABLE(gpsp_config_file, g_gpsp_config);
    FILE_CLOSE(gpsp_config_file);
    return 0;
  }
  FILE_CLOSE(gpsp_config_file);
  return -1;
}

/******************************************************************************
 * ローカル関数の定義
 ******************************************************************************/
static void get_savestate_snapshot(char *savestate_filename, u32 slot_num)
  {
    static u16 snapshot_buffer[240 * 160];
    char savestate_timestamp_string[80];
    char savestate_path[MAX_PATH];
    static char old_savestate_path[MAX_PATH];
    FILE_ID savestate_file;
    static u64 savestate_time_flat;
    u64 local_time;
    int wday;
    pspTime current_time;
    static u32 valid_flag;

    if (g_default_save_dir != NULL) {
      sprintf(savestate_path, "%s/%s", g_default_save_dir, savestate_filename);
    }
    else
      {
        strcpy(savestate_path, savestate_filename);
      }

    if (slot_num != MEM_STATE_NUM)
    {
      if(strcmp(savestate_path, old_savestate_path) != 0)
      {
        FILE_OPEN(savestate_file, savestate_path, READ);
        strcpy(old_savestate_path, savestate_path);
        if(FILE_CHECK_VALID(savestate_file))
        {
          char str[4];
          FILE_READ_ARRAY(savestate_file, str);
          if(memcmp(str, g_state_str, 4) == 0)
            FILE_SEEK(savestate_file, 8, SEEK_SET);
          else
            FILE_SEEK(savestate_file, 0, SEEK_SET);
          FILE_READ_ARRAY(savestate_file, snapshot_buffer);
          FILE_READ_VARIABLE(savestate_file, savestate_time_flat);
          FILE_CLOSE(savestate_file);
          valid_flag = 1;
        }
        else
          valid_flag = 0;
      }
    }
    else
    {
      if (mem_save_flag == 1)
      {
        g_state_buffer_ptr = savestate_write_buffer;
        if(memcmp(g_state_buffer_ptr, g_state_str, 4) == 0)
        {
          g_state_buffer_ptr += 8;
          FILE_READ_MEM_ARRAY(g_state_buffer_ptr, snapshot_buffer);
          FILE_READ_MEM_VARIABLE(g_state_buffer_ptr, savestate_time_flat);
          valid_flag = 1;
        }
      }
      else
        valid_flag = 0;
    }

    if (valid_flag == 1)
      {
        sceRtcConvertUtcToLocalTime(&savestate_time_flat, &local_time);

        sceRtcSetTick(&current_time, &local_time);
        wday = sceRtcGetDayOfWeek(current_time.year, current_time.month, current_time.day);
        get_timestamp_string(savestate_timestamp_string, MSG_STATE_MENU_DATE_FMT_0, current_time.year, current_time.month, current_time.day,
            wday, current_time.hour, current_time.minutes, current_time.seconds, 0);

        savestate_timestamp_string[40] = 0;

        PRINT_STRING_BG(savestate_timestamp_string, COLOR_HELP_TEXT, COLOR_BG, 10, 40);
      }
    else
      {
        memset(snapshot_buffer, 0, 240 * 160 * 2);
        PRINT_STRING_EXT_BG(msg[MSG_STATE_MENU_STATE_NONE], 0xFFFF, 0x0000, 15, 75, snapshot_buffer, 240);
        get_timestamp_string(savestate_timestamp_string, MSG_STATE_MENU_DATE_NONE_0, 0, 0, 0, 0, 0, 0, 0, 0);
        PRINT_STRING_BG(savestate_timestamp_string, COLOR_HELP_TEXT, COLOR_BG, 10, 40);
      }
    blit_to_screen(snapshot_buffer, 240, 160, 230, 40);
  }

static void get_savestate_filename(u32 slot, char *name_buffer)
{
  char savestate_ext[16];

  sprintf(savestate_ext, "_%d.svs", (int)slot);
  change_ext(gamepak_filename, name_buffer, savestate_ext);

  get_savestate_snapshot(name_buffer, g_savestate_slot_num);
}

static u32 parse_line(char *current_line, char *current_str)
{
  char *line_ptr;
  char *line_ptr_new;

  line_ptr = current_line;
  /* NULL or comment or other */
  if((current_line[0] == 0) || (current_line[0] == '#') || (current_line[0] != '!'))
    return -1;

  line_ptr++;

  line_ptr_new = strchr(line_ptr, '\r');
  while (line_ptr_new != NULL)
  {
    *line_ptr_new = '\n';
    line_ptr_new = strchr(line_ptr, '\r');
  }

  line_ptr_new = strchr(line_ptr, '\n');
  if (line_ptr_new == NULL)
    return -1;

  *line_ptr_new = 0;

  // "\n" to '\n'
  line_ptr_new = strstr(line_ptr, "\\n");
  while (line_ptr_new != NULL)
  {
    *line_ptr_new = '\n';
    memmove((line_ptr_new + 1), (line_ptr_new + 2), (strlen(line_ptr_new + 2) + 1));
    line_ptr_new = strstr(line_ptr_new, "\\n");
  }

  strcpy(current_str, line_ptr);
  return 0;
}

static void print_status(u32 mode)
{
  char print_buffer_1[256];
  char print_buffer_2[256];
  char utf8[2048];
  pspTime current_time;

  sceRtcGetCurrentClockLocalTime(&current_time);
  int wday = sceRtcGetDayOfWeek(current_time.year, current_time.month , current_time.day);

  get_timestamp_string(print_buffer_1, MSG_MENU_DATE_FMT_0, current_time.year, current_time.month , current_time.day, wday,
    current_time.hour, current_time.minutes, current_time.seconds, 0);
  sprintf(print_buffer_2,"%s%s", msg[MSG_MENU_DATE], print_buffer_1);
  PRINT_STRING_BG(print_buffer_2, COLOR_HELP_TEXT, COLOR_BG, 0, 0);

  sprintf(print_buffer_1, msg[MSG_MENU_BATTERY], scePowerGetBatteryLifePercent(), scePowerGetBatteryLifeTime());
  PRINT_STRING_BG(print_buffer_1, COLOR_HELP_TEXT, COLOR_BG, 240, 0);

  sprintf(print_buffer_1, "MAX ROM BUF: %02d MB Ver:%d.%d %s %d Build %d",
      (int)(gamepak_ram_buffer_size/1024/1024), VERSION_MAJOR, VERSION_MINOR, VER_RELEASE, VERSION_BUILD, BUILD_COUNT);
  PRINT_STRING_BG(print_buffer_1, COLOR_HELP_TEXT, COLOR_BG, 240, 10);

  if (mode == 0)
  {
    strncpy(print_buffer_1, gamepak_filename, 80);
    PRINT_STRING_BG_SJIS(utf8, print_buffer_1, COLOR_ROM_INFO, COLOR_BG, 10, 10);
    sprintf(print_buffer_1, "%s  %s  %s  %0X", gamepak_title, gamepak_code, gamepak_maker, (unsigned int)gamepak_crc32);
    PRINT_STRING_BG(print_buffer_1, COLOR_ROM_INFO, COLOR_BG, 10, 20);
  }
}

static void get_timestamp_string(char *buffer, u16 msg_id, u16 year, u16 mon, u16 day, u16 wday, u16 hour, u16 min, u16 sec, u32 msec)
{
  char *weekday_strings[] =
  {
    msg[MSG_WDAY_0], msg[MSG_WDAY_1], msg[MSG_WDAY_2], msg[MSG_WDAY_3],
    msg[MSG_WDAY_4], msg[MSG_WDAY_5], msg[MSG_WDAY_6], ""
  };

  switch(date_format)
  {
    case PSP_SYSTEMPARAM_DATE_FORMAT_YYYYMMDD:
      sprintf(buffer, msg[msg_id    ], year, mon, day, weekday_strings[wday], hour, min, sec, msec / 1000);
      break;
    case PSP_SYSTEMPARAM_DATE_FORMAT_MMDDYYYY:
      sprintf(buffer, msg[msg_id + 1], weekday_strings[wday], mon, day, year, hour, min, sec, msec / 1000);
      break;
    case PSP_SYSTEMPARAM_DATE_FORMAT_DDMMYYYY:
      sprintf(buffer, msg[msg_id + 2], weekday_strings[wday], day, mon, year, hour, min, sec, msec / 1000);
      break;
  }
}

static void save_ss_bmp(u16 *image)
{
  static unsigned char header[] ={ 'B',  'M',  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
                                   0x00, 0x00,  240, 0x00, 0x00, 0x00,  160, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  char ss_filename[MAX_FILE];
  char timestamp[MAX_FILE];
  char save_ss_path[MAX_PATH];
  pspTime current_time;
  u8 *rgb_data = (u8 *)UNIVERSAL_VRAM_ADDR;
  u8 x,y;
  u16 col;
  u8 r,g,b;

  sceRtcGetCurrentClockLocalTime(&current_time);

  change_ext(gamepak_filename, ss_filename, "_");

  get_timestamp_string(timestamp, MSG_SS_FMT_0, current_time.year, current_time.month , current_time.day, 7,
    current_time.hour, current_time.minutes, current_time.seconds, current_time.microseconds);

  if (g_default_ss_dir != NULL) {
    sprintf(save_ss_path, "%s/%s%s.bmp", g_default_ss_dir, ss_filename, timestamp);
  }
  else
  {
    sprintf(save_ss_path, "%s_%s.bmp", ss_filename, timestamp);
  }

  for(y = 0; y < 160; y++)
  {
    for(x = 0; x < 240; x++)
    {
      col = image[x + y * 240];
      r = (col >> 10) & 0x1F;
      g = (col >> 5) & 0x1F;
      b = (col) & 0x1F;

      rgb_data[(159-y)*240*3+x*3+2] = b * 255 / 31;
      rgb_data[(159-y)*240*3+x*3+1] = g * 255 / 31;
      rgb_data[(159-y)*240*3+x*3+0] = r * 255 / 31;
    }
  }

    FILE *ss = fopen( save_ss_path, "wb" );
    if( ss == NULL ) return;

    fwrite( header, sizeof(header), 1, ss );
    fwrite( rgb_data, 240*160*3, 1, ss);
    fclose( ss );
}

void _flush_cache()
{
//    sceKernelDcacheWritebackAll();
    invalidate_all_cache();
}
