# -x assembler-with-cpp
# gpSP makefile
# Gilead Kutnick - Exophase

# Global definitions

PSPSDK          = ${shell psp-config --pspsdk-path}
PREFIX          = ${shell psp-config --psp-prefix}

include ${PSPSDK}/lib/build.mak

clean: dvemgr homehookprx gpsp

all: dvemgr homehookprx rmfile gpsp

gpsp:
	make -C ./gpsp/

dvemgr:
	make -C ./prx/dvemgr/

read_ctrl:
	make -C ./prx/read_ctrl/

rmfile:
	rm ./gpsp/PARAM.SFO
	