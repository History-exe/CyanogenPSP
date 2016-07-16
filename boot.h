#include <oslib/oslib.h>

#define MAX_LANG_DISPLAY		3
#define LANG_DISPLAY_X			240
#define LANG_DISPLAY_Y			157 

//declaration
OSL_IMAGE * bootAnim[13], *langSelection;

int bootAnimation();
void langDisplay();
void langControls();
char * langBrowse(const char * path);
void displayLangSelection(char * browseDirectory);
int spawnLangSelection();