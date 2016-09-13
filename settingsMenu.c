#include "appDrawer.h"
#include "clock.h"
#include "fileManager.h"
#include "gallery.h"
#include "gameLauncher.h"
#include "homeMenu.h"
#include "include/common.h"
#include "include/pspusbdevice.h"
#include "include/ram.h"
#include "include/utils.h"
#include "language.h"
#include "lockScreen.h"
#include "musicPlayer.h"
#include "powerMenu.h"
#include "recentsMenu.h"
#include "recoveryMenu.h"
#include "screenshot.h"
#include "settingsMenu.h"

struct settingsFontColor fontColor;
struct timeAndBatteryStatusFontColor fontColorTime;
	
void switchStatus(int n)
{
	if (n == 1)
	{
		if (sceWlanGetSwitchState() == 0)
			oslDrawImageXY(offswitch, 370, 62);
	
		else
			oslDrawImageXY(onswitch, 370, 62);
	}
	
	else if (n == 3)
	{
		if (sceWlanGetSwitchState() == 0)
		{
			oslDrawImageXY(offswitch, 392, 68);
			oslDrawStringf(58,74,"Off");
		}
		
		else
		{
			oslDrawImageXY(onswitch, 392, 68);
			oslDrawStringf(58,74,"On");
		}
	}
}

fw_version getFwVersion(fw_version *v);

void aboutMenu()
{	
	if (DARK == 0)
	{
		aboutbg = oslLoadImageFilePNG(aboutBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		aboutbg = oslLoadImageFilePNG("system/settings/Dark/aboutbg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}
	
	if (!aboutbg || !highlight)
		debugDisplay();

	u8 *macAddress = oslAdhocGetMacAddress();
	
	fw_version version;
	getFwVersion(&version);
	
	char message[100] = "";
    int dialog = OSL_DIALOG_NONE;
	
	int updateActivation = 1;

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	

		oslDrawImageXY(aboutbg, 0, 0);
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);

		oslDrawStringf(20, 78, "%s", lang_settingsAbout[language][0]);
		oslDrawStringf(20, 92, "%s", lang_settingsAbout[language][1]);
		pspGetModel(20, 143);
		oslDrawStringf(20, 129, "%s %s-%d%02d%02d-%s", lang_settingsAbout[language][2], VERSION, YEAR, MONTH + 1, DAY, lang_settingsAbout[language][3]);
		oslDrawStringf(20, 157, "%s %02X:%02X:%02X:%02X:%02X:%02X", lang_settingsAbout[language][4], macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
		oslDrawStringf(20, 185, "%s %d.%d", lang_settingsAbout[language][5], version.major, version.minor);
		oslDrawStringf(20, 199, "%s %s", lang_settingsAbout[language][6], OSL_VERSION);
		oslDrawStringf(20, 213, "Developer: Joel16");

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 62 && cursor->y <= 119)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,78,"%s", lang_settingsAbout[language][0]);
			oslDrawStringf(20,92,"%s", lang_settingsAbout[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(aboutbg);
				oslDeleteImage(highlight);
				updatesMenu();
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 120 && cursor->y <= 178)
		{
			oslDrawImageXY(highlight, 0, 122);
			pspGetModel(20,143);
			oslDrawStringf(20,129,"%s %s-%d%02d%02d-%s", lang_settingsAbout[language][2], VERSION, YEAR, MONTH + 1, DAY, lang_settingsAbout[language][3]);
			oslDrawStringf(20,157,"%s %02X:%02X:%02X:%02X:%02X:%02X", lang_settingsAbout[language][4], macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 179 && cursor->y <= 233)
		{
			oslDrawImageXY(highlight, 0, 179);
			oslDrawStringf(20,185,"%s %d.%d", lang_settingsAbout[language][5], version.major, version.minor);
			oslDrawStringf(20,199,"%s %s", lang_settingsAbout[language][6], OSL_VERSION);
			oslDrawStringf(20,213,"Developer: Joel16");
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(aboutbg);
				oslDeleteImage(highlight);
				creditsMenu();
			}
		}
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
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
						flashUpdate();
					else if (button == PSP_UTILITY_MSGDIALOG_RESULT_NO)
						updateActivation = 0;
						oslEndDialog();
				}
            oslEndDialog();
            }
		}
		
		if (dialog == OSL_DIALOG_NONE)
		{
			if (updateReady == 1)
			{
				if (updateActivation == 1 && (fileExists("ms0:/PSP/GAME/update.zip")))
				{
					oslInitMessageDialog("Update.zip has been found. Would you like to flash the update?", 1);
					memset(message, 0, sizeof(message));
				}
			}
		}
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{	
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			settingsMenu();	
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			settingsMenu();	
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			home();	
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void creditsMenu()
{
	if (DARK == 0)
		aboutbg = oslLoadImageFilePNG(aboutBgPath, OSL_IN_RAM, OSL_PF_8888);
	else
		aboutbg = oslLoadImageFilePNG("system/settings/Dark/aboutbg.png", OSL_IN_RAM, OSL_PF_8888);

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	

		oslDrawImageXY(aboutbg, 0, 0);
		if (DARK == 0)
			oslDrawFillRect(0, 62, 444, 272, RGB(255,255,255));
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, 0.5, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, 0.5, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);

		oslDrawStringf(10,68,"I'd like to say thanks to all the Developers, contributors and");
		oslDrawStringf(10,80,"supporters, who motivated me to continue this project for");
		oslDrawStringf(10,92,"such a long time. Thank you, all of you! :)");
		
		oslDrawStringf(10,118,"Developers: Omega2058, qwikrazor87, hgoel0974 and GBOT");
		
		oslDrawStringf(10,138,"Translators: French - noname120 and Mahmoudos, German -");
		oslDrawStringf(10,150,"Dimistoteles and Ciapa, Polish - pawelosro, Dutch - Arikuzo,");
		oslDrawStringf(10,162,"Spanish - Randomdude0, Brazilian Portuguese - Hargrun,");
		oslDrawStringf(10,174,"Portugues - Gustavo Fraga Pacheco, Norwegian - Ridge,");
		oslDrawStringf(10,186,"Romanian- imhotep4, Italian - nerdvana8,");
		oslDrawStringf(10,198,"Russan and Ukrainian - Vasniktel and Raithwall.");

		oslDrawStringf(10,218,"Supporters: PSX-Place - STLcardsWS, Tranced, atreyu187, pinky,");
		oslDrawStringf(10,230,"bitsbubba, kozarovv, pinkfloydviste. Genetik57 for his assets.");
		oslDrawStringf(10,242,"Other memebers from Hack Informer, GBAtemp and wololo.net.");
		oslDrawStringf(10,254,"I hope I didn't forget anyone. Once again, thank you all :)");
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{	
			oslDeleteImage(aboutbg);
			aboutMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(aboutbg);
			aboutMenu();	
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(aboutbg);
			home();	
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void updatesMenu()
{		    
	if (DARK == 0)
	{
		updatesbg = oslLoadImageFilePNG(updatesBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		updatesbg = oslLoadImageFilePNG("system/settings/Dark/updatesbg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}

	if (!updatesbg || !highlight)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	

		oslDrawImageXY(updatesbg, 0, 0);
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(20,93, "%s", lang_settingsUpdates[language][0]);

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 63 && cursor->y <= 113)
		{
			oslDrawImageXY(highlight, 0, 70);
			oslDrawStringf(20,93, "%s", lang_settingsUpdates[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				onlineUpdater();
			}
		}

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(updatesbg);
			oslDeleteImage(highlight);
			aboutMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(updatesbg);
			oslDeleteImage(highlight);
			aboutMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(updatesbg);
			oslDeleteImage(highlight);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void performanceMenu()
{	
	if (DARK == 0)
	{
		performancebg = oslLoadImageFilePNG(performanceBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		performancebg = oslLoadImageFilePNG("system/settings/Dark/performancebg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}

	if (!performancebg || !highlight)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	

		oslDrawImageXY(performancebg, 0, 0);
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);

		oslDrawStringf(20,86, "%s", lang_settingsPerformance[language][0]);
		oslDrawStringf(20,140, "%s", lang_settingsPerformance[language][1]);
		oslDrawStringf(20,194, "%s", lang_settingsPerformance[language][2]);
		oslDrawStringf(20,248, "%s", lang_settingsPerformance[language][3]);

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsPerformance[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(highlight);
				oslDeleteImage(performancebg);
				processorMenu();
			}
		}

		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsPerformance[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(highlight);
				oslDeleteImage(performancebg);
				ramMenu();
			}
		}

		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(20,194, "%s", lang_settingsPerformance[language][2]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(highlight);
				oslDeleteImage(performancebg);
				storageMenu();
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 216 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 227);
			oslDrawStringf(20,248, "%s", lang_settingsPerformance[language][3]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(highlight);
				oslDeleteImage(performancebg);
				batteryMenu();
			}
		}

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			settingsMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void setCpuBoot()
{	
	processorState = setFileDefaultsInt("system/settings/processorInfo.bin", 4, processorState);
	
	switch(processorState)
	{
		case 0:
			scePowerSetClockFrequency(20, 20, 10);
			break;
		
		case 1:
			scePowerSetClockFrequency(75, 75, 37);
			break;
			
		case 2:
			scePowerSetClockFrequency(100, 100, 50);
			break;
		
		case 3:
			scePowerSetClockFrequency(133, 133, 66);
			break;
		
		case 4:
			scePowerSetClockFrequency(222, 222, 111);
			break;
		
		case 5:
			scePowerSetClockFrequency(266, 266, 133);
			break;
		
		case 6:
			scePowerSetClockFrequency(300, 300, 150);
			break;
		
		case 7:
			scePowerSetClockFrequency(333, 333, 166);
			break;
	}
}

void processorMenu()
{	
	FILE * processorInfo;
	
	int currentState = stateOff;
	int cpufreq, cpu, bus, state = 0;
	
	if (DARK == 0)
	{
		processorbg = oslLoadImageFilePNG(performanceBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		processorbg = oslLoadImageFilePNG("system/settings/Dark/performancebg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}
	
	if (!processorbg || !highlight)
		debugDisplay(); 
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	
		
		oslDrawImageXY(processorbg, 0, 0);
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(20,76, "%s", lang_settingsProcessor[language][0]);
		oslDrawStringf(20,128, "%s", lang_settingsProcessor[language][1]);
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 118 && cursor->y <= 174)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,128, "%s", lang_settingsProcessor[language][1]);
			oslDrawStringf(20,145, "%s", lang_settingsProcessor[language][2]);
		}
		
		if (osl_keys->pressed.L && batteryM != 0 && isPlaying != 1)
		{
			currentState = stateUnderClock;
		}
		else if (osl_keys->pressed.R && batteryM != 0 && isPlaying != 1)
		{
			currentState = stateOverClock;
		}
		
		if (currentState == stateOverClock)
		{
			processorInfo = fopen("system/settings/processorInfo.bin", "w");
			cpufreq = scePowerGetCpuClockFrequency();
			
			if (cpufreq <70)
			{
				state = 1;
		        scePowerSetClockFrequency(75, 75, 37);	
			}
			else if (cpufreq <95)
			{
				state = 2;
				scePowerSetClockFrequency(100, 100, 50);
			}
			else if (cpufreq <125)
			{
				state = 3;
				scePowerSetClockFrequency(133, 133, 66);
			}
			else if (cpufreq <215)
			{
				state = 4;
				scePowerSetClockFrequency(222, 222, 111);
			}
			else if (cpufreq <260)
			{
				state = 5;
				scePowerSetClockFrequency(266, 266, 133);
			}
			else if (cpufreq <290)
			{
				state = 6;
				scePowerSetClockFrequency(300, 300, 150);
			}
			else if (cpufreq <325)
			{
				state = 7;
				scePowerSetClockFrequency(333, 333, 166);
			}
			
			fprintf(processorInfo, "%d", state);
			fclose(processorInfo);
			currentState = stateNoClock;
		}
		
		if (currentState == stateUnderClock)
		{
			processorInfo = fopen("system/settings/processorInfo.bin", "w");
			cpufreq = scePowerGetCpuClockFrequency();
			
			if (cpufreq> 300)
			{
				state = 6;
				scePowerSetClockFrequency(300, 300, 150);
			}
			else if (cpufreq> 266)
			{
				state = 5;
				scePowerSetClockFrequency(266, 266, 133);
			}
			else if (cpufreq> 222)
			{
				state = 4;
				scePowerSetClockFrequency(222, 222, 111);
			}
			else if (cpufreq> 133)
			{
				state = 3;
				scePowerSetClockFrequency(133, 133, 66);
			}
			else if (cpufreq> 100)
			{
				state = 2;
				scePowerSetClockFrequency(100, 100, 50);
			}
			else if (cpufreq> 75)
		    {
				state = 1;
				scePowerSetClockFrequency(75, 75, 37);
			}
			else if (cpufreq> 20)
			{
				state = 0;
				scePowerSetClockFrequency(20, 20, 10);
			}
			
			fprintf(processorInfo, "%d", state);
			fclose(processorInfo);
			currentState = stateNoClock; // This state allows the frequency to not climb on each cycle.
		}
		
		cpu = getCpuClock(); 
		bus = getBusClock(); 
		
		oslDrawStringf(20,87,"%d/%d",cpu, bus);
		oslDrawStringf(20,189, "%s", lang_settingsProcessor[language][0]);
		oslDrawStringf(20,202,"%d MHz", cpu);
		oslDrawStringf(20,241, "%s", lang_settingsProcessor[language][3]);
		oslDrawStringf(20,254,"%d MHz", bus);

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(processorbg);
			oslDeleteImage(highlight);
			performanceMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(processorbg);
			oslDeleteImage(highlight);
			performanceMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(processorbg);
			oslDeleteImage(highlight);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void ramMenu()
{	
	if (DARK == 0)
	{
		performancebg = oslLoadImageFilePNG(performance2BgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		performancebg = oslLoadImageFilePNG("system/settings/Dark/performancebg2.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}
	
	if (!performancebg || !highlight)
		debugDisplay();
	
	OSL_MEMSTATUS ram = oslGetRamStatus();
	int availableRam = (ram.maxAvailable/1000000);
	int totalRam = ((20 * 1024 * 1024)/1000000);
	
	char status[20];
	
	switch(batteryM)
	{
		case 0:
			strcpy(status, "   Powersave");
			break;
	
		case 1:
			strcpy(status, "    Balanced");
			break;
			
		case 2:
			strcpy(status, "Performance");
			break;
	}
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	
		
		oslDrawImageXY(performancebg, 0, 0);

		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, 0.9f, RGBA(0, 149, 135, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, 0.9f, RGBA(0, 149, 135, 255), 0, INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(20, 78, "%dMB\n", (20 - (availableRam))); 
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);
		
		oslDrawFillRect(20, 100, 424, 120, RGB(206, 215, 219));
		oslDrawFillRect(20, 100, ((availableRam) * 21.2), 120, RGB(r, g, b));
		
		oslDrawStringf(20, 132, "%s:                                          %s", lang_settingsMain[language][3], status);
		oslDrawStringf(20, 148, "Total memory                                                  %dMB", totalRam);
		oslDrawStringf(20, 164, "Average Used(%%)                                               %d%%", (((availableRam) * 100)/(totalRam)));
		oslDrawStringf(20, 180, "Free                                                             %dMB", availableRam);

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			performanceMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			performanceMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void storageMenu()
{	
	if (DARK == 0)
	{
		performancebg = oslLoadImageFilePNG(performance2BgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		performancebg = oslLoadImageFilePNG("system/settings/Dark/performancebg2.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}
	
	if (!performancebg || !highlight)
		debugDisplay();
	
	double usedStorage = (storageGetTotalSize() - storageGetFreeSize());
	double totalStorage = storageGetTotalSize();
	double fill = ((usedStorage / totalStorage) * 404.0);
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	
		
		oslDrawImageXY(performancebg, 0, 0);
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, 0.9, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, 0.9, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(20, 80, "%.2f MB", usedStorage); 
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(20, 100, "Total used of %.2f MB", totalStorage); 
		
		
		oslDrawStringf(20, 136, "Storage:"); 
		oslDrawStringf(20, 152, "Total used of %.2f MB", totalStorage); 
		
		oslDrawFillRect(20, 172, 424, 176, RGB(206, 215, 219));
		oslDrawFillRect(20, 172, fill, 176, RGB(r, g, b));
		
		//oslDrawStringf(20, 80, "%s", lang_settingsStorage[language][0]); 
		//oslDrawStringf(20, 100,"%s %.2llu MB", lang_settingsStorage[language][1], storageGetTotalSize());
		//oslDrawStringf(20, 120, "%s %.2llu MB", lang_settingsStorage[language][2], storageGetFreeSize());
		//oslDrawStringf(20, 100,"%s %.2llu MB", lang_settingsStorage[language][1], storageGetTotalSize());
		//oslDrawStringf(20, 140, "Used Storage Capcitiy %.2llu MB", storageGetTotalSize() - storageGetFreeSize());
		
		if (osl_keys->pressed.select)
		{
			enableUsb();
		}
		
		else if (osl_keys->pressed.select)
		{
			disableUsb();
		}
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			performanceMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			performanceMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void batteryMenu()
{	
	FILE * batteryManagement;

	if (DARK == 0)
	{
		performancebg = oslLoadImageFilePNG(performanceBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		performancebg = oslLoadImageFilePNG("system/settings/Dark/performancebg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}
	select = oslLoadImageFilePNG(selectPath, OSL_IN_RAM, OSL_PF_8888);
	deselect = oslLoadImageFilePNG("system/settings/deselect.png", OSL_IN_RAM, OSL_PF_8888);
	
	batteryM = setFileDefaultsInt("system/settings/battery.bin", 1, batteryM);

	if (!performancebg || !highlight || !select || !deselect)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	

		oslDrawImageXY(performancebg, 0, 0);
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);

		oslDrawStringf(20,86, "%s", lang_settingsBattery[language][0]);
		oslDrawStringf(20,140, "%s", lang_settingsBattery[language][1]);
		oslDrawStringf(20,194, "%s", lang_settingsBattery[language][2]);

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsBattery[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				batteryManagement = fopen("system/settings/battery.bin", "w"); 
				fprintf(batteryManagement, "0");
				fclose(batteryManagement);
				batteryM = setFileDefaultsInt("system/settings/battery.bin", 1, batteryM);
				setPowerManagement();
			}
		}

		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsBattery[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				batteryManagement = fopen("system/settings/battery.bin", "w"); 
				fprintf(batteryManagement, "1");
				fclose(batteryManagement);
				batteryM = setFileDefaultsInt("system/settings/battery.bin", 1, batteryM);
				setPowerManagement();
			}
		}

		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(20,194, "%s", lang_settingsBattery[language][2]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				batteryManagement = fopen("system/settings/battery.bin", "w"); 
				fprintf(batteryManagement, "2");
				fclose(batteryManagement);
				batteryM = setFileDefaultsInt("system/settings/battery.bin", 1, batteryM);
				setPowerManagement();
			}
		}
		
		switch(batteryM)
		{
			case 0:
				oslDrawImageXY(select, 400, 80);
				oslDrawImageXY(deselect, 400, 135);
				oslDrawImageXY(deselect, 400, 190);
				break;
	
			case 1:
				oslDrawImageXY(select, 400, 135);
				oslDrawImageXY(deselect, 400, 80);
				oslDrawImageXY(deselect, 400, 190);
				break;
			
			case 2:
				oslDrawImageXY(select, 400, 190);
				oslDrawImageXY(deselect, 400, 80);
				oslDrawImageXY(deselect, 400, 135);
				break;
		}

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			oslDeleteImage(select);
			oslDeleteImage(deselect);
			performanceMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			oslDeleteImage(select);
			oslDeleteImage(deselect);
			performanceMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			oslDeleteImage(select);
			oslDeleteImage(deselect);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void setPowerManagement()
{
	FILE * processorInfo;
	batteryM = setFileDefaultsInt("system/settings/battery.bin", 1, batteryM);
	
	switch(batteryM)
	{
		case 0:
			processorState = 4;
			processorInfo = fopen("system/settings/processorInfo.bin", "w");
			fprintf(processorInfo, "%d", processorState);
			fclose(processorInfo);
			setCpuBoot();
			imposeSetBrightness(0);
			break;
	
		case 1:
			processorState = 4;
			processorInfo = fopen("system/settings/processorInfo.bin", "w");
			fprintf(processorInfo, "%d", processorState);
			fclose(processorInfo);
			setCpuBoot();
			imposeSetBrightness(1);
			break;
			
		case 2:
			processorState = 7;
			processorInfo = fopen("system/settings/processorInfo.bin", "w");
			fprintf(processorInfo, "%d", processorState);
			fclose(processorInfo);
			setCpuBoot();
			break;
	}
}

void displayMenu()
{	
	if (DARK == 0)
	{
		displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		displaybg = oslLoadImageFilePNG("system/settings/Dark/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}

	if (!displaybg || !highlight)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);

		oslDrawImageXY(displaybg, 0, 0);
		
		oslDrawStringf(20,86, "%s", lang_settingsDisplay[language][0]);
		oslDrawStringf(20,140, "%s", lang_settingsDisplay[language][1]);
		oslDrawStringf(20,195, "%s", lang_settingsDisplay[language][2]);
		oslDrawStringf(20,245, "%s", lang_settingsDisplay[language][3]);
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsDisplay[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displayThemes();
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsDisplay[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displayTime();
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(20,195, "%s", lang_settingsDisplay[language][2]);
			if (osl_keys->pressed.cross)
			{	
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displaySubThemes("system/settings/language", 4);
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 216 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 228);
			oslDrawStringf(20,245, "%s", lang_settingsDisplay[language][3]);
			if (osl_keys->pressed.cross)
			{	
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displayMiscellaneous();
			}
		}

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			settingsMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void displayThemes()
{	
	if (DARK == 0)
	{
		displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		displaybg = oslLoadImageFilePNG("system/settings/Dark/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}

	if (!displaybg || !highlight)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);

		oslDrawImageXY(displaybg, 0, 0);
		
		oslDrawStringf(20,86, "%s", lang_settingsThemes[language][0]);
		oslDrawStringf(20,140, "%s", lang_settingsThemes[language][1]);
		oslDrawStringf(20,194, "%s", lang_settingsThemes[language][2]);
		oslDrawStringf(20,248, "%s", lang_settingsThemes[language][3]);
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsThemes[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displaySubThemes("system/themes", 3);
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsThemes[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displaySubThemes("system/framework/framework-res/res", 1);
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(20,194, "%s", lang_settingsThemes[language][2]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displayFontMenu();
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 216 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 227);
			oslDrawStringf(20,248, "%s", lang_settingsThemes[language][3]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displaySubThemes("system/icons", 2);
			}
		}

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			displayMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			displayMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void displayFontMenu()
{	
	if (DARK == 0)
	{
		displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		displaybg = oslLoadImageFilePNG("system/settings/Dark/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}

	if (!displaybg || !highlight)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);

		oslDrawImageXY(displaybg, 0, 0);
		
		oslDrawStringf(20,86, "%s", lang_settingsFonts[language][0]);
		oslDrawStringf(20,140, "%s", lang_settingsFonts[language][1]);
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsFonts[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displaySubThemes("system/fonts", 0);
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsFonts[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displayFontSizeMenu();
			}
		}

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			displayThemes();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			displayThemes();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void displayFontSizeMenu()
{	
	FILE * fontManagement;
	int tempFontData = 0;

	if (DARK == 0)
	{
		displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		displaybg = oslLoadImageFilePNG("system/settings/Dark/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}

	select = oslLoadImageFilePNG(selectPath, OSL_IN_RAM, OSL_PF_8888);
	deselect = oslLoadImageFilePNG("system/settings/deselect.png", OSL_IN_RAM, OSL_PF_8888);
	
	fontSize = setFileDefaultsFloat("system/settings/fontSize.bin", 0.5, fontSize);
	
	if (fontSize < 0.5)
		tempFontData = 0;
	else if (fontSize == 0.5)
		tempFontData = 1;
	else if (fontSize > 0.6)
		tempFontData = 2;

	if (!displaybg || !highlight || !select || !deselect)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	

		oslDrawImageXY(displaybg, 0, 0);
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);

		oslDrawStringf(20,86, "%s", lang_settingsFonts[language][2]);
		oslDrawStringf(20,140, "%s", lang_settingsFonts[language][3]);
		oslDrawStringf(20,194, "%s", lang_settingsFonts[language][4]);

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsFonts[language][2]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				fontManagement = fopen("system/settings/fontSize.bin", "w"); 
				fprintf(fontManagement, "0.4");
				fclose(fontManagement);
				tempFontData = 0;
				fontSize = setFileDefaultsFloat("system/settings/fontSize.bin", 1, fontSize);
			}
		}

		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsFonts[language][3]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				fontManagement = fopen("system/settings/fontSize.bin", "w"); 
				fprintf(fontManagement, "0.5");
				fclose(fontManagement);
				tempFontData = 1;
				fontSize = setFileDefaultsFloat("system/settings/fontSize.bin", 1, fontSize);
			}
		}

		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(20,194, "%s", lang_settingsFonts[language][4]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				fontManagement = fopen("system/settings/fontSize.bin", "w"); 
				fprintf(fontManagement, "0.6");
				fclose(fontManagement);
				tempFontData = 2;
				fontSize = setFileDefaultsFloat("system/settings/fontSize.bin", 1, fontSize);
			}
		}
		
		switch(tempFontData)
		{
			case 0:
				oslDrawImageXY(select, 400, 80);
				oslDrawImageXY(deselect, 400, 135);
				oslDrawImageXY(deselect, 400, 190);
				break;
	
			case 1:
				oslDrawImageXY(select, 400, 135);
				oslDrawImageXY(deselect, 400, 80);
				oslDrawImageXY(deselect, 400, 190);
				break;
			
			case 2:
				oslDrawImageXY(select, 400, 190);
				oslDrawImageXY(deselect, 400, 80);
				oslDrawImageXY(deselect, 400, 135);
				break;
		}

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(displaybg);
			oslDeleteImage(select);
			oslDeleteImage(deselect);
			displayFontMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(displaybg);
			oslDeleteImage(select);
			oslDeleteImage(deselect);
			displayFontMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(displaybg);
			oslDeleteImage(select);
			oslDeleteImage(deselect);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void settingsDisplay()
{	
	oslDrawImageXY(displaybg, 0, 0);
	oslDrawImageXY(highlight,0,(current - curScroll)*56+SETTINGS_CURR_DISPLAY_Y);

	battery(330,2,0);
	digitaltime(381,4,0,hrTime);
	
	// Displays the directories, while also incorporating the scrolling
	for(i=curScroll;i<MAX_SETTINGS_DISPLAY+curScroll;i++) 
	{
		// Handles the cursor and the display to not move past 4.
		// For moving down
		//if ((folderIcons[i].active == 0) && (current >= i-1)) {
	
		if ((folderIcons[i].active == 0) && (current >= i-1)) 
		{
			current = i-1;
			break;
		}
		// For moving up
		if (current <= curScroll-1) 
		{
			current = curScroll-1;
			break;
		}
		
		// If the currently selected item is active, then display the name
		if (folderIcons[i].active == 1) 
		{
			if (DARK == 0)
				oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
			else
				oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(SETTINGS_DISPLAY_X, (i - curScroll)*55+SETTINGS_DISPLAY_Y, "%.56s", folderIcons[i].name);	// change the X & Y value accordingly if you want to move it (for Y, just change the +10)		
		}
	}
}

void changeFont() //Created a separated function for this only because deleting a font while its in use causes it to crash.
{
	while (!osl_quit)
	{		
		oslDeleteFont(Roboto);
		oslPlaySound(KeypressStandard, 1);  
		FILE * fontPathTXT = fopen("system/settings/font.bin", "w");
		fprintf(fontPathTXT,"%s", folderIcons[current].filePath);
		fclose(fontPathTXT);
		Roboto = oslLoadIntraFontFile(folderIcons[current].filePath, INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
		oslSetFont(Roboto);
		return;
	}
}

int changeLanguage() //Created a separated function for this only because deleting a font while its in use causes it to crash.
{
	if (strcmp(folderIcons[current].filePath, "system/settings/language/English") == 0)
		language = 0;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/French") == 0)
		language = 1;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Polish") == 0)
		language = 2;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/German") == 0)
		language = 3;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Dutch") == 0)
		language = 4;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Spanish") == 0)
		language = 5;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Portuguese") == 0)
		language = 6;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Brazilian-Portuguese") == 0)
		language = 7;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Norwegian") == 0)
		language = 8;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Romanian") == 0)
		language = 9;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Italian") == 0)
		language = 10;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Russian") == 0)
		language = 11;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Ukrainian") == 0)
		language = 12;
	
	FILE * languagePath = fopen("system/settings/language.bin", "w");
	fprintf(languagePath, "%d", language);
	fclose(languagePath);
	
	return language;
}

void replaceAsset(char tempData[], char path[], char imgPath[], char redirectPath[])
{
	strcpy(tempData, path);
	strcat(tempData, imgPath); 
	strcpy(redirectPath, tempData);
}

void themesLoad()
{	
	strcpy(themeDirPath, setFileDefaultsChar("system/settings/theme.bin", "system", themeDirPath));

	char highlightImg[100] = "/settings/highlight.png";
	char settingsBgImg[100] = "/settings/settingsbg.png";
	char displayBgImg[100] = "/settings/displaybg.png";
	char aboutBgImg[100] = "/settings/aboutbg.png";
	char developerBgImg[100] = "/settings/developerbg.png";
	char performanceBgImg[100] = "/settings/performancebg.png";
	char performance2BgImg[100] = "/settings/performancebg2.png";
	char securityBgImg[100] = "/settings/securitybg.png";
	char updatesBgImg[100] = "/settings/updatesbg.png";
	char wifiBgImg[100] = "/settings/wifibg.png";
	char offSwitchImg[100] = "/settings/offswitch.png";
	char onSwitchImg[100] = "/settings/onswitch.png";
	char selectImg[100] = "/settings/select.png";
	
	char aboutImg[100] = "/settings/about.png";
	char developerImg[100] = "/settings/developeroptions.png";
	char performanceImg[100] = "/settings/performance.png";
	char securityImg[100] = "/settings/security.png";
	char themesImg[100] = "/settings/themes.png";
	char wifiImg[100] = "/settings/wifi.png";
	
	char apolloBgImg[100] = "/app/musicplayer/mp3bg.png";
	char apolloSelectorImg[100] = "/app/musicplayer/mp3_select.png";
	char nowplayingBgImg[100] = "/app/musicplayer/nowplaying.png";
	
	char fmBgImg[100] = "/app/filemanager/filemanagerbg.png";
	char fmSelectorImg[100] = "/app/filemanager/bar.png";
	char diriconImg[100] = "/app/filemanager/dir.png";
	
	char galleryBgImg[100] = "/app/gallery/gallerybg.png";
	char gallerySelectorImg[100] = "/app/gallery/highlight.png";
	
	char gameImg[100] = "/app/game/gamebg.png";
	char gameSelectorImg[100] = "/app/game/gameselector.png";
	
	char quickSettingsBgImg[100] = "/home/menu/quickSettings.png";
	char controlsImg[100] = "/home/menu/brightnesscontrol.png";
	char volumeControlImg[100] = "/home/menu/volumeControl.png";
	char backdropImg[100] = "/home/icons/backdrop.png";
	char navbarImg[100] = "/home/icons/navbar.png";
	char navbar2Img[100] = "/home/icons/navbar2.png";
	char navbarHighlightImg[100] = "/home/icons/navbarHighlight.png";
	char navbarHighlight2Img[100] = "/home/icons/navbarHighlight2.png";
	
	char cursorImg[100] = "/cursor/cursor.png";
	char keyBoardBgImg[100] = "/settings/keyboard.png";
	
	char rgbValues[100] = "/settings/rgb.bin";
	char appDrawerFontColor[100] = "/app/rgb.bin";
	char fileManagerFontColor[100] = "/app/filemanager/rgb.bin";
	char galleryFontColor[100] = "/app/gallery/rgb.bin";
	char gameFontColor[100] = "/app/game/rgb.bin";
	char apolloFontColor[100] = "/app/musicplayer/rgb.bin";
	char settingsFontColor[100] = "/settings/srgb.bin";
	char timeAndBatteryFontColor[100] = "/home/rgb.bin";
	char lockScreenWidgetFontColor[100] = "/app/clock/rgb.bin";
	char backgroundData[100] = "/settings/background.bin";
	
	replaceAsset(tempData, themeDirPath, highlightImg, highlightPath);
	replaceAsset(tempData, themeDirPath, aboutBgImg, aboutBgPath);
	replaceAsset(tempData, themeDirPath, developerBgImg, developerBgPath);
	replaceAsset(tempData, themeDirPath, performanceBgImg, performanceBgPath);
	replaceAsset(tempData, themeDirPath, performance2BgImg, performance2BgPath);
	replaceAsset(tempData, themeDirPath, securityBgImg, securityBgPath);
	replaceAsset(tempData, themeDirPath, updatesBgImg, updatesBgPath);
	replaceAsset(tempData, themeDirPath, wifiBgImg, wifiBgPath);
	replaceAsset(tempData, themeDirPath, offSwitchImg, offSwitchPath);
	replaceAsset(tempData, themeDirPath, onSwitchImg, onSwitchPath);
	replaceAsset(tempData, themeDirPath, selectImg, selectPath);
	replaceAsset(tempData, themeDirPath, aboutImg, aboutPath);
	replaceAsset(tempData, themeDirPath, developerImg, developerPath);
	replaceAsset(tempData, themeDirPath, performanceImg, performancePath);
	replaceAsset(tempData, themeDirPath, securityImg, securityPath);
	replaceAsset(tempData, themeDirPath, themesImg, themesPath);
	replaceAsset(tempData, themeDirPath, wifiImg, wifiPath);
	replaceAsset(tempData, themeDirPath, settingsBgImg, settingsBgPath);
	replaceAsset(tempData, themeDirPath, displayBgImg, displayBgPath);
	replaceAsset(tempData, themeDirPath, apolloBgImg, apolloBgPath);
	replaceAsset(tempData, themeDirPath, apolloSelectorImg, apolloSelectorPath);
	replaceAsset(tempData, themeDirPath, nowplayingBgImg, nowplayingBgPath);
	replaceAsset(tempData, themeDirPath, fmBgImg, fmBgPath);
	replaceAsset(tempData, themeDirPath, fmSelectorImg, fmSelectorPath);
	replaceAsset(tempData, themeDirPath, diriconImg, diriconPath);
	replaceAsset(tempData, themeDirPath, galleryBgImg, galleryBgPath);
	replaceAsset(tempData, themeDirPath, gallerySelectorImg, gallerySelectorPath);
	replaceAsset(tempData, themeDirPath, gameImg, gameBgPath);
	replaceAsset(tempData, themeDirPath, gameSelectorImg, gameSelectorPath);
	replaceAsset(tempData, themeDirPath, quickSettingsBgImg, quickSettingsBgPath);
	replaceAsset(tempData, themeDirPath, controlsImg, controlsPath);
	replaceAsset(tempData, themeDirPath, volumeControlImg, volumeControlPath);
	replaceAsset(tempData, themeDirPath, navbarImg, navbarPath);
	replaceAsset(tempData, themeDirPath, navbar2Img, navbar2Path);
	replaceAsset(tempData, themeDirPath, navbarHighlightImg, navbarHighlightPath);
	replaceAsset(tempData, themeDirPath, navbarHighlight2Img, navbarHighlight2Path);
	replaceAsset(tempData, themeDirPath, backdropImg, backdropPath);
	replaceAsset(tempData, themeDirPath, cursorImg, cursorPath);
	replaceAsset(tempData, themeDirPath, keyBoardBgImg, keyBoardBgPath);
	replaceAsset(tempData, themeDirPath, rgbValues, rgbValuesPath);
	replaceAsset(tempData, themeDirPath, backgroundData, backgroundPath);
	replaceAsset(tempData, themeDirPath, appDrawerFontColor, appDrawerFontColorPath);
	replaceAsset(tempData, themeDirPath, fileManagerFontColor, fileManagerFontColorPath);
	replaceAsset(tempData, themeDirPath, galleryFontColor, galleryFontColorPath);
	replaceAsset(tempData, themeDirPath, gameFontColor, gameFontColorPath);
	replaceAsset(tempData, themeDirPath, apolloFontColor, apolloFontColorPath);
	replaceAsset(tempData, themeDirPath, settingsFontColor, settingsFontColorPath);
	replaceAsset(tempData, themeDirPath, timeAndBatteryFontColor, timeAndBatteryFontColorPath);
	replaceAsset(tempData, themeDirPath, lockScreenWidgetFontColor, clockWidgetFontColorPath);
	
	FILE *temp;
	 
	if (!(fileExists(rgbValuesPath)))
	{
		temp = fopen(rgbValuesPath, "w");
		fprintf(temp, "0\n149\n135");
		fclose(temp);
	}
	
	temp = fopen(rgbValuesPath, "r");
	fscanf(temp, "%d %d %d", &r, &g, &b);
	fclose(temp);
	
	strcpy(backgroundPath, setFileDefaultsChar(backgroundPath, "system/framework/framework-res/res/background.png", backgroundPath));
}

void themesReload()
{
	oslDeleteImage(background);
	oslDeleteImage(navbar);
	oslDeleteImage(navbar2);
	oslDeleteImage(navbarHighlight);
	oslDeleteImage(navbarHighlight2);
	oslDeleteImage(quickSettings);
	oslDeleteImage(control);
	oslDeleteImage(volumeControl);
	oslDeleteImage(cursor);
	oslDeleteImage(displaybg);
	oslDeleteImage(highlight);

	background = oslLoadImageFile(backgroundPath, OSL_IN_RAM, OSL_PF_8888);
	navbar = oslLoadImageFile(navbarPath, OSL_IN_RAM, OSL_PF_8888);
	navbar2 = oslLoadImageFile(navbar2Path, OSL_IN_RAM, OSL_PF_8888);
	navbarHighlight = oslLoadImageFilePNG(navbarHighlightPath, OSL_IN_RAM, OSL_PF_8888);
	navbarHighlight2 = oslLoadImageFilePNG(navbarHighlight2Path, OSL_IN_RAM, OSL_PF_8888);
	quickSettings = oslLoadImageFile(quickSettingsBgPath, OSL_IN_VRAM, OSL_PF_8888);
	control = oslLoadImageFilePNG(controlsPath, OSL_IN_VRAM, OSL_PF_8888);
	cursor = oslLoadImageFilePNG(cursorPath, OSL_IN_RAM, OSL_PF_8888);
	volumeControl = oslLoadImageFile(volumeControlPath, OSL_IN_RAM, OSL_PF_8888);
	displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	
	FILE *temp;
	 
	if (!(fileExists(settingsFontColorPath)))
	{
		temp = fopen(settingsFontColorPath, "w");
		fprintf(temp, "0\n0\n0");
		fclose(temp);
	}
	
	temp = fopen(settingsFontColorPath, "r");
	fscanf(temp, "%d %d %d", &fontColor.r, &fontColor.g, &fontColor.b);
	fclose(temp);
	
	FILE *temp2;
	 
	if (!(fileExists(timeAndBatteryFontColorPath)))
	{
		temp2 = fopen(timeAndBatteryFontColorPath, "w");
		fprintf(temp2, "255\n255\n255");
		fclose(temp2);
	}
	
	temp2 = fopen(timeAndBatteryFontColorPath, "r");
	fscanf(temp2, "%d %d %d", &fontColorTime.r, &fontColorTime.g, &fontColorTime.b);
	fclose(temp2);
}

void iconPackLoad()
{
	strcpy(appDirPath, setFileDefaultsChar("system/settings/iconpack.bin", "system/icons/Default", appDirPath));
	
	char allappsImg[50] = "/allapps/ic_allapps.png";
	char allapps_pressedImg[50] = "/allapps/ic_allapps_pressed.png";
	char apolloImg[50] = "/music/ic_launcher_apollo.png";
	char browserImg[50] = "/browser/ic_launcher_browser.png";
	char calcImg[50] = "/calculator/ic_launcher_calculator.png";
	char clockImg[50] = "/clock/ic_launcher_clock.png";
	char fmImg[50] = "/filemanager/ic_launcher_filemanager.png";
	char galleryImg[50] = "/gallery/ic_launcher_gallery.png";
	char gameImg[50] = "/game/ic_launcher_game.png";
	char messagesImg[50] = "/messenger/ic_launcher_messenger.png";
	char settingsImg[50] = "/settings/ic_launcher_settings.png";
	char umdImg[50] = "/umd/ic_launcher_umd.png";
	
	replaceAsset(tempData, appDirPath, allappsImg, allappsPath);
	replaceAsset(tempData, appDirPath, allapps_pressedImg, allapps_pressedPath);
	replaceAsset(tempData, appDirPath, apolloImg, apolloPath);
	replaceAsset(tempData, appDirPath, browserImg, browserPath);
	replaceAsset(tempData, appDirPath, calcImg, calcPath);
	replaceAsset(tempData, appDirPath, clockImg, clockPath);
	replaceAsset(tempData, appDirPath, fmImg, fmPath);
	replaceAsset(tempData, appDirPath, galleryImg, galleryPath);
	replaceAsset(tempData, appDirPath, gameImg, gamePath);
	replaceAsset(tempData, appDirPath, messagesImg, messagesPath);
	replaceAsset(tempData, appDirPath, settingsImg, settingsPath);
	replaceAsset(tempData, appDirPath, umdImg, umdPath);
}

void iconPackReload()
{
	oslDeleteImage(ic_launcher_apollo);
	oslDeleteImage(ic_launcher_browser);
	oslDeleteImage(ic_launcher_calculator);
	oslDeleteImage(ic_launcher_filemanager);
	oslDeleteImage(ic_launcher_gallery);
	oslDeleteImage(ic_launcher_game);
	oslDeleteImage(ic_launcher_messenger);
	oslDeleteImage(ic_launcher_settings);
	oslDeleteImage(ic_launcher_umd);

	ic_launcher_apollo = oslLoadImageFilePNG(apolloPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_browser = oslLoadImageFile(browserPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_calculator = oslLoadImageFilePNG(calcPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_filemanager = oslLoadImageFilePNG(fmPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_gallery = oslLoadImageFilePNG(galleryPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_game = oslLoadImageFilePNG(gamePath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_messenger = oslLoadImageFilePNG(messagesPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_settings = oslLoadImageFilePNG(settingsPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_umd = oslLoadImageFilePNG(umdPath, OSL_IN_RAM, OSL_PF_8888);
}

void settingsControls(int n) //Controls
{
	oslReadKeys();	

	if (pad.Buttons != oldpad.Buttons) 
	{
		if (osl_keys->pressed.down)
		{
			selectionDown(MAX_SETTINGS_DISPLAY);
			timer = 0;
		}
		else if (osl_keys->pressed.up)
		{
			selectionUp();
			timer = 0;
		}
		
		if (osl_keys->pressed.right)
		{
			selectionDownx5(MAX_SETTINGS_DISPLAY);
			timer = 0;
		}
		else if (osl_keys->pressed.left)
		{
			selectionUpx5();	
			timer = 0;
		}
		
		if (osl_keys->pressed.triangle) 
		{
			curScroll = 1;
			current = 1;
		}
		
		if (osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
		}
	}
	
	volumeController();
	
	char * ext = strrchr(folderIcons[current].filePath, '.'); 
	
	if (((ext) != NULL) && (n == 0) && ((strcmp(ext ,".pgf") == 0) || (strcmp(ext ,".PGF") == 0)))
	{
		if ((osl_keys->pressed.cross) && (strcmp(folderIcons[current].filePath, "doesn't matter") != 0))
		{
			changeFont();
		}
	}
	else if (((ext) != NULL) && (n == 1) && ((strcmp(ext ,".png") == 0) || (strcmp(ext ,".jpg") == 0) || (strcmp(ext ,".jpeg") == 0) || (strcmp(ext ,".gif") == 0) || (strcmp(ext ,".PNG") == 0) || (strcmp(ext ,".JPG") == 0) || (strcmp(ext ,".JPEG") == 0) || (strcmp(ext ,".GIF") == 0)))
	{
		if ((osl_keys->pressed.cross) && (strcmp(folderIcons[current].filePath, "doesn't matter") != 0))
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);	
			showImage(folderIcons[current].filePath, 1);
		}
	}
	
	else if (n == 2)
	{
		if ((osl_keys->pressed.cross) && (strcmp(folderIcons[current].filePath, "doesn't matter") != 0))
		{
			strcpy(appDirPath, folderIcons[current].filePath);
			FILE * iconPackTxt = fopen("system/settings/iconpack.bin", "w");
			fprintf(iconPackTxt,"%s", appDirPath);
			fclose(iconPackTxt);
			iconPackLoad();
			iconPackReload();
		}
	}
	
	else if (n == 3)
	{
		if ((osl_keys->pressed.cross) && (strcmp(folderIcons[current].filePath, "doesn't matter") != 0))
		{
			strcpy(themeDirPath, folderIcons[current].filePath);
			FILE * themeTxt = fopen("system/settings/theme.bin", "w");
			if (strcmp(themeDirPath, "system/themes/Default") == 0)
			{
				fprintf(themeTxt,"system");
				fclose(themeTxt);
				themesLoad();
				themesReload();
			}
			else
			{
				fprintf(themeTxt,"%s", themeDirPath);
				fclose(themeTxt);
				themesLoad();
				themesReload();
			}
		}
	}
	
	else if (n == 4)
	{
		if ((osl_keys->pressed.cross) && (strcmp(folderIcons[current].filePath, "doesn't matter") != 0))
		{
			changeLanguage();
			language = setFileDefaultsInt("system/settings/language.bin", 0, language);
		}
	}
	
	if (osl_keys->pressed.circle && n == 0)
	{	
		oslDeleteImage(displaybg);
		oslDeleteImage(highlight);
		displayFontMenu();
	}
	else if (osl_keys->pressed.circle && n == 4)
	{	
		oslDeleteImage(displaybg);
		oslDeleteImage(highlight);
		displayMenu();
	}
	else if (osl_keys->pressed.circle)
	{	
		oslDeleteImage(displaybg);
		oslDeleteImage(highlight);
		displayThemes();
	}
	
	if (osl_keys->pressed.square)
	{
		powermenu();
	}
		
	if (osl_keys->pressed.L)
	{
		oslPlaySound(Lock, 1);  
		lockscreen();
    }
	
	captureScreenshot();
	
	timer++;
	if ((timer > 30) && (pad.Buttons & PSP_CTRL_UP))
	{
		selectionUp();
		timer = 25;
	} 
	else if ((timer > 30) && (pad.Buttons & PSP_CTRL_DOWN))
	{
		selectionDown(MAX_SETTINGS_DISPLAY);
		timer = 25;
	}

	if (current < 1) 
		current = 1;
	if (current > MAX_FILES) 
		current = MAX_FILES;
}

char * settingsBrowse(const char * path, int n) // n is used here to search for fonts or wallpaper
{
	folderScan(path);
	dirVars();
	
	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));	
		oldpad = pad;
		sceCtrlReadBufferPositive(&pad, 1);
		settingsDisplay();
		
		if (n == 0)
			settingsControls(0); //0 is to used for selecting a font
		else if (n == 1)
			settingsControls(1); // 1 is used for selecting a wallpaper
		else if (n == 2)
			settingsControls(2); // 2 is to used for selecting a folder for iconpacks
		else if (n == 3)
			settingsControls(3); // 3 is to used for selecting a folder for themes
		else if (n == 4)
			settingsControls(4); // 4 is to used for selecting a language
			
		if (strlen(returnMe) > 4) 
			break;
			
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();
	}
	return returnMe;
}

void displaySubThemes(char * browseDirectory, int n) // n is used here to search for fonts or wallpaper
{	
	if (DARK == 0)
	{
		displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		displaybg = oslLoadImageFilePNG("system/settings/Dark/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}

	if (!displaybg || !highlight)
		debugDisplay();
		
	if (n == 0)	
		browseDirectory = settingsBrowse("system/fonts", 0); //For fonts
	else if (n == 1)
		browseDirectory = settingsBrowse("system/framework/framework-res/res", 1); //For wallpapers
	else if (n == 2)
		browseDirectory = settingsBrowse("system/icons", 2); //For icon packs
	else if (n == 3)
		browseDirectory = settingsBrowse("system/themes", 3); //For themes
	else if (n == 4)
		browseDirectory = settingsBrowse("system/settings/language", 4); //For language
	
	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		centerText(480/2, 272/2, browseDirectory, 50);
		
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();	
	}
}

void displayTime()
{	
	FILE * timeSetTxt;
	
	if (DARK == 0)
	{
		displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		displaybg = oslLoadImageFilePNG("system/settings/Dark/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}

	offswitch = oslLoadImageFilePNG(offSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG(onSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	
	if (!displaybg || !highlight || !offswitch || !onswitch)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);

		oslDrawImageXY(displaybg, 0, 0);
		
		oslDrawStringf(20,86, "%s", lang_settingsTime[language][0]);
		
		if (hrTime == 0)
			oslDrawImageXY(offswitch,392,80);
				
		else if (hrTime == 1)
			oslDrawImageXY(onswitch,392,80);
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsTime[language][0]);
			if (hrTime == 0)
			{
				oslDrawImageXY(offswitch,392,80);
				
				if (osl_keys->pressed.cross)
				{
					oslPlaySound(KeypressStandard, 1);
					hrTime = 1;
					timeSetTxt = fopen("system/app/clock/timeSet.bin", "w");
					fprintf(timeSetTxt, "%d", hrTime);
					fclose(timeSetTxt);
				}
			}
			else if (hrTime == 1)
			{
				oslDrawImageXY(onswitch,392,80);
			
				if (osl_keys->pressed.cross)
				{
					oslPlaySound(KeypressStandard, 1);
					hrTime = 0;
					timeSetTxt = fopen("system/app/clock/timeSet.bin", "w");
					fprintf(timeSetTxt, "%d", hrTime);
					fclose(timeSetTxt);
				}
			}
		}

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			displayMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			displayMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void displayMiscellaneous()
{	
	FILE * widgetActivation;
	FILE * eDesktopActivation;
	FILE * bootAnimActivation;
	FILE * gBootActivation;
	
	bootAnimActivation = fopen("system/boot/bootAnimActivator.bin", "r");
	fscanf(bootAnimActivation,"%d",&bootAnimActivator);
	fclose(bootAnimActivation);

	if (DARK == 0)
	{
		displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		displaybg = oslLoadImageFilePNG("system/settings/Dark/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}

	offswitch = oslLoadImageFilePNG(offSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG(onSwitchPath, OSL_IN_RAM, OSL_PF_8888);

	if (!displaybg || !highlight || !offswitch || !onswitch)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);

		oslDrawImageXY(displaybg, 0, 0);
		
		oslDrawStringf(20,86, "%s", lang_settingsMiscellaneous[language][0]);
		oslDrawStringf(20,140, "%s", lang_settingsMiscellaneous[language][1]);
		oslDrawStringf(20,195, "%s", lang_settingsMiscellaneous[language][2]);
		oslDrawStringf(20,245, "%s", lang_settingsMiscellaneous[language][3]);
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsMiscellaneous[language][0]);
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsMiscellaneous[language][1]);
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(20,195, "%s", lang_settingsMiscellaneous[language][2]);
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 216 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 228);
			oslDrawStringf(20,245, "%s", lang_settingsMiscellaneous[language][3]);
		}

		if (widgetActivator == 0)
		{
			oslDrawImageXY(offswitch,392,80);
			
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				widgetActivation = fopen("system/widget/widgetActivator.bin", "w");
				widgetActivator = 1;
				fprintf(widgetActivation, "1");
				fclose(widgetActivation);
			}
		}
		
		else if (widgetActivator == 1)
		{
			oslDrawImageXY(onswitch,392,80);
		
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				widgetActivation = fopen("system/widget/widgetActivator.bin", "w");
				widgetActivator = 0;
				fprintf(widgetActivation, "0");
				fclose(widgetActivation);
			}
		}
		
		if (eDesktopActivator == 0)
		{
			oslDrawImageXY(offswitch,392,133);
			
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				eDesktopActivation = fopen("system/home/eDesktopActivator.bin", "w");
				eDesktopActivator = 1;
				fprintf(eDesktopActivation, "1");
				fclose(eDesktopActivation);
			}
		}
		
		else if (eDesktopActivator == 1)
		{
			oslDrawImageXY(onswitch,392,133);
		
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				eDesktopActivation = fopen("system/home/eDesktopActivator.bin", "w");
				eDesktopActivator = 0;
				fprintf(eDesktopActivation, "0");
				fclose(eDesktopActivation);
			}
		}
		
		
		if (bootAnimActivator == 0)
		{
			oslDrawImageXY(offswitch,392,186);
			
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				bootAnimActivation = fopen("system/boot/bootAnimActivator.bin", "w");
				bootAnimActivator = 1;
				fprintf(bootAnimActivation, "1");
				fclose(bootAnimActivation);
			}
		}
		
		else if (bootAnimActivator == 1)
		{
			oslDrawImageXY(onswitch,392,186);
		
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				bootAnimActivation = fopen("system/boot/bootAnimActivator.bin", "w");
				bootAnimActivator = 0;
				fprintf(bootAnimActivation, "0");
				fclose(bootAnimActivation);
			}
		}
		if (gBootActivator == 0)
		{
			oslDrawImageXY(offswitch,392,236);
			
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 216 && cursor->y <= 272 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				gBootActivation = fopen("system/app/game/boot/gBootActivator.bin", "w");
				gBootActivator = 1;
				fprintf(gBootActivation, "1");
				fclose(gBootActivation);
			}
		}
		
		else if (gBootActivator == 1)
		{
			oslDrawImageXY(onswitch,392,236);
		
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 216 && cursor->y <= 272 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				gBootActivation = fopen("system/app/game/boot/gBootActivator.bin", "w");
				gBootActivator = 0;
				fprintf(gBootActivation, "0");
				fclose(gBootActivation);
			}
		}
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			displayMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			displayMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void securityMenu()
{	
	if (DARK == 0)
	{
		securitybg = oslLoadImageFilePNG(securityBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		securitybg = oslLoadImageFilePNG("system/settings/Dark/securitybg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}
	
	offswitch = oslLoadImageFilePNG(offSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG(onSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	
	if (!securitybg || !highlight || !offswitch || !onswitch)
		debugDisplay();
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	
		
		oslDrawImageXY(securitybg, 0, 0);
		
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(20,83, "%s", lang_settingsSecuirty[language][0]); 
		oslDrawStringf(20,144, "%s", lang_settingsSecuirty[language][1]); 
		oslDrawStringf(20,200, "%s", lang_settingsSecuirty[language][2]); 
		
		if (cursor->x  >= 0 && cursor->x  <= 444 && cursor->y >= 61 && cursor->y <= 118)
		{	
			oslDrawImageXY(highlight, 0, 62);
			oslDrawStringf(20,83, "%s", lang_settingsSecuirty[language][0]); 
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				if (fileExists("system/settings/password.bin"))
					sceIoRemove("system/settings/password.bin");
				else if (fileExists("system/settings/pin.bin"))
					sceIoRemove("system/settings/pin.bin");
				openOSK("Enter Password", "", 20, -1);
				FILE * password = fopen("system/settings/password.bin", "w");
				fprintf(password, "%s", tempMessage);
				fclose(password);
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 119 && cursor->y <= 178)
		{	
			oslDrawImageXY(highlight, 0, 120);
			oslDrawStringf(20,144, "%s", lang_settingsSecuirty[language][1]); 
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				if (fileExists("system/settings/password.bin"))
					sceIoRemove("system/settings/password.bin");
				else if (fileExists("system/settings/pin.bin"))
					sceIoRemove("system/settings/pin.bin");
				openOSK("Enter Pin", "", 5, -1);
				FILE * pin = fopen("system/settings/pin.bin", "w");
				fprintf(pin, "%s", tempPin);
				fclose(pin);
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 180 && cursor->y <= 235)
		{	
			oslDrawImageXY(highlight, 0, 178);
			oslDrawStringf(20,200, "%s", lang_settingsSecuirty[language][2]); 
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				if (fileExists("system/settings/password.bin"))
					sceIoRemove("system/settings/password.bin");
				else if (fileExists("system/settings/pin.bin"))
					sceIoRemove("system/settings/pin.bin");
			}
		}
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(securitybg);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(securitybg);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(securitybg);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void wifiMenu()
{	
	static char wifiData[100];

	if (DARK == 0)
		wifibg = oslLoadImageFilePNG(wifiBgPath, OSL_IN_RAM, OSL_PF_8888);
	else
		wifibg = oslLoadImageFilePNG("system/settings/Dark/wifibg.png", OSL_IN_RAM, OSL_PF_8888);

	offswitch = oslLoadImageFilePNG(offSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG(onSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	
	if (!wifibg || !offswitch || !onswitch)
		debugDisplay();
		
	int skip = 0;	
	int enabled = 1;
    int selectedConfig = 0;
	int wifi_y = 95;
	static char buffer[100] = "";
	
	//Get connections list:
    struct oslNetConfig configs[OSL_MAX_NET_CONFIGS];
    int numconfigs = oslGetNetConfigs(configs);
	if (!numconfigs)
	{
        sprintf(wifiData, "%s", lang_settingsWifi[language][0]);
        enabled = 0;
    }
	
	if (!oslIsWlanPowerOn())
        sprintf(wifiData, "%s", lang_settingsWifi[language][1]);

	while (!osl_quit)
	{		
		if (!skip)
		{
			LowMemExit();
	
			oslStartDrawing();
		
			oslClearScreen(RGB(0, 0, 0));
		
			controls();	

			oslDrawImageXY(wifibg, 0, 0);
			
			if (DARK == 0)
				oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
			else
				oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);

			if (enabled)
			{
				sprintf(buffer, "%s", configs[selectedConfig].name);
				oslDrawStringf(10, wifi_y+28, configs[selectedConfig].name);
				oslDrawStringf(10, 206, "%s", lang_settingsWifi[language][2]);
			}
		
			oslDrawStringf(10, 220, wifiData);
	
			switchStatus(3);
			navbarButtons(2);
			battery(330,2,0);
			digitaltime(381,4,0,hrTime);
			androidQuickSettings();
			volumeController();
		
			if (osl_keys->released.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				connectToAP(selectedConfig + 1);
			}
			else if (osl_keys->released.up)
			{
				if (++selectedConfig >= numconfigs)
					selectedConfig = numconfigs - 1;
			} 
			else if (osl_keys->released.down)
			{
				if (--selectedConfig < 0)
					selectedConfig = 0;
			}
		
			oslDrawImage(cursor);
		
			if (osl_keys->pressed.square)
			{
				powermenu();
			}
		
			if (osl_keys->pressed.L)
			{
				oslPlaySound(Lock, 1);  
				lockscreen();
			}
		
			if (osl_keys->pressed.circle)
			{	
				oslDeleteImage(wifibg);	
				oslDeleteImage(offswitch);	
				oslDeleteImage(onswitch);
				settingsMenu();
			}	

			if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(wifibg);	
				oslDeleteImage(offswitch);	
				oslDeleteImage(onswitch);
				settingsMenu();
			}
		
			if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(wifibg);	
				oslDeleteImage(offswitch);	
				oslDeleteImage(onswitch);
				home();
			}

			if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
			{	
				oslPlaySound(KeypressStandard, 1);  
				multitask();
			}
		
			captureScreenshot();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	skip = oslSyncFrame();
	}
}

void developerMenu()
{	
	SceUID module;

	if (DARK == 0)
	{
		developerbg = oslLoadImageFilePNG(developerBgPath, OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
	}
	else
	{
		developerbg = oslLoadImageFilePNG("system/settings/Dark/developerbg.png", OSL_IN_RAM, OSL_PF_8888);
		highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	}
	
	offswitch = oslLoadImageFilePNG(offSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG(onSwitchPath, OSL_IN_RAM, OSL_PF_8888); 
	
	if (!developerbg || !highlight || !offswitch || !onswitch)
		debugDisplay();
		
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	

		oslDrawImageXY(developerbg, 0, 0);
		
		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(10,75, "%s", lang_settingsDeveloperOptions[language][0]);
		oslDrawStringf(10,90, "%s", lang_settingsDeveloperOptions[language][1]);
		oslDrawStringf(10,140, "%s", lang_settingsDeveloperOptions[language][2]);
		oslDrawStringf(10,195, "%s", lang_settingsDeveloperOptions[language][3]);
		
		if (experimentalF == 0)
			oslDrawImageXY(offswitch,392,133);
		else
			oslDrawImageXY(onswitch,392,133);
		
		if (DARK == 0)
			oslDrawImageXY(offswitch,392,186);
		else
			oslDrawImageXY(onswitch,392,186);
			
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(10,75, "%s", lang_settingsDeveloperOptions[language][0]);
			oslDrawStringf(10,90, "%s", lang_settingsDeveloperOptions[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				module = loadStartModule("modules/RemoteJoyLite.prx");
				if (module < 0)
				{
					pspDebugScreenPrintf("Error 0x%08X starting module.\n", module);
					break;
				}
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(10,140, "%s", lang_settingsDeveloperOptions[language][2]);
			
			if (experimentalF == 0)
			{
				oslDrawImageXY(offswitch,392,133);
				
				if (osl_keys->pressed.cross)
				{
					oslPlaySound(KeypressStandard, 1);  
					FILE * experimentalFeatures = fopen("system/settings/experimentalFeatures.bin", "w");
					experimentalF = 1;
					fprintf(experimentalFeatures, "1");
					fclose(experimentalFeatures);
				}
			}
			else if (experimentalF == 1)
			{
				oslDrawImageXY(onswitch,392,133);
			
				if (osl_keys->pressed.cross)
				{
					oslPlaySound(KeypressStandard, 1);  
					FILE * experimentalFeatures = fopen("system/settings/experimentalFeatures.bin", "w");
					experimentalF = 0;
					fprintf(experimentalFeatures, "0");
					fclose(experimentalFeatures);
				}
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(10,195, "%s", lang_settingsDeveloperOptions[language][3]);
			if (DARK == 0)
			{
				oslDrawImageXY(offswitch,392,186);
				
				if (osl_keys->pressed.cross)
				{
					oslPlaySound(KeypressStandard, 1);  
					FILE * darkTheme = fopen("system/settings/darkTheme.bin", "w");
					DARK = 1;
					fprintf(darkTheme, "1");
					fclose(darkTheme);
					oslDeleteImage(developerbg);
					oslDeleteImage(highlight);
					developerbg = oslLoadImageFilePNG("system/settings/Dark/developerbg.png", OSL_IN_RAM, OSL_PF_8888);
					highlight = oslLoadImageFilePNG("system/settings/Dark/highlight.png", OSL_IN_RAM, OSL_PF_8888);	
				}
			}
			else if (DARK == 1)
			{
				oslDrawImageXY(onswitch,392,186);
			
				if (osl_keys->pressed.cross)
				{
					oslPlaySound(KeypressStandard, 1);  
					FILE * darkTheme = fopen("system/settings/darkTheme.bin", "w");
					DARK = 0;
					fprintf(darkTheme, "0");
					fclose(darkTheme);
					oslDeleteImage(developerbg);
					oslDeleteImage(highlight);
					developerbg = oslLoadImageFilePNG(developerBgPath, OSL_IN_RAM, OSL_PF_8888);
					highlight = oslLoadImageFilePNG(highlightPath, OSL_IN_RAM, OSL_PF_8888);
				}
			}
		}

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}	
}

void settingsHighlight()
{		
	if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 98 && cursor->y <= 154)
	{
		oslDrawImageXY(wifi, 1, 100);
		oslDrawStringf(50,120,"%s", lang_settingsMain[language][0]);
	}
		
	if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 98 && cursor->y <= 154)
	{
		oslDrawImageXY(security, 221, 102);
		oslDrawStringf(280,120,"%s", lang_settingsMain[language][1]);
	}
	
	if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 155 && cursor->y <= 210)
	{
		oslDrawImageXY(themes, 0, 156);
		oslDrawStringf(50,177,"%s", lang_settingsMain[language][2]);
	}

	if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 155 && cursor->y <= 210)
	{
		oslDrawImageXY(performance, 225, 155);
		oslDrawStringf(280,177,"%s", lang_settingsMain[language][3]);
	}
		
	if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 211 && cursor->y <= 267)
	{
		oslDrawImageXY(developeroptions, 0, 211);
		oslDrawStringf(50,234,"%s", lang_settingsMain[language][4]);
	}

	if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 211 && cursor->y <= 267)
	{
		oslDrawImageXY(about, 226, 211);
		oslDrawStringf(280,234,"%s", lang_settingsMain[language][5]);
	}
}

void settingsUnloadAssets()
{
	oslDeleteImage(settingsbg);
	oslDeleteImage(about);
	oslDeleteImage(themes);
	oslDeleteImage(developeroptions);
	oslDeleteImage(wifi);
	oslDeleteImage(performance);
	oslDeleteImage(security);
}

void settingsMenu()
{	
	FILE * file = fopen(settingsFontColorPath, "r");
	fscanf(file, "%d %d %d", &fontColor.r, &fontColor.g, &fontColor.b);
	fclose(file);

	themesLoad();
	
	if (DARK == 0)
	{
		settingsbg = oslLoadImageFilePNG(settingsBgPath, OSL_IN_RAM, OSL_PF_8888);
		about = oslLoadImageFilePNG(aboutPath, OSL_IN_RAM, OSL_PF_8888);
		developeroptions = oslLoadImageFilePNG(developerPath, OSL_IN_RAM, OSL_PF_8888);
		performance = oslLoadImageFilePNG(performancePath, OSL_IN_RAM, OSL_PF_8888);
		security = oslLoadImageFilePNG(securityPath, OSL_IN_RAM, OSL_PF_8888);
		themes = oslLoadImageFilePNG(themesPath, OSL_IN_RAM, OSL_PF_8888);
		wifi = oslLoadImageFilePNG(wifiPath, OSL_IN_RAM, OSL_PF_8888);
	}	
	else
	{
		settingsbg = oslLoadImageFilePNG("system/settings/Dark/settingsbg.png", OSL_IN_RAM, OSL_PF_8888);
		about = oslLoadImageFilePNG("system/settings/Dark/about.png", OSL_IN_RAM, OSL_PF_8888);
		developeroptions = oslLoadImageFilePNG("system/settings/Dark/developeroptions.png", OSL_IN_RAM, OSL_PF_8888);
		performance = oslLoadImageFilePNG("system/settings/Dark/performance.png", OSL_IN_RAM, OSL_PF_8888);
		security = oslLoadImageFilePNG("system/settings/Dark/security.png", OSL_IN_RAM, OSL_PF_8888);
		themes = oslLoadImageFilePNG("system/settings/Dark/themes.png", OSL_IN_RAM, OSL_PF_8888);
		wifi = oslLoadImageFilePNG("system/settings/Dark/wifi.png", OSL_IN_RAM, OSL_PF_8888);
	}
	
	if (!settingsbg || !about || !developeroptions || !wifi || !themes || !performance || !security)
		debugDisplay();
	
	oslSetFont(Roboto);

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0, 0, 0));
		
		controls();	

		oslDrawImageXY(settingsbg, 0, 0);

		if (DARK == 0)
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		else
			oslIntraFontSetStyle(Roboto, fontSize, LITEGRAY, 0, INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(50,120,"%s", lang_settingsMain[language][0]);
		oslDrawStringf(280,120,"%s", lang_settingsMain[language][1]);
		oslDrawStringf(50,177,"%s", lang_settingsMain[language][2]);
		oslDrawStringf(280,177,"%s", lang_settingsMain[language][3]);
		oslDrawStringf(50,234,"%s", lang_settingsMain[language][4]);
		oslDrawStringf(280,234,"%s", lang_settingsMain[language][5]);
		
		settingsHighlight();

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{	
			settingsUnloadAssets();
			appdrawer();
		}
		
		if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 98 && cursor->y <= 154 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsUnloadAssets();
			wifiMenu();
		}
		
		if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 98 && cursor->y <= 154 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsUnloadAssets();
			securityMenu();
		}
		
		if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 155 && cursor->y <= 210 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsUnloadAssets();
			displayMenu();
		}

		if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 155 && cursor->y <= 210 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsUnloadAssets();
			performanceMenu();
		}
		
		if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 211 && cursor->y <= 267 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsUnloadAssets();
			developerMenu();
		}

		if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 211 && cursor->y <= 267 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsUnloadAssets();
			aboutMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsUnloadAssets();
			appdrawer();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsUnloadAssets();
			home();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}

		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}