#include <oslib/oslib.h>

OSL_IMAGE *messengerbg, *new_message, *cursor;

int initiateMessage();
void printInfo();
void clientConnected(struct remotePsp *aPsp);
void doClient();
void serverConnected(struct remotePsp *aPsp);
void doServer();
void newMessage();
int messenger();