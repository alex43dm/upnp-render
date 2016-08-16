#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include <queue>
#include <map>

#include <string.h>

#include <upnp/ixml.h>
#include <upnp/upnp.h>

#include "Xml.h"

class VarElement {
    public:
        std::string name;
        std::string value;
        VarElement(std::string n, std::string v) {
            name = n;
            value = v;
        }
};

enum ServiceType {
    AVTransport,
    ConnectionManager,
    ContentDirectory,
    MediaRenderer,
    MediaServer,
    RenderingControl,
    ScheduledRecording,
    X_MS_MediaReceiverRegistrar,
    ServiceTypeUnknown
};

class Service {
    public:
        std::string ID;
        std::string serviceName;
        ServiceType type;
        std::string  scpdUrl;
        std::string  controlUrl;
        std::string eventUrl;


        //Upnp_Action_Request* event;

        std::map<std::string, std::string> *StateVariables;
        std::queue<VarElement *> *actionResponse;
        std::queue<VarElement *> *notifyResponse;

        virtual bool action(struct Upnp_Action_Request *event) = 0;
        std::string upnpGetString(Upnp_Action_Request *event,
                                  const std::string &key);

        Service();
        static std::string toString(int enumVal);
        virtual ~Service();

    protected:
    private:
        int commandCount;
        //        CLock serviceMutex;
};

#endif // SERVICE_H
