#include <string>
#include <map>

#include <stdio.h>
#include <stdlib.h>

#include "RControl.h"

enum controlVariable {
    CV_G_GAIN,
    CV_B_BLACK,
    CV_VER_KEYSTONE,
    CV_G_BLACK,
    CV_VOLUME,
    CV_LOUDNESS,
    CV_AAT_INSTANCE_ID,
    CV_R_GAIN,
    CV_COLOR_TEMP,
    CV_SHARPNESS,
    CV_AAT_PRESET_NAME,
    CV_R_BLACK,
    CV_B_GAIN,
    CV_MUTE,
    CV_LAST_CHANGE,
    CV_AAT_CHANNEL,
    CV_HOR_KEYSTONE,
    CV_VOLUME_DB,
    CV_PRESET_NAME_LIST,
    CV_CONTRAST,
    CV_BRIGHTNESS,
    CV_UNKNOWN,
    CV_COUNT
};

enum controlCmd {
    CC_GET_BLUE_BLACK,
    CC_GET_BLUE_GAIN,
    CC_GET_BRIGHTNESS,
    CC_GET_COLOR_TEMP,
    CC_GET_CONTRAST,
    CC_GET_GREEN_BLACK,
    CC_GET_GREEN_GAIN,
    CC_GET_HOR_KEYSTONE,
    CC_GET_LOUDNESS,
    CC_GET_MUTE,
    CC_GET_RED_BLACK,
    CC_GET_RED_GAIN,
    CC_GET_SHARPNESS,
    CC_GET_VERT_KEYSTONE,
    CC_GET_VOL,
    CC_GET_VOL_DB,
    CC_GET_VOL_DBRANGE,
    CC_LIST_PRESETS,
    //CC_SELECT_PRESET,
    //CC_SET_BLUE_BLACK,
    //CC_SET_BLUE_GAIN,
    //CC_SET_BRIGHTNESS,
    //CC_SET_COLOR_TEMP,
    //CC_SET_CONTRAST,
    //CC_SET_GREEN_BLACK,
    //CC_SET_GREEN_GAIN,
    //CC_SET_HOR_KEYSTONE,
    //CC_SET_LOUDNESS,
    CC_SET_MUTE,
    //CC_SET_RED_BLACK,
    //CC_SET_RED_GAIN,
    //CC_SET_SHARPNESS,
    //CC_SET_VERT_KEYSTONE,
    CC_SET_VOL,
    CC_SET_VOL_DB,
    CC_UNKNOWN,
    CC_COUNT
};

static std::map<controlVariable, std::string> *controlVariableNames = 0;
static std::map<controlVariable, std::string> *controlVariableValues = 0;
static std::map<std::string, controlCmd> *controlCmdValues = 0;

static void initMaps() {
    controlVariableNames = new std::map<controlVariable, std::string>;
    (*controlVariableNames)[CV_LAST_CHANGE] =   "LastChange";
    (*controlVariableNames)[CV_PRESET_NAME_LIST] =    "ConnectionManager";
    (*controlVariableNames)[CV_AAT_CHANNEL] =         "A_ARG_TYPE_Channel";
    (*controlVariableNames)[CV_AAT_INSTANCE_ID] =         "A_ARG_TYPE_InstanceID";
    (*controlVariableNames)[CV_AAT_PRESET_NAME] =        "A_ARG_TYPE_PresetName";
    (*controlVariableNames)[CV_BRIGHTNESS] =        "Brightness";
    (*controlVariableNames)[CV_CONTRAST] =        "Contrast";
    (*controlVariableNames)[CV_SHARPNESS] =        "Sharpness";
    (*controlVariableNames)[CV_R_GAIN] =        "RedVideoGain";
    (*controlVariableNames)[CV_G_GAIN ] =        "GreenVideoGain";
    (*controlVariableNames)[CV_B_GAIN] =        "BlueVideoGain";
    (*controlVariableNames)[CV_R_BLACK] =        "RedVideoBlackLevel";
    (*controlVariableNames)[CV_G_BLACK] =        "GreenVideoBlackLevel";
    (*controlVariableNames)[CV_B_BLACK] =        "BlueVideoBlackLevel";
    (*controlVariableNames)[CV_COLOR_TEMP] =        "ColorTemperature";
    (*controlVariableNames)[CV_HOR_KEYSTONE] =        "HorizontalKeystone";
    (*controlVariableNames)[CV_VER_KEYSTONE] =        "VerticalKeystone";
    (*controlVariableNames)[CV_MUTE] =        "Mute";
    (*controlVariableNames)[CV_VOLUME] =        "Volume";
    (*controlVariableNames)[CV_VOLUME_DB] =        "VolumeDB";
    (*controlVariableNames)[CV_LOUDNESS] =        "Loudness";
    (*controlVariableNames)[CV_UNKNOWN] =    "";

    controlVariableValues = new std::map<controlVariable, std::string>;
    (*controlVariableValues)[CV_LAST_CHANGE     ] =    "<Event xmlns = \"urn:schemas-upnp-org:metadata-1-0/RCS/\"/>";
    (*controlVariableValues)[CV_PRESET_NAME_LIST] = "";
    (*controlVariableValues)[CV_AAT_CHANNEL] =         "";
    (*controlVariableValues)[CV_AAT_INSTANCE_ID] =   "0";
    (*controlVariableValues)[CV_AAT_PRESET_NAME] =   "";
    (*controlVariableValues)[CV_BRIGHTNESS] =             "0";
    (*controlVariableValues)[CV_CONTRAST] =                "0";
    (*controlVariableValues)[CV_SHARPNESS] =        "0";
    (*controlVariableValues)[CV_R_GAIN] =        "0";
    (*controlVariableValues)[CV_G_GAIN ] =        "0";
    (*controlVariableValues)[CV_B_GAIN] =        "0";
    (*controlVariableValues)[CV_R_BLACK] =        "0";
    (*controlVariableValues)[CV_G_BLACK] =        "0";
    (*controlVariableValues)[CV_B_BLACK] =        "0";
    (*controlVariableValues)[CV_COLOR_TEMP] =   "0";
    (*controlVariableValues)[CV_HOR_KEYSTONE] =   "0";
    (*controlVariableValues)[CV_VER_KEYSTONE] =    "0";
    (*controlVariableValues)[CV_MUTE] =        "0";
    (*controlVariableValues)[CV_VOLUME] =        "0";
    (*controlVariableValues)[CV_VOLUME_DB] =        "0";
    (*controlVariableValues)[CV_LOUDNESS] =        "0";
    (*controlVariableValues)[CV_UNKNOWN] =    "";

    controlCmdValues = new std::map<std::string, controlCmd>;
    (*controlCmdValues)["SetMute"] = CC_SET_MUTE;
    (*controlCmdValues)["GetMute"] = CC_GET_MUTE;
    (*controlCmdValues)["GetVolume"] = CC_GET_VOL;
    (*controlCmdValues)["SetVolume"] = CC_SET_VOL;

}

//---------------------------------------------------------------------------------------------------------------
RControl::RControl(Player *pPlayer) : m_pPlayer(pPlayer) {
    initMaps();
    ID = "urn:schemas-upnp-org:service:RenderingControl";

    std::map<std::string, std::string>::iterator it = StateVariables->begin();
    it = StateVariables->insert(it, std::pair<std::string, std::string>("LastChange", Xml::escape("<Event xmlns = \"urn:schemas-upnp-org:metadata-1-0/RCS/\"/>", 0)));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("PresetNameList", "SHRenderer"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("Volume", m_pPlayer->GetVol()));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("Mute", "FALSE"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("A_ARG_TYPE_Channel", "Master"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("A_ARG_TYPE_InstanceID", "0"));
    StateVariables->insert(it, std::pair<std::string, std::string>("A_ARG_TYPE_PresetName", "SHRenderer"));

    mute = false;
}
//---------------------------------------------------------------------------------------------------------------
RControl::~RControl() {
    delete controlVariableNames;
    delete controlVariableValues;
    delete controlCmdValues;
}
//---------------------------------------------------------------------------------------------------------------
bool RControl::notify(const std::string &varName, const std::string &varVal) {
    char buf[4096];

    snprintf(buf, sizeof(buf),
             "<Event xmlns = \"urn:schemas-upnp-org:metadata-1-0/RCS/ \"><InstanceID val=\"0\"><%s val=\"%s\"/></InstanceID></Event>",
             varName.c_str(), varVal.c_str());

    notifyResponse->push(new VarElement("LastChange", Xml::escape(buf, 0)));

    (*StateVariables)[varName] = varVal;
    return true;
    //
}
//---------------------------------------------------------------------------------------------------------------
bool RControl::action(struct Upnp_Action_Request *event) {
    std::string sw = event->ActionName;
    switch ((*controlCmdValues)[sw]) {
        case CC_SET_MUTE:
            SetMute(event);
            notify("Mute", mute ? "1" : "0");
            break;
        case CC_GET_MUTE:
            notify("Mute", mute ? "1" : "0");
            break;
        case CC_GET_VOL:
            GetVol();
            break;
        case CC_SET_VOL:
            SetVol(event);
            break;
        default:
            return false;
    }

    return true;
}

bool RControl::SetMute(struct Upnp_Action_Request *event) {
    std::string dmute = upnpGetString(event, "DesiredMute");
    //mute = dmute != "0" ? true : false;
    mute = mute ? 0 : 1;
    m_pPlayer->Mute(mute);
    notify("Mute", mute ? "TRUE" : "FALSE");
    return true;
}

bool RControl::GetVol() {
    actionResponse->push(new VarElement("CurrentVolume", m_pPlayer->GetVol()));
    return true;
}

bool RControl::SetVol(struct Upnp_Action_Request *event) {
    SetVol(upnpGetString(event, "DesiredVolume"));
    return true;
}

bool RControl::SetVol(const std::string &dvol) {
    m_pPlayer->SetVol(atoi(dvol.c_str()));
    notify("Volume", m_pPlayer->GetVol());
    return true;
}
