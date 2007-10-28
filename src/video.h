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

#ifndef VIDEO_H
#define VIDEO_H

#define FONT_WIDTH  6
#define FONT_HEIGHT 10

#define NON_INTERLACE 0
#define INTERLACE     1
#define R4_3  0
#define R16_9 1

void update_scanline();
void update_screen();
void init_video();
void video_resolution_large();
void video_resolution_small();
void print_string(char *str, u16 fg_color, u16 bg_color, u32 x, u32 y);
void print_string_pad(char *str, u16 fg_color, u16 bg_color, u32 x, u32 y, u32 pad);
void print_string_ext(char *str, u16 fg_color, u16 bg_color, u32 x, u32 y, void *_dest_ptr, u32 pitch, u32 pad);
void clear_screen(u16 color);
void blit_to_screen(u16 *src, u32 w, u32 h, u32 x, u32 y);
u16 *copy_screen();
void flip_screen();
void video_read_mem_savestate(FILE_TAG_TYPE savestate_file);
void video_write_mem_savestate(FILE_TAG_TYPE savestate_file);
void video_read_savestate(FILE_TAG_TYPE savestate_file);

void debug_screen_clear();
void debug_screen_start();
void debug_screen_end();
void debug_screen_printf(const char *format, ...);
void debug_screen_printl(const char *format, ...);
void debug_screen_newline(u32 count);
void debug_screen_update();

extern u32 frame_speed;

extern s32 affine_reference_x[2];
extern s32 affine_reference_y[2];

typedef void (* tile_render_function)(u32 layer_number, u32 start, u32 end,
 void *dest_ptr);
typedef void (* bitmap_render_function)(u32 start, u32 end, void *dest_ptr);

typedef struct
{
  tile_render_function normal_render_base;
  tile_render_function normal_render_transparent;
  tile_render_function alpha_render_base;
  tile_render_function alpha_render_transparent;
  tile_render_function color16_render_base;
  tile_render_function color16_render_transparent;
  tile_render_function color32_render_base;
  tile_render_function color32_render_transparent;
} tile_layer_render_struct;

typedef struct
{
  bitmap_render_function normal_render;
} bitmap_layer_render_struct;

typedef enum
{
  unscaled,
  scaled_aspect,
  fullscreen,
} video_scale_type;

typedef enum
{
  filter_nearest,
  filter_bilinear
} video_filter_type;

typedef struct
{
  float u;
  float v;
  float x;
  float y;
  float z;
} VERTEX;

typedef struct
{
  VERTEX p1;
  VERTEX p2;
} SPRITE;

extern u16 *screen_address;
extern u32 screen_pitch;
extern u32 screen_width;
extern u32 screen_height;
extern u32 screen_width2;
extern u32 screen_height2;

extern u32 current_scale;

void set_gba_resolution_small(video_scale_type scale);
void set_gba_resolution_large();

#endif
