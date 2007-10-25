# -x assembler-with-cpp
# gpSP makefile
# Gilead Kutnick - Exophase

# Global definitions

clean: systembuttons_prx dvemgr homehookprx gpsp

all: systembuttons_prx dvemgr homehookprx main

dvemgr:
	make -C ./prx/dvemgr/

main:
		make -C ./gpsp/

homehookprx:
	make -C ./prx/homehookprx/

systembuttons_prx:
	make -C ./prx/systembuttons_prx/
