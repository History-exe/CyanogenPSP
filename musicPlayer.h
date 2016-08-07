#include <oslib/oslib.h>

#define MAX_MP3_DISPLAY			3 // max amount of files displayed on-screen.
#define MP3_DISPLAY_X			20 // X value of where the filebrowser is displayed.
#define MP3_DISPLAY_Y			78 // Y value of the filebrowser is displayed.
#define MP3_CURR_DISPLAY_Y     	55 
#define MP3DISPLAY_X        20

#define mp3XSelection 0 //The distance between the X axis of the two selections
#define mp3YSelection 57 //The distance between the Y axis of the two selections

OSL_IMAGE *mp3bg, *tempCoverArt, *coverArt, *cursor, *nowplaying, *mp3_select, *mp3Play, *mp3Pause;

char apolloFontColorPath[200];

struct musicPlayerFontColor 
{
   int	r;
   int	g;
   int	b;
};

char artistStatus[100], playingStatus[100];

char *compact_str(char *s, int max_length);
void MP3Play(char * path);
int soundPlay(char * path);
void mp3Controls();
char * mp3Browse(const char * path);
int mp3View(char * browseDirectory);
int mp3player();