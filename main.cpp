#include "Log.h"
#include "Device.h"

int
main(int argc, const char* argv[] )
{
    //open syslog
    Log(LOG_LOCAL0);

 /*   Device* d = new Device();
    d->ok();

    for(;;)
    {
        sleep(1000);
    }*/
}
