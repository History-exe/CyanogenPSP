#include "appDrawer.h"
#include "clock.h"
#include "fileManager.h"
#include "homeMenu.h"
#include "include/utils.h"
#include "language.h"
#include "lockScreen.h"
#include "messenger.h"
#include "musicPlayer.h"
#include "powerMenu.h"
#include "recentsMenu.h"
#include "screenshot.h"
#include "settingsMenu.h"

//variables
int notif_y = -272;
int yPos1 = -272;
int yPos2 = -272;
int yLine1 = -272;
int yLine2 = -272;
int controlX = 25;

struct timeAndBatteryStatusFontColor fontColorTime;
struct clockWidgetFontColor lFontColor;

void set_volume(int vol) 
{
	if(vol > 30)
		vol = 30;
	if(vol < 0)
		vol = 0;
		
	imposeSetVolume(vol);
}

void increase_volume(int n) 
{
	int v = imposeGetVolume();
	
	set_volume(v+n);
}

void decrease_volume(int n) 
{
	int v = imposeGetVolume();
	
	set_volume(v-n);
}

unsigned int getbuttons(); 

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
            oslStartDrawing();

            if (browser)
			{
                oslDrawBrowser();
                if (oslGetBrowserStatus() == PSP_UTILITY_DIALOG_NONE)
				{
                    oslEndBrowser();
					home();
                }
            }
            oslEndDrawing();
		}
		oslEndFrame();
		skip = oslSyncFrame();

        if (!browser)
		{
            oslReadKeys();
            oslBrowserInit("http://www.google.com", "/PSP/GAME/CyanogenPSP/downloads", 5*1024*1024, //Downloads will be saved into this directory
                                         PSP_UTILITY_HTMLVIEWER_DISPLAYMODE_SMART_FIT,
                                         PSP_UTILITY_HTMLVIEWER_DISABLE_STARTUP_LIMITS,
                                         PSP_UTILITY_HTMLVIEWER_INTERFACEMODE_FULL,
                                         PSP_UTILITY_HTMLVIEWER_CONNECTMODE_MANUAL_ALL);
			memset(message, 0, sizeof(message));

        }
    }
	oslNetTerm();
}

void controls() //Main controller function - allows cursor movement
{
	//Intialize the limits
	int llimit = 0;
	int rlimit = 460;
	int ulimit = 0;
	int dlimit = 252;
		
	//Read keys
	oslReadKeys();
		
	if (!osl_pad.held.cross)	
	{	
		if(osl_keys->analogX >= 50)
			cursor->x+= osl_keys->analogX/30;
		
		if(osl_keys->analogX <= -50)
			cursor->x+=osl_keys->analogX/30;

		if(osl_keys->analogY >= 50)
			cursor->y+= osl_keys->analogY/30;
		
		if(osl_keys->analogY <= -50)
			cursor->y+= osl_keys->analogY/30;
	}
	
	/* Prevents the cursor from going off screen */
	
	if (cursor->x <= llimit)
	{
		cursor->x = llimit;
	}
		
	else if (cursor->x >= rlimit)
	{
		cursor->x = rlimit;
	}
		
	if (cursor->y <= ulimit)
	{	
		cursor->y = ulimit;
	}
		
	else if (cursor->y >= dlimit)
	{
		cursor->y = dlimit;
	}
}

void battery(int batX, int batY, int n) // Draws the battery icon depending on its percentage. 
{
	int batteryLife, y = 0;
	
	batteryLife = scePowerGetBatteryLifePercent(); //Gets battery percentage
	
	oslIntraFontSetStyle(Roboto, 0.5f, RGBA(fontColorTime.r, fontColorTime.g, fontColorTime.b, 255), 0, INTRAFONT_ALIGN_LEFT);
	
	if (n == 0 || n == 1)
	{
		if (batteryM == 0)
		{
			oslDrawFillRect(0, 0, 480, 21, RGB(245,81,30));
		}
		
		if (batteryLife <= 99)
		{
			oslDrawStringf(batX+18, 4, "%d%%", batteryLife);
		}
		else  if (batteryLife > 99)
		{
			oslDrawStringf(batX+10, 4, "%d%%", batteryLife);
		}
		
		if (usbStat == 1 && (isUSBCableConnected() == 1))
		{
			oslDrawImageXY(usbdebug, 5, -1);
		}
			
		else if (isPlaying == 1)
		{
			oslDrawImageXY(music, 5, 2);
		}
		
		if ((usbStat == 1 && (isUSBCableConnected() == 1)) && isPlaying == 1)
		{
			oslDrawImageXY(usbdebug, 5, -1);
			oslDrawImageXY(music, 31, 2);
		}
	}
	
	else if (n == 3)
	{
		if ((cursor->y <= 16) || (cursor->y >= 226))
		{
			if (batteryM == 0)
				oslDrawFillRect(0, 0, 480, 21, RGB(245,81,30));
			else
				oslDrawImageXY(layerA, 0, 0);
			y+=2;
		}
		
		else if (cursor->y >= 16 && cursor->y <= 226)
		{
			y-=17;
		}
		
		if (batteryLife <= 99)
		{
			batY = y;
			oslDrawStringf(batX+18, y+2, "%d%%", batteryLife);
		}
		else  if (batteryLife > 99)
		{
			batY = y;
			oslDrawStringf(batX+10, y+2, "%d%%", batteryLife);
		}
		
		if (usbStat == 1 && (isUSBCableConnected() == 1))
		{
			oslDrawImageXY(usbdebug, 5, y-3);
		}
			
		else if (isPlaying == 1)
		{
			oslDrawImageXY(music, 5, y);
		}
		
		if ((usbStat == 1 && (isUSBCableConnected() == 1)) && isPlaying == 1)
		{
			oslDrawImageXY(usbdebug, 5, y-3);
			oslDrawImageXY(music, 31, y);
		}
	}
	
	if (batteryLife == 100)
	{
		if (n == 2)
			oslDrawImageXY(batt100, batX-2, batY);
		else 
			oslDrawImageXY(batt100, batX-10, batY);
	}
	else if (batteryLife >80 && batteryLife <= 100) 
		oslDrawImageXY(batt80, batX, batY);
	else if (batteryLife >60 && batteryLife <= 80)
		oslDrawImageXY(batt60, batX, batY);
	else if (batteryLife >40 && batteryLife <= 60)
		oslDrawImageXY(batt40, batX, batY);
	else if (batteryLife >20 && batteryLife <= 40) 
		oslDrawImageXY(batt20, batX, batY);
	else if (batteryLife >10 && batteryLife <= 20)
		oslDrawImageXY(batt10, batX, batY);
	else if (batteryLife >0 && batteryLife <= 10)
	{
		//oslPlaySound(LowBattery, 1);  
		oslDrawImageXY(batt0, batX, batY);
	}
	
	if (scePowerIsBatteryCharging() == 1) // If the battery's charging, draw the charging icon on the battery icon.
	{
		if (batteryLife == 100)
		{
			if (n == 2)
				oslDrawImageXY(battcharge, batX-1, batY);
			else 
				oslDrawImageXY(battcharge, batX-9, batY);
		}
		else
		{
			//oslPlaySound(WirelessChargingStarted, 1);  
			oslDrawImageXY(battcharge, batX, batY);
		}
	}
}

void volumeController()
{	
	int vControlX = 0;
	unsigned int kernelButtons = getbuttons(); 
	
	int llimit = 161;
	int rlimit = 341;
	
	int volume = imposeGetVolume();
	
	if (volume == 0)
	{
		vControlX = 161;
	}
	else if (volume == 1)
	{
		vControlX = 167;
	}
	else if (volume == 2)
	{
		vControlX = 173;
	}
	else if (volume == 3)
	{
		vControlX = 179;
	}
	else if (volume == 4)
	{
		vControlX = 185;
	}
	else if (volume == 5)
	{
		vControlX = 191;
	}
	else if (volume == 6)
	{
		vControlX = 197;
	}
	else if (volume == 7)
	{
		vControlX = 203;
	}
	else if (volume == 8)
	{
		vControlX = 209;
	}
	else if (volume == 9)
	{
		vControlX = 215;
	}
	else if (volume == 10)
	{
		vControlX = 221;
	}
	else if (volume == 11)
	{
		vControlX = 227;
	}
	else if (volume == 12)
	{
		vControlX = 233;
	}
	else if (volume == 13)
	{
		vControlX = 239;
	}
	else if (volume == 14)
	{
		vControlX = 245;
	}
	else if (volume == 15)
	{
		vControlX = 251;
	}
	else if (volume == 16)
	{
		vControlX = 257;
	}
	else if (volume == 17)
	{
		vControlX = 263;
	}
	else if (volume == 18)
	{
		vControlX = 269;
	}
	else if (volume == 19)
	{
		vControlX = 275;
	}
	else if (volume == 20)
	{
		vControlX = 281;
	}
	else if (volume == 21)
	{
		vControlX = 287;
	}
	else if (volume == 22)
	{
		vControlX = 293;
	}
	else if (volume == 23)
	{
		vControlX = 299;
	}
	else if (volume == 24)
	{
		vControlX = 305;
	}
	else if (volume == 25)
	{
		vControlX = 311;
	}
	else if (volume == 26)
	{
		vControlX = 317;
	}
	else if (volume == 27)
	{
		vControlX = 323;
	}
	else if (volume == 28)
	{
		vControlX = 329;
	}
	else if (volume == 29)
	{
		vControlX = 335;
	}
	else if (volume == 30)
	{
		vControlX = 341;
	}
	
	if (vControlX <= llimit)
	{
		vControlX = llimit;
	}
	else if (vControlX >= rlimit)
	{
		vControlX = rlimit;
	}
	
	oslIntraFontSetStyle(Roboto, 0.5f, WHITE, 0, 0);
	
	if (kernelButtons & PSP_CTRL_VOLUP)
	{
		increase_volume(1);
		oslDrawImageXY(volumeBar, 117,30);
		oslDrawFillRect(165, 56, vControlX+5, 58, RGB(r, g, b));	
		oslDrawImageXY(volumeControl, vControlX, 52);
		oslDrawStringf(130,70, "Vol: %d", imposeGetVolume());
	}
	
	else if (kernelButtons & PSP_CTRL_VOLDOWN)
	{
		decrease_volume(1);
		oslDrawImageXY(volumeBar, 117,30);
		oslDrawFillRect(165, 56, vControlX+5, 58, RGB(r, g, b));
		oslDrawImageXY(volumeControl, vControlX, 52);	
		oslDrawStringf(130,70, "Vol: %d", imposeGetVolume());
	}
}

void appDrawerIcon() //Draws the app drawer icon. Draws a different icon of the same size once hovered with the cursor.
{
	if (cursor->x  >= 215 && cursor->x  <= 243 && cursor->y >= 195 && cursor->y <= 230)
		oslDrawImageXY(ic_allapps_pressed,218,197);
	
	else
		oslDrawImageXY(ic_allapps,218,197);
}

void navbarButtons(int n) //Draws the navbar buttons in the bottom as seen on androids.
{		
	int y = 272, ulimit = 237, dlimit = 272;
	
	if (batteryM == 0 && n != 2 && n != 1)
		n = 3;

	if (n == 0)
	{		
		oslDrawImageXY(navbar, 109, 237);
		
		if (cursor->x  >= 144 && cursor->x  <= 204 && cursor->y >= 226 && cursor->y <= 271)
			oslDrawImageXY(navbarHighlight, 130, 237); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
		else
			oslDrawImageXY(navbar, 109, 237);
		
		if (cursor->x  >= 205 && cursor->x  <= 271 && cursor->y >= 226 && cursor->y <= 271)
			oslDrawImageXY(navbarHighlight, 195, 237); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
		else
			oslDrawImageXY(navbar, 109, 237);
		
		if (cursor->x  >= 272 && cursor->x  <= 332 && cursor->y >= 226 && cursor->y <= 271)
		{
			oslDrawImageXY(navbarHighlight, 260, 237); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon. else it just draws the navbar.
			oslDrawImageXY(navbar, 109, 237);
		}
		else
			oslDrawImageXY(navbar, 109, 237);
	}
	
	else if (n == 1)
	{
		if ((cursor->y >= 226) || (cursor->y <= 16) )
		{
			if (batteryM == 0)
				oslDrawFillRect(0, 237, 480, 272, RGB(245,81,30));
			else
				oslDrawImageXY(layerB, 0, 237);
				
			y-=35;
			
			if (y <= ulimit)
			{	
				y = ulimit;
			}
	
			oslDrawImageXY(navbar, 109, y);
		
			if (cursor->x  >= 144 && cursor->x  <= 204 && cursor->y >= 226 && cursor->y <= 271)
				oslDrawImageXY(navbarHighlight, 130, y); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
	
			else
				oslDrawImageXY(navbar, 109, y);
		
			if (cursor->x  >= 205 && cursor->x  <= 271 && cursor->y >= 226 && cursor->y <= 271)
				oslDrawImageXY(navbarHighlight, 195, y); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
	
			else
				oslDrawImageXY(navbar,109, y);
		
			if (cursor->x  >= 272 && cursor->x  <= 332 && cursor->y >= 226 && cursor->y <= 271)
			{
				oslDrawImageXY(navbarHighlight, 260, y); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon. else it just draws the navbar.
				oslDrawImageXY(navbar, 109, y);
			}
			else
				oslDrawImageXY(navbar, 109, y);
		}
		else if (cursor->y >= 16 && cursor->y <= 226)
		{
			y+=35;
			
			if (y >= dlimit)
			{	
				y = dlimit;
			}
		}
	}
	
	else if (n == 2)
	{
		oslDrawFillRect(444, 0, 480, 272, RGB(0, 0, 0));
		oslDrawImageXY(navbar2, 444, 19);
		oslDrawImageXY(navbar2, 444, 19);
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75))
		{
			oslDrawImageXY(navbarHighlight2, 444, 26); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
			oslDrawImageXY(navbar2, 444, 19);
		}
		else if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155))
		{
			oslDrawImageXY(navbarHighlight2, 444, 96); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
			oslDrawImageXY(navbar2, 444, 19);
		}
		else if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213))
		{
			oslDrawImageXY(navbarHighlight2, 444, 165); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon. else it just draws the navbar.
			oslDrawImageXY(navbar2, 444, 19);
		}
	}
	
	else if (n == 3) //Powersave
	{
		oslDrawFillRect(0, 237, 480, 272, RGB(245,81,30));
		oslDrawImageXY(navbar, 109, 237);
		
		if (cursor->x  >= 144 && cursor->x  <= 204 && cursor->y >= 226 && cursor->y <= 271)
			oslDrawImageXY(navbarHighlight, 130, 237); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
	
		else
			oslDrawImageXY(navbar, 109, 237);
	
		if (cursor->x  >= 205 && cursor->x  <= 271 && cursor->y >= 226 && cursor->y <= 271)
			oslDrawImageXY(navbarHighlight, 195, 237); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
	
		else
			oslDrawImageXY(navbar, 109, 237);
		
		if (cursor->x  >= 272 && cursor->x  <= 332 && cursor->y >= 226 && cursor->y <= 271)
		{
			oslDrawImageXY(navbarHighlight, 260, 237); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon. else it just draws the navbar.
			oslDrawImageXY(navbar, 109, 237);
		}
		else
			oslDrawImageXY(navbar, 109, 237);
	}
}

void androidQuickSettings()
{
	int notif_up;
	int notif_down;
	
	int llimit = 20;
	int rlimit = 420;
	
	pspTime time;
    sceRtcGetCurrentClockLocalTime(&time);

	int day = sceRtcGetDayOfWeek(time.year, time.month, time.day);
	
	oslDrawImageXY(quickSettings,0,notif_y);
	
	battery(374,yPos2-2,2);
	getDayOfWeek(15,yPos2+5,1);
	if (day == 3)
		getMonthOfYear(90,yPos2+5);
	else if (day == 4 || day == 6)
		getMonthOfYear(80,yPos2+5);
	else 
		getMonthOfYear(72,yPos2+5);
	
	oslIntraFontSetStyle(Roboto, 0.5f, RGBA(fontColorTime.r, fontColorTime.g, fontColorTime.b, 255), 0, INTRAFONT_ALIGN_CENTER);

	oslDrawStringf(150,yLine1, "%s", lang_quickSettings[language][0]);
	oslDrawStringf(350,yPos2, "%d%%",scePowerGetBatteryLifePercent());
	oslDrawStringf(436,yPos2, "%.7s", pspname);
	
	int cpu = getCpuClock();
	
	if (cpu <= 133)
	{
		oslDrawStringf(320,yLine1, "%s", lang_quickSettings[language][1]);
	}
	else if (cpu >= 300)
	{
		oslDrawStringf(320,yLine1, "%s", lang_quickSettings[language][3]);
	}
	else 
	{
		oslDrawStringf(320,yLine1, "%s", lang_quickSettings[language][2]);
	}
		
	oslDrawStringf(235,yLine2, "%s", lang_quickSettings[language][4]);
	
	oslDrawFillRect(40, yPos2+85, controlX+10, yPos2+87, RGB(r, g, b));	
	oslDrawImageXY(control,controlX,yPos2+66);
	
	if (hrTime == 0)
	{
		if((time.hour >= 10 && time.hour < 13) || (time.hour == 0)) 
			digitaltime(32,yPos1,53,hrTime);
		else
			digitaltime(27,yPos1,53,hrTime);
	}
	else if (hrTime == 1)
	{
		if(time.hour >= 10 && time.hour <= 23) 
			digitaltime(19,yPos1,53,hrTime);
		else
			digitaltime(12,yPos1,53,hrTime);
	}
	
	if (getBrightness() == 10)
	{
		controlX = 20;
	}
	else if (getBrightness() == 20)
	{
		controlX = 70;
	}
	else if (getBrightness() == 30)
	{
		controlX = 120;
	}
	else if (getBrightness() == 40)
	{
		controlX = 170;
	}
	else if (getBrightness() == 50)
	{
		controlX = 220;
	}
	else if (getBrightness() == 60)
	{
		controlX = 270;
	}
	else if (getBrightness() == 70)
	{
		controlX = 320;
	}
	else if (getBrightness() == 80)
	{
		controlX = 370;
	}
	else if (getBrightness() == 90)
	{
		controlX = 420;
	}
		
	//This was in while (1) {}
	int amt = getBrightness();
	
	if (amt < 10) 
	{
		setBrightness(10);
	}

	if (amt > 90) 
	{
		setBrightness(90);
	}
	
	if (controlX <= llimit)
	{
		controlX = llimit;
	}
	else if (controlX >= rlimit)
	{
		controlX = rlimit;
	}

	if (osl_pad.held.cross && cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 0 && cursor->y <= 1) 
	{
		notif_down = 1;
	}

	else if (osl_pad.held.cross && cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 250 && notif_y == 0)
	{
		notif_up = 1;
	}
			
	if (notif_down == 1 && cursor->y <= 10)
	{	
		if (osl_pad.held.cross && osl_keys->analogY >= 50)
		{
			notif_y = notif_y+6;
			yPos1 = yPos1+6;
			yPos2 = yPos2+6;
			yLine1 = yLine1+6;
			yLine2 = yLine2+6;
		}
		
		if (notif_y >= 0)
		{
			notif_y = 0;
		}
		
		if (yPos1 >= 10)
		{
			yPos1 = 10;
		}
		if (yPos2 >= 20)
		{
			yPos2 = 20;
		}
		if (yLine1 >= 180)
		{
			yLine1 = 180;
		}
		if (yLine2 >= 245)
		{
			yLine2 = 245;
		}
	}

	if (yLine2 >= 136)
	{
		if (osl_keys->held.right)
		{
			controlX += 4;
			setBrightness(getBrightness() + 1);
		}
		else if (osl_keys->held.left)
		{
			controlX -= 4;
			setBrightness(getBrightness() - 1);
		}	
		
		if ((cursor->x >= 386 && cursor->x <= 414 && cursor->y >= 12 && cursor->y <= 38) && osl_keys->pressed.cross)
		{	
			oslPlaySound(KeypressStandard, 1);  
			notif_y = notif_y-272;
			yPos1 = yPos1-272;
			yPos2 = yPos2-272;
			yLine1 = yLine1-272;
			yLine2 = yLine2-272;
			settingsMenu();
		}
	
		if ((cursor->x >= 96 && cursor->x <= 206 && cursor->y >= 126 && cursor->y <= 192) && osl_keys->pressed.cross)
		{	
			oslPlaySound(KeypressStandard, 1);  
			notif_y = notif_y-272;
			yPos1 = yPos1-272;
			yPos2 = yPos2-272;
			yLine1 = yLine1-272;
			yLine2 = yLine2-272;
			wifiMenu();
		}
		
		if ((cursor->x >= 198 && cursor->x <= 272 && cursor->y >= 204 && cursor->y <= 258) && osl_keys->pressed.cross)
		{
			oslPlaySound(Lock, 1);
			notif_y = notif_y-272;
			yPos1 = yPos1-272;
			yPos2 = yPos2-272;
			yLine1 = yLine1-272;
			yLine2 = yLine2-272;
			lockscreen();
		}
	}
	
	if (notif_down == 1 && osl_keys->pressed.circle)
	{
		notif_y = notif_y-272;
		yPos1 = yPos1-272;
		yPos2 = yPos2-272;
		yLine1 = yLine1-272;
		yLine2 = yLine2-272;
	}
	
	if (notif_up == 1)
	{				
		if (osl_pad.held.cross && osl_keys->analogY <= -50)
		{
			notif_y = notif_y-6;
			yPos1 = yPos1-6;
			yPos2 = yPos2-6;
			yLine1 = yLine1-6;
			yLine2 = yLine2-6;
		}
		
		if (notif_y <= -272)
		{	
			notif_y = -272;
			yPos1 = -272;
			yPos2 = -272;
			yLine1 = -272;
			yLine2 = -272;
		}
	}
}

void dayNightCycleWidget()
{
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);
	
    oslIntraFontSetStyle(Roboto, 1.7f, RGBA(lFontColor.r, lFontColor.g, lFontColor.b, 255), 0, INTRAFONT_ALIGN_CENTER);
	
	centerClock(1);
		
	oslIntraFontSetStyle(Roboto, 0.5f, RGBA(lFontColor.r, lFontColor.g, lFontColor.b, 255), 0, INTRAFONT_ALIGN_CENTER);
	getMonthOfYear(312,102);
	getDayOfWeek(195,102,2);
	
	if (time.hour < 6)
	oslDrawImageXY(wDay, 210, 82);
	
	else
	oslDrawImageXY(wNight, 205, 82);
}

void homeUnloadAssets()
{
	oslDeleteImage(wDay);
	oslDeleteImage(wNight);
}

void home()
{	
	firstBoot = setFileDefaultsInt("system/settings/boot.bin", 1, firstBoot);
	
	FILE * file = fopen(clockWidgetFontColorPath, "r");
	fscanf(file, "%d %d %d", &lFontColor.r, &lFontColor.g, &lFontColor.b);
	fclose(file);

	file = fopen(timeAndBatteryFontColorPath, "r");
	fscanf(file, "%d %d %d", &fontColorTime.r, &fontColorTime.g, &fontColorTime.b);
	fclose(file);

	char message[100] = "";
	char *updateData;
    int dialog = OSL_DIALOG_NONE;
	int read = 0;

	wDay = oslLoadImageFilePNG("system/widget/Day.png", OSL_IN_RAM, OSL_PF_8888);
	wNight = oslLoadImageFile("system/widget/Night.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!wDay || !wNight)
		debugDisplay();

	oslSetFont(Roboto);
	
	widgetActivator = setFileDefaultsInt("system/widget/widgetActivator.bin", 1, widgetActivator);
	eDesktopActivator = setFileDefaultsInt("system/home/eDesktopActivator.bin", 0, eDesktopActivator);

	unsigned int kernelButtons = getbuttons(); 
	
	if (fileExists("ms0:/PSP/GAME/CyanogenPSP/update.txt"))
		read = 1;
	else 
		read = 0;
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();

		controls();	
		
		oslDrawImage(background);		
		oslDrawImageXY(ic_launcher_apollo, 105, 190);
		oslDrawImageXY(ic_launcher_browser, 276, 190);
		oslDrawImageXY(ic_launcher_settings, 331, 190);
		oslDrawImageXY(ic_launcher_messenger, 160, 190);
		
		appDrawerIcon();
		
		if (widgetActivator == 1)
			dayNightCycleWidget();
		
		if (eDesktopActivator == 1)
		{
			navbarButtons(1);
			battery(370,2,3);
			if ((cursor->y <= 16) || (cursor->y >= 226))
			{
				digitaltime(420,4,0,hrTime);
			}
			else if (cursor->y >= 16 && cursor->y <= 226)
			{
				digitaltime(420,-10,0,hrTime);
			}	
		}
		else if (eDesktopActivator == 0)
		{
			navbarButtons(0);
			battery(370,2,1);
			digitaltime(420,4,0,hrTime);
		}
		
		androidQuickSettings();
		volumeController();
		appHighlight(0);
		
		if (firstBoot!= 0)
		{
			oslDrawImageXY(transbackground, 0, 0);
			oslDrawImageXY(welcome, 0, 0);
			
			oslIntraFontSetStyle(Roboto, 0.8f,BLACK,0,INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(20,30, "%s", lang_welcome[language][0]);
			
			oslIntraFontSetStyle(Roboto, 0.6f,DARKGRAY,0,INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(20,60, "%s", lang_welcome[language][1]);
			
			oslDrawStringf(20,80, "%s", lang_welcome[language][2]);
			
			oslIntraFontSetStyle(Roboto, 0.6f,WHITE,0,INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(385,110, "%s", lang_welcome[language][3]);
		}
		
		oslDrawImage(cursor);
		
		if (firstBoot!= 0)
		{
			if (cursor->x >= 388 && cursor->x <= 464 && cursor->y >= 98 && cursor->y <= 132 && osl_keys->pressed.cross)
			{
				FILE * firstBootTxt = fopen("system/settings/boot.bin", "w"); 
				fprintf(firstBootTxt, "0");
				fclose(firstBootTxt);
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(welcome);
				oslDeleteImage(transbackground);
				home();
			}
		}
		
		dialog = oslGetDialogType();
        if (dialog)
		{
			oslDrawDialog();
            if (oslGetDialogStatus() == PSP_UTILITY_DIALOG_NONE)
			{
				oslEndDialog();
            }
		}
		
		if (dialog == OSL_DIALOG_NONE)
		{
			if (read == 1)
			{
				updateData = readTextFromFile("ms0:/PSP/GAME/CyanogenPSP/update.txt");
				oslInitMessageDialog(updateData, 0);
				memset(message, 0, sizeof(message));
				read = 2;
			}		
			else if (read == 2)
			{
				sceIoRemove("ms0:/PSP/GAME/CyanogenPSP/update.txt");
				read = 0;
			}
		}
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}

		if (cursor->x >= 276 && cursor->x <= 321 && cursor->y >= 195 && cursor->y <= 240 && osl_keys->pressed.cross)
		{
			homeUnloadAssets();
			internet();
		}
		
		if (cursor->x >= 330 && cursor->x <= 374 && cursor->y >= 190 && cursor->y <= 240 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			homeUnloadAssets();
			settingsMenu();
		}
		
		if (cursor->x >= 100 && cursor->x <= 154 && cursor->y >= 195 && cursor->y <= 240 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			homeUnloadAssets();
			mp3player();
		}
		
		if (experimentalF == 1)
		{
			if (cursor->x >= 155 && cursor->x <= 210 && cursor->y >= 195 && cursor->y <= 240 && osl_keys->pressed.cross)
			{
				homeUnloadAssets();
				messenger();
			}
		}
			
		if (cursor->x >= 215 && cursor->x <= 243 && cursor->y >= 195 && cursor->y <= 230 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			homeUnloadAssets();
			appdrawer();
		}

		if (osl_keys->pressed.L)
		{	
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();

		if(kernelButtons & PSP_CTRL_HOME) 
		{ 
			powermenu();
		}

	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
}