#include "boot.h"
#include "clock.h"
#include "fileManager.h"
#include "homeMenu.h"
#include "include/utils.h"
#include "language.h"
#include "lockScreen.h"
#include "screenshot.h"
#include "settingsMenu.h"

/*
	This part of the source includes all the first time boot stuff, such as boot animation and language prompt during startup.
*/

int bootAnimation()
{
	int currentFrame = 0, i = 0;
	
	bootAnimActivator = setFileDefaultsInt("system/boot/bootAnimActivator.bin", 0, bootAnimActivator);
	firstBoot = setFileDefaultsInt("system/settings/boot.bin", 1, firstBoot);
	
	if (bootAnimActivator != 1)
	{
		if (firstBoot == 1)
			displayLangSelection("system/settings/language");
		else 
		{
			oslDeleteImage(welcome);
			oslDeleteImage(transbackground);	
			lockscreen();
			home();
		}
	}
	
	bootAnim[0] = oslLoadImageFilePNG("system/boot/part1/boot0.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[1] = oslLoadImageFilePNG("system/boot/part1/boot1.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[2] = oslLoadImageFilePNG("system/boot/part1/boot2.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[3] = oslLoadImageFilePNG("system/boot/part1/boot3.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[4] = oslLoadImageFilePNG("system/boot/part1/boot4.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[5] = oslLoadImageFilePNG("system/boot/part1/boot5.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[6] = oslLoadImageFilePNG("system/boot/part1/boot6.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[7] = oslLoadImageFilePNG("system/boot/part1/boot7.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[8] = oslLoadImageFilePNG("system/boot/part1/boot8.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[9] = oslLoadImageFilePNG("system/boot/part1/boot9.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[10] = oslLoadImageFilePNG("system/boot/part1/boot10.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[11] = oslLoadImageFilePNG("system/boot/part1/boot11.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[12] = oslLoadImageFilePNG("system/boot/part1/boot12.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	
	while(!osl_quit)
	{
		oslStartDrawing();
		oslDrawImageXY(bootAnim[currentFrame], 80, -25); 
		sceDisplayWaitVblankStart();
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
		sceKernelDelayThread(10000 * 2);
   
		currentFrame++;
		if(currentFrame > 12)
		{
			currentFrame = 0;
			i++;
		}
		
		if (i > 21)
		{
			for (i = 0; i <=12; i++)
			{
				oslDeleteImage(bootAnim[i]);
			}
			if (firstBoot == 1)
				displayLangSelection("system/settings/language");
			else 
			{
				oslDeleteImage(welcome);
				oslDeleteImage(transbackground);	
				lockscreen();
				home();
			}
		}
    }
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	return 0;
}

void langDisplay()
{	
	oslDrawImage(langSelection);
	
	oslIntraFontSetStyle(Roboto, 0.7f, WHITE, 0, INTRAFONT_ALIGN_LEFT);
	oslDrawStringf(20, 85, "CyanogenPSP");
	oslIntraFontSetStyle(Roboto, 0.65f, WHITE, 0, INTRAFONT_ALIGN_LEFT);
	oslDrawStringf(20, 110, "%s", lang_welcome[language][0]);
	
	oslIntraFontSetStyle(Roboto, 0.5f, WHITE, 0, INTRAFONT_ALIGN_LEFT);
	digitaltime(420,4,0,hrTime);
	battery(370,2,1);		
	
	oslDrawFillRect(60, 179, 420, 180, LITEGRAY);
	oslDrawFillRect(60, 217, 420, 218, LITEGRAY);

	for(i = curScroll; i < MAX_LANG_DISPLAY + curScroll; i++) 
	{
		if ((folderIcons[i].active == 0) && (current >= i-1)) 
		{
			current = i-1;
			break;
		}

		if (current <= curScroll-1) 
		{
			current = curScroll-1;
			break;
		}

		if (folderIcons[i].active == 1) 
		{
			oslIntraFontSetStyle(Roboto, 0.55f, BLACK, 0, INTRAFONT_ALIGN_CENTER);
			oslDrawStringf(LANG_DISPLAY_X, (i - curScroll)*36 + LANG_DISPLAY_Y, "%.56s", folderIcons[i].name);	
		}
	}
}

void langControls()
{
	oslReadKeys();

	if (pad.Buttons != oldpad.Buttons) 
	{
		if (osl_keys->pressed.down)
		{
			selectionDown(MAX_LANG_DISPLAY);
			if (strcmp(folderIcons[current].filePath, "doesn't matter") != 0) 
				changeLanguage();
			timer = 0;
		}
		else if (osl_keys->pressed.up)
		{
			selectionUp();
			if (strcmp(folderIcons[current].filePath, "doesn't matter") != 0)
				changeLanguage();
			timer = 0;
		}
	}
	
	if ((osl_keys->pressed.cross) && (strcmp(folderIcons[current].filePath, "doesn't matter") != 0))
	{
		oslPlaySound(KeypressStandard, 1); 
		language = setFileDefaultsInt("system/settings/language.bin", 0, language);
		oslDeleteImage(langSelection);
		home();
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
		selectionDown(MAX_LANG_DISPLAY);
		timer = 25;
	}

	if (current < 1) 
		current = 1;
	if (current > MAX_FILES) 
		current = MAX_FILES;
}

char * langBrowse(const char * path)
{
	folderScan(path);
	dirVars();
	
	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));	
		oldpad = pad;
		sceCtrlReadBufferPositive(&pad, 1);
		langDisplay();

		langControls(); //0 is to used for selecting a font
			
		if (strlen(returnMe) > 4) 
			break;
			
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();
	}
	return returnMe;
}

void displayLangSelection(char * browseDirectory)
{	
	langSelection = oslLoadImageFilePNG("system/settings/langSelection.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	browseDirectory = langBrowse("system/settings/language"); //For language

	if (!langSelection)
		debugDisplay();
	
	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		centerText(480/2, 272/2, browseDirectory, 50);
		
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();	
	}
}