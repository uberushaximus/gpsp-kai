# -x assembler-with-cpp
# gpSP makefile
# Gilead Kutnick - Exophase

# Global definitions

clean: dvemgr homehookprx gpsp

all: dvemgr homehookprx main

dvemgr:
	make -C ./prx/dvemgr/

main:
		make -C ./gpsp/

homehookprx:
	make -C ./prx/homehookprx/
