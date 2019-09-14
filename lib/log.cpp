#include "log.h"

void log(const char* sender, const char* msg)
{
    cout << "[" << sender << "] " << msg << endl;
}
