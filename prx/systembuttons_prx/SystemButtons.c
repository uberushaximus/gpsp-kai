#include <pspmoduleexport.h>
#define NULL ((void *) 0)

void extern module_start;
void extern module_info;
static const unsigned int __syslib_exports[4] __attribute__((section(".rodata.sceResident"))) = {
	0xD632ACDB,
	0xF01D73A7,
	(unsigned int) &module_start,
	(unsigned int) &module_info,
};

void extern initSystemButtons;
void extern readSystemButtons;
void extern readHomeButton;
void extern readVolumeButtons;
void extern readVolUpButton;
void extern readVolDownButton;
void extern readNoteButton;
void extern readScreenButton;
void extern readHoldSwitch;
void extern readWLANSwitch;
void extern readMainVolume;
static const unsigned int __SystemButtons_exports[22] __attribute__((section(".rodata.sceResident"))) = {
	0x494C24C0,
	0xC3E44941,
	0x40DC34E4,
	0x1BED91EE,
	0x4E7C27BF,
	0x8F7FA8C3,
	0x52DF22A1,
	0xE6884A40,
	0x32A5C599,
	0x3337F3E1,
	0x8DDE496C,
	(unsigned int) &initSystemButtons,
	(unsigned int) &readSystemButtons,
	(unsigned int) &readHomeButton,
	(unsigned int) &readVolumeButtons,
	(unsigned int) &readVolUpButton,
	(unsigned int) &readVolDownButton,
	(unsigned int) &readNoteButton,
	(unsigned int) &readScreenButton,
	(unsigned int) &readHoldSwitch,
	(unsigned int) &readWLANSwitch,
	(unsigned int) &readMainVolume,
};

const struct _PspLibraryEntry __library_exports[2] __attribute__((section(".lib.ent"), used)) = {
	{ NULL, 0x0000, 0x8000, 4, 1, 1, &__syslib_exports },
	{ "SystemButtons", 0x0000, 0x4001, 4, 0, 11, &__SystemButtons_exports },
};
