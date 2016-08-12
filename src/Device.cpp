#include <iostream>

#include <string.h>
#include <stdlib.h>

#include "Device.h"
#include "Config.h"
#include "WebServer.h"
#include "Log.h"

//---------------------------------------------------------------------------------------------------------------
Device::Device(const string &title,
               Player *pPlayer, const char *psIp,
               const unsigned char *pMacAddr) :
    cfg(0),
    cm(0),
    tr(0),
    rc(0),
    srv(0) {
    //clog<<"thread: "<<id<<" start...\n";
    cfg = new Config(title, pMacAddr);
    if (!cfg->m_bError) {
        srv = new WebServer(cfg);
        WebPage *index = srv->getPage("description.xml");

        if (index) {
            port = cfg->getPort();
            UDN = cfg->UDN;
            //clog<<"port: "<<port<<" udn "<<UDN<<endl;
            expTimeOut = atoi(cfg->AdvertisementExpTimeOut.c_str());
            cm = new ConManager(cfg->mime);
            rc = new RControl(pPlayer);
            tr = new Transport(cfg, pPlayer);
            UPnPError("UpnpInit", UpnpInit(psIp, port));
            UPnPError("UpnpEnableWebserver", UpnpEnableWebserver(true));
            UPnPError("UpnpSetVirtualDirCallbacks", UpnpSetVirtualDirCallbacks(srv->callbacks()));
            UPnPError("UpnpAddVirtualDir", UpnpAddVirtualDir("/upnp"));
            UPnPError("UpnpAddVirtualDir", UpnpRegisterRootDevice2(UPNPREG_BUF_DESC, index->content.c_str(), index->content.length(), 1,  eventHandler, this, &deviceHandle));
            UPnPError("UpnpSendAdvertisement", UpnpSendAdvertisement(deviceHandle, expTimeOut));
            //UpnpSetMaxContentLength DEFAULT_SOAP_CONTENT_LENGTH = 16K bytes.
        }
        else {
                Log::info("%s Cannot get index", __FUNCTION__);
        }


    }
}
//---------------------------------------------------------------------------------------------------------------
Device::~Device() {
    //  lock.lock();
    list<SubscribedClient *>::iterator it;
    for (it = SubscribedClientList.begin(); it != SubscribedClientList.end(); ++it)
        delete(*it);
    //  lock.unlock();

    UpnpFinish();
    delete cfg;
    delete tr;
    delete cm;
    delete rc;
    delete srv;
}
//---------------------------------------------------------------------------------------------------------------
int Device::eventHandler(Upnp_EventType eventType, void *event, void *cookie) {
    Device *d = (Device *)cookie;

    switch (eventType) {
        case UPNP_CONTROL_ACTION_REQUEST:
            d->handleActionRequest((Upnp_Action_Request *)event);
            break;

        case UPNP_CONTROL_GET_VAR_REQUEST:
            d->handleGetVarRequest((Upnp_State_Var_Request *)event);
            break;

        case UPNP_EVENT_SUBSCRIPTION_REQUEST:
            d->handleSubscriptionRequest((Upnp_Subscription_Request *) event);
            break;

        default:
            Log::info("%s Unknown event type: %d", __FUNCTION__, eventType);
            break;
    }

    return 0;
}

//---------------------------------------------------------------------------------------------------------------
void Device::UPnPError(std::string mes, int res) {
    Log::info("%s UPnP: %s %s", __FUNCTION__, mes.c_str(), UpnpGetErrorMessage(res));
//    if (res != UPNP_E_SUCCESS) {
//    }
    //pthread_exit(NULL);
}

string Device::getTime() {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return string(asctime(timeinfo));
}
//---------------------------------------------------------------------------------------------------------------
bool Device::handleActionRequest(struct Upnp_Action_Request *event) {
    queue<VarElement *> *actionResponseQueue = 0;
    //std::clog<< getTime()<<"-Device::handleActionRequest: "<<" ActionName: "<< event->ActionName<<" ServiceID:"<<event->ServiceID<<std::endl;
    //  lock.lock();

    if (strcmp(event->ServiceID, cm->ID.c_str()) == 0) {
        if (cm->action(event)) {
            event->ErrCode = UPNP_E_SUCCESS;
        }

        actionResponseQueue = cm->actionResponse;
    }
    else if (strcmp(event->ServiceID, rc->ID.c_str()) == 0) {
        if (rc->action(event)) {
            event->ErrCode = UPNP_E_SUCCESS;
        }

        actionResponseQueue = rc->actionResponse;
    }
    else if (strcmp(event->ServiceID, tr->ID.c_str()) == 0) {
        if (tr->action(event)) {
            event->ErrCode = UPNP_E_SUCCESS;
        }

        actionResponseQueue = tr->actionResponse;
    }

    if (event->ActionResult == NULL) {
        event->ActionResult = UpnpMakeActionResponse(event->ActionName, event->ServiceID, 0, NULL);
    }

    while (!actionResponseQueue->empty()) {
        VarElement *el = actionResponseQueue->front();
        UPnPError("UpnpAddToActionResponse", UpnpAddToActionResponse(
                      &event->ActionResult, event->ActionName, event->ServiceID,
                      el->name.c_str(), el->value.c_str()));
        //clog<<"Response: name:"<<el->name<<" val: "<<el->value<<endl;
        delete el;
        actionResponseQueue->pop();
    }
    //  clog<<"xml: "<<ixmlDocumenttoString(event->ActionResult)<<endl;
    //  lock.unlock();
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Device::handleSubscriptionRequest(struct Upnp_Subscription_Request *event) {
    //if(event->UDN == cfg->UDN)
    //return false;
    std::map<std::string, std::string> *Variables = 0;
    Log::debug("%s: event subscription request: ServiceId: %s UDN: %s Sid: %s",
               __func__, event->ServiceId, event->UDN, event->Sid);
    //  lock.lock();

    SubscribedClient *c = new SubscribedClient(event->ServiceId, event->UDN, event->Sid);

    if (strcmp(event->ServiceId, cm->ID.c_str()) == 0) {
        Variables = cm->StateVariables;
    }
    else if (strcmp(event->ServiceId, rc->ID.c_str()) == 0) {
        Variables = rc->StateVariables;
    }
    else if (strcmp(event->ServiceId, tr->ID.c_str()) == 0) {
        Variables = tr->StateVariables;
    }

    int Count = Variables->size();

    if (Count > 0) {
        const char **VarNames = (const char **)malloc((Count + 1) * sizeof(const char *));
        const char **VarValues = (const char **)malloc((Count + 1) * sizeof(const char *));
        map<string, string>::iterator it;
        int i;

        for (it = Variables->begin(), i = 0; it != Variables->end(); ++it, i++) {
            //          clog<<"count: "<<i<<" first: "<<it->first<<" size: "<<it->first.size()<<" second: "<<it->second<<" size: "<<it->second.size()<<endl;

            VarNames[i] = (char *)malloc(it->first.size() + 1);
            memset((void *)VarNames[i], 0, it->first.size() + 1);
            memcpy((char *)VarNames[i], it->first.c_str(), it->first.size());

            VarValues[i] = (char *)malloc(it->second.size() + 1);
            memset((void *)VarValues[i], 0, it->second.size() + 1);
            memcpy((char *)VarValues[i], it->second.c_str(), it->second.size());
            //          VarNames[i] = strdup(it->first.c_str());
            //          VarValues[i] = strdup(it->second.c_str());
            //          clog<<"count: "<<i;
            //          clog<<" name: "<<VarNames[i];
            //          clog<<" len: "<<strlen(VarNames[i])<<endl;
            //          clog<<"count: "<<i<<" name: "<<VarNames[i]<<" len: "<<strlen(VarNames[i])<<" value: "<<VarValues[i]<<" len: "<<strlen(VarValues[i])<<endl;
        }

        VarNames[Count] = NULL;
        VarValues[Count] = NULL;
        UPnPError("UpnpAcceptSubscription", UpnpAcceptSubscription(deviceHandle, event->UDN, event->ServiceId, VarNames, VarValues, Count, c->Sid.c_str()));

        SubscribedClientList.push_back(c);
        //clog<<"ok\n";
        /*
                for (i=0; i < Count; i++)
                {
                    if( VarNames[i] )free( (void*)VarNames[i] );
                    if( VarValues[i] )free( (void*)VarValues[i] );
                }
                if( VarNames )free((void*)VarNames);
                if( VarValues )free((void*)VarValues);

        */
    }

    //  lock.unlock();
    return true;
}

//---------------------------------------------------------------------------------------------------------------
bool Device::loop() {
    if (!tr)
    {
        Log::err("%s: transport is null", __func__);
        return false;
    }

    tr->loop();

    if (!tr->notifyResponse->empty()) {
        notify(tr->notifyResponse, tr->ID);
    }
    else if (!cm->notifyResponse->empty()) {
        notify(cm->notifyResponse, cm->ID);
    }
    else if (!rc->notifyResponse->empty()) {
        notify(rc->notifyResponse, rc->ID);
    }

    return true;
}

bool Device::notify(std::queue<VarElement *> *Variables, string id) {
    int i = 0;
    int Count = Variables->size();
    const char **VarNames = (const char **)malloc((Count + 1) * sizeof(const char *));
    const char **VarValues = (const char **)malloc((Count + 1) * sizeof(const char *));

    while (!Variables->empty()) {
        VarElement *el = Variables->front();
        //clog<<"Notify: name:"<<el->name<<" val: "<<el->value<<endl;
        VarNames[i] = strdup(el->name.c_str());
        VarValues[i] = strdup(el->value.c_str());
        i++;
        delete el;
        Variables->pop();
    }

    VarNames[Count] = NULL;
    VarValues[Count] = NULL;
    UPnPError("UpnpNotify", UpnpNotify(deviceHandle, UDN.c_str(), id.c_str(), VarNames, VarValues, Count));
    return true;
}


bool Device::handleGetVarRequest(struct Upnp_State_Var_Request *event) {
    Log::debug("Device::handleGetVarRequest: id: %s event->StateVarName: %s",
                event->ServiceID, event->StateVarName);
    event->CurrentVal = NULL;
    if (strcmp(event->ServiceID, cm->ID.c_str()) == 0) {
    }
    else if (strcmp(event->ServiceID, rc->ID.c_str()) == 0) {
    }
    else if (strcmp(event->ServiceID, tr->ID.c_str()) == 0) {
    }
    return true;
}

void Device::onStop() {
    if (tr)
        tr->Stop();
}

void Device::onPlay() {
    if (tr)
        tr->Play();
}

void Device::onPause() {
    if (tr)
        tr->Pause();
}

void Device::onVolume(int nVolume) {
    if (rc)
        rc->SetVol(std::to_string(nVolume));
}