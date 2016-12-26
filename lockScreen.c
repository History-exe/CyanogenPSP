#include "clock.h"
#include "fileManager.h"
#include "homeMenu.h"
#include "include/audio/mp3playerME.h"
#include "include/utils.h"
#include "lockScreen.h"
#include "musicPlayer.h"
#include "screenshot.h"
#include "settingsMenu.h"

struct timeAndBatteryStatusFontColor fontColorTime;
struct clockWidgetFontColor lFontColor;

int lockscreen()
{
	FILE * file = fopen(clockWidgetFontColorPath, "r");
	fscanf(file, "%d %d %d", &lFontColor.r, &lFontColor.g, &lFontColor.b);
	fclose(file);
	
	file = fopen(timeAndBatteryFontColorPath, "r");
	fscanf(file, "%d %d %d", &fontColorTime.r, &fontColorTime.g, &fontColorTime.b);
	fclose(file);
	
	FILE * password;
	FILE * pin;
	char passwordData[20] = "";
	char pinData[5] = "";
	
	lockscreenBg = oslLoadImageFilePNG("system/lockscreen/lockscreenBg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!lockscreenBg)
		debugDisplay();
	
	oslSetFont(Roboto);
		
	if (fileExists("system/settings/password.bin"))
		passProtect = 1;	
	else if (fileExists("system/settings/pin.bin"))
		passProtect = 2;
	else if ((!(fileExists("system/settings/password.bin"))) || (!(fileExists("system/settings/pin.bin"))))
		passProtect = 0;
		
	while (!osl_quit)
	{	
		LowMemExit();
	
		initDrawing();
		
		controls();	

		oslDrawImage(background);		
		oslDrawImage(lockscreenBg);
		
		oslIntraFontSetStyle(Roboto, 0.5f, RGBA(255, 255, 255, 255), 0, 0);
		
		if (isPlaying)
		{
			if (lsCoverArt)
			{
				drawGenreColors(1);
				oslDrawImageXY(lsMusicBg, 104, 0);
			}
			
			oslDrawImage(lockscreenBg);
			oslDrawImageXY(lsMusic, 40, 120);
			if (MP3ME_isPlaying == 1)
				oslDrawImageXY(mp3Play, 310, 147);
			if (MP3ME_isPlaying == 0)
				oslDrawImageXY(mp3Pause, 310, 147);
			oslDrawStringf(120, 140, "%.14s", playingStatus);
			oslDrawStringf(120, 156, "%.14s", artistStatus);
			
			if((MP3ME_isPlaying == 1) && (!(cursor->x >= 200 && cursor->x <= 280 && cursor->y >= 100 && cursor->y <= 272)) && (osl_keys->pressed.cross))
			{
				oslPlaySound(KeypressStandard, 1); 
				MP3ME_Pause();
				for(i=0; i<10; i++) 
				{
					sceDisplayWaitVblankStart();
				}
			}
		
			else if (MP3ME_isPlaying == 0 && osl_keys->pressed.cross)
			{
				MP3ME_Play();
			}
		}
		
		centerClock(1);
		
		oslIntraFontSetStyle(Roboto, 0.5f, RGBA(lFontColor.r, lFontColor.g, lFontColor.b, 255), 0, 0);
		getDayOfWeek(178,90,1);
		getMonthOfYear(252,90);
		//oslDrawStringf(20,20,"%d",passProtect); //Used for debugging only

		oslIntraFontSetStyle(Roboto, 0.5f, RGBA(fontColorTime.r, fontColorTime.g, fontColorTime.b, 255), 0, 0);
		
		displayMenuBar(3);
		
		if (passProtect != 1)
			androidQuickSettings();
		
		oslDrawImage(cursor);

		if (passProtect == 1)
		{
			if (cursor->x >= 200 && cursor->x <= 280 && cursor->y >= 100 && cursor->y <= 272) 
			{	
				if (osl_pad.held.cross && osl_keys->analogY <= -50)
				{
					oslPlaySound(KeypressStandard, 1);
					openOSK("Enter Password", "", 20, -1);
					password = fopen("system/settings/password.bin", "r");
					fscanf(password,"%s",passwordData);
					fclose(password);

					if (strcmp(tempMessage, passwordData) == 0)
					{
						oslPlaySound(Unlock, 1); 
						oslDeleteImage(lockscreenBg);
						homeMenu();
					}
					else if ((strcmp(tempMessage, passwordData) != 0) || (oslOskGetResult() == OSL_OSK_CANCEL))
					{
						oslDeleteImage(lockscreenBg);
						lockscreen();
					}	
				}
			}
		}
		
		else if (passProtect == 2)
		{
			if (cursor->x >= 200 && cursor->x <= 280 && cursor->y >= 100 && cursor->y <= 272) 
			{	
				if (osl_pad.held.cross && osl_keys->analogY <= -50)
				{
					oslPlaySound(KeypressStandard, 1);
					openOSK("Enter Pin", "", 5, -1);
					pin = fopen("system/settings/pin.bin", "r");
					fscanf(pin,"%s", pinData);
					fclose(pin);

					if (strcmp(tempPin, pinData) == 0)
					{
						oslPlaySound(Unlock, 1); 
						oslDeleteImage(lockscreenBg);
						homeMenu();
					}
					else if ((strcmp(tempPin, pinData) != 0) || (oslOskGetResult() == OSL_OSK_CANCEL))
					{
						oslDeleteImage(lockscreenBg);
						lockscreen();
					}	
				}
			}
		}
		
		else if (passProtect == 0)
		{
			if (cursor->x >= 200 && cursor->x <= 280 && cursor->y >= 100 && cursor->y <= 272) 
			{
				if (osl_pad.held.cross && osl_keys->analogY <= -50)
				{
					oslPlaySound(Unlock, 1); 
					oslDeleteImage(lockscreenBg);
					homeMenu();
				}
			}
		}
		
		captureScreenshot();
		
		termDrawing();
	}
	return 0;
}