#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <string>
#include <ctime>
#include <list>

#include <upnp/upnp.h>
#include <upnp/ixml.h>
#include <upnp/upnptools.h>

//#include <pthread.h>

#include "Player.h"
#include "ConManager.h"
#include "RControl.h"
#include "Transport.h"

class WebServer;

struct SubscribedClient {
    public:
        std::string ServiceId;
        std::string UDN;
        std::string Sid;

        SubscribedClient(std::string id, std::string udn, std::string sid) {
            ServiceId = id;
            UDN = udn;
            Sid = sid;
        };
};

class Device {
    public:
        unsigned short port;
        int expTimeOut;//seconds

        Device();
        virtual ~Device();

        bool handleActionRequest(struct Upnp_Action_Request *event);
        bool handleSubscriptionRequest(struct Upnp_Subscription_Request *event);
        bool handleGetVarRequest(struct Upnp_State_Var_Request *event);

        //fake
        bool loop();

        inline bool isInited() const {
            return transport != 0;
        }
        void onStop();
        void onPlay();
        void onPause();
        void onVolume(int nVolume);

    protected:

    private:
        Player *player;
        ConManager *conManager;
        Transport *transport;
        RControl *remoteControl;
        WebServer *webSrv;
        //    CLock lock;

        std::string UDN;
        std::list<SubscribedClient *> SubscribedClientList;

        void UPnPError(std::string mes, int res);
        UpnpDevice_Handle deviceHandle;

        static int eventHandler(Upnp_EventType eventType, void *event, void *cookie);
        bool notify(std::queue<VarElement *> *Variables, std::string id);
        std::string getTime();
};
#endif // _DEVICE_H_
