#include "log.h"
#include <mutex>

mutex logmtx;
void log(const char* sender, const char* msg)
{
    logmtx.lock();
    cout << "[" << sender << "] " << msg << endl;
    logmtx.unlock();
}
