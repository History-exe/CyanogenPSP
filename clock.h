#include <oslib/oslib.h>
#include <psprtc.h>

OSL_IMAGE *clockBg, *timeBg, *cursor, *stopWatchBg, *timerBg, *timerPause, *timerPlay, *reset;

void startTimer();
void pauseTimer();
int getTimer();
void resetTimer();
void getDayOfWeek(int x, int y, int n); //Outputs the Day of the Week
void getMonthOfYear(int x, int y); //Outputs the Month of the Year
void digitaltime(int x, int y, int color, int hr);
void centerClock(int n);
int pspTimer();
int cyanogenPSPStopWatch();
void loadClockBg();
int cyanogenPSPClock();