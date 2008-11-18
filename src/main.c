/* unofficial gameplaySP kai
 *
 * Copyright (C) 2006 Exophase <exophase@gmail.com>
 * Copyright (C) 2007 takka <takka@tfact.net>
 * Copyright (C) 2007 NJ
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
 *s
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/******************************************************************************
 * ヘッダファイルの読込み
 ******************************************************************************/
#include "common.h"

PSP_MODULE_INFO("gpSP", PSP_MODULE_USER, VERSION_MAJOR, VERSION_MINOR);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER|PSP_THREAD_ATTR_VFPU);
PSP_MAIN_THREAD_PRIORITY(0x11);
PSP_MAIN_THREAD_STACK_SIZE_KB(640);
PSP_HEAP_SIZE_MAX();

/******************************************************************************
 * グローバル変数の定義
 ******************************************************************************/
TIMER_TYPE timer[4];                              // タイマー

/******************************************************************************
 * グローバル変数の定義
 ******************************************************************************/

u32 global_cycles_per_instruction = 1;
//u64 frame_count_initial_timestamp = 0;
//u64 last_frame_interval_timestamp;
u32 psp_fps_debug = 0;
u32 skip_next_frame_flag = 0;
//u32 frameskip_counter = 0;

u32 cpu_ticks = 0;
u32 frame_ticks = 0;

u32 execute_cycles = 960;
s32 video_count = 960;
//u32 ticks;

//u32 arm_frame = 0;
//u32 thumb_frame = 0;
u32 last_frame = 0;

u32 synchronize_flag = 1;

char main_path[MAX_PATH];
char rom_path[MAX_PATH];

//vu32 quit_flag;
vu32 power_flag;

vu32 real_frame_count = 0;
u32 virtual_frame_count = 0;
vu32 vblank_count = 0;
u32 num_skipped_frames = 0;
u32 frames;

char *lang[12] =
  { "japanese",                // 0
    "english",                 // 1
    "french",                  // 2
    "spanish",                 // 3
    "german",                  // 4
    "italian",                 // 5
    "dutch",                   // 6
    "portuguese",              // 7
    "russian",                 // 8
    "korean",                  // 9
    "chinese_traditional",     // 10
    "chinese_simplified"       // 11
  };

#define MAX_LANG_NUM 11

int date_format;

u32 prescale_table[] = { 0, 6, 8, 10 };

char *file_ext[] = { ".gba", ".bin", ".zip", NULL };

// PSPの種類
MODEL_TYPE psp_model;

/******************************************************************************
 * マクロ等の定義
 ******************************************************************************/

// エミュレーション サイクルの決定
#define CHECK_COUNT(count_var)                                                \
  if((count_var) < execute_cycles)                                            \
    execute_cycles = count_var;                                               \

#define CHECK_TIMER(timer_number)                                             \
  if(timer[timer_number].status == TIMER_PRESCALE)                            \
    CHECK_COUNT(timer[timer_number].count);                                   \

// タイマーのアップデート
// 実機では0~0xFFFFだが、gpSP内部では (0x10000~1)<<prescale(0,6,8,10)の値をとる
#define update_timer(timer_number)                                            \
  if(timer[timer_number].status != TIMER_INACTIVE)                            \
  {                                                                           \
    /* タイマーがアクティブだった場合 */                                      \
    if(timer[timer_number].status != TIMER_CASCADE)                           \
    {                                                                         \
      /* タイマーがプリスケールモードだった場合 */                            \
      /* タイマー変更 */                                                      \
      timer[timer_number].count -= execute_cycles;                            \
      /* レジスタに書込 */                                                    \
      io_registers[REG_TM##timer_number##D] =                                 \
      0x10000 - (timer[timer_number].count >> timer[timer_number].prescale);  \
    }                                                                         \
                                                                              \
    if(timer[timer_number].count <= 0)                                        \
    {                                                                         \
      /* タイマーがオーバーフローした場合 */                                  \
      /* IRQのトリガをON */                                                   \
      if(timer[timer_number].irq == TIMER_TRIGGER_IRQ)                        \
        irq_raised |= IRQ_TIMER##timer_number;                                \
                                                                              \
      if((timer_number != 3) &&                                               \
       (timer[timer_number + 1].status == TIMER_CASCADE))                     \
      {                                                                       \
        /* タイマー0～2 かつ 次のタイマーがカスケードモードの場合 */          \
        /* カウンタを変更 */                                                  \
        timer[timer_number + 1].count--;                                      \
        /* レジスタに書込 */                                                  \
        io_registers[REG_TM0D + (timer_number + 1) * 2] =                     \
        0x10000 - (timer[timer_number + 1].count);                            \
      }                                                                       \
                                                                              \
      if(timer_number < 2)                                                    \
      {                                                                       \
        if(timer[timer_number].direct_sound_channels & 0x01)                  \
          sound_timer(timer[timer_number].frequency_step, 0);                 \
                                                                              \
        if(timer[timer_number].direct_sound_channels & 0x02)                  \
          sound_timer(timer[timer_number].frequency_step, 1);                 \
      }                                                                       \
                                                                              \
      /* タイマーのリロード */                                                \
      timer[timer_number].count +=                                            \
        (timer[timer_number].reload << timer[timer_number].prescale);         \
      io_registers[REG_TM##timer_number##D] =                                 \
      0x10000 - (timer[timer_number].count >> timer[timer_number].prescale);  \
    }                                                                         \
  }                                                                           \

// ローカル関数の宣言
void vblank_interrupt_handler(u32 sub, u32 *parg);
void init_main();
int main(int argc, char *argv[]);
void print_memory_stats(u32 *counter, u32 *region_stats, u8 *stats_str);
u32 into_suspend();
int exit_callback(int arg1, int arg2, void *common);
SceKernelCallbackFunction power_callback(int unknown, int powerInfo, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks();
int user_main(SceSize args, char *argp[]);
void psp_exception_handler(PspDebugRegBlock *regs);

// クロックの変更
void set_cpu_clock(u32 num)
{
  const u32 clock_speed_table[11] = {33, 66, 100, 133, 166, 200, 233, 266, 300, 333, 222};
  num = num % 11;
  scePowerSetClockFrequency(clock_speed_table[num], clock_speed_table[num], clock_speed_table[num] / 2);
}

void init_main()
{
  u32 i;

  skip_next_frame_flag = 0;

  for(i = 0; i < 4; i++)
  {
    dma[i].start_type = DMA_INACTIVE;
    dma[i].direct_sound_channel = DMA_NO_DIRECT_SOUND;
    timer[i].status = TIMER_INACTIVE;
    timer[i].reload = 0x10000;
  }

  timer[0].direct_sound_channels = TIMER_DS_CHANNEL_BOTH;
  timer[1].direct_sound_channels = TIMER_DS_CHANNEL_NONE;

  cpu_ticks = 0;
  frame_ticks = 0;

  execute_cycles = 960;
  video_count = 960;

  bios_mode = USE_BIOS;

  flush_translation_cache_rom();
  flush_translation_cache_ram();
  flush_translation_cache_bios();
}

int exit_callback(int arg1, int arg2, void *common)
{
  quit(0);
//  quit_flag = 1; // TODO
  return 0;
}

// サスペンド/復旧時の処理
SceKernelCallbackFunction power_callback(int unknown, int powerInfo, void *common)
{
  if (powerInfo & PSP_POWER_CB_POWER_SWITCH)
  {
    // サスペンド時の処理
    power_flag = 1;

#ifdef USE_EXT_MEM
    // 新型PSPの場合、増設メモリの一部をメインメモリに待避
    if(psp_model == PSP_2000)
    {
      memcpy(gamepak_rom_resume,(void *)(PSP2K_MEM_TOP + 0x1c00000), 0x400000);
    }
#endif

  }
  else if (powerInfo & PSP_POWER_CB_RESUME_COMPLETE)
  {
    power_flag = 0;

#ifdef USE_EXT_MEM
    // 新型PSPの場合、メインメモリから増設メモリへ内容を復旧
    if(psp_model == PSP_2000)
    {
      memcpy((void *)(PSP2K_MEM_TOP + 0x1c00000),gamepak_rom_resume, 0x400000);
    }
#endif

  }
  return 0;
}

int CallbackThread(SceSize args, void *argp)
{
  int id;

  // 終了周りのコールバック
  id = sceKernelCreateCallback("Exit Callback", (void *)exit_callback, NULL);
  sceKernelRegisterExitCallback(id);

  // 電源周りのコールバック
  id = sceKernelCreateCallback("Power Callback", (void *)power_callback, NULL);
  scePowerRegisterCallback(-1, id); // TODO

  sceKernelSleepThreadCB();

  return 0;
}

int SetupCallbacks()
{
  int callback_thread_id = 0;

  callback_thread_id = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
  if (callback_thread_id >= 0)
  {
    sceKernelStartThread(callback_thread_id, 0, 0);
  }

  return callback_thread_id;
}

// mode = 0 NORMAL
// mode = 1 INIT ERROR
// mode = 2 FONT INIT ERROR
void quit(u32 mode)
{
  if(mode == 0)
    {
      save_game_config_file();
      save_config_file();

      update_backup_force();
    }
#ifdef USE_ADHOC
  adhoc_exit();
#endif

  sound_exit();
  if(mode != 2)
    fbm_freeall();

#ifdef USE_DEBUG
  fclose(g_dbg_file);
#endif

  set_cpu_clock(10);

  sceKernelExitGame();
}

int main(int argc, char *argv[])
{
  char load_filename[MAX_FILE];
  char filename[MAX_FILE];

  sceDisplaySetMode(0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT);
  pspDebugScreenInit();

  // Copy the directory path of the executable into main_path
  getcwd(main_path, MAX_FILE);
  chdir(main_path);

  // 設定ファイルの読込み
  load_config_file();

  psp_model = get_model();

  // HOMEボタンを読取り可能にする
  if(pspSdkLoadStartModule("homehook.prx", PSP_MEMORY_PARTITION_KERNEL) < 0)
  {
    error_msg("Error in load/start homehook module.\n");
    quit(1);
  }

  // PSP-2000(CFW3.71)なら、外部出力用のモジュールを読込む
  if(psp_model == PSP_2000)
  {
    if(pspSdkLoadStartModule("dvemgr.prx", PSP_MEMORY_PARTITION_KERNEL) < 0)
      error_msg("Error in load/start TV OUT module.\n");
  }

//  quit_flag = 0;
  power_flag = 0;
  SetupCallbacks();

  sceKernelRegisterSubIntrHandler(PSP_VBLANK_INT, 0, vblank_interrupt_handler, NULL);
  sceKernelEnableSubIntr(PSP_VBLANK_INT, 0);

#ifdef USE_DEBUG
  // デバッグ出力ファイルのオープン
  g_dbg_file = fopen(DBG_FILE_NAME, "awb");
  DBGOUT("\nStart gpSP\n");
#endif

#ifdef USE_ADHOC
  // adhoc用モジュールのロード
  if (load_adhoc_modules() != 0)
    error_msg("not load adhoc modules!!\n");
#endif

  // 初期化
  init_game_config();

  init_main();
  init_sound();

  init_video();
  init_input();

  video_resolution(FRAME_MENU);
  video_resolution(FRAME_GAME);
  video_resolution(FRAME_MENU);

  clear_screen(COLOR_BG);
  flip_screen();
  clear_screen(COLOR_BG);
  flip_screen();

  // 初期設定用のプログレスバー
  init_progress(7, "");

  // ディレクトリ設定の読込み
  if (load_dircfg("cfg/dir.cfg") != 0)
  {
    pspDebugScreenInit();
    error_msg("dir.cfg Error!!");
    quit(1);
  }
  update_progress();

  // フォント設定の読込み
  sprintf(filename,"cfg/%s.fnt",lang[g_gpsp_config.language]);
  if (load_fontcfg(filename) != 0)
  {
    pspDebugScreenInit();
    error_msg("font.cfg Error!!");
    quit(1);
  }
  update_progress();

  // メッセージファイルの読込み
  sprintf(filename,"cfg/%s.msg",lang[g_gpsp_config.language]);
  if (load_msgcfg(filename) != 0)
  {
    pspDebugScreenInit();
    error_msg("message.cfg Error!!");
    quit(1);
  }
  update_progress();

  // フォントの読込み
  if (load_font() != 0)
  {
    pspDebugScreenInit();
    error_msg("font init Error!!");
    quit(2);
  }
  update_progress();

  // ROMバッファの初期化
  init_gamepak_buffer();
  update_progress();

  sceImposeSetHomePopup(g_gpsp_config.enable_home);

  // ROMファイル名の初期化
  gamepak_filename[0] = 0;

  // BIOSの読込み
  u32 bios_ret = load_bios("gba_bios.bin");
  if(bios_ret == -1) // 読込めない場合
  {
    PRINT_STRING_BG(msg[MSG_ERR_BIOS_1], 0xFFFF, 0x0000, 0, 0);
    PRINT_STRING_BG(msg[MSG_ERR_BIOS_2], 0xFFFF, 0x0000, 0, 60);
    flip_screen();
    error_msg("");
    quit(1);
  }
  update_progress();
  if(bios_ret == -2) // MD5が違う場合
  {
    PRINT_STRING_BG(msg[MSG_ERR_BIOS_MD5], 0xFFFF, 0x0000, 0, 0);
    flip_screen();
    sceKernelDelayThread(5000000);
  }
  update_progress();

  show_progress(msg[MSG_INIT_END]);

  if(argc > 1)
  {
    if(load_gamepak((char *)argv[1]) == -1)
    {
      printf("Failed to load gamepak %s, exiting.\n", load_filename);
      quit(1);
    }

    clear_screen(0);
    flip_screen();
    clear_screen(0);
    flip_screen();
    video_resolution(FRAME_GAME);

    init_cpu();
    init_memory();
    reset_sound();
  }
  else
  {
    if(load_file(file_ext, load_filename, g_default_rom_dir) == -1)
    {
      u16 *screen_copy = copy_screen();
      menu(screen_copy);
      free(screen_copy);
    }
    else
    {
      if(load_gamepak(load_filename) == -1)
      {
        video_resolution(FRAME_MENU);
        printf("Failed to load gamepak %s, exiting.\n", load_filename);
        sceKernelDelayThread(5000000);
        quit(1);
      }

      clear_screen(0);
      flip_screen();
      clear_screen(0);
      flip_screen();
      video_resolution(FRAME_GAME);

      init_cpu();
      init_memory();
      reset_sound();
    }
  }

  last_frame = 0;
  set_cpu_clock(g_game_config.clock_speed_number);

  pause_sound(0);
  real_frame_count = 0;
  virtual_frame_count = 0;

  // エミュレートの開始
#ifdef USE_C_CORE
  if(g_gpsp_config.emulate_core == ASM_CORE)
    execute_arm_translate(execute_cycles);
  else
    execute_arm(execute_cycles);
#else
  execute_arm_translate(execute_cycles);
#endif

  return 0;
}

// サスペンド時にメニューに入る処理
u32 into_suspend()
{
//  if (power_flag == 0) return 0; // TODO この処理はupdate_gba()側に移動
  FILE_CLOSE(gamepak_file_large);
  u16 *screen_copy = copy_screen();
  u32 ret_val = menu(screen_copy);
  free(screen_copy);
  FILE_OPEN(gamepak_file_large, gamepak_filename_full_path, READ);
  return ret_val;
}

u32 sync_flag = 0;

#define V_BLANK   (0x01)
#define H_BLANK   (0x02)
#define V_COUNTER (0x04)

// video_countの初期値は960(水平表示クロック)

u32 update_gba()
{
  // TODO このあたりのログをとる必要があるかも
  IRQ_TYPE irq_raised = IRQ_NONE;

  do
    {
      cpu_ticks += execute_cycles;

      reg[CHANGED_PC_STATUS] = 0;

      if(gbc_sound_update)
        {
          update_gbc_sound(cpu_ticks);
          gbc_sound_update = 0;
        }

    update_timer(0);
    update_timer(1);
    update_timer(2);
    update_timer(3);

    video_count -= execute_cycles;

    if(video_count <= 0)
    { // 状態移行の発生
      u32 vcount = io_registers[REG_VCOUNT];
      u32 dispstat = io_registers[REG_DISPSTAT];

      if(!(dispstat & H_BLANK))
      { // H BLANKでないとき
        // H BLANKに移行
        video_count += 272;
        dispstat |= H_BLANK; // H BLANKに設定

        if((dispstat & 0x01) == 0)
        {  // VBLANKでないとき
          // フレームスキップ時は描画しない
          if(!skip_next_frame_flag)
              update_scanline();

          // If in visible area also fire HDMA
          if(dma[0].start_type == DMA_START_HBLANK)
            dma_transfer(dma);
          if(dma[1].start_type == DMA_START_HBLANK)
            dma_transfer(dma + 1);
          if(dma[2].start_type == DMA_START_HBLANK)
            dma_transfer(dma + 2);
          if(dma[3].start_type == DMA_START_HBLANK)
            dma_transfer(dma + 3);

          if(dispstat & 0x10)
            irq_raised |= IRQ_HBLANK; // HBLANK割込みはVBLANK中は発生しない
        }

      } // HBLANKでないとき
      else
      { // HBLANKのとき
        // 次のラインに移行
        video_count += 960;
        dispstat &= ~H_BLANK;

        vcount++;

        if(vcount == 160)
        {
          // Transition from vrefresh to vblank

          dispstat |= 0x01;
          if(dispstat & 0x8)
            irq_raised |= IRQ_VBLANK;

          affine_reference_x[0] = (s32)(ADDRESS32(io_registers, 0x28) << 4) >> 4;
          affine_reference_y[0] = (s32)(ADDRESS32(io_registers, 0x2C) << 4) >> 4;
          affine_reference_x[1] = (s32)(ADDRESS32(io_registers, 0x38) << 4) >> 4;
          affine_reference_y[1] = (s32)(ADDRESS32(io_registers, 0x3C) << 4) >> 4;

          if(dma[0].start_type == DMA_START_VBLANK)
            dma_transfer(dma);
          if(dma[1].start_type == DMA_START_VBLANK)
            dma_transfer(dma + 1);
          if(dma[2].start_type == DMA_START_VBLANK)
            dma_transfer(dma + 2);
          if(dma[3].start_type == DMA_START_VBLANK)
            dma_transfer(dma + 3);

       }
        else

        if(vcount == 228) {
          // Transition from vblank to next screen
          dispstat &= ~0x01;
          frame_ticks++;

          sceKernelDelayThread(10);

          if(update_input() != 0)
            continue;

          if((power_flag == 1) && (into_suspend() != 0))
            continue;

          update_gbc_sound(cpu_ticks);

          if(g_game_config.update_backup_flag == 1)
            update_backup();

          process_cheats();

          vcount = 0; // TODO vcountを0にするタイミングを検討

          //TODO 調整必要
          if((video_out_mode == 0) || (g_gpsp_config.screen_interlace == PROGRESSIVE))
            synchronize();
          else
          {
            if(sync_flag == 0)
              synchronize();
            sync_flag ^= 1;
          }

          if(!skip_next_frame_flag)
            flip_screen();
        } //(vcount == 228)

        // vcountによる割込
        if(vcount == (dispstat >> 8))
        {
          // vcount trigger
          dispstat |= 0x04;
          if(dispstat & 0x20)
          {
            irq_raised |= IRQ_VCOUNT;
          }
        }
        else
        {
          dispstat &= ~0x04;
        }

        io_registers[REG_VCOUNT] = vcount;
      }
      io_registers[REG_DISPSTAT] = dispstat;
    }

    execute_cycles = video_count;

    if(irq_raised)
      raise_interrupt(irq_raised);

    CHECK_TIMER(0);
    CHECK_TIMER(1);
    CHECK_TIMER(2);
    CHECK_TIMER(3);

    // 画面のシンクロ・フリップ・ウェイト処理はここで行うべきでは？

  } while(reg[CPU_HALT_STATE] != CPU_ACTIVE);
  return execute_cycles;
}

void vblank_interrupt_handler(u32 sub, u32 *parg)
{
  real_frame_count++;
  vblank_count++;
}

// TODO:最適化/タイマー使ったものに変更
// GBAの描画サイクルは 16.743 ms, 280896 cycles(16.78MHz) 59.737 Hz
void synchronize()
{
//  char char_buffer[64];
  static u32 fps = 60;
  static u32 frames_drawn = 0;
  static u32 frames_drawn_count = 0;

  // FPS等の表示
  if(psp_fps_debug)
  {
    char print_buffer[256];
    sprintf(print_buffer, "FPS:%02d DRAW:(%02d) S_BUF:%02d PC:%08X", (int)fps, (int)frames_drawn, (int)left_buffer, (int)reg[REG_PC]);
    PRINT_STRING_BG(print_buffer, 0xFFFF, 0x000, 0, 0);
  }

  // フレームスキップ フラグの初期化
  skip_next_frame_flag = 0;
  // 内部フレーム値の増加
  frames++;

  switch(g_game_config.frameskip_type)
  {
  // オートフレームスキップ時
    case auto_frameskip:
      virtual_frame_count++;

      // 内部フレーム数に遅れが出ている場合
      if(real_frame_count > virtual_frame_count)
      {
        if(num_skipped_frames < g_game_config.frameskip_value)  // スキップしたフレームが設定より小さい
        {
          // 次のフレームはスキップ
          skip_next_frame_flag = 1;
          // スキップしたフレーム数を増加
          num_skipped_frames++;
        }
        else
        {
          // 設定の上限に達した場合
//          real_frame_count = virtual_frame_count;
          // スキップしたフレーム数は0に初期化
          num_skipped_frames = 0;
          frames_drawn_count++;
        }
      }

      // 内部フレーム数が同じ場合
      if(real_frame_count == virtual_frame_count)
      {
        // スキップしたフレーム数は0に初期化
        num_skipped_frames = 0;
        frames_drawn_count++;
      }

      // 内部フレーム数が実機を上回る場合
      if(real_frame_count < virtual_frame_count)
      {
        num_skipped_frames = 0;
        frames_drawn_count++;
      }

      // 内部フレーム数が実機を上回る場合
      if((real_frame_count < virtual_frame_count) && (synchronize_flag) && (skip_next_frame_flag == 0))
      {
        // VBANK待ち
        synchronize_sound();
        sceDisplayWaitVblankStart();
        real_frame_count = 0;
        virtual_frame_count = 0;
      }
      break;

    // マニュアルフレームスキップ時
    case manual_frameskip:
      virtual_frame_count++;
      // フレームスキップ数増加
      num_skipped_frames = (num_skipped_frames + 1) % (g_game_config.frameskip_value + 1);
      if(g_game_config.random_skip)
      {
        if(num_skipped_frames != (rand() % (g_game_config.frameskip_value + 1)))
          skip_next_frame_flag = 1;
        else
          frames_drawn_count++;
      }
      else
      {
        // フレームスキップ数=0の時だけ画面更新
        if(num_skipped_frames != 0)
          skip_next_frame_flag = 1;
        else
          frames_drawn_count++;
      }

      // 内部フレーム数が実機を上回る場合
      if((real_frame_count < virtual_frame_count) && (synchronize_flag) && (skip_next_frame_flag == 0))
      {
        // VBANK待ち
        synchronize_sound();
        sceDisplayWaitVblankStart();
      }
      real_frame_count = 0;
      virtual_frame_count = 0;
      break;

    // フレームスキップなし時
    case no_frameskip:
      frames_drawn_count++;
      virtual_frame_count++;
      if((real_frame_count < virtual_frame_count) && (synchronize_flag))
      {
        // 内部フレーム数が実機を上回る場合
        // VBANK待ち
        synchronize_sound();
        sceDisplayWaitVblankStart();
      }
      real_frame_count = 0;
      virtual_frame_count = 0;
      break;
  }

  // FPSのカウント
  // 1/60秒のVBLANK割込みがあるので、タイマは使用しないようにした
  if(frames == 60)
  {
    frames = 0;
    fps = 3600 / vblank_count;
    vblank_count = 0;
    frames_drawn = frames_drawn_count;
    frames_drawn_count = 0;
  }

  if(!synchronize_flag)
    PRINT_STRING_BG("--FF--", 0xFFFF, 0x000, 0, 10);
}

void reset_gba()
{
  init_main();
  init_memory();
  init_cpu();
  reset_sound();
}

u32 file_length(const char *filename)
{
  SceIoStat stats;
  sceIoGetstat(filename, &stats);
  return stats.st_size;
}

void change_ext(char *src, char *buffer, char *extension)
{
  char *dot_position;
  strcpy(buffer, src);
  dot_position = strrchr(buffer, '.');

  if(dot_position)
    strcpy(dot_position, extension);
}

// type = READ / WRITE_MEM
#define MAIN_SAVESTATE_BODY(type)                                             \
{                                                                             \
  FILE_##type##_VARIABLE(g_state_buffer_ptr, cpu_ticks);                      \
  FILE_##type##_VARIABLE(g_state_buffer_ptr, execute_cycles);                 \
  FILE_##type##_VARIABLE(g_state_buffer_ptr, video_count);                    \
  FILE_##type##_ARRAY(g_state_buffer_ptr, timer);                             \
}                                                                             \

void main_read_mem_savestate()
MAIN_SAVESTATE_BODY(READ_MEM);

void main_write_mem_savestate()
MAIN_SAVESTATE_BODY(WRITE_MEM);

void error_msg(char *text)
{
    gui_action_type gui_action = CURSOR_NONE;

    printf(text);

    while(gui_action == CURSOR_NONE)
    {
      gui_action = get_gui_input();
      sceKernelDelayThread(15000); /* 0.0015s */
    }
}

void set_cpu_mode(CPU_MODE_TYPE new_mode)
{
  u32 i;
  CPU_MODE_TYPE cpu_mode = reg[CPU_MODE];

  if(cpu_mode != new_mode)
  {
    if(new_mode == MODE_FIQ)
    {
      for(i = 8; i < 15; i++)
      {
        reg_mode[cpu_mode][i - 8] = reg[i];
      }
    }
    else
    {
      reg_mode[cpu_mode][5] = reg[REG_SP];
      reg_mode[cpu_mode][6] = reg[REG_LR];
    }

    if(cpu_mode == MODE_FIQ)
    {
      for(i = 8; i < 15; i++)
      {
        reg[i] = reg_mode[new_mode][i - 8];
      }
    }
    else
    {
      reg[REG_SP] = reg_mode[new_mode][5];
      reg[REG_LR] = reg_mode[new_mode][6];
    }

    reg[CPU_MODE] = new_mode;
  }
}

void raise_interrupt(IRQ_TYPE irq_raised)
{
  // The specific IRQ must be enabled in IE, master IRQ enable must be on,
  // and it must be on in the flags.
  io_registers[REG_IF] |= irq_raised;

  if((io_registers[REG_IME] & 0x01) && (io_registers[REG_IE] & io_registers[REG_IF]) && ((reg[REG_CPSR] & 0x80) == 0))
  {
    bios_read_protect = 0xe55ec002;

    // Interrupt handler in BIOS
    reg_mode[MODE_IRQ][6] = reg[REG_PC] + 4;
    spsr[MODE_IRQ] = reg[REG_CPSR];
    reg[REG_CPSR] = 0xD2;
    reg[REG_PC] = 0x00000018;
    bios_region_read_allow();
    set_cpu_mode(MODE_IRQ);
    reg[CPU_HALT_STATE] = CPU_ACTIVE;
    reg[CHANGED_PC_STATUS] = 1;
  }
}


MODEL_TYPE get_model()
{
  if((kuKernelGetModel() <= 0 /* original PSP */) || ( g_gpsp_config.fake_fat == YES))
  {
    return PSP_1000;
  }
  else
    if(sceKernelDevkitVersion() < 0x03070110 || sctrlSEGetVersion() < 0x00001012)
    {
      return PSP_1000;
    }
    else
    {
      return PSP_2000;
    }
}
