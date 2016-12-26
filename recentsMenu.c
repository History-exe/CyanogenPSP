#include "appDrawer.h"
#include "clock.h"
#include "homeMenu.h"
#include "include/utils.h"
#include "lockScreen.h"
#include "powerMenu.h"
#include "recentsMenu.h"
#include "screenshot.h"
#include "settingsMenu.h"

int multitask()
{	
	multi_task = oslLoadImageFilePNG("system/home/menu/multi_task.png", OSL_IN_RAM, OSL_PF_8888);

	if (!multi_task)
		debugDisplay();
	
	oslSetFont(Roboto);

	while (!osl_quit)
	{
		LowMemExit();
	
		initDrawing();
		
		controls();
		
		oslDrawImageXY(background, 0, 0);
		oslDrawImageXY(multi_task, 0, 10);

		oslIntraFontSetStyle(Roboto, fontSize, WHITE, 0, 0);
		oslDrawString(130, 136, "Your recent screens appear here");
		
		displayMenuBar(2);
		
		oslDrawImage(cursor);
		
		if (osl_pad.held.circle)
		{	
			oslDeleteImage(multi_task);
			return 1;
		}
		
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(multi_task);
			return 1;
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(multi_task);
			homeMenu();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(multi_task);
			multitask();
		}
		
		coreNavigation(0);
	
		termDrawing();
	}
	return 0;
}