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

const float screen_setting_small_init[][10] =
{
  /* PSP */
    /* 4:3 */
      /* non-interlace */
        /* unscaled */
/*             u1,  v1,  x1,      y1,z1,                    u2,                     v2,                     x2,                        y2,z2 */
          {    0,    0, 120,      56, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+120  , GBA_SCREEN_HEIGHT+56     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* aspect */
          { 0.25, 0.25,  36,       0, 0, GBA_SCREEN_WIDTH-0.25, GBA_SCREEN_HEIGHT-0.25, 408+36                , PSP_SCREEN_HEIGHT        , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* fullscreen */
          { 0.25, 0.25,   0,       0, 0, GBA_SCREEN_WIDTH-0.25, GBA_SCREEN_HEIGHT-0.25, PSP_SCREEN_WIDTH      , PSP_SCREEN_HEIGHT        , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 1 */
          {    0,    0, 120,      56, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+120  , GBA_SCREEN_HEIGHT+56     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 2 */
          {    0,    0, 120,      56, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+120  , GBA_SCREEN_HEIGHT+56     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
      /* interlace */
        /* unscaled */
          {    0,    0, 120,      56, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+120  , GBA_SCREEN_HEIGHT+56     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* aspect */
          { 0.25, 0.25,  36,       0, 0, GBA_SCREEN_WIDTH-0.25, GBA_SCREEN_HEIGHT-0.25, 408+36                , PSP_SCREEN_HEIGHT        , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* fullscreen */
          { 0.25, 0.25,   0,       0, 0, GBA_SCREEN_WIDTH-0.25, GBA_SCREEN_HEIGHT-0.25, PSP_SCREEN_WIDTH      , PSP_SCREEN_HEIGHT        , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 1 */
          {    0,    0, 120,      56, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+120  , GBA_SCREEN_HEIGHT+56     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 2 */
          {    0,    0, 120,      56, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+120  , GBA_SCREEN_HEIGHT+56     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
    /* 16:9 */
      /* non-interlace */
        /* unscaled */
          {    0,    0, 120,      56, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+120  , GBA_SCREEN_HEIGHT+56     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* aspect */
          { 0.25, 0.25,  36,       0, 0, GBA_SCREEN_WIDTH-0.25, GBA_SCREEN_HEIGHT-0.25, 408+36                , PSP_SCREEN_HEIGHT        , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* fullscreen */
          { 0.25, 0.25,   0,       0, 0, GBA_SCREEN_WIDTH-0.25, GBA_SCREEN_HEIGHT-0.25, PSP_SCREEN_WIDTH      , PSP_SCREEN_HEIGHT        , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 1 */
          {    0,    0, 120,      56, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+120  , GBA_SCREEN_HEIGHT+56     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 2 */
          {    0,    0, 120,      56, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+120  , GBA_SCREEN_HEIGHT+56     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
      /* interlace */
        /* unscaled */
          {    0,    0, 120,      56, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+120  , GBA_SCREEN_HEIGHT+56     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* aspect */
          { 0.25, 0.25,  36,       0, 0, GBA_SCREEN_WIDTH-0.25, GBA_SCREEN_HEIGHT-0.25, 408+36                , PSP_SCREEN_HEIGHT        , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* fullscreen */
          { 0.25, 0.25,   0,       0, 0, GBA_SCREEN_WIDTH-0.25, GBA_SCREEN_HEIGHT-0.25, PSP_SCREEN_WIDTH      , PSP_SCREEN_HEIGHT        , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 1 */
          {    0,    0, 120,      56, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+120  , GBA_SCREEN_HEIGHT+56     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 2 */
          {    0,    0, 120,      56, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+120  , GBA_SCREEN_HEIGHT+56     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
  /* analog */
    /* 4:3 */
      /* non-interlace */
        /* unscaled */
          {    0,    0, 120,      40, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                        0,  0},
        /* aspect */
          {    0,    0,  20,      10, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 674+20                , 220+10                   , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* fullscreen */
          {    0,    0,   0,       0, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 720                   , 240                      , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 1 */
          {    0,    0, 120,      40, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 2 */
          {    0,    0, 120,      40, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
      /* interlace */
        /* unscaled */
          {    0,    0, 120,      40, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40     , 0},
          {    0,    0, 120,  40+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40+262 , 0},
        /* aspect */
          {    0,    0,  20,      10, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 674+20                , 220+10                   , 0},
          {    0,    0,  20,  10+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 674+20                , 220+10+262               , 0},
        /* fullscreen */
          {    0,    0,   0,       0, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 720                 , 240                        , 0},
          {    0,    0,   0,   0+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 720                 , 240+262                    , 0},
        /* etc 1 */
          {    0,    0, 120,      40, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40     , 0},
          {    0,    0, 120,  40+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40+262 , 0},
        /* etc 2 */
          {    0,    0, 120,      40, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40     , 0},
          {    0,    0, 120,  40+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40+262 , 0},
    /* 16:9 */
      /* non-interlace */
        /* unscaled */
          {    0,    0, 120,      40, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* aspect */
          {    0,    0,  20,      10, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 674+20                , 220+10                   , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* fullscreen */
          {    0,    0,   0,       0, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 720                   , 240                      , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 1 */
          {    0,    0, 120,      40, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 2 */
          {    0,    0, 120,      40, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40     , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
      /* interlace */
        /* unscaled */
          {    0,    0, 120,      40, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40     , 0},
          {    0,    0, 120,  40+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40+262 , 0},
        /* aspect */
          {    0,    0,  20,      10, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 674+20                , 220+10                   , 0},
          {    0,    0,  20,  10+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 674+20                , 220+10+262               , 0},
        /* fullscreen */
          {    0,    0,   0,       0, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 720                   , 240                      , 0},
          {    0,    0,   0,   0+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 720                   , 240+262                  , 0},
        /* etc 1 */
          {    0,    0, 120,      40, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40     , 0},
          {    0,    0, 120,  40+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40+262 , 0},
        /* etc 2 */
          {    0,    0, 120,      40, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40     , 0},
          {    0,    0, 120,  40+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH*2+120, GBA_SCREEN_HEIGHT+40+262 , 0},
  /* digital */
    /* 4:3 */
      /* non-interlace */
        /* unscaled */
          {    0,    0, 240,     160, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160    , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* aspect */
          {    0,    0,  15,      10, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 675+15                , 450+10                   , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* fullscreen */
          {    0,    0,   0,       0, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 720                   , 480                      , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 1 */
          {    0,    0, 240,     160, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160    , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 2 */
          {    0,    0, 240,     160, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160    , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
      /* interlace */
        /* unscaled */
          {    0,    0, 240,     160, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160    , 0},
          {    0,    0, 240, 160+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160+262, 0},
        /* aspect */
          {    0,    0,  15,      10, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 675+15                , 450+10                   , 0},
          {    0,    0,  15,  10+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 675+15                , 450+10+262               , 0},
        /* fullscreen */
          {    0,    0,   0,       0, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 720                   , 480                      , 0},
          {    0,    0,   0,   0+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 720                   , 480+262                  , 0},
        /* etc 1 */
          {    0,    0, 240,     160, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160    , 0},
          {    0,    0, 240, 160+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160+262, 0},
        /* etc 2 */
          {    0,    0, 240,     160, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160    , 0},
          {    0,    0, 240, 160+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160+262, 0},
    /* 16:9 */
      /* non-interlace */
        /* unscaled */
          {    0,    0, 240,     160, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160    , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* aspect */
          {    0,    0,  15,      10, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 675+15                , 450+10                   , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* fullscreen */
          {    0,    0,   0,       0, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 720                   , 480                      , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 1 */
          {    0,    0, 240,     160, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160    , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
        /* etc 2 */
          {    0,    0, 240,     160, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160    , 0},
          {    0,    0,   0,       0, 0,                     0,                      0,                      0,                         0, 0},
      /* interlace */
        /* unscaled */
          {    0,    0, 240,     160, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160    , 0},
          {    0,    0, 240, 160+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160+262, 0},
        /* aspect */
          {    0,    0,  15,      10, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 675+15                , 450+10                   , 0},
          {    0,    0,  15,  10+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 675+15                , 450+10+262               , 0},
        /* fullscreen */
          {    0,    0,   0,       0, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 720                   , 480                      , 0},
          {    0,    0,   0,   0+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , 720                   , 480+262                  , 0},
        /* etc 1 */
          {    0,    0, 240,     160, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160    , 0},
          {    0,    0, 240, 160+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160+262, 0},
        /* etc 2 */
          {    0,    0, 240,     160, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160    , 0},
          {    0,    0, 240, 160+262, 0, GBA_SCREEN_WIDTH     , GBA_SCREEN_HEIGHT     , GBA_SCREEN_WIDTH+240  , GBA_SCREEN_HEIGHT+160+262, 0}
};

#endif
