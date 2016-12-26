#include <oslib/oslib.h>

#include <string.h>

OSL_IMAGE *ic_launcher_calculator, *ic_launcher_clock, *ic_launcher_umd, *ic_launcher_gallery, *ic_launcher_filemanager, *ic_launcher_game, *backdrop;

char appDrawerFontColorPath[200];

struct appDrawerFontColor 
{
   int	r;
   int	g;
   int	b;
};

char appDirPath[100];
char allappsPath[250];
char allapps_pressedPath[250];
char apolloPath[250];
char browserPath[250];
char calcPath[250];
char clockPath[250];
char fmPath[250];
char galleryPath[250];
char gamePath[250];
char messagesPath[250];
char settingsPath[250];
char umdPath[250];

void appDrawerLoadRes();
void appDrawerUnloadRes();
void appHighlight(int n);
int appDrawer();
