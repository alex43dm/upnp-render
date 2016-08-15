#include <stdio.h>
#include <getopt.h>

#include "Log.h"
#include "Config.h"
#include "Device.h"

#define DEFAULT_CONFIG "confg.xml"

extern char *__progname;
//------------------------------------------------------------------------------
void PrintUsage()
{
    printf("%s -f config file\n",__progname);
}
//------------------------------------------------------------------------------
int
main(int argc, char* argv[] )
{
    int opt;
    const char *fname = DEFAULT_CONFIG;

    //open syslog
    Log(LOG_LOCAL0);

    while ((opt = getopt(argc, argv, "f:")) != -1)
    {
        switch(opt)
        {
        case 'f':
            fname = optarg;
            break;
        default:
            PrintUsage();
            return 0;
        }
    }

    //read config
    Config::Instance()->LoadConfig(fname);

    Device *dev = new Device();
    dev->loop();

    return 0;
}
