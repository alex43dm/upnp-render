#ifndef CONMANAGER_H
#define CONMANAGER_H

#include "Service.h"

using namespace std;

class ConManager : public Service {
    public:
        ConManager(string);
        virtual ~ConManager();
        bool action(struct Upnp_Action_Request *event);
    protected:
    private:
        bool notify(int count, ...);
        bool GetProtocolInfo();
        string SinkInfo;
};

#endif // CONMANAGER_H
