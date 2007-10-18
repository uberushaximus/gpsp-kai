/******************************************************************************

	homehook.prx

******************************************************************************/

#include <pspsdk.h>
#include <pspctrl.h>


PSP_MODULE_INFO("homehook", PSP_MODULE_KERNEL, 0, 0);
PSP_MAIN_THREAD_ATTR(0);


/******************************************************************************
	prototypes
******************************************************************************/

int sceCtrl_driver_3A622550(SceCtrlData *pad_data, int count);
int sceCtrl_driver_C4AAD55F(SceCtrlData *pad_data, int count);

#define sceCtrlPeekBufferPositive		sceCtrl_driver_3A622550
#define sceCtrlPeekBufferPositive371	sceCtrl_driver_C4AAD55F

#define CTRL_MASK (PSP_CTRL_NOTE | PSP_CTRL_SCREEN | PSP_CTRL_VOLUP | PSP_CTRL_VOLDOWN | PSP_CTRL_HOME)

/******************************************************************************
	local variables
******************************************************************************/

static volatile int home_active;
static unsigned int home_button;
static SceUID home_thread;

static int (*__sceCtrlPeekBufferPositive)(SceCtrlData *pad_data, int count);


/******************************************************************************
	functions
******************************************************************************/

static int home_button_thread(SceSize args, void *argp)
{
	SceCtrlData paddata;

	home_active = 1;

	while (home_active)
	{
		if (__sceCtrlPeekBufferPositive)
		{
			(*__sceCtrlPeekBufferPositive)(&paddata, 1);
			//HOMEボタン以外の特殊ボタンも読込む(将来の拡張用)
			home_button = paddata.Buttons & CTRL_MASK;
		}

		sceKernelDelayThread(1000000/60);
	}

	sceKernelExitThread(0);

	return 0;
}


void initHomeButton(int devkit_version)
{
	if (devkit_version < 0x03060010)
		__sceCtrlPeekBufferPositive = sceCtrlPeekBufferPositive;
	else
		__sceCtrlPeekBufferPositive = sceCtrlPeekBufferPositive371;
}


unsigned int readHomeButton(void)
{
	return home_button;
}


int module_start(SceSize args, void *argp)
{
	__sceCtrlPeekBufferPositive = NULL;

	home_button = 0;
	home_active = 0;
	home_thread = sceKernelCreateThread("Home Button Thread",
								home_button_thread,
								0x11,
								0x200,
								0,
								NULL);

	if (home_thread >= 0)
		sceKernelStartThread(home_thread, 0, 0);

#if 0
	// user mode用のモジュールをアンロードする。
	// Homeボタンのダイアログは表示されなくなるが、
	// AdHocのモジュールが読み込めなくなる等、問題あり。
	SceUID modIDs[16];
	int count;

	if (sceKernelGetModuleIdList(modIDs, 16, &count) < 0)
	{
		sceKernelStopModule(modIDs[1], 0, NULL, NULL, NULL);
		sceKernelDelayThread(500*1000);
		sceKernelUnloadModule(modIDs[1]);
	}
#endif

	return 0;
}


int module_stop(void)
{
	if (home_thread >= 0)
	{
		home_active = 0;
		sceKernelWaitThreadEnd(home_thread, NULL);
	}
	return 0;
}
