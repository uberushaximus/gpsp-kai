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

#include "common.h"

// VIDEO OUTの有効画素は660x440(GBAの2.75倍)
// インタレース時の有効画素は660x220},

const SCREEN_PARAMETER screen_parameter_psp_menu_init =
{
    // LCD OUT / MENU SCREEN (FIX)
    {0, 0x000, 480, 272, 1, 15, 0},
    {0, 0},
    {480, 480, 272},
    {9, 9},
    {480, 272},
    {0, 0, 480, 272},
    {0, 0, 0, 0, 0, 479, 271, 479, 271, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const SCREEN_PARAMETER screen_parameter_composite_menu_init[] =
{
  {
    // Composite OUT / MENU SCREEN / 4:3 / Progressive (FIX)
    {2, 0x1D2, 720, 480, 1, 15, 0},
    {1, 1},
    {480, 480, 272},
    {9, 9},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 30, 20, 0, 479.75, 271.75, 689, 459, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Composite OUT / MENU SCREEN / 4:3 / Interlace (FIX)
    {2, 0x1D1, 720, 503, 1, 15, 0},
    {1, 1},
    {480, 480, 272},
    {9, 9},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 30, 10, 0, 479.75, 272.75, 689, 229, 0},
    {0.25, 0.25, 30, 272, 0, 479.75, 271.75, 689, 491, 0}
  },
  {
    // Composite OUT / MENU SCREEN / 16:9 / Progressive (FIX)
    {2, 0x1D2, 720, 480, 1, 15, 0},
    {1, 1},
    {480, 480, 272},
    {9, 9},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 30, 53, 0, 479.75, 271.75, 689, 426, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Composite OUT / MENU SCREEN / 16:9 / Interlace (FIX)
    {2, 0x1D1, 720, 503, 1, 15, 0},
    {1, 1},
    {480, 480, 272},
    {9, 9},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 30, 27, 0, 479.75, 272.75, 689, 212, 0},
    {0.25, 0.25, 30, 289, 0, 479.75, 271.75, 689, 474, 0}
  }
};

const SCREEN_PARAMETER screen_parameter_component_menu_init[] =
{
  {
    // Component OUT / MENU SCREEN / 4:3 / Progressive (FIX)
    {0, 0x1D2, 720, 480, 1, 15, 0},
    {1, 1},
    {480, 480, 272},
    {9, 9},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 30, 20, 0, 479.75, 271.75, 689, 459, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Component OUT / MENU SCREEN / 4:3 / Interlace (FIX)
    {0, 0x1D1, 720, 503, 1, 15, 0},
    {1, 1},
    {480, 480, 272},
    {9, 9},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 30, 10, 0, 479.75, 272.75, 689, 229, 0},
    {0.25, 0.25, 30, 272, 0, 479.75, 271.75, 689, 491, 0}
  },
  {
    // Component OUT / MENU SCREEN / 16:9 / Progressive (FIX)
    {0, 0x1D2, 720, 480, 1, 15, 0},
    {1, 1},
    {480, 480, 272},
    {9, 9},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 30, 53, 0, 479.75, 271.75, 689, 426, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Component OUT / MENU SCREEN / 16:9 / Interlace (FIX)
    {0, 0x1D1, 720, 503, 1, 15, 0},
    {1, 1},
    {480, 480, 272},
    {9, 9},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 30, 27, 0, 479.75, 272.75, 689, 212, 0},
    {0.25, 0.25, 30, 289, 0, 479.75, 271.75, 689, 474, 0}
  }
};


const SCREEN_PARAMETER screen_parameter_psp_game_init[] =
{
  {
    // LCD OUT / GAME SCREEN / unscaled (FIX)
    {0, 0x000, 480, 272, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {480, 272},
    {0, 0, 480, 272},
    {0, 0, 120, 56, 0, 240, 160, 359, 215, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // LCD OUT / GAME SCREEN / aspect(FIX)
    {0, 0x000, 480, 272, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {480, 272},
    {0, 0, 480, 272},
    {0.25, 0.25, 36, 0, 0, 239.75, 159.75, 443, 271, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // LCD OUT / GAME SCREEN / fullscreen (FIX)
    {0, 0x000, 480, 272, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {480, 272},
    {0, 0, 480, 272},
    {0.25, 0.25, 0, 0, 0, 239.75, 159.75, 479, 271, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // LCD OUT / GAME SCREEN / option 1 180 rotate (FIX)
    {0, 0x000, 480, 272, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {480, 272},
    {0, 0, 480, 272},
    {239.75, 159.75, 36, 0, 0, 0.25, 0.25, 443, 271, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // LCD OUT / GAME SCREEN / option 2 (FIX)
    {0, 0x000, 480, 272, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {480, 272},
    {0, 0, 480, 272},
    {0, 0, 120, 56, 0, 240, 160, 359, 215, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  }
};

const SCREEN_PARAMETER screen_parameter_composite_game_init[] =
{
  {
    // Composite OUT / GAME SCREEN / 4:3 / Progressive / unscaled (FIX)
    {2, 0x1D2, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 240, 160, 0, 239.75, 159.75, 479, 319, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 4:3 / Progressive / aspect (FIX)
    {2, 0x1D2, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 30, 20, 0, 239.75, 159.75, 689, 459, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 4:3 / Progressive / fullscreen (FIX)
    {2, 0x1D2, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 30, 20, 0, 239.75, 159.75, 689, 459, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 4:3 / Progressive / option 1 (FIX)
    {2, 0x1D2, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 240, 160, 0, 239.75, 159.75, 479, 319, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 4:3 / Progressive / option 2 (FIX)
    {2, 0x1D2, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 240, 160, 0, 239.75, 159.75, 479, 319, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 4:3 / Interlace / unscaled (FIX)
    {2, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 240, 80, 0, 239.75, 160.75, 479, 159, 0},
    {0.25, 0.25, 240, 342, 0, 239.75, 159.75, 479, 421, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 4:3 / Interlace / aspect (FIX)
    {2, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 30, 10, 0, 239.75, 159.75, 689, 229, 0},
    {0.25, 0.25, 30, 272, 0, 239.75, 159.75, 689, 491, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 4:3 / Interlace / fullscreen (FIX)
    {2, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 30, 10, 0, 239.75, 159.75, 689, 229, 0},
    {0.25, 0.25, 30, 272, 0, 239.75, 159.75, 689, 491, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 4:3 / Interlace / option 1 (FIX)
    {2, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 240, 80, 0, 239.75, 160.75, 479, 159, 0},
    {0.25, 0.25, 240, 342, 0, 239.75, 159.75, 479, 421, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 4:3 / Interlace / option 2 (FIX)
    {2, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 240, 80, 0, 239.75, 160.75, 479, 159, 0},
    {0.25, 0.25, 240, 342, 0, 239.75, 159.75, 479, 421, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 16:9 / Progressive / unscaled (FIX)
    {2, 0x1D2, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 240, 160, 0, 239.75, 159.75, 479, 319, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 16:9 / Progressive / aspect (FIX)
    {2, 0x1D2, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 30, 20, 0, 239.75, 159.75, 689, 459, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 16:9 / Progressive / fullscreen (FIX)
    {2, 0x1D2, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 30, 53, 0, 239.75, 159.75, 689, 426, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 16:9 / Progressive / option 1 (FIX)
    {2, 0x1D2, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 240, 160, 0, 239.75, 159.75, 479, 319, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 16:9 / Progressive / option 2 (FIX)
    {2, 0x1D2, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 240, 160, 0, 239.75, 159.75, 479, 319, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 16:9 / Interlace / unscaled (FIX)
    {2, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 240, 80, 0, 239.75, 160.75, 479, 159, 0},
    {0.25, 0.25, 240, 342, 0, 239.75, 159.75, 479, 421, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 16:9 / Interlace / aspect (FIX)
    {2, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 30, 10, 0, 239.75, 159.75, 689, 229, 0},
    {0.25, 0.25, 30, 272, 0, 239.75, 159.75, 689, 491, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 16:9 / Interlace / fullscreen (FIX)
    {2, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 30, 27, 0, 239.75, 159.75, 689, 212, 0},
    {0.25, 0.25, 30, 289, 0, 239.75, 159.75, 689, 474, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 16:9 / Interlace / option 1 (FIX)
    {2, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 240, 80, 0, 239.75, 160.75, 479, 159, 0},
    {0.25, 0.25, 240, 342, 0, 239.75, 159.75, 479, 421, 0}
  },
  {
    // Composite OUT / GAME SCREEN / 16:9 / Interlace / option 2 (FIX)
    {2, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 240, 80, 0, 239.75, 160.75, 479, 159, 0},
    {0.25, 0.25, 240, 342, 0, 239.75, 159.75, 479, 421, 0}
  }
};

const SCREEN_PARAMETER screen_parameter_component_game_init[] =
{
  {
    // Component OUT / GAME SCREEN / 4:3 / Progressive / unscaled (FIX)
    {0, 0x1D2, 720, 480, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 240, 160, 0, 239.75, 159.75, 479, 319, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Component OUT / GAME SCREEN / 4:3 / Progressive / aspect (FIX)
    {0, 0x1D2, 720, 480, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 30, 20, 0, 239.75, 159.75, 689, 459, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Component OUT / GAME SCREEN / 4:3 / Progressive / fullscreen (FIX)
    {0, 0x1D2, 720, 480, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 30, 20, 0, 239.75, 159.75, 689, 459, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Component OUT / GAME SCREEN / 4:3 / Progressive / option 1 (FIX)
    {0, 0x1D2, 720, 480, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 240, 160, 0, 239.75, 159.75, 479, 319, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Component OUT / GAME SCREEN / 4:3 / Progressive / option 2 (FIX)
    {0, 0x1D2, 720, 480, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 240, 160, 0, 239.75, 159.75, 479, 319, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Component OUT / GAME SCREEN / 4:3 / Interlace / unscaled (FIX)
    {0, 0x1D1, 720, 503, 1, 15, 0},
    {1, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 240, 80, 0, 239.75, 160.75, 479, 159, 0},
    {0.25, 0.25, 240, 342, 0, 239.75, 159.75, 479, 421, 0}
  },
  {
    // Component OUT / GAME SCREEN / 4:3 / Interlace / aspect (FIX)
    {0, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 30, 10, 0, 239.75, 159.75, 689, 229, 0},
    {0.25, 0.25, 30, 272, 0, 239.75, 159.75, 689, 491, 0}
  },
  {
    // Component OUT / GAME SCREEN / 4:3 / Interlace / fullscreen (FIX)
    {0, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 30, 10, 0, 239.75, 159.75, 689, 229, 0},
    {0.25, 0.25, 30, 272, 0, 239.75, 159.75, 689, 491, 0}
  },
  {
    // Component OUT / GAME SCREEN / 4:3 / Interlace / option 1 (FIX)
    {0, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 240, 80, 0, 239.75, 160.75, 479, 159, 0},
    {0.25, 0.25, 240, 342, 0, 239.75, 159.75, 479, 421, 0}
  },
  {
    // Component OUT / GAME SCREEN / 4:3 / Interlace / option 2 (FIX)
    {0, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 240, 80, 0, 239.75, 160.75, 479, 159, 0},
    {0.25, 0.25, 240, 342, 0, 239.75, 159.75, 479, 421, 0}
  },
  {
    // Component OUT / GAME SCREEN / 16:9 / Progressive / unscaled (FIX)
    {0, 0x1D2, 720, 480, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 240, 160, 0, 239.75, 159.75, 479, 319, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Component OUT / GAME SCREEN / 16:9 / Progressive / aspect (FIX)
    {0, 0x1D2, 720, 480, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 30, 20, 0, 239.75, 159.75, 689, 459, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Component OUT / GAME SCREEN / 16:9 / Progressive / fullscreen (FIX)
    {0, 0x1D2, 720, 480, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 30, 53, 0, 239.75, 159.75, 689, 426, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Component OUT / GAME SCREEN / 16:9 / Progressive / option 1 (FIX)
    {0, 0x1D2, 720, 480, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 240, 160, 0, 239.75, 159.75, 479, 319, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Component OUT / GAME SCREEN / 16:9 / Progressive / option 2 (FIX)
    {0, 0x1D2, 720, 480, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 480},
    {0, 0, 720, 480},
    {0.25, 0.25, 240, 160, 0, 239.75, 159.75, 479, 319, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // Component OUT / GAME SCREEN / 16:9 / Interlace / unscaled (FIX)
    {0, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 240, 80, 0, 239.75, 160.75, 479, 159, 0},
    {0.25, 0.25, 240, 342, 0, 239.75, 159.75, 479, 421, 0}
  },
  {
    // Component OUT / GAME SCREEN / 16:9 / Interlace / aspect (FIX)
    {0, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 0.25, 30, 10, 0, 239.75, 159.75, 689, 229, 0},
    {0.25, 0.25, 30, 272, 0, 239.75, 159.75, 689, 491, 0}
  },
  {
    // Component OUT / GAME SCREEN / 16:9 / Interlace / fullscreen (FIX)
    {0, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 480},
    {0.25, 0.25, 30, 27, 0, 239.75, 159.75, 689, 212, 0},
    {0.25, 0.25, 30, 289, 0, 239.75, 159.75, 689, 474, 0}
  },
  {
    // Component OUT / GAME SCREEN / 16:9 / Interlace / option 1 (FIX)
    {0, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 240, 80, 0, 239.75, 160.75, 479, 159, 0},
    {0.25, 0.25, 240, 342, 0, 239.75, 159.75, 479, 421, 0}
  },
  {
    // Component OUT / GAME SCREEN / 16:9 / Interlace / option 2 (FIX)
    {0, 0x1D1, 720, 503, 1, 15, 0},
    {0, 1},
    {240, 240, 160},
    {8, 8},
    {720, 503},
    {0, 0, 720, 503},
    {0.25, 1.25, 240, 80, 0, 239.75, 160.75, 479, 159, 0},
    {0.25, 0.25, 240, 342, 0, 239.75, 159.75, 479, 421, 0}
  }
};