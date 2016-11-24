#include "appDrawer.h"
#include "clock.h"
#include "fileManager.h"
#include "homeMenu.h"
#include "include/audio/mp3playerME.h"
#include "include/utils.h"
#include "lockScreen.h"
#include "musicPlayer.h"
#include "powerMenu.h"
#include "screenshot.h"
#include "settingsMenu.h"

struct musicPlayerFontColor fontColor;

void drawGenreColors(int style)
{
	int color = 0;
	
	if ((strcmp(genreStatus ,"Hip-Hop") == 0))
		color = 1;
	else if ((strcmp(genreStatus ,"Pop") == 0) || (strcmp(genreStatus ,"Pop-Folk") == 0) || (strcmp(genreStatus ,"Pop/Funk") == 0) || (strcmp(genreStatus ,"Instrumental Pop") == 0) || (strcmp(genreStatus ,"pop") == 0))
		color = 2;
	else if ((strcmp(genreStatus ,"Rock") == 0) || (strcmp(genreStatus ,"Classic Rock") == 0) || (strcmp(genreStatus ,"Folk-Rock") == 0) || (strcmp(genreStatus ,"Gothic Rock") == 0) || (strcmp(genreStatus ,"Punk Rock") == 0) || (strcmp(genreStatus ,"Christian Rock") == 0) || (strcmp(genreStatus ,"Progressive Rock") == 0) || (strcmp(genreStatus ,"Psychedelic Rock") == 0) || (strcmp(genreStatus ,"Symphonic Rock") == 0) || (strcmp(genreStatus ,"Instrumental Rock") == 0) || (strcmp(genreStatus ,"Southern Rock") == 0) || (strcmp(genreStatus ,"Slow Rock") == 0) || (strcmp(genreStatus ,"Rock & Roll") == 0) || (strcmp(genreStatus ,"Alternative Rock") == 0) || (strcmp(genreStatus ,"Hard Rock") == 0) || (strcmp(genreStatus ,"Metal") == 0) || (strcmp(genreStatus ,"Black Metal") == 0) || (strcmp(genreStatus ,"Heavy Metal") == 0) || (strcmp(genreStatus ,"Thrash Metal") == 0) || (strcmp(genreStatus ,"Death Metal") == 0))
		color = 3;
	else if (strcmp(genreStatus ,"Country") == 0)
		color = 4;
	else if ((strcmp(genreStatus ,"Jazz") == 0) || (strcmp(genreStatus ,"Acid Jazz") == 0) || (strcmp(genreStatus ,"Jazz+Funk") == 0) || (strcmp(genreStatus ,"Classical") == 0) || (strcmp(genreStatus ,"Blues") == 0))
		color = 5;
	else if ((strcmp(genreStatus ,"Disco") == 0) || (strcmp(genreStatus ,"Funk") == 0) || (strcmp(genreStatus ,"Techno") == 0) || (strcmp(genreStatus ,"Euro-Techno") == 0) || (strcmp(genreStatus ,"Techno-Industrial") == 0) || (strcmp(genreStatus ,"Dance") == 0))
		color = 6;
	
	if (style == 0)
	{
		if (color == 0)
			oslDrawFillRect(0, 67, 223, 256, RGB(0, 121, 107)); // Teal
		else if (color == 1)
			oslDrawFillRect(0, 67, 223, 256, RGB(156, 39, 176)); // Purple
		else if (color == 2)
			oslDrawFillRect(0, 67, 223, 256, RGB(244, 67, 54)); // Red
		else if (color == 3)
			oslDrawFillRect(0, 67, 223, 256, RGB(158, 158, 158)); //Gray
		else if (color == 4)
			oslDrawFillRect(0, 67, 223, 256, RGB(63, 81, 181)); // Indigo
		else if (color == 5)
			oslDrawFillRect(0, 67, 223, 256, RGB(121, 85, 72)); // Brown
		else if (color == 6)
			oslDrawFillRect(0, 67, 223, 256, RGB(33, 150, 243)); // Blue
	}
	
	else if (style == 1)
	{
		if (color == 0)
			oslDrawFillRect(0, 0, 480, 272, RGB(0, 121, 107)); // Teal
		else if (color == 1)
			oslDrawFillRect(0, 0, 480, 272, RGB(156, 39, 176)); // Purple
		else if (color == 2)
			oslDrawFillRect(0, 0, 480, 272, RGB(244, 67, 54)); // Red
		else if (color == 3)
			oslDrawFillRect(0, 0, 480, 272, RGB(158, 158, 158)); //Gray
		else if (color == 4)
			oslDrawFillRect(0, 0, 480, 272, RGB(63, 81, 181)); // Indigo
		else if (color == 5)
			oslDrawFillRect(0, 0, 480, 272, RGB(121, 85, 72)); // Brown
		else if (color == 6)
			oslDrawFillRect(0, 0, 480, 272, RGB(33, 150, 243)); // Blue
	}
}

void MP3Play(char * path)
{	
	struct ID3Tag ID3;

	nowplaying = oslLoadImageFilePNG(nowplayingBgPath, OSL_IN_RAM, OSL_PF_8888);
	
	if (!nowplaying)
		debugDisplay();
	
	scePowerSetClockFrequency(333, 333, 166);
	
	pspAudioInit();
	
	MP3ME_Init(1);
	ParseID3(path, &ID3);
	MP3ME_Load(path);
	MP3ME_Play();
	
	isPlaying = 1;
	
	char mp3time[200];
    char mp3timefeed[200];
	
	if (experimentalF == 1)
	{
		if (ID3.ID3EncapsulatedPictureOffset && ID3.ID3EncapsulatedPictureLength <= MAX_IMAGE_DIMENSION)
		{
			SceUID fd = sceIoOpen(path, PSP_O_RDONLY, 0777);
			if (fd >= 0) 
			{
				char *buffer = malloc(ID3.ID3EncapsulatedPictureLength);
				
				if (buffer) 
				{
					sceIoLseek32(fd, ID3.ID3EncapsulatedPictureOffset, PSP_SEEK_SET);
					sceIoRead(fd, buffer, ID3.ID3EncapsulatedPictureLength);
					sceIoClose(fd);

					if (ID3.ID3EncapsulatedPictureType == JPEG_IMAGE)
					{
						oslSetTempFileData(buffer, ID3.ID3EncapsulatedPictureLength, &VF_MEMORY);
						tempCoverArt = oslLoadImageFileJPG(oslGetTempFileName(), OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
					}
					
					else if (ID3.ID3EncapsulatedPictureType == PNG_IMAGE)
					{
						oslSetTempFileData(buffer, ID3.ID3EncapsulatedPictureLength, &VF_MEMORY);
						tempCoverArt = oslLoadImageFilePNG(oslGetTempFileName(), OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
					}
						
					free(buffer);
				}
			}
		}
		
		if (!tempCoverArt)
		{
			coverArtFound = 0;
			tempCoverArt = NULL;
		}
		
		else if (tempCoverArt)
		{
			coverArt = oslScaleImageCreate(tempCoverArt, OSL_IN_RAM | OSL_SWIZZLED, 223, 205, OSL_PF_8888);
			oslDeleteImage(tempCoverArt);
			coverArtFound = 1;
		}
	}
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();		
		
		oslClearScreen(RGB(0,0,0));

		oslReadKeys();
		
		oslIntraFontSetStyle(Roboto, fontSize, RGBA(255, 255, 255, 255), 0, 0);
		
		drawGenreColors(0);
		
		oslDrawImageXY(nowplaying, 0, 0);
		if ((experimentalF == 1) && (coverArtFound == 1))
			oslDrawImageXY(coverArt, 0, 67.5);
		
		//oslDrawStringf(240,76, "Playing: %.19s", folderIcons[current].name);
		strcpy(playingStatus, ID3.ID3Title);
		strcpy(artistStatus, ID3.ID3Artist);
		strcpy(genreStatus, ID3.ID3GenreText);
		if (MP3ME_isPlaying == 1)
		{
			MP3ME_GetTimeString(mp3time);
			sprintf(mp3timefeed, "%s", mp3time);
		}
		oslDrawStringf(15, 28, "%.44s", strupr(ID3.ID3Title));
		oslDrawStringf(15, 48, "%.44s", strupr(ID3.ID3Artist));
		
		//oslDrawStringf(240, 136, "Album: %.21s", ID3.ID3Album);
		//oslDrawStringf(240, 156, "Year: %.22s", ID3.ID3Year);
		//oslDrawStringf(240, 176, "Genre: %.21s", ID3.ID3GenreText);
		//oslDrawStringf(435, 206, "0%d:%.f", mp3Min, MP3ME_playingTime);
		
		if (MP3ME_isPlaying == 1)
			oslDrawImageXY(mp3Play, 328, 147);
		if (MP3ME_isPlaying == 0)
			oslDrawImageXY(mp3Pause, 328, 147);
		
		oslDrawStringf(268, 155, "%s", mp3timefeed);
		
		//oslDrawStringf(397, 155, "%d", MP3ME_GetLength());
		
		battery(370,2,1);
		digitaltime(420,4,0,hrTime);
		volumeController();
		
		if(osl_keys->pressed.select) 
		{
			oslDeleteImage(nowplaying);
			if (experimentalF == 1)
				oslDeleteImage(coverArt);
			return;
		}
		
		if(MP3ME_isPlaying == 1 && osl_keys->pressed.cross) 
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
			
		if (MP3ME_EndOfStream() == 1) 
		{
			isPlaying = 0;
			endAudioLib();
			MP3ME_Stop();
			releaseAudio();
			MP3ME_Play();
		}
		
		if(osl_keys->pressed.circle)
		{
			endAudioLib();
			MP3ME_Stop();
			releaseAudio();
			oslDeleteImage(nowplaying);
			if (experimentalF == 1)
				oslDeleteImage(coverArt);
			isPlaying = 0;
			setCpuBoot(); //Restore previous CPU state
			return;
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
}

int soundPlay(char * path)
{	
	nowplaying = oslLoadImageFilePNG(nowplayingBgPath, OSL_IN_RAM, OSL_PF_8888);
	
	OSL_SOUND * sound = oslLoadSoundFile(path, OSL_FMT_NONE);

	if (!nowplaying)
		debugDisplay();
	
	oslInitAudioME(3);
	
	oslPlaySound(sound,0);
	
	isPlaying = 1;
	
	int Play = 1;
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();		
		
		oslClearScreen(RGB(0,0,0));

		oslReadKeys();
		
		oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, 0);
		
		oslDrawFillRect(0, 0, 480, 272, RGB(156, 39, 176));
		oslDrawImageXY(nowplaying, 0, 0);

		if (Play == 1)
			oslDrawImageXY(mp3Play, 328, 147);
		if (Play == 0)
			oslDrawImageXY(mp3Pause, 328, 147);
		
		strcpy(playingStatus, folderIcons[current].name);
		oslDrawStringf(15, 28, "%.44s", folderIcons[current].name);
		
		battery(370,2,1);
		digitaltime(420,4,0,hrTime);
		volumeController();
		
		if(osl_keys->pressed.select) 
		{
			oslDeleteImage(nowplaying);
			return 1;
		}
		
		if(osl_keys->pressed.cross && Play == 1) 
		{
			Play = 0;
			oslPlaySound(KeypressStandard, 1); 
			oslPauseSound(sound,-1);
		}
		else if(osl_keys->pressed.cross && Play == 0) 
		{
			Play = 1;
			oslPlaySound(KeypressStandard, 1); 
			oslPauseSound(sound,-1);
		}
		
		if(osl_keys->pressed.circle)
		{
			isPlaying = 0;
			oslStopSound(sound);
			oslDeleteSound(sound);
			oslDeleteImage(nowplaying);
			return 1;
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
	oslStopSound(sound);
	oslDeleteSound(sound);
	oslDeleteImage(nowplaying);
	oslDeleteImage(mp3Play);
	oslDeleteImage(mp3Pause);
	return 0;
}

void musicSettings()
{
	mp3bg = oslLoadImageFilePNG(apolloBgPath, OSL_IN_RAM, OSL_PF_8888);
	mp3_select = oslLoadImageFilePNG(apolloSelectorPath, OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG(offSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG(onSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	
	if (!offswitch || !onswitch)
		debugDisplay();

	while (!osl_quit)
	{		
		LowMemExit();
		
		oslStartDrawing();		
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslDrawImageXY(mp3bg, 0, 0);
		
		oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, 0);
		
		oslDrawStringf(20, 80, "Enable music background on lock screen");
		
		if (lsCoverArt == 0)
			oslDrawImageXY(offswitch, 410, 72);
				
		else if (lsCoverArt == 1)
			oslDrawImageXY(onswitch, 410, 72);
		
		if (cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 38 && cursor->y <= 115) 
		{	
			oslDrawImageXY(mp3_select, 0, 56);
			oslDrawStringf(20, 80, "Enable music background on lock screen");
			if (lsCoverArt == 0)
			{
				oslDrawImageXY(offswitch, 410, 72);
				
				if (osl_keys->pressed.cross)
				{
					oslPlaySound(KeypressStandard, 1);
					lsCoverArt = 1;
					FILE * _CoverArt = fopen("system/app/musicplayer/lsCoverArt.bin", "w");
					fprintf(_CoverArt, "%d", lsCoverArt);
					fclose(_CoverArt);
				}
			}
			else if (lsCoverArt == 1)
			{
				oslDrawImageXY(onswitch, 410, 72);
			
				if (osl_keys->pressed.cross)
				{
					oslPlaySound(KeypressStandard, 1);
					lsCoverArt = 0;
					FILE * _CoverArt = fopen("system/app/musicplayer/lsCoverArt.bin", "w");
					fprintf(_CoverArt, "%d", lsCoverArt);
					fclose(_CoverArt);
				}
			}
		}
		
		battery(370, 2, 1);
		digitaltime(420, 4, 0, hrTime);
		volumeController();
		
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			mp3player();
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
}

void mp3FileDisplay()
{
	oslDrawImageXY(mp3bg, 0, 0);
	oslDrawImageXY(mp3_select, 0,(current - curScroll) * 57 + MP3_CURR_DISPLAY_Y);
	
	oslIntraFontSetStyle(Roboto, fontSize, RGBA(255, 255, 255, 255), 0, 0);
	
	if (isPlaying)
	{
		oslDrawStringf(20, 238, "%.50s", playingStatus);
		oslDrawStringf(20, 252, "%.50s", artistStatus);
	}
	
	oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, 0);
	
	// Displays the directories, while also incorporating the scrolling
	for(i=curScroll;i<MAX_MP3_DISPLAY+curScroll;i++) 
	{
		// Handles the cursor and the display to not move past the MAX_MP3_DISPLAY.
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
			oslDrawStringf(MP3_DISPLAY_X, (i - curScroll) * 57 + MP3_DISPLAY_Y, "%.62s", folderIcons[i].name);	// change the X & Y value accordingly if you want to move it (for Y, just change the +10)		
		}
	}

	battery(370,2,1);
	digitaltime(420,4,0,hrTime);
}

void mp3Controls() //Controls
{
	oslReadKeys();	
	
	if (pad.Buttons != oldpad.Buttons) 
	{
		if (osl_keys->pressed.down) 
		{
			selectionDown(MAX_MP3_DISPLAY);
			timer = 0;
		}
		else if (osl_keys->pressed.up) 
		{
			selectionUp();
			timer = 0;
		}
	
		if (osl_keys->pressed.right) 
		{
			selectionDownx5(MAX_MP3_DISPLAY);
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
	
	char * ext = strrchr(folderIcons[current].filePath, '.'); 
	
	if (osl_keys->pressed.circle)
	{			
		if((strcmp("ms0:/MUSIC", curDir)==0) || (strcmp("ms0:/PSP/MUSIC", curDir)==0) || (strcmp("ms0:/PSP/GAME/CyanogenPSP/downloads", curDir)==0) || (strcmp("ms0:/", curDir)==0))
		{
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			mp3player();
		}
		else if((strcmp("ms0:/MUSIC", curDir)!=0)) 
		{
			dirBack(3);
		}
		else if((strcmp("ms0:/PSP/MUSIC", curDir)!=0))
		{
			dirBack(4);
		}	
		else if((strcmp("ms0:/PSP/GAME/CyanogenPSP/downloads", curDir)!=0))
		{
			dirBack(5);
		}	
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".mp3") == 0) || ((strcmp(ext ,".MP3") == 0))) && (osl_keys->pressed.cross))
	{
		if (isPlaying == 1)
		{
			endAudioLib();
			MP3ME_Stop();
			releaseAudio();
			MP3Play(folderIcons[current].filePath);
		}
		else 
		{
			oslPlaySound(KeypressStandard, 1); 
			MP3Play(folderIcons[current].filePath);
		}
	}
	
	if (osl_keys->pressed.start)
		musicSettings();
	
	if (osl_keys->pressed.square)
		powermenu();
		
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
		selectionDown(MAX_MP3_DISPLAY);
		timer = 25;
	}

	if (current < 1) 
		current = 1;
	if (current > MAX_FILES) 
		current = MAX_FILES;

}

char * mp3Browse(const char * path)
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

		mp3FileDisplay();
		mp3Controls();
		
		if (strlen(returnMe) > 4) 
			break;	
			
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();	
	}
	return returnMe;
}

int mp3View(char * browseDirectory)
{	
	mp3bg = oslLoadImageFilePNG(apolloBgPath, OSL_IN_RAM, OSL_PF_8888);
	mp3_select = oslLoadImageFilePNG(apolloSelectorPath, OSL_IN_RAM, OSL_PF_8888);

	char * Directory = mp3Browse(browseDirectory);

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

int mp3player() 
{
	FILE * file = fopen(apolloFontColorPath, "r");
	fscanf(file, "%d %d %d", &fontColor.r, &fontColor.g, &fontColor.b);
	fclose(file);
	
	mp3bg = oslLoadImageFilePNG(apolloBgPath, OSL_IN_RAM, OSL_PF_8888);
	mp3_select = oslLoadImageFilePNG(apolloSelectorPath, OSL_IN_RAM, OSL_PF_8888);
	
	if (!mp3bg || !mp3_select)
		debugDisplay();

	oslSetFont(Roboto);
	
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = -2; //The y position of the first selection
	int selector_image_x; //Determines the starting x position of the selection
	int selector_image_y = 57; //Determines the starting y position of the selection
	int numMenuItems = 3; //Amount of items in the menu
	int selection = 0;

	while (!osl_quit)
	{		
		LowMemExit();
		
		selector_image_x = selector_x+(mp3XSelection*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(mp3YSelection*MenuSelection); //Determines where the selection image is drawn for each selection
	
		oslStartDrawing();
		oslReadKeys();
		oslClearScreen(RGB(0,0,0));	
		
		oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, 0);
		
		oslDrawImageXY(mp3bg, 0, 0);
		oslDrawImageXY(mp3_select, selector_image_x, selector_image_y);
		
		oslDrawStringf(20, 78, "MUSIC");
		oslDrawStringf(20, 135, "PSP/MUSIC");
		oslDrawStringf(20, 192, "PSP/GAME/CyanogenPSP/Downloads");
		
		oslIntraFontSetStyle(Roboto, fontSize, RGBA(255, 255, 255, 255), 0, 0);
		
		if (isPlaying)
		{
			oslDrawStringf(20, 238, "%.50s", playingStatus);
			oslDrawStringf(20, 252, "%.50s", artistStatus);
		}
		
		battery(370, 2, 1);
		digitaltime(420, 4, 0,hrTime);
		volumeController();
		
        if (osl_keys->pressed.down) 
			MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) 
			MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) 
			MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) 
			MenuSelection = numMenuItems; //Sets the selection back to last
		
		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			mp3View("ms0:/MUSIC");
        }
		else if (MenuSelection == 2 && osl_keys->pressed.cross)
        {		
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			mp3View("ms0:/PSP/MUSIC");
        }
		else if (MenuSelection == 3 && osl_keys->pressed.cross)
        {	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			mp3View("ms0:/PSP/GAME/CyanogenPSP/downloads");
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			appdrawer();
		}
		
		if (osl_keys->pressed.start)
			musicSettings();
		
		if (osl_keys->pressed.square)
			powermenu();
		
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
