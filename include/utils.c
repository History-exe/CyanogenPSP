#include "unzip.h"
#include "utils.h"
#include "../appDrawer.h"
#include "../clock.h"
#include "../fileManager.h"
#include "../gallery.h"
#include "../gameLauncher.h"
#include "../homeMenu.h"
#include "../lockScreen.h"
#include "../musicPlayer.h"
#include "../screenshot.h"
#include "../settingsMenu.h"

int transp;//For transparency
int fade;//To hold the colour of the rectangle

char usbStatus = 0;
char usbModuleStatus = 0;

void initDrawing()
{
	oslStartDrawing();
	oslClearScreen(RGB(0,0,0));
}

void termDrawing()
{
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
}

void LowMemExit() //This is temporary until I come up with a solution. It exits the app, once the memory is less than/equal to 1.5 MB
{
	if (oslGetRamStatus().maxAvailable <= 1500000)
		oslQuit();
}

void debugDisplay()
{
	debug = oslLoadImageFilePNG("system/debug/debug.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	while (!osl_quit)
	{	
		oslStartDrawing();

		controls();	
		
		oslIntraFontSetStyle(Roboto, 0.5f, BLACK, 0, 0);

		oslDrawImageXY(debug, 65, 67);
		oslDrawStringf(110, 95, "Unfortunately some files required by");
		oslDrawStringf(110, 110, "the program are missing. The program");
		oslDrawStringf(110, 125, "has stopped, press (X) to exit to XMB.");

		if (osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			sceKernelExitGame();
		}
	
		termDrawing();
	}
}

void internet() //Draws the browser
{
	int skip = 0;
    int browser = 0;
	char message[100] = "";
	
	oslNetInit();

    while(!osl_quit)
	{
        browser = oslBrowserIsActive();
		if (!skip)
		{
			initDrawing();

            if (browser)
			{
                oslDrawBrowser();
                if (oslGetBrowserStatus() == PSP_UTILITY_DIALOG_NONE)
				{
                    oslEndBrowser();
					homeMenu();
                }
            }
            oslEndDrawing();
		}
		oslEndFrame();
		skip = oslSyncFrame();

        if (!browser)
		{
            oslReadKeys();
            oslBrowserInit("http://www.google.com", "/PSP/GAME/CyanogenPSP/downloads", 5 * 1024 * 1024, //Downloads will be saved into this directory
                                         PSP_UTILITY_HTMLVIEWER_DISPLAYMODE_SMART_FIT,
                                         PSP_UTILITY_HTMLVIEWER_DISABLE_STARTUP_LIMITS,
                                         PSP_UTILITY_HTMLVIEWER_INTERFACEMODE_FULL,
                                         PSP_UTILITY_HTMLVIEWER_CONNECTMODE_MANUAL_ALL);
			memset(message, 0, sizeof(message));

        }
    }
	oslNetTerm();
}

int getCpuClock()
{
    return scePowerGetCpuClockFrequency();
}

int getBusClock()
{
    return scePowerGetBusClockFrequency();
}

char * getModel()
{
	int pspModel = kuKernelGetModel();
	
	static char model[20] = "";
	
	if (dirExists("ms0:/adrenaline"))
		strcpy(model, "Model: PS Vita");
	
	else
	{
		switch(pspModel)
		{
			case 0:
				strcpy(model, "Model: PSP 1000");
				break;
   
			case 1:
				strcpy(model, "Model: PSP 2000");
				break;
   
			case 2:
				strcpy(model, "Model: PSP 3000");
				break;
   
			case 3:
				strcpy(model, "Model: PSP 3000");
				break;
		
			case 4:
				strcpy(model, "Model: PSP Go N1000");
				break;
			
			default:
				strcpy(model, "Model: Unknown");
				break;
		}
	}
	
	return model;
}

int setFileDefaultsInt(char *path, int value, int var)
{
	FILE *temp;
	 
	if (!(fileExists(path)))
	{
		temp = fopen(path, "w");
		fprintf(temp, "%d", value);
		fclose(temp);
	} 
	
	temp = fopen(path, "r");
	fscanf(temp, "%d", &var);
	fclose(temp);
	
	return var;
}

float setFileDefaultsFloat(char *path, float value, float var)
{
	FILE *temp;
	 
	if (!(fileExists(path)))
	{
		temp = fopen(path, "w");
		fprintf(temp, "%f", value);
		fclose(temp);
	} 
	
	temp = fopen(path, "r");
	fscanf(temp, "%f", &var);
	fclose(temp);
	
	return var;
}

char * setFileDefaultsChar(char path[], char data[], char var[])
{
	FILE * temp;
	
	if (!(fileExists(path)))
	{
		temp = fopen(path, "w");
		fprintf(temp, "%s", data);
		fclose(temp);
	}
	
	temp = fopen(path, "r");
	fscanf(temp, "%s", var);
	fclose(temp);
	
	return var;
}

void installRequiredFiles()
{
	SceUID dir;
	
	if (!(dirExists("ms0:/PICTURE")))
	{
		dir = sceIoDopen("ms0:/PICTURE");
		sceIoMkdir("ms0:/PICTURE", 0777);
		sceIoDclose(dir);
	}

	if (!(dirExists("ms0:/PSP/PHOTO")))
	{
		dir = sceIoDopen("ms0:/PSP/PHOTO");
		sceIoMkdir("ms0:/PSP/PHOTO", 0777);
		sceIoDclose(dir);
	}

	if (!(dirExists("ms0:/MUSIC")))
	{
		dir = sceIoDopen("ms0:/MUSIC");
		sceIoMkdir("ms0:/MUSIC", 0777);
		sceIoDclose(dir);
	}
	
	if (!(dirExists("ms0:/PSP/MUSIC")))
	{
		dir = sceIoDopen("ms0:/PSP/MUSIC");
		sceIoMkdir("ms0:/PSP/MUSIC", 0777);
		sceIoDclose(dir);
	}
	
	if (!(dirExists("ms0:/ISO")))
	{
		dir = sceIoDopen("ms0:/ISO");
		sceIoMkdir("ms0:/ISO", 0777);
		sceIoDclose(dir);
	}
	
	if (!(dirExists("ms0:/PSP/GAME/CyanogenPSP/screenshots")))
	{
		dir = sceIoDopen("ms0:/PSP/GAME/CyanogenPSP/screenshots");
		sceIoMkdir("ms0:/PSP/GAME/CyanogenPSP/screenshots", 0777);
		sceIoDclose(dir);
	}
	
	if (!(dirExists("ms0:/PSP/GAME/CyanogenPSP/downloads")))
	{
		dir = sceIoDopen("ms0:/PSP/GAME/CyanogenPSP/downloads");
		sceIoMkdir("ms0:/PSP/GAME/CyanogenPSP/downloads", 0777);
		sceIoDclose(dir);
	}
	
	if (!(dirExists("ms0:/PSP/GAME/CyanogenPSP/updates")))
	{
		dir = sceIoDopen("ms0:/PSP/GAME/CyanogenPSP/updates");
		sceIoMkdir("ms0:/PSP/GAME/CyanogenPSP/updates", 0777);
		sceIoDclose(dir);
	}
	
	if (!(dirExists("ms0:/PSP/GAME/CyanogenPSP/system/themes")))
	{
		dir = sceIoDopen("ms0:/PSP/GAME/CyanogenPSP/system/themes");
		sceIoMkdir("ms0:/PSP/GAME/CyanogenPSP/system/themes", 0777);
		sceIoDclose(dir);
	}
	
	if (!(dirExists("ms0:/PSP/GAME/CyanogenPSP/system/themes/Default")))
	{
		dir = sceIoDopen("ms0:/PSP/GAME/CyanogenPSP/system/themes/Default");
		sceIoMkdir("ms0:/PSP/GAME/CyanogenPSP/system/themes/Default", 0777);
		sceIoDclose(dir);
	}
	
	SceUID file;
	 
	if (!(fileExists(appDrawerFontColorPath)))
	{
		file = sceIoOpen(appDrawerFontColorPath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		sceIoWrite(file, "0\n0\n0", 15);
		sceIoClose(file);
	}
	
	if (!(fileExists(fileManagerFontColorPath)))
	{	
		file = sceIoOpen(fileManagerFontColorPath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		sceIoWrite(file, "0\n0\n0", 15);
		sceIoClose(file);
	}
	
	if (!(fileExists(clockWidgetFontColorPath)))
	{	
		file = sceIoOpen(clockWidgetFontColorPath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		sceIoWrite(file, "255\n255\n255", 15);
		sceIoClose(file);
	}
	
	if (!(fileExists(timeAndBatteryFontColorPath)))
	{
		file = sceIoOpen(timeAndBatteryFontColorPath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		sceIoWrite(file, "255\n255\n255", 15);
		sceIoClose(file);
	}
	
	if (!(fileExists(settingsFontColorPath)))
	{
		file = sceIoOpen(settingsFontColorPath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		sceIoWrite(file, "0\n0\n0", 15);
		sceIoClose(file);
	}
	
	if (!(fileExists(apolloFontColorPath)))
	{
		file = sceIoOpen(apolloFontColorPath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		sceIoWrite(file, "0\n0\n0", 15);
		sceIoClose(file);
	}
	
	if (!(fileExists(gameFontColorPath)))
	{	
		file = sceIoOpen(gameFontColorPath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		sceIoWrite(file, "0\n0\n0", 15);
		sceIoClose(file);
	}
	
	if (!(fileExists(galleryFontColorPath)))
	{
		file = sceIoOpen(galleryFontColorPath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		sceIoWrite(file, "255\n255\n255", 15);
		sceIoClose(file);
	}
}

void openOSK(char * message, char * initialMessage, int textLimit, int lang)
{
	int skip = 0;
	int Keyboard = 0;
	char data[20] = "";
	
	OSL_IMAGE *keyboardBg = oslLoadImageFilePNG(keyBoardBgPath, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);

	while(!osl_quit)
	{
	if (!skip)
	{
		oslStartDrawing();
		oslDrawImageXY(keyboardBg, 0, 0);
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		battery(370,2,1);
		digitaltime(420,4,0,hrTime);
		if (Keyboard == 0)
		{
			oslInitOsk(message, initialMessage, textLimit, 1, lang);  
			Keyboard = 1; 
			oslDrawOsk(); 
			if (oslGetOskStatus() == PSP_UTILITY_DIALOG_NONE)
			{
				if (oslOskGetResult() == OSL_OSK_CANCEL) 
					Keyboard = 2; 
				else
				{
					oslOskGetText(data);
					sprintf(tempMessage, "%s", data);
					sprintf(tempPin, "%s", data);
					sprintf(tempData, "%s", data);
					Keyboard = 2; 
					oslDeleteImage(keyboardBg);
				}
				oslEndOsk();		
				return;
			}
			oslEndDrawing(); 
		}
		Keyboard = 0; 
		
		oslReadKeys();
		
		captureScreenshot();
	}
	oslEndFrame();
    skip = oslSyncFrame();
}
}

int isEmpty(FILE *file) //Check to make sure its not a empty file
{
    long savedOffset = ftell(file);
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0)
    {
        return 1;
    }

    fseek(file, savedOffset, SEEK_SET);
    return 0;
}

/*This was originally coded by Blade_punk. I modified it to add the X, and Y bits. */
void fadeOut(OSL_IMAGE* bg,int x, int y)//Name and params taken
{
	for(transp = 0; transp < 255; transp += 3)//Sets the transparency to 0 aka invisible. Then it increases by 3 until it equals 255 aka completely visible
	{
		oslStartDrawing();//Start drawing
		fade = RGBA(0, 0, 0, transp);//Set the colour of the rectangle. Sets the alpha to what transp currently is
		oslDrawImageXY(bg,x,y);//Draws an image in the background
		oslDrawFillRect(0, 0, 480, 272, fade);//Draws the rectangle over the screen
		termDrawing();
	}
}
void fadeIn(OSL_IMAGE* bg, int x, int y)//Name and params taken
{
	for(transp = 255; transp > 0; transp -= 3)//Sets the transparency to 255 aka completely visible. Then it decreases by 3 until it equals 255 aka invisible
	{
		oslStartDrawing();//Start drawing
		fade = RGBA(0, 0, 0, transp);//Set the colour of the rectangle. Sets the alpha to what transp currently is
		oslDrawImageXY(bg,x,y);//Draws an image in the background
		oslDrawFillRect(0, 0, 480, 272, fade);//Draws the rectangle over the screen
		termDrawing();
	}
}

int disableUsb(void)
{
	if(usbStatus)
	{
		sceUsbDeactivate(0);
		pspUsbDeviceFinishDevice();
		sceUsbStop(PSP_USBSTOR_DRIVERNAME, 0, 0);
		sceUsbStop(PSP_USBBUS_DRIVERNAME, 0, 0);
		usbStatus = 0;
		usbStat = 0;
		sceKernelDelayThread(300000);
	}
	return 0;
}

int enableUsb()
{
	if (usbStatus == 1)
	{
		disableUsb();
		return 0;
	}

	if(!usbModuleStatus)
	{
		pspSdkLoadStartModule("flash0:/kd/semawm.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstor.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstormgr.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstorms.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstorboot.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbdevice.prx", PSP_MEMORY_PARTITION_KERNEL);
		usbModuleStatus = 1;
	}

	if (!usbStatus)
	{
		sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
		sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
		sceUsbstorBootSetCapacity(0x800000);
		sceUsbActivate(0x1c8);
		usbStatus = 1;
		usbStat = 1;
		sceKernelDelayThread(300000);
	}
	
	return 1;
}

int enableUsbEx(u32 device)
{
	if (usbStatus == 1)
	{
		disableUsb();
		return 0;
	}

	pspUsbDeviceSetDevice(device, 0, 0);
	
	if(!usbModuleStatus)
	{
		pspSdkLoadStartModule("flash0:/kd/usbdevice.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/semawm.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstor.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstormgr.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstorms.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstorboot.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbdevice.prx", PSP_MEMORY_PARTITION_KERNEL);
		usbModuleStatus = 1;
	}
	
	if (!usbStatus)
	{
		sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
		sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
		sceUsbActivate(0x1c8);
		usbStatus = 1;
		usbStat = 1;
		sceKernelDelayThread(300000);
	}
	
	return 1;
}

int netInit(void)
{	
	int result;
	
	result = sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
	
	if(result < 0)
		return result;
		
	result = sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
	
	if(result < 0)
		return result;
		
	result = sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEURI);
	
	if(result < 0)
		return result;

	result = sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEHTTP);
	
	if(result < 0)
		return result;
	
	result = sceUtilityLoadNetModule(PSP_NET_MODULE_HTTP);
	
	if(result < 0)
		return result;
			
	result = sceNetInit(128*1024, 42, 4*1024, 42, 4*1024);
	
	if(result < 0)
		return result;
			
	result = sceNetInetInit();
	
	if(result < 0)
		return result;
			
	result = sceNetApctlInit(0x10000, 48);
	
	if(result < 0)
		return result;
			
	result = sceNetResolverInit();
	
	if(result < 0)
		return result;

	return(1);
}

int  netShutdown(void)
{
	int result;
	
	result = sceNetApctlTerm();
	
	if(result < 0)
		return result;
		
	result = sceNetResolverTerm();
	
	if(result < 0)
		return result;
		
	result = sceNetInetTerm();
	
	if(result < 0)
		return result;
		
	result = sceNetTerm();
	
	if(result < 0)
		return result;
		
	result = sceUtilityUnloadNetModule(PSP_NET_MODULE_HTTP);
	
	if(result < 0)
		return result;
		
	result = sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEHTTP);
	
	if(result < 0)
		return result;
		
	result = sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEURI);
	
	if(result < 0)
		return result;

	result = sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
	
	if(result < 0)
		return result;
	
	result = sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
	
	if(result < 0)
		return result;
	
	return 1;
}

static pspUtilityNetconfData netConfData;

int netDialogInit(void)
{		
	memset(&netConfData, 0, sizeof(netConfData));
	netConfData.base.size = sizeof(netConfData);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &netConfData.base.language);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &netConfData.base.buttonSwap);
	netConfData.base.graphicsThread = 17;
	netConfData.base.accessThread = 19;
	netConfData.base.fontThread = 18;
	netConfData.base.soundThread = 16;
	netConfData.action = 0;
	
	struct pspUtilityNetconfAdhoc adhocparam;
	memset(&adhocparam, 0, sizeof(adhocparam));
	netConfData.adhocparam = &adhocparam;

	int result = sceUtilityNetconfInitStart(&netConfData);
	
	if(result == 0)
		return 1;
		
	return result;
}

int netDialogUpdate(void)
{
		int result = -1;
		
		switch(sceUtilityNetconfGetStatus())
		{
			case PSP_UTILITY_DIALOG_NONE:
				result = netConfData.base.result;
				break;

			case PSP_UTILITY_DIALOG_VISIBLE:
				sceUtilityNetconfUpdate(1);
				break;

			case PSP_UTILITY_DIALOG_QUIT:
				sceUtilityNetconfShutdownStart();
				break;
				
			case PSP_UTILITY_DIALOG_FINISHED:
				break;

			default:
				break;
		}
		
	return(result);
}

void netGetFile(const char *url, const char *filepath) 
{
	int template = sceHttpCreateTemplate("CyanogenPSP Update Client", 1, 1);

	int connection = sceHttpCreateConnectionWithURL(template, url, 0);

	int request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_GET, (char *)url, 0);

	sceHttpSendRequest(request, NULL, 0);

	int file = sceIoOpen(filepath, PSP_O_WRONLY | PSP_O_CREAT, 0777);

	unsigned char data[16*1024];
	int read = 0;
	
	while ((read = sceHttpReadData(request, &data, sizeof(data))) > 0) 
		sceIoWrite(file, data, read);

	sceIoClose(file);
}

int connectAPCallback(int state) //Internet stuff
{
    oslStartDrawing();
    oslDrawImageXY(wifibg, 0, 19);
    oslDrawStringf(30, 175, "Connecting to AP...");
    sprintf(buffer, "State: %i", state);
    oslDrawStringf(30, 195, buffer);
    termDrawing();

    return 0;
} 
 
int connectToAP(int config) //Internet stuff
{
    oslStartDrawing();
    oslDrawImageXY(wifibg, 0, 19);
    oslDrawStringf(30, 175, "Connecting to AP...");
    termDrawing();

    int result = oslConnectToAP(config, 30, connectAPCallback);
    if (!result){
        char ip[30] = "";
        char resolvedIP[30] = "";

        oslStartDrawing();
        oslDrawImageXY(wifibg, 0, 19);
        oslGetIPaddress(ip);
        sprintf(buffer, "IP address: %s", ip);
        oslDrawStringf(30, 175, buffer);

        sprintf(buffer, "Resolving %s", Address);
        oslDrawStringf(30, 195, buffer);
        termDrawing();

        result = oslResolveAddress(Address, resolvedIP);

        oslStartDrawing();
        oslDrawImageXY(wifibg, 0, 19);
        oslGetIPaddress(ip);
        if (!result)
            sprintf(buffer, "Resolved IP address: %s", ip);
        else
            sprintf(buffer, "Error resolving address!");
        oslDrawStringf(30, 195, buffer);
        termDrawing();
		sceKernelDelayThread(3*1000000);
    }else{
        oslStartDrawing();
        oslDrawImageXY(wifibg, 0, 19);
        sprintf(buffer, "Error connecting to AP!");
        oslDrawStringf(30, 195, buffer);
        termDrawing();
		sceKernelDelayThread(3*1000000);
    }
    oslDisconnectFromAP();
    return 0;
} 

void flashUpdate()
{
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!recoverybg)
		debugDisplay();

	oslClearScreen(RGB(0, 0, 0));
	oslDrawImageXY(recoverybg, 0, 0);
		
	oslDrawStringf(10, 250, "Flashing Zip...");
	oslSyncFrame();
	sceKernelDelayThread(3000000);
		
	Zip* zip = ZipOpen("../update.zip");
	chdir("ms0:/PSP/GAME/");
	ZipExtract(zip, NULL);
	ZipClose(zip);
		
	oslDrawStringf(10, 250, "Zip flashed successfully.");
	oslSyncFrame();
	sceKernelDelayThread(3 * 1000000);
	
	oslDrawStringf(10, 250, "Rebooting..");
	oslSyncFrame();
	sceKernelDelayThread(3 * 1000000);
	
	sceKernelExitGame();
}

void removeUpdateContents()
{
	if (fileExists("ms0:/PSP/GAME/update.zip"))
		sceIoRemove("ms0:/PSP/GAME/update.zip");
}

int downloadUpdate()
{
	netInit();
	netDialogInit();
	
	OSL_IMAGE * background = oslLoadImageFilePNG(keyBoardBgPath, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	dialog = oslLoadImageFilePNG("system/debug/debug.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!dialog)
		debugDisplay();
	
	//int checkVer = 0, initVersion = 0;
	
	//initVersion = concatenate(VER_MAJOR, VER_MINOR);
	
	while (!osl_quit)
	{		
		oslStartDrawing();
		oslDrawImageXY(background, 0, 0);
		oslEndDrawing(); 
		
		int result = netDialogUpdate();
		
		if(result != -1)
			break;
		
		oslSyncFrame();	
	}
	
		while (!osl_quit) 
		{
	
			oslStartDrawing();

			controls();	
	
			oslIntraFontSetStyle(Roboto, fontSize, BLACK, 0, INTRAFONT_ALIGN_LEFT);
	
			oslDrawImageXY(dialog, 65, 67);
			oslDrawStringf(110, 95, "An update has been found");
			oslDrawStringf(110, 115, "Press (X) to download or (O) to cancel.");
			
			if (osl_keys->pressed.cross) 
			{
				oslPlaySound(KeypressStandard, 1);  
				netGetFile("http://cyanogenpsp.000webhostapp.com/updates/update.zip", "ms0:/PSP/GAME/update.zip");
				oslDeleteImage(dialog);
				break;
				if (fileExists("ms0:/PSP/GAME/update.zip"))
					flashUpdate();
			}
		
			if (osl_keys->pressed.circle) 
			{
				oslDeleteImage(dialog);
				break;
			}
		
			termDrawing();
		}
	
	while(1)
	{
		oslReadKeys();
		if (fileExists("ms0:/PSP/GAME/update.zip"))
			flashUpdate();
	}
	
	netShutdown();
	return 0;
}
	
u64 storageGetTotalSize() //returns data in MB
{
	SystemDevCtl devctl;
    memset(&devctl, 0, sizeof(SystemDevCtl));
    SystemDevCommand command;
    command.pdevinf = &devctl;
    sceIoDevctl("ms0:", 0x02425818, &command, sizeof(SystemDevCommand), NULL, 0);
    u64 size = (devctl.maxclusters * devctl.sectorcount) * devctl.sectorsize;  

	return size/1048576;
}

u64 storageGetFreeSize() //returns data in MB
{
	SystemDevCtl devctl;
    memset(&devctl, 0, sizeof(SystemDevCtl));
    SystemDevCommand command;
    command.pdevinf = &devctl;
    sceIoDevctl("ms0:", 0x02425818, &command, sizeof(SystemDevCommand), NULL, 0);
    u64 size = (devctl.freeclusters * devctl.sectorcount) * devctl.sectorsize; 

	return size/1048576;
}

int isUSBCableActivated()
{
	unsigned long state = oslGetUsbState();
	if (state & PSP_USB_ACTIVATED)
		return 1;
	else
		return 0;
}

int isUSBCableConnected()
{
	unsigned long state = oslGetUsbState();
	if (state & OSL_USB_CABLE_CONNECTED)
		return 1;
	else
		return 0;
}

char getPSPNickname()
{
	return sceUtilityGetSystemParamString(1, nickname, 25);
}

unsigned concatenate(unsigned x, unsigned y) 
{
    unsigned pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;        
}