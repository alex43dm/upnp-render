#include <map>
#include <iostream>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "ConManager.h"

typedef enum {
    CONNMGR_CMD_GETCURRENTCONNECTIONIDS,
    CONNMGR_CMD_SETCURRENTCONNECTIONINFO,
    CONNMGR_CMD_GETPROTOCOLINFO,
    CONNMGR_CMD_PREPAREFORCONNECTION,
    //CONNMGR_CMD_CONNECTIONCOMPLETE,
    CONNMGR_CMD_UNKNOWN,
    CONNMGR_CMD_COUNT
} conCmd;

typedef enum {
    CONNMGR_VAR_AAT_CONN_MGR,
    CONNMGR_VAR_SINK_PROTO_INFO,
    CONNMGR_VAR_AAT_CONN_STATUS,
    CONNMGR_VAR_AAT_AVT_ID,
    CONNMGR_VAR_AAT_DIR,
    CONNMGR_VAR_AAT_RCS_ID,
    CONNMGR_VAR_AAT_PROTO_INFO,
    CONNMGR_VAR_AAT_CONN_ID,
    CONNMGR_VAR_SRC_PROTO_INFO,
    CONNMGR_VAR_CUR_CONN_IDS,
    CONNMGR_VAR_UNKNOWN,
    CONNMGR_VAR_COUNT
} conVariable;

static std::map<std::string, conCmd> *conmanagerCmdValues = 0;
static std::map<conVariable, std::string> *conVariableNames = 0;
static std::map<conVariable, std::string> *conVariableValues = 0;

static void initMaps() {
    conmanagerCmdValues = new std::map<std::string, conCmd>;
    (*conmanagerCmdValues)["GetCurrentConnectionIDS"] = CONNMGR_CMD_GETCURRENTCONNECTIONIDS;
    (*conmanagerCmdValues)["SetCurrentConnectionIDS"] = CONNMGR_CMD_SETCURRENTCONNECTIONINFO;
    (*conmanagerCmdValues)["GetProtocolInfo"] = CONNMGR_CMD_GETPROTOCOLINFO;
    (*conmanagerCmdValues)["PrepareForConnection"] = CONNMGR_CMD_PREPAREFORCONNECTION;

    conVariableNames = new std::map<conVariable, std::string>;
    (*conVariableNames)[CONNMGR_VAR_SRC_PROTO_INFO] = "SourceProtocolInfo";
    (*conVariableNames)[CONNMGR_VAR_SINK_PROTO_INFO] = "SinkProtocolInfo";
    (*conVariableNames)[CONNMGR_VAR_CUR_CONN_IDS] = "CurrentConnectionIDs";
    (*conVariableNames)[CONNMGR_VAR_AAT_CONN_STATUS] = "A_ARG_TYPE_ConnectionStatus";
    (*conVariableNames)[CONNMGR_VAR_AAT_CONN_MGR] = "A_ARG_TYPE_ConnectionManager";
    (*conVariableNames)[CONNMGR_VAR_AAT_DIR] = "A_ARG_TYPE_Direction";
    (*conVariableNames)[CONNMGR_VAR_AAT_PROTO_INFO] = "A_ARG_TYPE_ProtocolInfo";
    (*conVariableNames)[CONNMGR_VAR_AAT_CONN_ID] = "A_ARG_TYPE_ConnectionID";
    (*conVariableNames)[CONNMGR_VAR_AAT_AVT_ID] = "A_ARG_TYPE_AVTransportID";
    (*conVariableNames)[CONNMGR_VAR_AAT_RCS_ID] = "A_ARG_TYPE_RcsID";
    (*conVariableNames)[CONNMGR_VAR_UNKNOWN] = "";

    conVariableValues = new std::map<conVariable, std::string>;
    (*conVariableValues)[CONNMGR_VAR_SRC_PROTO_INFO] = "";
    (*conVariableValues)[CONNMGR_VAR_SINK_PROTO_INFO] = "http-get:*:audio/mpeg:*";
    (*conVariableValues)[CONNMGR_VAR_CUR_CONN_IDS] = "0";
    (*conVariableValues)[CONNMGR_VAR_AAT_CONN_STATUS] = "Unknown";
    (*conVariableValues)[CONNMGR_VAR_AAT_CONN_MGR] = "/";
    (*conVariableValues)[CONNMGR_VAR_AAT_DIR] = "Input";
    (*conVariableValues)[CONNMGR_VAR_AAT_PROTO_INFO] = ":::";
    (*conVariableValues)[CONNMGR_VAR_AAT_CONN_ID] = "-1";
    (*conVariableValues)[CONNMGR_VAR_AAT_AVT_ID] = "0";
    (*conVariableValues)[CONNMGR_VAR_AAT_RCS_ID] = "0";
    (*conVariableValues)[CONNMGR_VAR_UNKNOWN] = "";
}

//---------------------------------------------------------------------------------------------------------------
ConManager::ConManager(string mimes) {
    initMaps();
    ID = "urn:schemas-upnp-org:service:ConnectionManager";
    SinkInfo = mimes;
    map<string, string>::iterator it = StateVariables->begin();
    it = StateVariables->insert(it, pair<string, string>("SourceProtocolInfo", ""));
    it = StateVariables->insert(it, pair<string, string>("SinkProtocolInfo", SinkInfo));
    it = StateVariables->insert(it, pair<string, string>("CurrentConnectionIDs", ""));
    it = StateVariables->insert(it, pair<string, string>("A_ARG_TYPE_ConnectionStatus", ""));
    it = StateVariables->insert(it, pair<string, string>("A_ARG_TYPE_ConnectionManager", ""));
    it = StateVariables->insert(it, pair<string, string>("A_ARG_TYPE_Direction", ""));
    it = StateVariables->insert(it, pair<string, string>("A_ARG_TYPE_ProtocolInfo", ""));
    it = StateVariables->insert(it, pair<string, string>("A_ARG_TYPE_ConnectionID", "0"));
    it = StateVariables->insert(it, pair<string, string>("A_ARG_TYPE_AVTransportID", "0"));
    StateVariables->insert(it, pair<string, string>("A_ARG_TYPE_RcsID", "0"));
}
//---------------------------------------------------------------------------------------------------------------
ConManager::~ConManager() {
    delete conmanagerCmdValues;
    delete conVariableNames;
    delete conVariableValues;
}
//---------------------------------------------------------------------------------------------------------------
bool ConManager::action(struct Upnp_Action_Request *event) {
    string sw = event->ActionName;
    //clog<<"event->ActionName: "<<event->ActionName<<endl;
    switch ((*conmanagerCmdValues)[sw]) {
        case CONNMGR_CMD_GETPROTOCOLINFO:
            GetProtocolInfo();
            break;
        default:
            return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------
#define BUFLEN 8192

bool ConManager::notify(int count, ...) {

    char *buf;
    int n;
    va_list va;

    buf = (char *)malloc(BUFLEN);
    memset(buf, 0, BUFLEN);

    snprintf(buf, BUFLEN, "<Event xmlns = \"urn:schemas-upnp-org:service:ConnectionManager:1\">");

    va_start(va, count);
    for (n = 0; n < count; n++) {
        char *arg_name, *arg_val;
        arg_name = va_arg(va, char *);
        arg_val  = va_arg(va, char *);
        char buf1[4096];
        snprintf(buf1, sizeof(buf1), "<%s val=\"%s\"/>", arg_name, arg_val);
        buf = strcat(buf, buf1);
    }
    va_end(va);
    sprintf(buf, "%s</Event>", buf);

    //  notifyVarValues[0]  = Xml::escape(buf , 0);
    if (buf)free((void *)buf);

    return true;
}

bool ConManager::GetProtocolInfo() {
    actionResponse->push(new VarElement("Source", ""));
    actionResponse->push(new VarElement("Sink", SinkInfo));
    return true;
}
