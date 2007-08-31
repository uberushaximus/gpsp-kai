/* unofficial gameplaySP kai
 *
 * Copyright (C) 2006 Exophase <exophase@gmail.com>
 * Copyright (C) 2007 takka <takka@tfact.net>
 * Copyright (C) 2007 ????? <?????>
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

#include "common.h"

// Special thanks to psp298 for the analog->dpad code!

void trigger_key(u32 key)
{
  u32 p1_cnt = io_registers[REG_P1CNT];
//  u32 p1;

  if((p1_cnt >> 14) & 0x01)
  {
    u32 key_intersection = (p1_cnt & key) & 0x3FF;

    if(p1_cnt >> 15)
    {
      if(key_intersection == (p1_cnt & 0x3FF))
        raise_interrupt(IRQ_KEYPAD);
    }
    else
    {
      if(key_intersection)
        raise_interrupt(IRQ_KEYPAD);
    }
  }
}

u32 key = 0;

u32 gamepad_config_map[16] =
{
  BUTTON_ID_MENU,
  BUTTON_ID_A,
  BUTTON_ID_B,
  BUTTON_ID_START,
  BUTTON_ID_L,
  BUTTON_ID_R,
  BUTTON_ID_DOWN,
  BUTTON_ID_LEFT,
  BUTTON_ID_UP,
  BUTTON_ID_RIGHT,
  BUTTON_ID_SELECT,
  BUTTON_ID_START,
  BUTTON_ID_UP,
  BUTTON_ID_DOWN,
  BUTTON_ID_LEFT,
  BUTTON_ID_RIGHT
};

extern u32 psp_fps_debug;

u32 global_enable_analog = 1;
u32 analog_sensitivity_level = 4;

#define PSP_ALL_BUTTON_MASK 0xFFFF

u32 last_buttons = 0;
u64 button_repeat_timestamp;

typedef enum
{
  BUTTON_NOT_HELD,
  BUTTON_HELD_INITIAL,
  BUTTON_HELD_REPEAT
} button_repeat_state_type;

button_repeat_state_type button_repeat_state = BUTTON_NOT_HELD;
u32 button_repeat = 0;
gui_action_type cursor_repeat = CURSOR_NONE;

u32 sensorX;
u32 sensorY;
u32 sensorR;

#define BUTTON_REPEAT_START    200000
#define BUTTON_REPEAT_CONTINUE 50000

gui_action_type get_gui_input()
{
  SceCtrlData ctrl_data;
  gui_action_type new_button = CURSOR_NONE;
  u32 new_buttons;
  u32 analog_sensitivity = 92 - (analog_sensitivity_level * 4);
  u32 inv_analog_sensitivity = 256 - analog_sensitivity;

  delay_us(25000);

  if (quit_flag == 1)
    quit();

  sceCtrlPeekBufferPositive(&ctrl_data, 1);

  if(!(ctrl_data.Buttons & PSP_CTRL_HOLD))
  {
    if(ctrl_data.Lx < analog_sensitivity)
      ctrl_data.Buttons = PSP_CTRL_LEFT;

    if(ctrl_data.Lx > inv_analog_sensitivity)
      ctrl_data.Buttons = PSP_CTRL_RIGHT;

    if(ctrl_data.Ly < analog_sensitivity)
      ctrl_data.Buttons = PSP_CTRL_UP;

    if(ctrl_data.Ly > inv_analog_sensitivity)
      ctrl_data.Buttons = PSP_CTRL_DOWN;
  }

  ctrl_data.Buttons &= PSP_ALL_BUTTON_MASK;

  new_buttons = (last_buttons ^ ctrl_data.Buttons) & ctrl_data.Buttons;
  last_buttons = ctrl_data.Buttons;

  if(new_buttons & PSP_CTRL_LEFT)
    new_button = CURSOR_LEFT;

  if(new_buttons & PSP_CTRL_RIGHT)
    new_button = CURSOR_RIGHT;

  if(new_buttons & PSP_CTRL_UP)
    new_button = CURSOR_UP;

  if(new_buttons & PSP_CTRL_DOWN)
    new_button = CURSOR_DOWN;

  if(new_buttons & PSP_CTRL_START)
    new_button = CURSOR_SELECT;

  if(new_buttons & PSP_CTRL_CIRCLE)
    new_button = CURSOR_SELECT;

  if(new_buttons & PSP_CTRL_CROSS)
    new_button = CURSOR_EXIT;

  if(new_buttons & PSP_CTRL_SQUARE)
    new_button = CURSOR_BACK;

  if(new_buttons & PSP_CTRL_RTRIGGER)
    new_button = CURSOR_RTRIGGER;

  if(new_buttons & PSP_CTRL_LTRIGGER)
    new_button = CURSOR_LTRIGGER;

  if(new_buttons & PSP_CTRL_SELECT)
    new_button = KEY_SELECT;

  if(new_button != CURSOR_NONE)
  {
    get_ticks_us(&button_repeat_timestamp);
    button_repeat_state = BUTTON_HELD_INITIAL;
    button_repeat = new_buttons;
    cursor_repeat = new_button;
  }
  else
  {
    if(ctrl_data.Buttons & button_repeat)
    {
      u64 new_ticks;
      get_ticks_us(&new_ticks);

      if(button_repeat_state == BUTTON_HELD_INITIAL)
      {
        if((new_ticks - button_repeat_timestamp) >
         BUTTON_REPEAT_START)
        {
          new_button = cursor_repeat;
          button_repeat_timestamp = new_ticks;
          button_repeat_state = BUTTON_HELD_REPEAT;
        }
      }

      if(button_repeat_state == BUTTON_HELD_REPEAT)
      {
        if((new_ticks - button_repeat_timestamp) >
         BUTTON_REPEAT_CONTINUE)
        {
          new_button = cursor_repeat;
          button_repeat_timestamp = new_ticks;
        }
      }
    }
  }

  return new_button;
}

#define PSP_CTRL_ANALOG_UP    (1 << 28)
#define PSP_CTRL_ANALOG_DOWN  (1 << 29)
#define PSP_CTRL_ANALOG_LEFT  (1 << 30)
#define PSP_CTRL_ANALOG_RIGHT (1 << 31)

u32 button_psp_mask_to_config[] =
{
  PSP_CTRL_TRIANGLE,
  PSP_CTRL_CIRCLE,
  PSP_CTRL_CROSS,
  PSP_CTRL_SQUARE,
  PSP_CTRL_LTRIGGER,
  PSP_CTRL_RTRIGGER,
  PSP_CTRL_DOWN,
  PSP_CTRL_LEFT,
  PSP_CTRL_UP,
  PSP_CTRL_RIGHT,
  PSP_CTRL_SELECT,
  PSP_CTRL_START,
  PSP_CTRL_ANALOG_UP,
  PSP_CTRL_ANALOG_DOWN,
  PSP_CTRL_ANALOG_LEFT,
  PSP_CTRL_ANALOG_RIGHT
};

u32 button_id_to_gba_mask[] =
{
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_A,
  BUTTON_B,
  BUTTON_L,
  BUTTON_R,
  BUTTON_START,
  BUTTON_SELECT,
  BUTTON_NONE,
  BUTTON_NONE,
  BUTTON_NONE,
  BUTTON_NONE
};

gui_action_type get_gui_input_fs_hold(u32 button_id)
{
  gui_action_type new_button = get_gui_input();
  if((last_buttons & button_psp_mask_to_config[button_id]) == 0)
    return CURSOR_BACK;

  return new_button;
}

u32 rapidfire_flag = 1;

u32 update_input()
{
  SceCtrlData ctrl_data;
  u32 buttons;
  u32 non_repeat_buttons;
  u32 button_id = 0;
  u32 i;
  u32 new_key = 0;
  u32 analog_sensitivity = 92 - (analog_sensitivity_level * 4);
  u32 inv_analog_sensitivity = 256 - analog_sensitivity;
  sensorX = 0x800;
  sensorY = 0x800;
  sensorR = 0x650;

  sceCtrlPeekBufferPositive(&ctrl_data, 1);

  buttons = ctrl_data.Buttons;

  if((global_enable_analog) && !(ctrl_data.Buttons & PSP_CTRL_HOLD))
  {
    sensorX = ctrl_data.Lx * 16;  // センター 2048(0x800) 最小値 0(0x0) 最大値 1143(0xFFF) 幅 4096
    sensorY = ctrl_data.Ly * 16;  // センター 2048(0x800) 最小値 0(0x0) 最大値 1152(0xFFF) 幅 4096
    sensorR = ctrl_data.Lx * 12.5;  // センター 1600(0x640) 最小値 0(0x0) 最大値 3200(0xC80) 幅 3200
    if(ctrl_data.Lx < analog_sensitivity)
      buttons |= PSP_CTRL_ANALOG_LEFT;

    if(ctrl_data.Lx > inv_analog_sensitivity)
      buttons |= PSP_CTRL_ANALOG_RIGHT;

    if(ctrl_data.Ly < analog_sensitivity)
      buttons |= PSP_CTRL_ANALOG_UP;

    if(ctrl_data.Ly > inv_analog_sensitivity)
      buttons |= PSP_CTRL_ANALOG_DOWN;
  }

  non_repeat_buttons = (last_buttons ^ buttons) & buttons;
  last_buttons = buttons;

  for(i = 0; i < 16; i++)
  {
    if(non_repeat_buttons & button_psp_mask_to_config[i])
      button_id = gamepad_config_map[i];
    // HOMEが押されたらMENUに移行
    if (ctrl_data.Buttons & PSP_CTRL_HOME) button_id = BUTTON_ID_MENU;

    switch(button_id)
    {
      case BUTTON_ID_MENU:
      {
        u16 *screen_copy = copy_screen();
        u32 ret_val = menu(screen_copy);
        free(screen_copy);

        return ret_val;
      }

      case BUTTON_ID_LOADSTATE:
      {
        char current_savestate_filename[512];
        get_savestate_filename_noshot(SAVESTATE_SLOT, current_savestate_filename);
        load_state(current_savestate_filename, SAVESTATE_SLOT);
        return 1;
      }

      case BUTTON_ID_SAVESTATE:
      {
        char current_savestate_filename[512];
        u16 *current_screen = copy_screen();
        get_savestate_filename_noshot(SAVESTATE_SLOT, current_savestate_filename);
        save_state(current_savestate_filename, current_screen, SAVESTATE_SLOT);
        free(current_screen);
        return 0;
      }

      case BUTTON_ID_FASTFORWARD:
//        PRINT_STRING_BG("FASTFORWARD", 0xFFFF, 0x0000, 0, 50);
        synchronize_flag ^= 1;
        break;
//        return 0;

      case BUTTON_ID_VOLUP:
//        gp2x_sound_volume(1);
        break;

      case BUTTON_ID_VOLDOWN:
//        gp2x_sound_volume(0);
        break;

      case BUTTON_ID_FPS:
        psp_fps_debug ^= 1;
        break;
    }

    if(buttons & button_psp_mask_to_config[i])
    {
      button_id = gamepad_config_map[i];
      if(button_id < BUTTON_ID_MENU)
      {
        new_key |= button_id_to_gba_mask[button_id];
      }
      else

      if((button_id >= BUTTON_ID_RAPIDFIRE_A) &&
       (button_id <= BUTTON_ID_RAPIDFIRE_L))
      {
        rapidfire_flag ^= 1;
        if(rapidfire_flag)
        {
          new_key |= button_id_to_gba_mask[button_id -
           BUTTON_ID_RAPIDFIRE_A + BUTTON_ID_A];
        }
        else
        {
          new_key &= ~button_id_to_gba_mask[button_id -
           BUTTON_ID_RAPIDFIRE_A + BUTTON_ID_A];
        }
      }
    }
  }

  if((new_key | key) != key)
    trigger_key(new_key);

  key = new_key;

  io_registers[REG_P1] = (~key) & 0x3FF;

  return 0;
}

void init_input()
{
  sceCtrlSetSamplingCycle(0);
  sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

// type = READ / WRITE_MEM
#define input_savestate_body(type)                                            \
{                                                                             \
  FILE_##type##_VARIABLE(savestate_file, key);                                \
}                                                                             \

void input_read_savestate(FILE_TAG_TYPE savestate_file)                   \
input_savestate_body(READ);

void input_read_mem_savestate(FILE_TAG_TYPE savestate_file)                   \
input_savestate_body(READ_MEM);

void input_write_mem_savestate(FILE_TAG_TYPE savestate_file)                   \
input_savestate_body(WRITE_MEM);

// 以降OSK用のコード
#ifdef OSK

static unsigned int __attribute__((aligned(16))) list[262144];

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)
#define PIXEL_SIZE (4) /* change this if you change to another screenmode */
#define FRAME_SIZE (BUF_WIDTH * SCR_HEIGHT * PIXEL_SIZE)
#define ZBUF_SIZE (BUF_WIDTH SCR_HEIGHT * 2) /* zbuffer seems to be 16-bit? */

int SetupCallbacks();
void SetupGu();

int main(int argc, char* argv[])
{
     int done = 0;
     SetupCallbacks();

     // INIT GU!!!! it will not work without!!!
     SetupGu();

     // INIT OSK
     unsigned short intext[128]  = { 0 }; // text already in the edit box on start
     unsigned short outtext[128] = { 0 }; // text after input
     unsigned short desc[128]    = { 'E', 'n', 't', 'e', 'r', ' ', 'T', 'e', 'x', 't', 0 }; // description

     SceUtilityOskData data;
     memset(&data, 0, sizeof(data));
     data.language = 2; // english
     data.lines = 1; // just online
     data.unk_24 = 1; // set to 1
     data.desc = desc;
     data.intext = intext;
     data.outtextlength = 128; // sizeof(outtext) / sizeof(unsigned short)
     data.outtextlimit = 32; // just allow 32 chars
     data.outtext = outtext;

     SceUtilityOskParams osk;
     memset(&osk, 0, sizeof(osk));
     osk.size = sizeof(osk);
     osk.language = 2;
     osk.buttonswap = 0;
     osk.unk_12 = 17; // What
     osk.unk_16 = 19; // the
     osk.unk_20 = 18; // fuck
     osk.unk_24 = 16; // ???
     osk.unk_48 = 1;
     osk.data = &data;

     // Only ascii code is handled so only the input of the small letters is printed

     if(sceUtilityOskInitStart(&osk))
     {
          return 0;
     }

     while(!done)
     {
          sceGuStart(GU_DIRECT,list);
          sceGuClearColor(0x666666);
          sceGuClearDepth(0);
          sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

          sceGuFinish();
          sceGuSync(0,0);

          switch(sceUtilityOskGetStatus())
          {
          case PSP_OSK_NONE:
               break;
          case PSP_OSK_INIT:
               break;
          case PSP_OSK_VISIBLE:
               sceUtilityOskUpdate(2); // 2 is taken from ps2dev.org recommendation
               break;
          case PSP_OSK_QUIT:
               sceUtilityOskShutdownStart();
               break;
          case PSP_OSK_FINISHED:
               done = 1;
               break;
          default:
               break;
          }

          sceDisplayWaitVblankStart();
          sceGuSwapBuffers();
     }

     pspDebugScreenInit();
     pspDebugScreenSetXY(0, 0);
     pspDebugScreenPrintf("S:");
     int i;
     for(i = 0; data.outtext[i]; i++)
     {
          unsigned c = data.outtext[i];
          if(32 <= c && c <= 127)
          {
               pspDebugScreenPrintf("%c", data.outtext[i]); // print ascii only
          }
     }
     pspDebugScreenPrintf("\n");

     sceKernelSleepThread();
     return 0;
}

void SetupGu()
{
     sceGuInit();

     sceGuStart(GU_DIRECT,list);
     sceGuDrawBuffer(GU_PSM_8888,(void*)0,BUF_WIDTH);
     sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,(void*)0x88000,BUF_WIDTH);
     sceGuDepthBuffer((void*)0x110000,BUF_WIDTH);
     sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
     sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);
     sceGuDepthRange(0xc350,0x2710);
     sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
     sceGuEnable(GU_SCISSOR_TEST);
     sceGuDepthFunc(GU_GEQUAL);
     sceGuEnable(GU_DEPTH_TEST);
     sceGuFrontFace(GU_CW);
     sceGuShadeModel(GU_FLAT);
     sceGuEnable(GU_CULL_FACE);
     sceGuEnable(GU_TEXTURE_2D);
     sceGuEnable(GU_CLIP_PLANES);
     sceGuFinish();
     sceGuSync(0,0);

     sceDisplayWaitVblankStart();
     sceGuDisplay(GU_TRUE);
}

int exit_callback(int arg1, int arg2, void *common)
{
     sceGuTerm();
     sceKernelExitGame();
     return 0;
}

int CallbackThread(SceSize args, void *argp)
{
     int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
     sceKernelRegisterExitCallback(cbid);
     sceKernelSleepThreadCB();
     return 0;
}

int SetupCallbacks(void)
{
     int thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
     if(thid >= 0) {
          sceKernelStartThread(thid, 0, 0);
     }
     return thid;
}
#endif
