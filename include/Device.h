#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <string>
#include <ctime>
#include <list>

#include <upnp/upnp.h>
#include <upnp/ixml.h>
#include <upnp/upnptools.h>

//#include <pthread.h>

#include "ConManager.h"
#include "RControl.h"
#include "Transport.h"

using namespace std;

class WebServer;

class SubscribedClient {
    public:
        string ServiceId;
        string UDN;
        string Sid;

        SubscribedClient(string id, string udn, string sid) {
            ServiceId = id;
            UDN = udn;
            Sid = sid;
        };
};

class Device {
    public:
        unsigned short port;
        int expTimeOut;//seconds

        Device(const string &title, Player *pPlayer,
                const char *psIp = NULL,
                const unsigned char *pMacAddr = NULL);
        virtual ~Device();

        bool handleActionRequest(struct Upnp_Action_Request *event);
        bool handleSubscriptionRequest(struct Upnp_Subscription_Request *event);
        bool handleGetVarRequest(struct Upnp_State_Var_Request *event);

        //fake
        bool loop();

        inline bool isInited() const {
            return tr != 0;
        }
        void onStop();
        void onPlay();
        void onPause();
        void onVolume(int nVolume);

    protected:

    private:
        Config *cfg ;
        ConManager *cm;
        Transport *tr;
        RControl *rc;
        WebServer *srv;
        //    CLock lock;
        std::string UDN;
        list<SubscribedClient *> SubscribedClientList;

        void UPnPError(std::string mes, int res);
        UpnpDevice_Handle deviceHandle;

        static int eventHandler(Upnp_EventType eventType, void *event, void *cookie);
        bool notify(std::queue<VarElement *> *Variables, string id);
        string getTime();
};
#endif // _DEVICE_H_
