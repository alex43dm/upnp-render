#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <map>

#include <upnp/upnp.h>
#include <upnp/upnptools.h>

#include "Service.h"
/*
    std::map<ServiceType, std::string> ServiceNames;

    static void initServiceMaps()
    {
    ServiceNames[AVTransport] = "AVTransport";
    ServiceNames[ConnectionManager] = "ConnectionManager";
    ServiceNames[ContentDirectory] = "ContentDirectory";
    ServiceNames[MediaRenderer] =  "MediaRenderer";
    ServiceNames[MediaServer] = "MediaServer";
    ServiceNames[RenderingControl] = "RenderingControl";
    ServiceNames[ScheduledRecording] = "ScheduledRecording";
    ServiceNames[X_MS_MediaReceiverRegistrar] = "X_MS_MediaReceiverRegistrar";
    ServiceNames[ServiceTypeUnknown] = "";
    };*/

Service::Service() {
    //initMaps();
    actionResponse = new std::queue<VarElement *>();
    notifyResponse = new std::queue<VarElement *>();
    StateVariables = new std::map<std::string, std::string>();
}

Service::~Service() {
    u_int i;
    for (i = 0; i < actionResponse->size(); i++) {
        VarElement *el = actionResponse->front();
        delete el;
        //actionResponse->pop();
    }
    delete actionResponse;

    for (i = 0; i < notifyResponse->size(); i++) {
        VarElement *el = notifyResponse->front();
        delete el;
        notifyResponse->pop();
    }
    delete notifyResponse;

    delete StateVariables;
}

//---------------------------------------------------------------------------------------------------------------
std::string Service::upnpGetString(Upnp_Action_Request *event,
                                   const std::string &key) {
    IXML_Node *node;

    node = (IXML_Node *) event->ActionRequest;
    if (node == NULL) {
        event->ActionResult = NULL;
        event->ErrCode = UPNP_SOAP_E_INVALID_ARGS;
        sprintf(event->ErrStr, "Invalid action request document");
        return "";
    }
    node = ixmlNode_getFirstChild(node);
    if (node == NULL) {
        event->ActionResult = NULL;
        event->ErrCode = UPNP_SOAP_E_INVALID_ARGS;
        sprintf(event->ErrStr, "Invalid action request document");
        return "";
    }
    node = ixmlNode_getFirstChild(node);

    for (/**/; node != NULL; node = ixmlNode_getNextSibling(node)) {
        if (strcmp(ixmlNode_getNodeName(node), key.c_str()) == 0) {
            node = ixmlNode_getFirstChild(node);
            const char *node_value = (node != NULL
                                      ? ixmlNode_getNodeValue(node)
                                      : NULL);
            return std::string(node_value != NULL ? node_value : "");
        }
    }

    event->ActionResult = NULL;
    event->ErrCode = UPNP_SOAP_E_INVALID_ARGS;
    sprintf(event->ErrStr, "Missing action request argument (%s)", key.c_str());
    return "";
}

