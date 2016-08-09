#include "pgeZip.h"
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

void LowMemExit() //This is temporary until I come up with a solution. It exits the app, once the memory is less than/equal to 1.5 MB
{
	if (oslGetRamStatus().maxAvailable <= 1500000)
	{
		oslQuit();
	}
}

void debugDisplay()
{
	debug = oslLoadImageFilePNG("system/debug/debug.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	while (!osl_quit)
	{	
		oslStartDrawing();

		controls();	
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);

		oslDrawImageXY(debug, 65, 67);
		oslDrawStringf(110,95,"Unfortunately some files required by");
		oslDrawStringf(110,110,"the program are missing. The program");
		oslDrawStringf(110,125,"has stopped, press (X) to exit to XMB.");

		if (osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			sceKernelExitGame();
		}
	
		if (osl_pad.held.R && osl_keys->pressed.triangle) //Takes screenshot
		{
			screenshot();
		}
	
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();
	}
}

int getCpuClock()
{
    return scePowerGetCpuClockFrequency();
}

int getBusClock()
{
    return scePowerGetBusClockFrequency();
}

void pspGetModel(int x, int y)
{
	int pspmodel = kuKernelGetModel();
	
	switch(pspmodel)
	{
		case 0:
			oslDrawStringf(x,y,"Model: PSP 1000");
			break;
   
		case 1:
			oslDrawStringf(x,y,"Model: PSP 2000");
			break;
   
		case 2:
			oslDrawStringf(x,y,"Model: PSP 3000");
			break;
   
		case 3:
			oslDrawStringf(x,y,"Model: PSP 3000");
			break;
		
		case 4:
			oslDrawStringf(x,y,"Model: PSP Go N1000");
			break;
   
		default:
			oslDrawStringf(x,y,"Model: PS Vita");
			break;
	}
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

void removeUpdateZip()
{
	if (fileExists("ms0:/PSP/GAME/update.zip"))
	{
		sceIoRemove("ms0:/PSP/GAME/update.zip");
	}
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
		oslEndDrawing();//Finish drawing
		oslEndFrame();//End the current frame
    	oslSyncFrame();//Sync everything
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
		oslEndDrawing();//Finish drawing
		oslEndFrame();//End the current frame
    	oslSyncFrame();//Sync everything
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

void networkInit()
{
	connectionInitialized = false;
}

bool networkInitializeConnection()
{
	if(!connectionInitialized)
	{
		int result;

		result = sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);

		if(result < 0)
			return false;

		result = sceUtilityLoadNetModule(PSP_NET_MODULE_INET);

		if(result < 0)
			return false;

		result = sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEURI);

		if(result < 0)
			return false;

		result = sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEHTTP);

		if(result < 0)
			return false;

		result = sceUtilityLoadNetModule(PSP_NET_MODULE_HTTP);

		if(result < 0)
			return false;

		result = sceNetInit(128*1024, 42, 0, 42, 0);

		if(result < 0)
			return false;

		result = sceNetInetInit();

		if(result < 0)
			return false;

		result = sceNetApctlInit(0x10000, 48);

		if(result < 0)
			return false;

		result = sceNetResolverInit();

		if(result < 0)
			return false;

		connectionInitialized = true;
	}

	return connectionInitialized;
}

bool networkShutDownConnection()
{
	if(connectionInitialized)
	{
		int result;

		result = sceNetApctlTerm();

		if(result < 0)
			return false;

		result = sceNetResolverTerm();

		if(result < 0)
			return false;

		result = sceNetInetTerm();

		if(result < 0)
			return false;

		result = sceNetTerm();

		if(result < 0)
			return false;

		result = sceUtilityUnloadNetModule(PSP_NET_MODULE_HTTP);

		if(result < 0)
			return false;

		result = sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEHTTP);

		if(result < 0)
			return false;

		result = sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEURI);

		if(result < 0)
			return false;

		result = sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);

		if(result < 0)
			return false;

		result = sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);

		if(result < 0)
			return false;

		connectionInitialized = false;
	}

	return true;
}

int initNetDialog()
{
	char message[100] = "";
    int dialog = OSL_DIALOG_NONE;
	
	dialog = oslGetDialogType();
    if (dialog)
	{
		oslDrawDialog();
        if (oslGetDialogStatus() == PSP_UTILITY_DIALOG_NONE)
		{
            if (oslDialogGetResult() == OSL_DIALOG_CANCEL)
				sprintf(message, "Cancel");
            else if (dialog == OSL_DIALOG_MESSAGE)
			{
				int button = oslGetDialogButtonPressed();
				if (button == PSP_UTILITY_MSGDIALOG_RESULT_YES)
					sprintf(message, "You pressed YES");
				else if (button == PSP_UTILITY_MSGDIALOG_RESULT_NO)
					sprintf(message, "You pressed NO");
            }
			
			oslEndDialog();
        }
    }
	
	if (dialog == OSL_DIALOG_NONE)
	{
        oslInitNetDialog();
        memset(message, 0, sizeof(message));
    }
	
	return 1;
}
		
bool networkGetFile(const char *url, const char *filepath)
{
	int templ, connection, request, ret, status, dataend, fd, byteswritten;
	SceULong64 contentsize;
	unsigned char readbuffer[8192];

	ret = sceHttpInit(20000);
	
	if(ret < 0)
		return false;
	
	templ = sceHttpCreateTemplate("xxx-agent/0.0.1 libhttp/1.0.0", 1, 1);

	if(templ < 0)
		return false;

	ret = sceHttpSetResolveTimeOut(templ, 3000000);

	if(ret < 0)
		return false;

	ret = sceHttpSetRecvTimeOut(templ, 60000000);
	
	if(ret < 0)
		return false;

	ret = sceHttpSetSendTimeOut(templ, 60000000);

	if(ret < 0)
		return false;

	connection = sceHttpCreateConnectionWithURL(templ, url, 0);

	if(connection < 0)
		return false;

	request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_GET, (char*)url, 0);

	if(request < 0)
		return false;

	ret = sceHttpSendRequest(request, 0, 0);

	if(ret < 0)
		return false;

	ret = sceHttpGetStatusCode(request, &status);

	if(ret < 0)
		return false;

	if(status != 200)
		return false;

	ret = sceHttpGetContentLength(request, &contentsize);

	if(ret < 0)
		return false;

	dataend = 0;
	byteswritten = 0;
	fd = sceIoOpen(filepath, PSP_O_WRONLY | PSP_O_CREAT, 0777);

	while(dataend == 0)
	{
		ret = sceHttpReadData(request, readbuffer, 8192);

		if(ret < 0)
		{
			sceIoWrite(fd, filepath, 4);
			sceIoClose(fd);
			return false;
		}

		if(ret == 0)
		dataend = 1;

		if(ret > 0)
		{
			byteswritten += ret;
			sceIoWrite(fd, readbuffer, ret);
			//printf("Got and wrote %d bytes...\n", ret);
		}
	}

	sceIoClose(fd);
	//printf("File saved (size = %d bytes)... Exiting!\n", bytes_written);
	sceHttpDeleteRequest(request);
	sceHttpDeleteConnection(connection);
	sceHttpDeleteTemplate(templ);
	sceHttpEnd();

	return true;
}

int connectAPCallback(int state) //Internet stuff
{
    oslStartDrawing();
    oslDrawImageXY(wifibg, 0, 19);
    oslDrawStringf(30, 175, "Connecting to AP...");
    sprintf(buffer, "State: %i", state);
    oslDrawStringf(30, 195, buffer);
    oslEndDrawing();
    oslEndFrame();
    oslSyncFrame();

    return 0;
} 
 
int connectToAP(int config) //Internet stuff
{
    oslStartDrawing();
    oslDrawImageXY(wifibg, 0, 19);
    oslDrawStringf(30, 175, "Connecting to AP...");
    oslEndDrawing();
    oslEndFrame();
    oslSyncFrame();

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
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();

        result = oslResolveAddress(Address, resolvedIP);

        oslStartDrawing();
        oslDrawImageXY(wifibg, 0, 19);
        oslGetIPaddress(ip);
        if (!result)
            sprintf(buffer, "Resolved IP address: %s", ip);
        else
            sprintf(buffer, "Error resolving address!");
        oslDrawStringf(30, 195, buffer);
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();
		sceKernelDelayThread(3*1000000);
    }else{
        oslStartDrawing();
        oslDrawImageXY(wifibg, 0, 19);
        sprintf(buffer, "Error connecting to AP!");
        oslDrawStringf(30, 195, buffer);
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();
		sceKernelDelayThread(3*1000000);
    }
    oslDisconnectFromAP();
    return 0;
} 

void onlineUpdater()
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
            oslStartDrawing();
			
            if (browser)
			{
                oslDrawBrowser();
                if (oslGetBrowserStatus() == PSP_UTILITY_DIALOG_NONE)
				{
                    oslEndBrowser();
					updateReady = 1;
					aboutMenu();
                }
            }
            oslEndDrawing();
		}
		oslEndFrame();
		skip = oslSyncFrame();

        if (!browser)
		{
            oslReadKeys();
            oslBrowserInit("http://downloads.sourceforge.net/project/cyanogenpsp/Updates/update.zip", "/PSP/GAME", 14*1024*1024,
                                         PSP_UTILITY_HTMLVIEWER_DISPLAYMODE_SMART_FIT,
                                         PSP_UTILITY_HTMLVIEWER_DISABLE_STARTUP_LIMITS,
                                         PSP_UTILITY_HTMLVIEWER_INTERFACEMODE_FULL,
                                         PSP_UTILITY_HTMLVIEWER_CONNECTMODE_MANUAL_ALL);
			memset(message, 0, sizeof(message));

        }
		
		captureScreenshot();
    }
	oslNetTerm();
}

void flashUpdate()
{
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!recoverybg)
		debugDisplay();
	
	while (!osl_quit)
	{	
		oslStartDrawing();
		oslClearScreen(RGB(0, 0, 0));
		oslDrawImageXY(recoverybg, 0, 0);
		oslDrawStringf(10, 60, "Flashing zip...");
		if (fileExists("ms0:/PSP/GAME/update.zip"))
		{		
			pgeZip* zipFiles = pgeZipOpen("../update.zip");
			chdir("..");
			pgeZipExtract(zipFiles, NULL);
			pgeZipClose(zipFiles);
			oslIntraFontSetStyle(Roboto, fontSize, WHITE, 0, INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(10,80,"Installed Successfully.");
			oslDrawStringf(10,90,"Exiting..");
			oslSyncFrame();
			sceKernelDelayThread(2*1000000);
			oslSyncFrame();
			oslDrawStringf(10,50,"Enjoy :)");
			sceKernelDelayThread(3*1000000);
			oslDeleteImage(recoverybg);
			sceIoRemove("ms0:/PSP/GAME/CyanogenPSP/system/build.prop");
			sceKernelExitGame();
		}
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();
	}
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
