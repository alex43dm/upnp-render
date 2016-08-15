#include <iostream>

#include <string.h>
#include <stdlib.h>

#include "Device.h"
#include "Config.h"
#include "WebServer.h"
#include "Log.h"

//------------------------------------------------------------------------------
Device::Device() :
    conManager(0),
    transport(0),
    remoteControl(0),
    webSrv(0)
{

    player = new Player();

    webSrv = new WebServer(Config::Instance());
    WebPage *index = webSrv->getPage("description.xml");

    if (index)
    {
        port = Config::Instance()->getPort();
        UDN = Config::Instance()->UDN;
        expTimeOut = atoi(Config::Instance()->AdvertisementExpTimeOut.c_str());
        conManager = new ConManager(Config::Instance()->mime);
        remoteControl = new RControl(player);
        transport = new Transport(Config::Instance(), player);
        UPnPError("UpnpInit", UpnpInit(Config::Instance()->ip.c_str(), port));
        UPnPError("UpnpEnableWebserver", UpnpEnableWebserver(true));
        UPnPError("UpnpSetVirtualDirCallbacks",
                  UpnpSetVirtualDirCallbacks(webSrv->callbacks()));
        UPnPError("UpnpAddVirtualDir", UpnpAddVirtualDir("/upnp"));
        UPnPError("UpnpAddVirtualDir",
            UpnpRegisterRootDevice2(UPNPREG_BUF_DESC, index->content.c_str(),
            index->content.length(), 1,  eventHandler, this, &deviceHandle));
        UPnPError("UpnpSendAdvertisement", UpnpSendAdvertisement(deviceHandle, expTimeOut));
        //UpnpSetMaxContentLength DEFAULT_SOAP_CONTENT_LENGTH = 16K bytes.
    }
    else
    {
        Log::info("%s Cannot get index", __FUNCTION__);
    }
}
//------------------------------------------------------------------------------
Device::~Device()
{
    //  lock.lock();
    list<SubscribedClient *>::iterator it;
    for (it = SubscribedClientList.begin();
                it != SubscribedClientList.end(); ++it)
        delete(*it);
    //  lock.unlock();

    UpnpFinish();

    delete transport;
    delete conManager;
    delete remoteControl;
    delete webSrv;
}
//------------------------------------------------------------------------------
int Device::eventHandler(Upnp_EventType eventType, void *event, void *cookie)
{
    Device *d = (Device *)cookie;

    switch (eventType)
    {
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

//------------------------------------------------------------------------------
void Device::UPnPError(std::string mes, int res)
{
    Log::info("%s UPnP: %s %s",
              __FUNCTION__, mes.c_str(), UpnpGetErrorMessage(res));
//    if (res != UPNP_E_SUCCESS) {
//    }
    //pthread_exit(NULL);
}

string Device::getTime()
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return string(asctime(timeinfo));
}
//------------------------------------------------------------------------------
bool Device::handleActionRequest(struct Upnp_Action_Request *event)
{
    queue<VarElement *> *actionResponseQueue = 0;
    //  lock.lock();

    if (strcmp(event->ServiceID, conManager->ID.c_str()) == 0)
    {
        if (conManager->action(event))
        {
            event->ErrCode = UPNP_E_SUCCESS;
        }

        actionResponseQueue = conManager->actionResponse;
    }
    else if (strcmp(event->ServiceID, remoteControl->ID.c_str()) == 0)
    {
        if (remoteControl->action(event))
        {
            event->ErrCode = UPNP_E_SUCCESS;
        }

        actionResponseQueue = remoteControl->actionResponse;
    }
    else if (strcmp(event->ServiceID, transport->ID.c_str()) == 0)
    {
        if (transport->action(event))
        {
            event->ErrCode = UPNP_E_SUCCESS;
        }

        actionResponseQueue = transport->actionResponse;
    }

    if (event->ActionResult == NULL)
    {
        event->ActionResult =
        UpnpMakeActionResponse(event->ActionName, event->ServiceID, 0, NULL);
    }

    while (!actionResponseQueue->empty())
    {
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
//------------------------------------------------------------------------------
bool Device::handleSubscriptionRequest(struct Upnp_Subscription_Request *event)
{
    //if(event->UDN == cfg->UDN)
    //return false;
    std::map<std::string, std::string> *Variables = 0;
    Log::debug("%s: event subscription request: ServiceId: %s UDN: %s Sid: %s",
               __func__, event->ServiceId, event->UDN, event->Sid);
    //  lock.lock();

    SubscribedClient *c = new SubscribedClient(event->ServiceId, event->UDN, event->Sid);

    if (strcmp(event->ServiceId, conManager->ID.c_str()) == 0)
    {
        Variables = conManager->StateVariables;
    }
    else if (strcmp(event->ServiceId, remoteControl->ID.c_str()) == 0)
    {
        Variables = remoteControl->StateVariables;
    }
    else if (strcmp(event->ServiceId, transport->ID.c_str()) == 0)
    {
        Variables = transport->StateVariables;
    }

    int Count = Variables->size();

    if (Count > 0)
    {
        const char **VarNames =
            (const char **)malloc((Count + 1) * sizeof(const char *));
        const char **VarValues =
            (const char **)malloc((Count + 1) * sizeof(const char *));

        map<string, string>::iterator it;
        int i;

        for (it = Variables->begin(), i = 0; it != Variables->end(); ++it, i++)
        {
            VarNames[i] = (char *)malloc(it->first.size() + 1);
            memset((void *)VarNames[i], 0, it->first.size() + 1);
            memcpy((char *)VarNames[i], it->first.c_str(), it->first.size());

            VarValues[i] = (char *)malloc(it->second.size() + 1);
            memset((void *)VarValues[i], 0, it->second.size() + 1);
            memcpy((char *)VarValues[i], it->second.c_str(), it->second.size());
        }

        VarNames[Count] = NULL;
        VarValues[Count] = NULL;
        UPnPError("UpnpAcceptSubscription",
                  UpnpAcceptSubscription(deviceHandle,
                    event->UDN, event->ServiceId, VarNames,
                    VarValues, Count, c->Sid.c_str()));

        SubscribedClientList.push_back(c);
    }

    //  lock.unlock();
    return true;
}

//------------------------------------------------------------------------------
bool Device::loop()
{
    if (!transport)
    {
        Log::err("%s: transport is null", __func__);
        return false;
    }

    transport->loop();

    if (!transport->notifyResponse->empty())
    {
        notify(transport->notifyResponse, transport->ID);
    }
    else if (!conManager->notifyResponse->empty())
    {
        notify(conManager->notifyResponse, conManager->ID);
    }
    else if (!remoteControl->notifyResponse->empty())
    {
        notify(remoteControl->notifyResponse, remoteControl->ID);
    }

    return true;
}

bool Device::notify(std::queue<VarElement *> *Variables, string id)
{
    int i = 0;
    int Count = Variables->size();
    const char **VarNames =
        (const char **)malloc((Count + 1) * sizeof(const char *));
    const char **VarValues =
        (const char **)malloc((Count + 1) * sizeof(const char *));

    while (!Variables->empty())
    {
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
    UPnPError("UpnpNotify", UpnpNotify(deviceHandle,
                                       UDN.c_str(), id.c_str(),
                                       VarNames, VarValues, Count));
    return true;
}


bool Device::handleGetVarRequest(struct Upnp_State_Var_Request *event)
{
    Log::debug("%s: id: %s event->StateVarName: %s",
               __func__,event->ServiceID, event->StateVarName);
    event->CurrentVal = NULL;
    if (strcmp(event->ServiceID, conManager->ID.c_str()) == 0)
    {
    }
    else if (strcmp(event->ServiceID, remoteControl->ID.c_str()) == 0)
    {
    }
    else if (strcmp(event->ServiceID, transport->ID.c_str()) == 0)
    {
    }
    return true;
}

void Device::onStop()
{
    if (transport)
        transport->Stop();
}

void Device::onPlay()
{
    if (transport)
        transport->Play();
}

void Device::onPause()
{
    if (transport)
        transport->Pause();
}

void Device::onVolume(int nVolume)
{
    if (remoteControl)
        remoteControl->SetVol(std::to_string(nVolume));
}
