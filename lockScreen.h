#include <pspkernel.h>
#include <oslib/oslib.h>

OSL_IMAGE *background, *cursor, *lockscreenBg, *lsMusic, *lsMusicBg;

char clockWidgetFontColorPath[200];

struct clockWidgetFontColor 
{
   int	r;
   int	g;
   int	b;
};

int lockScreen();