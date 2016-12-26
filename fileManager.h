#include <oslib/oslib.h>

#include <pspctrl.h>
#include <pspiofilemgr.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES			256 // max amount of files needed to load.
#define MAX_DISPLAY			5 // max amount of files displayed on-screen.
#define DISPLAY_X			85 // X value of where the filebrowser is displayed.
#define DISPLAY_Y			85 // Y value of the filebrowser is displayed.
#define ICON_DISPLAY_Y      70 
#define CURR_DISPLAY_Y     	68
#define MP3DISPLAY_X        20

// Copy Flags
#define COPY_FOLDER_RECURSIVE 2
#define COPY_DELETE_ON_FINISH 1
#define COPY_KEEP_ON_FINISH 0
#define NOTHING_TO_COPY -1

OSL_IMAGE 	*filemanagerbg, *diricon, *imageicon, *mp3icon, *txticon, *unknownicon, *documenticon, *binaryicon, *videoicon, *archiveicon, *bar, 
			*deletion, *action, *textview,  *gallerybar;
			
char fileManagerFontColorPath[200];

struct fileManagerFontColor 
{
   int	r;
   int	g;
   int	b;
};
			
SceCtrlData pad, oldpad;

// Current Path
char cwd[1024];

/* Globals */

typedef struct fileIcon {

	int		active;

	char	name[255];
	char	filePath[255];
	char	fileType[255];

	int		x;
	int		y;

} fileIcon;

fileIcon folderIcons[MAX_FILES];

typedef struct File {

	int exist;

	char path[255];
	char name[255];

	int size;
	int directory;
	
	// Next Item
	struct File * next;
	// Folder Flag
	int isFolder;

} File;

File dirScan[MAX_FILES];

SceIoDirent g_dir;

int i;
int current;
int curScroll;
char curDir[512];
int timer;
char returnMe[512];

/* Define a write buffer */
char write_buffer[128*1024];
char read_buffer[128*1024];

int copy_bytes(SceUID source, SceUID destination, unsigned bytes);
int copy_folder(char * source, char * destination);
File * findindex(int index);
void copy(int flag);
int paste(void);
int copy_file(char * a, char * b);
int copy_folder_recursive(char * a, char * b);
int fileExists(const char* path);
int dirExists(const char* path);
double getFileSize(const char * path);
int folderScan(const char* path);
int runFile(const char* path, char* type);
int openDir(const char* path, char* type);
void refresh();
void OptionMenu();
void renameFile();
void newFolder();
void deleteFile(char path[]);
int deleteRecursive(char path[]); //Thanks Moonchild!
char * readTextFromFile(char *path);
void displayTextFromFile(char * path);
void centerText(int centerX, int centerY, char * centerText, int centerLength);
void dirVars();
void selectionUp();
void selectionDown(int maxDisplay);
void selectionUpx5();
void selectionDownx5(int maxDisplay);
void dirDisplay();
void dirControls();
void dirBack(int n);
char * dirBrowse(const char * path);
void filemanagerUnloadAssets();
int cyanogenPSPFileManager();