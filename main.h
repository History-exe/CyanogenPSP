//OSLib
#include <oslib/oslib.h>

#include <pspsdk.h>

//function declarations
int initOSLib(); 
int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);
int main();