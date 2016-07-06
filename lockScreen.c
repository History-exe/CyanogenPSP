#include "clock.h"
#include "fileManager.h"
#include "homeMenu.h"
#include "lockScreen.h"
#include "screenshot.h"
#include "settingsMenu.h"
#include "include/utils.h"

struct timeAndBatteryStatusFontColor fontColorTime;
struct clockWidgetFontColor lFontColor;

int lockscreen()
{
	FILE *temp;
	 
	if (!(fileExists(clockWidgetFontColorPath)))
	{
		temp = fopen(clockWidgetFontColorPath, "w");
		fprintf(temp, "255\n255\n255");
		fclose(temp);
	}
	
	temp = fopen(clockWidgetFontColorPath, "r");
	fscanf(temp, "%d %d %d", &lFontColor.r, &lFontColor.g, &lFontColor.b);
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
	
	FILE * password;
	FILE * pin;
	char passwordData[20] = "";
	char pinData[5] = "";
	
	lockscreenBg = oslLoadImageFilePNG("system/lockscreen/lockscreenBg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!lockscreenBg)
		debugDisplay();
		
	if (fileExists("system/settings/password.bin"))
		passProtect = 1;	
	else if (fileExists("system/settings/pin.bin"))
		passProtect = 2;
	else if ((!(fileExists("system/settings/password.bin"))) || (!(fileExists("system/settings/pin.bin"))))
		passProtect = 0;
		
	while (!osl_quit)
	{	
		LowMemExit();
	
		oslStartDrawing();
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImage(background);	
		oslDrawImage(lockscreenBg);
		
		centerClock(1);
		
		oslIntraFontSetStyle(Roboto, 0.5f, RGBA(lFontColor.r, lFontColor.g, lFontColor.b, 255), 0, 0);
		getDayOfWeek(178,90,1);
		getMonthOfYear(252,90);
		//oslDrawStringf(20,20,"%d",passProtect); //Used for debugging only

		oslIntraFontSetStyle(Roboto, 0.5f, RGBA(fontColorTime.r, fontColorTime.g, fontColorTime.b, 255), 0, 0);
		
		battery(370,2,1);
		digitaltime(420, 4, 0, hrTime);
		
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
						home();
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
						home();
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
					home();
				}
			}
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
	return 0;
}