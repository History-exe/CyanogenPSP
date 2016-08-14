#include "appDrawer.h"
#include "calculator.h"
#include "clock.h"
#include "homeMenu.h"
#include "include/utils.h"
#include "lockScreen.h"
#include "powerMenu.h"
#include "screenshot.h"
#include "settingsMenu.h"

char x1[4][20] = 
{
	{"sin"},
	{"1/x"},
	{"."},
	{"."},
};

char x2[5][10] = 
{
	{"cos"},
	{"sqr"},
	{"."},
	{"."},
	{"C"},
};

char x3[4][10] = 
{
	{"tan"},
	{"^"},
	{""},
	{""},
};

int variable, selection;
 
int calculator()
{
	//int result = 0;
	int x = 1;
	int y = 1;
	
	//int enteredNum = 0;
	//int numUse = 0;
	//int suppression = 1;
	char * display = "";

	calcbackground = oslLoadImageFile("system/app/calculator/calcbg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!calcbackground)
		debugDisplay();
	
	oslSetFont(Roboto);
		
	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(calcbackground, 0, 0);
		
		battery(370,2,1);
		digitaltime(420,4,0,hrTime);
		volumeController();
		
		oslIntraFontSetStyle(Roboto, 0.8f,WHITE,0,0);
		
		oslDrawStringf(35, 120, "%s", display);

		oslDrawString(35, 120, "7");
		oslDrawString(90, 120, "8");
		oslDrawString(145, 120, "9");
		oslDrawString(35, 160, "4");
		oslDrawString(90, 160, "5");
		oslDrawString(145, 160, "6");
		oslDrawString(35, 200, "1");
		oslDrawString(90, 200, "2");
		oslDrawString(145, 200, "3");
		oslDrawString(90, 240, "0");

		oslDrawString(218, 120, "/");
		oslDrawString(218, 160, "x");
		oslDrawString(218, 200, "-");
		oslDrawString(218, 240, "+");
		oslIntraFontSetStyle(Roboto, 0.6f, WHITE, 0, 0);
		oslDrawString(260, 120, "DEL");

		oslDrawString(326, 120, "sin");
		oslDrawString(366, 120, "cos");
		oslDrawString(406, 120, "tan");
		oslDrawString(446, 120, "ln");
	
		if (osl_keys->pressed.left)
			x -= 1;
		if (x == 0)
			x = 8;

		if (osl_keys->pressed.right)
			x += 1;
		if (x == 10)
			x = 1;

		if (osl_keys->pressed.up)
			y -= 1;
		if (y == 0)
			y = 4;

		if (osl_keys->pressed.down)
			y += 1;
		if (y == 5)
			y = 1;
			
			if (y == 1 && x ==1)
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(35, 120, "7");
				//if (osl_keys->pressed.cross)
					//enteredNum = 7;
			}
			else if (y == 1 && x == 2)
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(90, 120, "8");
			}
			else if (y == 1 && x  == 3) 
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(145, 120, "9");
			}
			else if (y == 1 && x == 4)
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(218, 120, "/");
			}
			else if (y == 1 && x == 5)
			{
				oslIntraFontSetStyle(Roboto, 0.6f, RED, 0, 0);
				oslDrawString(260, 120, "DEL");
			}
			else if (y == 1 && x  == 6) 
			{
				oslIntraFontSetStyle(Roboto, 0.6f, RED, 0, 0);
				oslDrawString(326, 120, "sin");
			}
			else if (y == 1 && x  == 7)
			{
				oslIntraFontSetStyle(Roboto, 0.6f, RED, 0, 0);
				oslDrawString(366, 120, "cos");
			}
			else if (y == 1 && x  == 8)
			{
				oslIntraFontSetStyle(Roboto, 0.6f, RED, 0, 0);
				oslDrawString(406, 120, "tan");
			}
			else if (y == 1 && x  == 9)
			{
				oslIntraFontSetStyle(Roboto, 0.6f, RED, 0, 0);
				oslDrawString(446, 120, "ln");
			}
			
			if (y == 2 && x == 1)
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(35, 160, "4");
			}
			else if (y == 2 && x == 2) 
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(90, 160, "5");
			}
			else if (y == 2 && x == 3)
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(145, 160, "6");
			}
			else if (y == 2 && x == 4) 
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(218, 160, "x");
			}

			if (y == 3 && x == 1)
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(35, 200, "1");
			}
			else if (y == 3 && x == 2)
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(90, 200, "2");
			}
			else if (y == 3 && x == 3)
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(145, 200, "3");
			}
			else if (y == 3 && x == 4)
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(218, 200, "-");
			}	

			if (y == 4 && x == 1)
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(90, 240, "0");
			}
			else if (y == 4 && x == 2)
			{
				oslIntraFontSetStyle(Roboto, 0.8f, RED, 0, 0);
				oslDrawString(218, 240, "+");
			}
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.circle)
		{	
			oslDeleteImage(calcbackground);
			appdrawer();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		captureScreenshot();
		
        oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();
	}
	return 0;
}
