# -x assembler-with-cpp
# gpSP makefile
# Gilead Kutnick - Exophase

# Global definitions

include ../Makefile.set

PSPSDK          = ${shell psp-config --pspsdk-path}
PREFIX          = ${shell psp-config --psp-prefix}

PSP_EBOOT_ICON  = ../image/icon1.png
#文字列が表示されないので保留
#PSP_EBOOT_PIC1  = ../image/pic1.png 

OBJS            = main.o cpu.o video.o memory.o sound.o input.o
OBJS           += cpu_threaded.o gui.o zip.o cheats.o mips_stub.o fbm_print.o bios.o draw.o video_setting.o
OBJS           += pspDveManager.o homehook.o

ifeq ($(ADHOC_MODE), 1)
OBJS           += adhoc.o
COMMON_FILE += adhoc.h
endif

TARGET          = UO_gpSP_kai

PSP_FW_VERSION=300
VERSION_MODE = user

ifeq ($(M64_MODE), 1)
VERSION_MODE += 64M
endif

ifeq ($(TEST_MODE), 1)
VERSION_RELEASE = test
endif

VERSION_OPT = ${VERSION_RELEASE} ${VERSION_BUILD}

PSP_EBOOT_TITLE = UO gpSP kai ${VERSION_MAJOR}.${VERSION_MINOR} ${VERSION_OPT}

VPATH           += ..
CFLAGS          += -O3 -DPSP_BUILD -G0 -funsigned-char
CFLAGS          += -Wall
#CFLAGS          += -W -Wno-sign-compare
#CFLAGS          += -Wshadow -Wbad-function-cast -Wconversion -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes -Wnested-externs -Wredundant-decls 
CFLAGS          +=  -funsigned-char -ffast-math -fforce-addr -fmerge-all-constants -floop-optimize2 -funsafe-loop-optimizations -ftree-loop-linear

CFLAGS          += -DVERSION_MAJOR=${VERSION_MAJOR}
CFLAGS          += -DVERSION_MINOR=${VERSION_MINOR}
CFLAGS          += -DVERSION_BUILD=${VERSION_BUILD}

CFLAGS          += -DUSER_MODE

ifeq ($(M64_MODE), 1)
CFLAGS          += -DM64_MODE
endif

ifeq ($(TEST_MODE), 1)
CFLAGS          += -DTEST_MODE
endif

ifeq ($(ADHOC_MODE), 1)
CFLAGS          += -DADHOC_MODE
endif

ifeq ($(C_CORE_MODE), 1)
CFLAGS          += -DC_CORE_MODE
endif

ASFLAGS         = ${CFLAGS}
EXTRA_TARGETS   = EBOOT.PBP

LIBS            += -lpspgu -lpsprtc -lpspaudio -lz -lpsppower -lpspkubridge

ifdef ADHOC_MODE
LIBS            += -lpspnet -lpspwlan -lpspnet_adhoc -lpspnet_adhocctl -lpspnet_adhocmatching
endif

include ${PSPSDK}/lib/build.mak

SRC_DIR = ../src/

COMMON_FILE       = ${SRC_DIR}common.h \
                                 ${SRC_DIR}cpu.h \
                                 ${SRC_DIR}memory.h \
                                 ${SRC_DIR}video.h \
                                 ${SRC_DIR}input.h \
                                 ${SRC_DIR}sound.h \
                                 ${SRC_DIR}main.h \
                                 ${SRC_DIR}gui.h \
                                 ${SRC_DIR}zip.h \
                                 ${SRC_DIR}cheats.h

UI_COMMON_FILE  = ${SRC_DIR}fbm_print.h \
                                 ${SRC_DIR}draw.h \
                                 ${SRC_DIR}message.h

main.o					: ${COMMON_FILE} ${UI_COMMON_FILE} ${SRC_DIR}main.c
cpu.o						: ${COMMON_FILE} ${SRC_DIR}cpu.c
video.o					: ${COMMON_FILE} ${SRC_DIR}dvemgr.h ${SRC_DIR}video.c 
memory.o				: ${COMMON_FILE} ${SRC_DIR}memory.c
sound.o					: ${COMMON_FILE} ${SRC_DIR}sound.c
input.o					: ${COMMON_FILE} ${UI_COMMON_FILE} ${SRC_DIR}homehook.h ${SRC_DIR}input.c
cpu_threaded.o		: ${COMMON_FILE} ${SRC_DIR}cpu_threaded.c ${SRC_DIR}mips_emit.h ${SRC_DIR}bios.h
gui.o						: ${COMMON_FILE} ${UI_COMMON_FILE} ${SRC_DIR}homehook.h ${SRC_DIR}gui.c
zip.o						: ${COMMON_FILE} ${SRC_DIR}zip.c
cheats.o				: ${COMMON_FILE} ${SRC_DIR}cheats.c
mips_stub.o			: ${SRC_DIR}mips_stub.S
fbm_print.o			: ${COMMON_FILE} ${UI_COMMON_FILE} ${SRC_DIR}fbm_print.c
bios.o					: ${COMMON_FILE} ${SRC_DIR}bios.c ${SRC_DIR}bios.h
draw.o					: ${COMMON_FILE} ${UI_COMMON_FILE} ${SRC_DIR}draw.c ${SRC_DIR}draw.h
pspDveManager.o  : ${SRC_DIR}pspDveManager.S
homehook.o			: ${SRC_DIR}homehook.S
video_setting.o		: ${COMMON_FILE} ${SRC_DIR}video_setting.c

ifdef ADHOC_MODE
adhoc.o					: ${COMMON_FILE} ${SRC_DIR}adhoc.c ${SRC_DIR}adhoc.h
endif

%.o: ../src/%.c
	@echo Compiling $<...
	@$(CC) $(CDEFS) $(CFLAGS) -c $< -o$@

%.o: ../src/%.S
	@echo Assembling $<...
	@$(CC) $(CDEFS) $(CFLAGS) -c $< -o$@

%.o: ../src/%.s
	@echo Assembling $<...
	@$(AS) $(ASDEFS) $(ASFLAGS) -c $< -o$@

%.a:
	@echo Archiving $@...
	@$(AR) -r $@ $^

all:
	rm ./PARAM.SFO
	cp ./*.prx ../obj/
	cp ./*.PBP ../obj/
