# -x assembler-with-cpp
# gpSP makefile
# Gilead Kutnick - Exophase

# Global definitions

all: systembuttons_prx dvemgr homehookprx main

clean: systembuttons_prx_clean  dvemgr_clean homehookprx_clean main_clean

dvemgr:
	make -C ./prx/dvemgr/

main:
	./counter.pl
	make -C ./gpsp/

homehookprx:
	make -C ./prx/homehookprx/

systembuttons_prx:
	make -C ./prx/systembuttons_prx/

dvemgr_clean:
	make -C ./prx/dvemgr/ clean

main_clean:
	make -C ./gpsp/ clean

homehookprx_clean:
	make -C ./prx/homehookprx/ clean

systembuttons_prx_clean:
	make -C ./prx/systembuttons_prx/ clean
	