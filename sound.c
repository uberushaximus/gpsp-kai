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

/******************************************************************************
 * sound.c
 * サウンド周りの処理
 ******************************************************************************/

/******************************************************************************
 * ヘッダファイルの読込み
 ******************************************************************************/
#include "common.h"

/******************************************************************************
 * マクロ等の定義
 ******************************************************************************/

#define SOUND_TIMER_QUEUE(size, value)                                        \
  *((s##size *)(ds->fifo + ds->fifo_top)) = value;                            \
  ds->fifo_top = (ds->fifo_top + 1) % 32;                                     \

#define RENDER_SAMPLE_NULL()                                                  \

#define RENDER_SAMPLE_LEFT()                                                  \
  sound_buffer[buffer_index] += current_sample +                              \
   FP16_16_TO_U32((next_sample - current_sample) * fifo_fractional)           \

#define RENDER_SAMPLE_RIGHT()                                                 \
  sound_buffer[buffer_index + 1] += current_sample +                          \
   FP16_16_TO_U32((next_sample - current_sample) * fifo_fractional)           \

#define RENDER_SAMPLE_BOTH()                                                  \
  dest_sample = current_sample +                                              \
   FP16_16_TO_U32((next_sample - current_sample) * fifo_fractional);          \
  sound_buffer[buffer_index] += dest_sample;                                  \
  sound_buffer[buffer_index + 1] += dest_sample                               \

#define RENDER_SAMPLES(type)                                                  \
  while(fifo_fractional <= 0xFFFF)                                            \
  {                                                                           \
    RENDER_SAMPLE_##type();                                                   \
    fifo_fractional += frequency_step;                                        \
    buffer_index = (buffer_index + 2) % BUFFER_SIZE;                          \
  }                                                                           \

#define UPDATE_VOLUME_CHANNEL_ENVELOPE(channel)                               \
  volume_##channel = gbc_sound_envelope_volume_table[envelope_volume] *       \
   gbc_sound_channel_volume_table[gbc_sound_master_volume_##channel] *        \
   gbc_sound_master_volume_table[gbc_sound_master_volume]                     \

#define UPDATE_VOLUME_CHANNEL_NOENVELOPE(channel)                             \
  volume_##channel = gs->wave_volume *                                        \
   gbc_sound_channel_volume_table[gbc_sound_master_volume_##channel] *        \
   gbc_sound_master_volume_table[gbc_sound_master_volume]                     \

#define UPDATE_VOLUME(type)                                                   \
  UPDATE_VOLUME_CHANNEL_##type(left);                                         \
  UPDATE_VOLUME_CHANNEL_##type(right)                                         \

#define UPDATE_TONE_SWEEP()                                                   \
  if(gs->sweep_status)                                                        \
  {                                                                           \
    u32 sweep_ticks = gs->sweep_ticks - 1;                                    \
                                                                              \
    if(sweep_ticks == 0)                                                      \
    {                                                                         \
      u32 rate = gs->rate;                                                    \
                                                                              \
      if(gs->sweep_direction)                                                 \
        rate = rate - (rate >> gs->sweep_shift);                              \
      else                                                                    \
        rate = rate + (rate >> gs->sweep_shift);                              \
                                                                              \
      if(rate > 2047) {                                                       \
        frequency_step = 0;                                                   \
      } else {                                                                \
        frequency_step = FLOAT_TO_FP16_16(((131072.0 / (2048 - rate)) * 8.0)  \
        / SOUND_FREQUENCY);                                                   \
      }                                                                       \
                                                                              \
      gs->frequency_step = frequency_step;                                    \
      gs->rate = rate;                                                        \
                                                                              \
      sweep_ticks = gs->sweep_initial_ticks;                                  \
    }                                                                         \
    gs->sweep_ticks = sweep_ticks;                                            \
  }                                                                           \

#define UPDATE_TONE_NOSWEEP()                                                 \

#define UPDATE_TONE_ENVELOPE()                                                \
  if(gs->envelope_status)                                                     \
  {                                                                           \
    u32 envelope_ticks = gs->envelope_ticks - 1;                              \
    envelope_volume = gs->envelope_volume;                                    \
                                                                              \
    if(envelope_ticks == 0)                                                   \
    {                                                                         \
      if(gs->envelope_direction)                                              \
      {                                                                       \
        if(envelope_volume != 15)                                             \
          envelope_volume = gs->envelope_volume + 1;                          \
      }                                                                       \
      else                                                                    \
      {                                                                       \
        if(envelope_volume != 0)                                              \
          envelope_volume = gs->envelope_volume - 1;                          \
      }                                                                       \
                                                                              \
      UPDATE_VOLUME(ENVELOPE);                                                \
                                                                              \
      gs->envelope_volume = envelope_volume;                                  \
      gs->envelope_ticks = gs->envelope_initial_ticks;                        \
    }                                                                         \
    else                                                                      \
    {                                                                         \
      gs->envelope_ticks = envelope_ticks;                                    \
    }                                                                         \
  }                                                                           \

#define UPDATE_TONE_NOENVELOPE()                                              \

#define UPDATE_TONE_COUNTERS(envelope_op, sweep_op)                           \
  tick_counter += gbc_sound_tick_step;                                        \
  if(tick_counter > 0xFFFF)                                                   \
  {                                                                           \
    tick_counter &= 0xFFFF;                                                   \
                                                                              \
    if(gs->length_status)                                                     \
    {                                                                         \
      u32 length_ticks = gs->length_ticks - 1;                                \
      gs->length_ticks = length_ticks;                                        \
                                                                              \
      if(length_ticks == 0)                                                   \
      {                                                                       \
        gs->active_flag = 0;                                                  \
        break;                                                                \
      }                                                                       \
    }                                                                         \
                                                                              \
    UPDATE_TONE_##envelope_op();                                              \
    UPDATE_TONE_##sweep_op();                                                 \
  }                                                                           \

#define GBC_SOUND_RENDER_SAMPLE_RIGHT()                                       \
  sound_buffer[buffer_index + 1] += (current_sample * volume_right) >> 22     \

#define GBC_SOUND_RENDER_SAMPLE_LEFT()                                        \
  sound_buffer[buffer_index] += (current_sample * volume_left) >> 22          \

#define GBC_SOUND_RENDER_SAMPLE_BOTH()                                        \
  GBC_SOUND_RENDER_SAMPLE_RIGHT();                                            \
  GBC_SOUND_RENDER_SAMPLE_LEFT()                                              \

#define GBC_SOUND_RENDER_SAMPLES(type, sample_length, envelope_op, sweep_op)  \
  for(i = 0; i < buffer_ticks; i++)                                           \
  {                                                                           \
    current_sample =                                                          \
     sample_data[FP16_16_TO_U32(sample_index) % sample_length];               \
    GBC_SOUND_RENDER_SAMPLE_##type();                                         \
                                                                              \
    sample_index += frequency_step;                                           \
    buffer_index = (buffer_index + 2) % BUFFER_SIZE;                          \
                                                                              \
    UPDATE_TONE_COUNTERS(envelope_op, sweep_op);                              \
  }                                                                           \

#define GBC_NOISE_WRAP_FULL 32767

#define GBC_NOISE_WRAP_HALF 126

#define GET_NOISE_SAMPLE_FULL()                                               \
  current_sample =                                                            \
   ((s32)(noise_table15[FP16_16_TO_U32(sample_index) >> 5] <<                 \
   (FP16_16_TO_U32(sample_index) & 0x1F)) >> 31) & 0x0F                       \

#define GET_NOISE_SAMPLE_HALF()                                               \
  current_sample =                                                            \
   ((s32)(noise_table7[FP16_16_TO_U32(sample_index) >> 5] <<                  \
   (FP16_16_TO_U32(sample_index) & 0x1F)) >> 31) & 0x0F                       \

#define GBC_SOUND_RENDER_NOISE(type, noise_type, envelope_op, sweep_op)       \
  for(i = 0; i < buffer_ticks; i++)                                           \
  {                                                                           \
    GET_NOISE_SAMPLE_##noise_type();                                          \
    GBC_SOUND_RENDER_SAMPLE_##type();                                         \
                                                                              \
    sample_index += frequency_step;                                           \
                                                                              \
    if(sample_index >= U32_TO_FP16_16(GBC_NOISE_WRAP_##noise_type))           \
      sample_index -= U32_TO_FP16_16(GBC_NOISE_WRAP_##noise_type);            \
                                                                              \
    buffer_index = (buffer_index + 2) % BUFFER_SIZE;                          \
    UPDATE_TONE_COUNTERS(envelope_op, sweep_op);                              \
  }                                                                           \

#define GBC_SOUND_RENDER_CHANNEL(type, sample_length, envelope_op, sweep_op)  \
  buffer_index = gbc_sound_buffer_index;                                      \
  sample_index = gs->sample_index;                                            \
  frequency_step = gs->frequency_step;                                        \
  tick_counter = gs->tick_counter;                                            \
                                                                              \
  UPDATE_VOLUME(envelope_op);                                                 \
                                                                              \
  switch(gs->status)                                                          \
  {                                                                           \
    case GBC_SOUND_INACTIVE:                                                  \
      break;                                                                  \
                                                                              \
    case GBC_SOUND_LEFT:                                                      \
      GBC_SOUND_RENDER_##type(LEFT, sample_length, envelope_op, sweep_op);    \
      break;                                                                  \
                                                                              \
    case GBC_SOUND_RIGHT:                                                     \
      GBC_SOUND_RENDER_##type(RIGHT, sample_length, envelope_op, sweep_op);   \
      break;                                                                  \
                                                                              \
    case GBC_SOUND_LEFTRIGHT:                                                 \
      GBC_SOUND_RENDER_##type(BOTH, sample_length, envelope_op, sweep_op);    \
      break;                                                                  \
  }                                                                           \
                                                                              \
  gs->sample_index = sample_index;                                            \
  gs->tick_counter = tick_counter;                                            \

#define GBC_SOUND_LOAD_WAVE_RAM(bank)                                         \
  wave_bank = wave_samples + (bank * 32);                                     \
  for(i = 0, i2 = 0; i < 16; i++, i2 += 2)                                    \
  {                                                                           \
    current_sample = wave_ram[i];                                             \
    wave_bank[i2] = (((current_sample >> 4) & 0x0F) - 8);                     \
    wave_bank[i2 + 1] = ((current_sample & 0x0F) - 8);                        \
  }                                                                           \

#define SOUND_COPY_NORMAL()                                                   \
  current_sample = source[i]                                                  \

#define SOUND_COPY(source_offset, length, render_type)                        \
  _length = (length) / 2;                                                     \
  source = (s16 *)(sound_buffer + source_offset);                             \
  for(i = 0; i < _length; i++)                                                \
  {                                                                           \
    SOUND_COPY_##render_type();                                               \
    if(current_sample > 2047)                                                 \
      current_sample = 2047;                                                  \
    if(current_sample < -2048)                                                \
      current_sample = -2048;                                                 \
                                                                              \
    stream_base[i] = current_sample << 4;                                     \
    source[i] = 0;                                                            \
  }                                                                           \

#define SOUND_COPY_NULL(source_offset, length)                                \
  _length = (length) >> 2;                                                    \
  source = (s16 *)(sound_buffer + source_offset);                             \
  {                                                                           \
    u32 *ptr1 = (u32 *) stream_base;                                          \
    u32 *ptr2 = (u32 *) source;                                               \
    while (_length--) *ptr1++ = *ptr2++ = 0;                                  \
  }                                                                           \

#define SOUND_BUFFER_SIZE 4096

/******************************************************************************
 * グローバル変数の定義
 ******************************************************************************/
// マジカルバケーションの不具合修正
void sound_timer_queue32(u8 channel)
{
  direct_sound_struct *ds = direct_sound_channel + channel;
  u8 offset = channel * 4;
  u8 i;

  for(i = 0xA0; i <= 0xA3; i++)
  {
    ds->fifo[ds->fifo_top] = ADDRESS8(io_registers, i + offset);
    ds->fifo_top = (ds->fifo_top + 1) % 32;
  }
}

/******************************************************************************
 * ローカル変数の定義
 ******************************************************************************/

/******************************************************************************
 * ローカル関数の宣言
 ******************************************************************************/


u32 global_enable_audio = 1;

DIRECT_SOUND_STRUCT direct_sound_channel[2];
GBC_SOUND_STRUCT gbc_sound_channel[4];

//u32 sound_frequency = 44100;

u32 audio_buffer_size_number = 1;
u32 audio_buffer_size;
u32 audio_buffer_size_x2;
u32 sound_on = 0;
s16 sound_buffer[BUFFER_SIZE];    // サウンド バッファ 2n = Left / 2n+1 = Right
u32 sound_buffer_base = 0;
static int sound_handle;          // サウンド スレッドのハンドル
static SceUID sound_thread;

u32 sound_last_cpu_ticks = 0;
FIXED16_16 gbc_sound_tick_step;

u32 gbc_sound_wave_update;

u32 gbc_sound_wave_volume[4] = { 0, 16384, 8192, 4096 };

void sound_callback(/*void *userdata, Uint8 *stream, int length*/);
void init_noise_table(u32 *table, u32 period, u32 bit_length);

// Queue 1, 2, or 4 samples to the top of the DS FIFO, wrap around circularly


void sound_timer_queue8(u32 channel, u8 value)
{
  DIRECT_SOUND_STRUCT *ds = direct_sound_channel + channel;
  SOUND_TIMER_QUEUE(8, value);
}

void sound_timer_queue16(u32 channel, u16 value)
{
  DIRECT_SOUND_STRUCT *ds = direct_sound_channel + channel;
  SOUND_TIMER_QUEUE(8, value & 0xFF);
  SOUND_TIMER_QUEUE(8, value >> 8);
}

void sound_timer_queue32(u32 channel, u32 value)
{
  DIRECT_SOUND_STRUCT *ds = direct_sound_channel + channel;

  SOUND_TIMER_QUEUE(8, value & 0xFF);
  SOUND_TIMER_QUEUE(8, (value >> 8) & 0xFF);
  SOUND_TIMER_QUEUE(8, (value >> 16) & 0xFF);
  SOUND_TIMER_QUEUE(8, value >> 24);
}

// Unqueue 1 sample from the base of the DS FIFO and place it on the audio
// buffer for as many samples as necessary. If the DS FIFO is 16 bytes or
// smaller and if DMA is enabled for the sound channel initiate a DMA transfer
// to the DS FIFO.


void sound_timer(FIXED16_16 frequency_step, u32 channel)
{
  DIRECT_SOUND_STRUCT *ds = direct_sound_channel + channel;

  FIXED16_16 fifo_fractional = ds->fifo_fractional;
  u32 buffer_index = ds->buffer_index;
  s16 current_sample, next_sample, dest_sample;

  current_sample = ds->fifo[ds->fifo_base] << 4;
  ds->fifo_base = (ds->fifo_base + 1) % 32;
  next_sample = ds->fifo[ds->fifo_base] << 4;

  if(sound_on == 1)
  {
    if(ds->volume == DIRECT_SOUND_VOLUME_50)
    {
      current_sample >>= 1;
      next_sample >>= 1;
    }

    switch(ds->status)
    {
      case DIRECT_SOUND_INACTIVE:
        RENDER_SAMPLES(NULL);
        break;

      case DIRECT_SOUND_RIGHT:
        RENDER_SAMPLES(RIGHT);
        break;

      case DIRECT_SOUND_LEFT:
        RENDER_SAMPLES(LEFT);
        break;

      case DIRECT_SOUND_LEFTRIGHT:
        RENDER_SAMPLES(BOTH);
        break;
    }
  }
  else
  {
    RENDER_SAMPLES(NULL);
  }

  ds->buffer_index = buffer_index;
  ds->fifo_fractional = FP16_16_FRACTIONAL_PART(fifo_fractional);

// マジカルバケーションで動作が遅くなるのが改善される
  u8 fifo_length;

  if(ds->fifo_top > ds->fifo_base)
    fifo_length = ds->fifo_top - ds->fifo_base;
  else
    fifo_length = ds->fifo_top + (32 - ds->fifo_base);

  if(fifo_length <= 16)

  if(((ds->fifo_top - ds->fifo_base) % 32) <= 16)
  {
    if(dma[1].direct_sound_channel == channel)
      dma_transfer(dma + 1);

    if(dma[2].direct_sound_channel == channel)
      dma_transfer(dma + 2);
  }
}

void sound_reset_fifo(u32 channel)
{
  DIRECT_SOUND_STRUCT *ds = direct_sound_channel;

  memset(ds->fifo, 0, 32);
}

// Initial pattern data = 4bits (signed)
// Channel volume = 12bits
// Envelope volume = 14bits
// Master volume = 2bits

// Recalculate left and right volume as volume changes.
// To calculate the current sample, use (sample * volume) >> 16

// Square waves range from -8 (low) to 7 (high)

s8 square_pattern_duty[4][8] =
{
  { 0xF8, 0xF8, 0xF8, 0xF8, 0x07, 0xF8, 0xF8, 0xF8 },
  { 0xF8, 0xF8, 0xF8, 0xF8, 0x07, 0x07, 0xF8, 0xF8 },
  { 0xF8, 0xF8, 0x07, 0x07, 0x07, 0x07, 0xF8, 0xF8 },
  { 0x07, 0x07, 0x07, 0x07, 0xF8, 0xF8, 0x07, 0x07 },
};

s8 wave_samples[64];

u32 noise_table15[1024];
u32 noise_table7[4];

u32 gbc_sound_master_volume_table[4] = { 1, 2, 4, 0 };

u32 gbc_sound_channel_volume_table[8] =
{
  FIXED_DIV(0, 7, 12),
  FIXED_DIV(1, 7, 12),
  FIXED_DIV(2, 7, 12),
  FIXED_DIV(3, 7, 12),
  FIXED_DIV(4, 7, 12),
  FIXED_DIV(5, 7, 12),
  FIXED_DIV(6, 7, 12),
  FIXED_DIV(7, 7, 12)
};

u32 gbc_sound_envelope_volume_table[16] =
{
  FIXED_DIV(0, 15, 14),
  FIXED_DIV(1, 15, 14),
  FIXED_DIV(2, 15, 14),
  FIXED_DIV(3, 15, 14),
  FIXED_DIV(4, 15, 14),
  FIXED_DIV(5, 15, 14),
  FIXED_DIV(6, 15, 14),
  FIXED_DIV(7, 15, 14),
  FIXED_DIV(8, 15, 14),
  FIXED_DIV(9, 15, 14),
  FIXED_DIV(10, 15, 14),
  FIXED_DIV(11, 15, 14),
  FIXED_DIV(12, 15, 14),
  FIXED_DIV(13, 15, 14),
  FIXED_DIV(14, 15, 14),
  FIXED_DIV(15, 15, 14)
};

u32 gbc_sound_buffer_index = 0;
u32 gbc_sound_last_cpu_ticks = 0;
u32 gbc_sound_partial_ticks = 0;

u32 gbc_sound_master_volume_left;
u32 gbc_sound_master_volume_right;
u32 gbc_sound_master_volume;


void update_gbc_sound(u32 cpu_ticks)
{
  FIXED16_16 buffer_ticks = FLOAT_TO_FP16_16(((float)(cpu_ticks -
   gbc_sound_last_cpu_ticks) * SOUND_FREQUENCY) / 16777216.0);
  u32 i, i2;
  GBC_SOUND_STRUCT *gs = gbc_sound_channel;
  FIXED16_16 sample_index, frequency_step;
  FIXED16_16 tick_counter;
  u32 buffer_index;
  s32 volume_left, volume_right;
  u32 envelope_volume;
  s32 current_sample;
  u32 sound_status = ADDRESS16(io_registers, 0x84) & 0xFFF0;
  s8 *sample_data;
  s8 *wave_bank;
  u8 *wave_ram = ((u8 *)io_registers) + 0x90;

  gbc_sound_partial_ticks += FP16_16_FRACTIONAL_PART(buffer_ticks);
  buffer_ticks = FP16_16_TO_U32(buffer_ticks);

  if(gbc_sound_partial_ticks > 0xFFFF)
  {
    buffer_ticks += 1;
    gbc_sound_partial_ticks &= 0xFFFF;
  }

//  SDL_LockMutex(sound_mutex);

  if(synchronize_flag)
  {
    if(((gbc_sound_buffer_index - sound_buffer_base) % BUFFER_SIZE) >
     (audio_buffer_size_x2))
    {
//      while(((gbc_sound_buffer_index - sound_buffer_base) % BUFFER_SIZE) >
//       (audio_buffer_size_x2))
//      {
//        SDL_CondWait(sound_cv, sound_mutex);
//      }
//      if(game_config_frameskip_type == auto_frameskip)
//      {
//        sceDisplayWaitVblankStart();
//        real_frame_count = 0;
//        virtual_frame_count = 0;
//      }
    }
  }

  if(sound_on == 1)
  {
    gs = gbc_sound_channel + 0;
    if(gs->active_flag)
    {
      sound_status |= 0x01;
      sample_data = gs->sample_data;
      envelope_volume = gs->envelope_volume;
      GBC_SOUND_RENDER_CHANNEL(SAMPLES, 8, ENVELOPE, SWEEP);
    }

    gs = gbc_sound_channel + 1;
    if(gs->active_flag)
    {
      sound_status |= 0x02;
      sample_data = gs->sample_data;
      envelope_volume = gs->envelope_volume;
      GBC_SOUND_RENDER_CHANNEL(SAMPLES, 8, ENVELOPE, NOSWEEP);
    }

    gs = gbc_sound_channel + 2;
    if(gbc_sound_wave_update)
    {
      GBC_SOUND_LOAD_WAVE_RAM(gs->wave_bank);
      gbc_sound_wave_update = 0;
    }

    if((gs->active_flag) && (gs->master_enable))
    {
      sound_status |= 0x04;
      sample_data = wave_samples;
      if(gs->wave_type == 0)
      {
        if(gs->wave_bank == 1)
          sample_data += 32;

        GBC_SOUND_RENDER_CHANNEL(SAMPLES, 32, NOENVELOPE, NOSWEEP);
      }
      else
      {
        GBC_SOUND_RENDER_CHANNEL(SAMPLES, 64, NOENVELOPE, NOSWEEP);
      }
    }

    gs = gbc_sound_channel + 3;
    if(gs->active_flag)
    {
      sound_status |= 0x08;
      envelope_volume = gs->envelope_volume;

      if(gs->noise_type == 1)
      {
        GBC_SOUND_RENDER_CHANNEL(NOISE, HALF, ENVELOPE, NOSWEEP);
      }
      else
      {
        GBC_SOUND_RENDER_CHANNEL(NOISE, FULL, ENVELOPE, NOSWEEP);
      }
    }
  }

  ADDRESS16(io_registers, 0x84) = sound_status;

  gbc_sound_last_cpu_ticks = cpu_ticks;
  gbc_sound_buffer_index =
   (gbc_sound_buffer_index + (buffer_ticks * 2)) % BUFFER_SIZE;
}


static int sound_update_thread(SceSize args, void *argp)
  {
    return 0;
  }
/*--------------------------------------------------------
  サウンド コールバック
--------------------------------------------------------*/
void sound_callback(/*void *userdata, char *stream, int length*/)
{
/*  u32 sample_length = length / 2;
  u32 _length;
  u32 i;
  s16 *stream_base = (s16 *)stream;
  s16 *source;
  s32 current_sample;

//  SDL_LockMutex(sound_mutex);

  while(((gbc_sound_buffer_index - sound_buffer_base) % BUFFER_SIZE) <
   length)
  {
//    SDL_CondWait(sound_cv, sound_mutex);
  }

  if(global_enable_audio)
  {
    if((sound_buffer_base + sample_length) >= BUFFER_SIZE)
    {
      u32 partial_length = (BUFFER_SIZE - sound_buffer_base) * 2;
      SOUND_COPY(sound_buffer_base, partial_length, normal);
//      source = (s16 *)sound_buffer;
      SOUND_COPY(0, length - partial_length, normal);
      sound_buffer_base = (length - partial_length) / 2;
    }
    else
    {
      SOUND_COPY(sound_buffer_base, length, normal);
      sound_buffer_base += sample_length;
    }
  }
  else
  {
    if((sound_buffer_base + sample_length) >= BUFFER_SIZE)
    {
      u32 partial_length = (BUFFER_SIZE - sound_buffer_base) * 2;
      SOUND_COPY_NULL(sound_buffer_base, partial_length);
//      source = (s16 *)sound_buffer;
      SOUND_COPY_NULL(0, length - partial_length);
      sound_buffer_base = (length - partial_length) / 2;
    }
    else
    {
      SOUND_COPY_NULL(sound_buffer_base, length);
      sound_buffer_base += sample_length;
    }
  }

//  SDL_CondSignal(sound_cv);

//  SDL_UnlockMutex(sound_mutex);
*/
}

// Special thanks to blarrg for the LSFR frequency used in Meridian, as posted
// on the forum at http://meridian.overclocked.org:
// http://meridian.overclocked.org/cgi-bin/wwwthreads/showpost.pl?Board=merid
// angeneraldiscussion&Number=2069&page=0&view=expanded&mode=threaded&sb=4
// Hope you don't mind me borrowing it ^_-

void init_noise_table(u32 *table, u32 period, u32 bit_length)
{
  u32 shift_register = 0xFF;
  u32 mask = ~(1 << bit_length);
  s32 table_pos, bit_pos;
  u32 current_entry;
  u32 table_period = (period + 31) / 32;

  // Bits are stored in reverse order so they can be more easily moved to
  // bit 31, for sign extended shift down.

  for(table_pos = 0; table_pos < table_period; table_pos++)
  {
    current_entry = 0;
    for(bit_pos = 31; bit_pos >= 0; bit_pos--)
    {
      current_entry |= (shift_register & 0x01) << bit_pos;

      shift_register =
       ((1 & (shift_register ^ (shift_register >> 1))) << bit_length) |
       ((shift_register >> 1) & mask);
    }

    table[table_pos] = current_entry;
  }
}

void reset_sound()
{
  DIRECT_SOUND_STRUCT *ds = direct_sound_channel;
  GBC_SOUND_STRUCT *gs = gbc_sound_channel;
  u32 i;

  sound_on = 0;
  sound_buffer_base = 0;
  sound_last_cpu_ticks = 0;
  memset(sound_buffer, 0, audio_buffer_size);

  for(i = 0; i < 2; i++, ds++)
  {
    ds->buffer_index = 0;
    ds->status = DIRECT_SOUND_INACTIVE;
    ds->fifo_top = 0;
    ds->fifo_base = 0;
    ds->fifo_fractional = 0;
    ds->last_cpu_ticks = 0;
    memset(ds->fifo, 0, 32);
  }

  gbc_sound_buffer_index = 0;
  gbc_sound_last_cpu_ticks = 0;
  gbc_sound_partial_ticks = 0;

  gbc_sound_master_volume_left = 0;
  gbc_sound_master_volume_right = 0;
  gbc_sound_master_volume = 0;
  memset(wave_samples, 0, 64);

  for(i = 0; i < 4; i++, gs++)
  {
    gs->status = GBC_SOUND_INACTIVE;
    gs->sample_data = square_pattern_duty[2];
    gs->active_flag = 0;
  }
}

void sound_exit()
{
//  gbc_sound_buffer_index =
//   (sound_buffer_base + audio_buffer_size) % BUFFER_SIZE;
//  SDL_PauseAudio(1);
//  SDL_CondSignal(sound_cv);
}

void init_sound()
{
  audio_buffer_size = (audio_buffer_size_number * 1024) + 2048;
  audio_buffer_size_x2 = audio_buffer_size * 2;

  gbc_sound_tick_step = FLOAT_TO_FP16_16(256.0 / SOUND_FREQUENCY);

  init_noise_table(noise_table15, 32767, 14);
  init_noise_table(noise_table7, 127, 6);

  // 変数等の初期化
  reset_sound();

  // オーディオチャンネルの設定
  sound_handle = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL, audio_buffer_size / 4, PSP_AUDIO_FORMAT_STEREO);
  if (sound_handle < 0)
  {
    quit();
  }

  sound_thread = sceKernelCreateThread("Sound thread", sound_callback/*sound_update_thread*/, 0x08, 0x1000, 0, NULL);
  if (sound_thread < 0)
  {
    sceAudioChRelease(sound_handle);
    sound_handle = -1;
    quit();
  }


  //スレッドの開始
  sceKernelStartThread(sound_thread, 0, 0);

}

#define sound_savestate_body(type)                                          \
{                                                                           \
  FILE_##type##_VARIABLE(savestate_file, sound_on);                         \
  FILE_##type##_VARIABLE(savestate_file, sound_buffer_base);                \
  FILE_##type##_VARIABLE(savestate_file, sound_last_cpu_ticks);             \
  FILE_##type##_VARIABLE(savestate_file, gbc_sound_buffer_index);           \
  FILE_##type##_VARIABLE(savestate_file, gbc_sound_last_cpu_ticks);         \
  FILE_##type##_VARIABLE(savestate_file, gbc_sound_partial_ticks);          \
  FILE_##type##_VARIABLE(savestate_file, gbc_sound_master_volume_left);     \
  FILE_##type##_VARIABLE(savestate_file, gbc_sound_master_volume_right);    \
  FILE_##type##_VARIABLE(savestate_file, gbc_sound_master_volume);          \
  FILE_##type##_ARRAY(savestate_file, wave_samples);                        \
  FILE_##type##_ARRAY(savestate_file, direct_sound_channel);                \
  FILE_##type##_ARRAY(savestate_file, gbc_sound_channel);                   \
}                                                                           \

void sound_read_savestate(FILE_TAG_TYPE savestate_file)
sound_savestate_body(READ);

void sound_write_mem_savestate(FILE_TAG_TYPE savestate_file)
sound_savestate_body(WRITE_MEM);

