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

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fastmath.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <pspdisplay.h>
#include <pspaudio.h>
#include <pspaudiolib.h>
#include <psprtc.h>
#include <psppower.h>
#include <psputility_sysparam.h>
#include <pspsdk.h>
#include <psputilsforkernel.h>
#include <pspnet.h>
#include <psputility_netmodules.h>
#include <pspnet_adhoc.h>
#include <pspnet_adhocctl.h>
#include <pspnet_adhocmatching.h>
#include <pspwlan.h>
#include <psputility_osk.h>

typedef s32 FILE_TAG_TYPE;
typedef u32 FIXED16_16;

#define MAX_PATH 512
#define MAX_FILE 64
#define FILE_ID SceUID

#define ROR(dest, value, shift)                                             \
  dest = ((value) >> (shift)) | ((value) << (32 - (shift)))                 \

#define CONVERT_PALETTE(value)                                              \
  value = (((value) & 0x7FE0) << 1) | ((value) & 0x1F)                      \

#define PSP_FILE_OPEN_APPEND (PSP_O_CREAT | PSP_O_APPEND | PSP_O_TRUNC)

#define PSP_FILE_OPEN_READ PSP_O_RDONLY

#define PSP_FILE_OPEN_WRITE (PSP_O_CREAT | PSP_O_WRONLY | PSP_O_TRUNC)

#define FILE_OPEN(filename_tag, filename, mode)                             \
  filename_tag = sceIoOpen(filename, PSP_FILE_OPEN_##mode, 0777)            \

#define FILE_CHECK_VALID(filename_tag)                                      \
  (filename_tag >= 0)                                                       \

#define FILE_CLOSE(filename_tag)                                            \
  sceIoClose(filename_tag)                                                  \

#define FILE_DELETE(filename)                                               \
  sceIoRemove(filename)                                                     \

#define FILE_READ(filename_tag, buffer, size)                               \
  sceIoRead(filename_tag, buffer, size)                                     \

#define FILE_WRITE(filename_tag, buffer, size)                              \
  sceIoWrite(filename_tag, buffer, size)                                    \

// type = FILE_OPEN_READ / FILE_OPEN_WRITE
#define FILE_SEEK(filename_tag, offset, type)                               \
  sceIoLseek(filename_tag, offset, PSP_##type)                              \

#define FILE_WRITE_MEM(filename_tag, buffer, size)                          \
{                                                                           \
  memcpy(write_mem_ptr, buffer, size);                                      \
  write_mem_ptr += size;                                                    \
}                                                                           \

// These must be variables, not constants.

#define FILE_READ_VARIABLE(filename_tag, variable)                            \
  FILE_READ(filename_tag, &variable, sizeof(variable))                        \

#define FILE_WRITE_VARIABLE(filename_tag, variable)                           \
  FILE_WRITE(filename_tag, &variable, sizeof(variable))                       \

#define FILE_WRITE_MEM_VARIABLE(filename_tag, variable)                     \
  FILE_WRITE_MEM(filename_tag, &variable, sizeof(variable))                 \

// These must be statically declared arrays (ie, global or on the stack,
// not dynamically allocated on the heap)

#define FILE_READ_ARRAY(filename_tag, array)                                  \
  FILE_READ(filename_tag, array, sizeof(array))                               \

#define FILE_WRITE_ARRAY(filename_tag, array)                                 \
  FILE_WRITE(filename_tag, array, sizeof(array))                              \

#define FILE_WRITE_MEM_ARRAY(filename_tag, array)                           \
  FILE_WRITE_MEM(filename_tag, array, sizeof(array))                        \

#define FLOAT_TO_FP16_16(value)                                               \
  (FIXED16_16)((value) * 65536.0)                                             \

#define FP16_16_TO_FLOAT(value)                                               \
  (float)((value) / 65536.0)                                                  \

#define U32_TO_FP16_16(value)                                                 \
  ((value) << 16)                                                             \

#define FP16_16_TO_U32(value)                                                 \
  ((value) >> 16)                                                             \

#define FP16_16_FRACTIONAL_PART(value)                                        \
  ((value) & 0xFFFF)                                                          \

#define FIXED_DIV(numerator, denominator, bits)                               \
  ((((numerator) * (1 << (bits))) + ((denominator) / 2)) / (denominator))     \

#define ADDRESS8(base, offset)                                                \
  *((u8 *)((u8 *)(base) + (offset)))                                          \

#define ADDRESS16(base, offset)                                               \
  *((u16 *)((u8 *)(base) + (offset)))                                         \

#define ADDRESS32(base, offset)                                               \
  *((u32 *)((u8 *)(base) + (offset)))                                         \

#define printf pspDebugScreenPrintf

#define USE_BIOS 0
#define EMU_BIOS 1

// デバッグ用の設定
#define DBG_FILE_NAME "dbg_msg.txt"
#define DBGOUT(...) fprintf(dbg_file, __VA_ARGS__)
FILE *dbg_file;

#include "cpu.h"
#include "memory.h"
#include "video.h"
#include "input.h"
#include "sound.h"
#include "main.h"
#include "gui.h"
#include "zip.h"
#include "cheats.h"
#include "fbm_print.h"
#include "message.h"
#include "bios.h"
#include "adhoc.h"

#endif /* COMMON_H */
