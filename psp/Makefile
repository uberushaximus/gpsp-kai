# -x assembler-with-cpp
# gpSP makefile
# Gilead Kutnick - Exophase

# Global definitions

PSPSDK          = ${shell psp-config --pspsdk-path}
PREFIX          = ${shell psp-config --psp-prefix}
COMMON_FILE     = common.h cpu.h memory.h video.h input.h sound.h main.h gui.h zip.h cheats.h fbm_print.h message.h

PSP_EBOOT_ICON  = icon0.png

OBJS            = main.o cpu.o video.o memory.o sound.o input.o
OBJS           += cpu_threaded.o gui.o zip.o cheats.o mips_stub.o fbm_print.o bios.o adhoc.o

TARGET          = UO_gpSP_kai

VERSION_MAJOR = 3
VERSION_MINOR = 1
VERSION_BUILD = 5
VERSION_OPT = test ${VERSION_BUILD}

PSP_EBOOT_TITLE = UnOfficial_gpSP_kai_${VERSION_MAJOR}.${VERSION_MINOR} ${VERSION_OPT}

VPATH           += ..
CFLAGS          += -O3 -DPSP_BUILD -G0 -funsigned-char
CFLAGS          += -Wall -Wredundant-decls -Wmissing-declarations
CFLAGS          += -ffast-math -fforce-addr -fmerge-all-constants -floop-optimize2 -funsafe-loop-optimizations -ftree-loop-linear

CFLAGS          += -DVERSION_MAJOR=${VERSION_MAJOR}
CFLAGS          += -DVERSION_MINOR=${VERSION_MINOR}
CFLAGS          += -DVERSION_BUILD=${VERSION_BUILD}

ASFLAGS         = ${CFLAGS}
EXTRA_TARGETS   = EBOOT.PBP

LIBS            += -lpspgu -lpsprtc -lpspaudio
LIBS            += -lz -lpsppower -lpspnet -lpspwlan -lpspnet_adhoc -lpspnet_adhocctl -lpspnet_adhocmatching

include ${PSPSDK}/lib/build.mak

main.o         : ${COMMON_FILE} main.c

cpu.o          : ${COMMON_FILE} cpu.c

video.o        : ${COMMON_FILE} video.c

memory.o       : ${COMMON_FILE} memory.c

sound.o        : ${COMMON_FILE} sound.c

input.o        : ${COMMON_FILE} input.c

cpu_threaded.o : ${COMMON_FILE} cpu_threaded.c mips_emit.h bios.h

gui.o          : ${COMMON_FILE} gui.c

zip.o          : ${COMMON_FILE} zip.c

cheats.o       : ${COMMON_FILE} cheats.c

mips_stub.o    : mips_stub.S

fbm_print.o    : ${COMMON_FILE} fbm_print.c

bios.o         : ${COMMON_FILE} bios.c bios.h

adhoc.o         : ${COMMON_FILE} adhoc.c adhoc.h

all:
