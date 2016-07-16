#include "appDrawer.h"
#include "clock.h"
#include "fileManager.h"
#include "gallery.h"
#include "homeMenu.h"
#include "include/utils.h"
#include "lockScreen.h"
#include "powerMenu.h"
#include "screenshot.h"
#include "settingsMenu.h"

struct galleryFontColor fontColor;

void galleryDisplay()
{	
	oslDrawImageXY(gallerybg, 0, 0);
	oslDrawImageXY(gallerySelection,15,(current - curScroll)*56+GALLERY_CURR_DISPLAY_Y);
	oslDrawStringf(25,25,"Album");
	
	battery(370,2,1);
	digitaltime(420,4,0,hrTime);
	
	// Displays the directories, while also incorporating the scrolling
	for(i=curScroll;i<MAX_GALLERY_DISPLAY+curScroll;i++) 
	{
		// Handles the cursor and the display to not move past the MAX_GALLERY_DISPLAY.
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

		oslDrawImageXY(galleryThumbnail,20,(i - curScroll)*56+THUMBNAIL_DISPLAY_Y);
		
		// If the currently selected item is active, then display the name
		if (folderIcons[i].active == 1) 
		{	
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, 0);
			oslDrawStringf(GALLERY_DISPLAY_X, (i - curScroll)*55+GALLERY_DISPLAY_Y, "%.52s", folderIcons[i].name);	// change the X & Y value accordingly if you want to move it (for Y, just change the +10)		
		}
	}
}

int changeWallpaper()
{
	FILE * backgroundPathTXT;
	
	wallpaper = oslLoadImageFilePNG("system/app/gallery/wallpaper.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!wallpaper)
		debugDisplay();

	while (!osl_quit) {
	
	oslStartDrawing();

	controls();	
	
	oslIntraFontSetStyle(Roboto, fontSize, BLACK, 0, INTRAFONT_ALIGN_LEFT);
	
	oslDrawImageXY(wallpaper, 65, 67);
	oslDrawStringf(110,95,"Set picture as wallpaper?");
	oslDrawStringf(110,115,"Press (X) to accept or (O) to cancel.");
	
	if (osl_keys->pressed.cross) 
	{
		oslDeleteImage(background);
		oslPlaySound(KeypressStandard, 1);  
		backgroundPathTXT = fopen("system/settings/background.bin", "w");
		fprintf(backgroundPathTXT,"%s", folderIcons[current].filePath);
		fclose(backgroundPathTXT);
		strcpy(backgroundPath, setFileDefaultsChar("system/settings/background.bin", "system/framework/framework-res/res/background.png", backgroundPath));
		background = oslLoadImageFile(backgroundPath, OSL_IN_RAM, OSL_PF_8888);
		oslDeleteImage(wallpaper);
		return 1;
	}
		
	if (osl_keys->pressed.circle) 
	{
		oslDeleteImage(wallpaper);
		return 0;
	}
		
	oslEndDrawing(); 
	oslEndFrame(); 
	oslSyncFrame();
	}
	return 0;
}

int showImage(char * path, int n)
{
	int zoomIn = 0, zoomOut = 0;

	OSL_IMAGE * image = oslLoadImageFile(path, OSL_IN_RAM, OSL_PF_8888);
	OSL_IMAGE * galleryBar = oslLoadImageFilePNG("system/app/gallery/galleryBar.png", OSL_IN_RAM, OSL_PF_8888);
	 
	if(!image)
		return 0;
		
	if (!galleryBar)
		debugDisplay();
	
	//Draw image in the centre
	oslSetImageRotCenter(image);
	image->x = OSL_SCREEN_WIDTH / 2;
	image->y = OSL_SCREEN_HEIGHT / 2;
		
	while (!osl_quit) 
	{
		oslReadKeys();
		oslStartDrawing();	
		
		oslClearScreen(RGB(255,255,255));
		oslDrawImage(image);//draw image
		oslDrawImageXY(galleryBar,0,0);
		oslIntraFontSetStyle(Roboto, fontSize, WHITE, 0, INTRAFONT_ALIGN_LEFT);
		oslDrawStringf(40,12, "%.55s", folderIcons[current].name);
		
		if (n == 1)
		{
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				changeWallpaper();
			}
		}
		
		volumeController();
		
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();	
		
		if (experimentalF == 1)
		{
			if(osl_keys->analogY <= -50)
			{
				image->stretchX = image->sizeX * zoomIn++;
				image->stretchY = image->sizeY * zoomOut++;
			}
		
			if(osl_keys->analogY >= 50)
			{
				image->stretchX = image->sizeX * zoomIn--;
				image->stretchY = image->sizeY * zoomOut--;
			}
		}
		
		if (osl_keys->pressed.square) 
		{
			changeWallpaper();
		}
		
		if (osl_keys->pressed.circle) 
		{		
			if (n == 0)
			{
				oslDeleteImage(image);
				oslDeleteImage(galleryBar);
				return 1;
			}
			else if (n == 1)
			{
				oslDeleteImage(image);
				oslDeleteImage(galleryBar);
				displaySubThemes("system/framework/framework-res/res", 1);
			}
			
			else if ((n == 2) && (selection == 1))
			{
				oslDeleteImage(image);
				oslDeleteImage(galleryBar);
				galleryView("ms0:/PICTURE");
			}
			else if ((n == 2) && (selection == 2))
			{
				oslDeleteImage(image);
				oslDeleteImage(galleryBar);
				galleryView("ms0:/PSP/PHOTO");
			}
			else if ((n == 2) && (selection == 3))
			{
				oslDeleteImage(image);
				oslDeleteImage(galleryBar);
				galleryView("ms0:/PSP/GAME/CyanogenPSP/screenshots");
			}
			
			else
			{
				oslDeleteImage(image);
				oslDeleteImage(galleryBar);
				return 1;
			}
		}
		
		captureScreenshot();
		
	}
	//delete image
	oslDeleteImage(image);	
	return 0;
}

void galleryControls() //Controls
{
	oslReadKeys();	

	if (pad.Buttons != oldpad.Buttons) 
	{
		if (osl_keys->pressed.down) 
		{
			selectionDown(MAX_GALLERY_DISPLAY);
			timer = 0;
		}
		if (osl_keys->pressed.up) 
		{
			selectionUp();
			timer = 0;
		}
		
		if (osl_keys->pressed.right) 
		{
			selectionDownx5(MAX_GALLERY_DISPLAY);
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
			openDir(folderIcons[current].filePath, folderIcons[current].fileType);
		}
	}
	
	volumeController();
		
	if (osl_keys->pressed.cross  && (strcmp(folderIcons[current].filePath, "doesn't matter") != 0))
	{
		oslPlaySound(KeypressStandard, 1);  
		galleryUnloadAssets();
		showImage(folderIcons[current].filePath, 2);
	}
	
	if (osl_keys->pressed.circle)
	{			
		if((strcmp("ms0:/PICTURE", curDir)==0) || (strcmp("ms0:/PSP/PHOTO", curDir)==0) || (strcmp("ms0:/PSP/GAME/CyanogenPSP/screenshots", curDir)==0) || (strcmp("ms0:/", curDir)==0))
		{
			galleryUnloadAssets();
			galleryApp();
		}
		else if((strcmp("ms0:/PICTURE", curDir)!=0)) 
		{
			dirBack(6);
		}
		else if((strcmp("ms0:/PSP/PHOTO", curDir)!=0))
		{
			dirBack(7);
		}	
		else if((strcmp("ms0:/PSP/GAME/CyanogenPSP/screenshots", curDir)!=0))
		{
			dirBack(8);
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
	
	captureScreenshot();
	
	timer++;
	if ((timer > 30) && (pad.Buttons & PSP_CTRL_UP))
	{
		selectionUp();
		timer = 25;
	} 
	else if ((timer > 30) && (pad.Buttons & PSP_CTRL_DOWN))
	{
		selectionDown(MAX_GALLERY_DISPLAY);
		timer = 25;
	}
	
	if (current < 1) 
		current = 1;
	if (current > MAX_FILES) 
		current = MAX_FILES;
}

// Just call 'path' with whichever path you want it to start off in
char * galleryBrowse(const char * path)
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
		galleryDisplay();
		galleryControls();

		if (strlen(returnMe) > 4) 
			break;
		
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
	}
	return returnMe;
}

void galleryUnloadAssets()
{
	oslDeleteImage(gallerybg);
	oslDeleteImage(gallerySelection);
	oslDeleteImage(galleryThumbnail);
}

int galleryView(char * browseDirectory)
{	
	gallerybg = oslLoadImageFilePNG(galleryBgPath, OSL_IN_RAM, OSL_PF_8888);
	gallerySelection = oslLoadImageFilePNG(gallerySelectorPath, OSL_IN_RAM, OSL_PF_8888);
	galleryThumbnail = oslLoadImageFilePNG("system/app/gallery/ic_images.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);

	char * Directory = galleryBrowse(browseDirectory);

	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		oslClearScreen(RGB(0,0,0));	
		
		centerText(480/2, 272/2, Directory, 50);	// Shows the path that 'Directory' was supposed to receive from mp3Browse();
	 
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
	}	
	return 0;
}

int galleryApp() 
{
	FILE *temp;
	 
	if (!(fileExists(galleryFontColorPath)))
	{
		temp = fopen(galleryFontColorPath, "w");
		fprintf(temp, "255\n255\n255");
		fclose(temp);
	}
	
	temp = fopen(galleryFontColorPath, "r");
	fscanf(temp, "%d %d %d", &fontColor.r, &fontColor.g, &fontColor.b);
	fclose(temp);
	
	gallerybg = oslLoadImageFilePNG(galleryBgPath, OSL_IN_RAM, OSL_PF_8888);
	gallerySelection = oslLoadImageFilePNG(gallerySelectorPath, OSL_IN_RAM, OSL_PF_8888);
	galleryThumbnail = oslLoadImageFilePNG("system/app/gallery/ic_images.png", OSL_IN_RAM, OSL_PF_8888);
	
	oslSetFont(Roboto);
	
	if (!gallerybg || !gallerySelection || !galleryThumbnail)
		debugDisplay();
	
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 15; //The x position of the first selection
	int selector_y = 10; //The y position of the first selection
	int selector_image_x; //Determines the starting x position of the selection
	int selector_image_y = 26; //Determines the starting y position of the selection
	int numMenuItems = 3; //Amount of items in the menu

	while (!osl_quit)
	{		
		LowMemExit();
		
		selector_image_x = selector_x+(galley_xSelection*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(galley_ySelection*MenuSelection); //Determines where the selection image is drawn for each selection
	
		oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, 0);
	
		oslStartDrawing();
		oslReadKeys();
		oslClearScreen(RGB(0,0,0));	
		oslDrawImageXY(gallerybg, 0, 0);
		oslDrawStringf(25,25,"Album");
		oslDrawImageXY(gallerySelection, selector_image_x, selector_image_y);
		
		oslDrawStringf(25,89,"PICTURE");
		oslDrawStringf(25,145,"PSP/PHOTO");
		oslDrawStringf(25,201,"PSP/GAME/CyanogenPSP/screenshots");
		
		battery(370,2,1);
		digitaltime(420,4,0,hrTime);
		volumeController();
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last
		
		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {		
			selection = 1;
			oslPlaySound(KeypressStandard, 1);  
			galleryUnloadAssets();
			galleryView("ms0:/PICTURE");
        }
		else if (MenuSelection == 2 && osl_keys->pressed.cross)
        {		
			selection = 2;
			oslPlaySound(KeypressStandard, 1);  
			galleryUnloadAssets();
			galleryView("ms0:/PSP/PHOTO");
        
		}
		else if (MenuSelection == 3 && osl_keys->pressed.cross)
        {	
			selection = 3;
			oslPlaySound(KeypressStandard, 1);  
			galleryUnloadAssets();
			galleryView("ms0:/PSP/GAME/CyanogenPSP/screenshots");
        }
		
		if (osl_keys->pressed.circle)
		{
			galleryUnloadAssets();
			appdrawer();
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
		
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
	}	
	return selection;
}
