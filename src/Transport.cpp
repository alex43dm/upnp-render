#include <map>
#include <iostream>

#include <stdarg.h>

#include "Transport.h"

typedef enum {
    TRANSPORT_VAR_TRANSPORT_STATUS,
    TRANSPORT_VAR_NEXT_AV_URI,
    TRANSPORT_VAR_NEXT_AV_URI_META,
    TRANSPORT_VAR_CUR_TRACK_META,
    TRANSPORT_VAR_REL_CTR_POS,
    TRANSPORT_VAR_AAT_INSTANCE_ID,
    TRANSPORT_VAR_AAT_SEEK_TARGET,
    TRANSPORT_VAR_PLAY_MEDIUM,
    TRANSPORT_VAR_REL_TIME_POS,
    TRANSPORT_VAR_REC_MEDIA,
    TRANSPORT_VAR_CUR_PLAY_MODE,
    TRANSPORT_VAR_TRANSPORT_PLAY_SPEED,
    TRANSPORT_VAR_PLAY_MEDIA,
    TRANSPORT_VAR_ABS_TIME_POS,
    TRANSPORT_VAR_CUR_TRACK,
    TRANSPORT_VAR_CUR_TRACK_URI,
    TRANSPORT_VAR_CUR_TRANSPORT_ACTIONS,
    TRANSPORT_VAR_NR_TRACKS,
    TRANSPORT_VAR_AV_URI,
    TRANSPORT_VAR_ABS_CTR_POS,
    TRANSPORT_VAR_CUR_REC_QUAL_MODE,
    TRANSPORT_VAR_CUR_MEDIA_DUR,
    TRANSPORT_VAR_AAT_SEEK_MODE,
    TRANSPORT_VAR_AV_URI_META,
    TRANSPORT_VAR_REC_MEDIUM,
    TRANSPORT_VAR_REC_MEDIUM_WR_STATUS,
    TRANSPORT_VAR_LAST_CHANGE,
    TRANSPORT_VAR_CUR_TRACK_DUR,
    TRANSPORT_VAR_TRANSPORT_STATE,
    TRANSPORT_VAR_POS_REC_QUAL_MODE,
    TRANSPORT_VAR_UNKNOWN,
    TRANSPORT_VAR_COUNT
} transportVariable;

typedef enum {
    TRANSPORT_CMD_GETCURRENTTRANSPORTACTIONS,
    TRANSPORT_CMD_GETDEVICECAPABILITIES,
    TRANSPORT_CMD_GETMEDIAINFO,
    TRANSPORT_CMD_GETPOSITIONINFO,
    TRANSPORT_CMD_GETTRANSPORTINFO,
    TRANSPORT_CMD_GETTRANSPORTSETTINGS,
    TRANSPORT_CMD_NEXT,
    TRANSPORT_CMD_PAUSE,
    TRANSPORT_CMD_PLAY,
    TRANSPORT_CMD_PREVIOUS,
    TRANSPORT_CMD_SEEK,
    TRANSPORT_CMD_SETAVTRANSPORTURI,
    //TRANSPORT_CMD_SETPLAYMODE,
    TRANSPORT_CMD_STOP,
    TRANSPORT_CMD_SETNEXTAVTRANSPORTURI,
    //TRANSPORT_CMD_RECORD,
    //TRANSPORT_CMD_SETRECORDQUALITYMODE,
    TRANSPORT_CMD_UNKNOWN,
    TRANSPORT_CMD_COUNT
} transportCmd ;

static std::map<std::string, transportCmd> *transportCmdValues;
static std::map<transportVariable, const char *> *transportVariableNames;
static std::map<transportVariable, const char *> *transportVariableValues;
static std::map<transportState, const char *> *transportStateValues;

static void initMaps() {
    transportCmdValues = new std::map<std::string, transportCmd>;
    (*transportCmdValues)["GetCurrentTransportActions"] = TRANSPORT_CMD_GETCURRENTTRANSPORTACTIONS;
    (*transportCmdValues)["GetDeviceCapabilities"] = TRANSPORT_CMD_GETDEVICECAPABILITIES;
    (*transportCmdValues)["GetMediaInfo"] = TRANSPORT_CMD_GETMEDIAINFO;
    (*transportCmdValues)["SetAVTransportURI"] = TRANSPORT_CMD_SETAVTRANSPORTURI;
    (*transportCmdValues)["SetNextAVTransportURI"] = TRANSPORT_CMD_SETNEXTAVTRANSPORTURI;
    (*transportCmdValues)["GetTransportInfo"] = TRANSPORT_CMD_GETTRANSPORTINFO;
    (*transportCmdValues)["GetPositionInfo"] = TRANSPORT_CMD_GETPOSITIONINFO;
    (*transportCmdValues)["GetTransportSettings"] = TRANSPORT_CMD_GETTRANSPORTSETTINGS;
    (*transportCmdValues)["Stop"] = TRANSPORT_CMD_STOP;
    (*transportCmdValues)["Play"] = TRANSPORT_CMD_PLAY;
    (*transportCmdValues)["Pause"] = TRANSPORT_CMD_PAUSE;
    //        (*transportCmdValues)["Record"] = TRANSPORT_CMD_RECORD;
    (*transportCmdValues)["Seek"] = TRANSPORT_CMD_SEEK;
    (*transportCmdValues)["Next"] = TRANSPORT_CMD_NEXT;
    (*transportCmdValues)["Previous"] = TRANSPORT_CMD_PREVIOUS;
    //(*transportCmdValues)["SetPlayMode"] = TRANSPORT_CMD_SETPLAYMODE;
    //(*transportCmdValues)["SetRecordQualityMode"] = TRANSPORT_CMD_SETRECORDQUALITYMODE;

    transportVariableNames = new std::map<transportVariable, const char *>;
    (*transportVariableNames)[TRANSPORT_VAR_TRANSPORT_STATE] =  "TransportState";
    (*transportVariableNames)[TRANSPORT_VAR_TRANSPORT_STATUS] =  "TransportStatus";
    (*transportVariableNames)[TRANSPORT_VAR_PLAY_MEDIUM] =  "PlaybackStorageMedium";
    (*transportVariableNames)[TRANSPORT_VAR_REC_MEDIUM] =  "RecordStorageMedium";
    (*transportVariableNames)[TRANSPORT_VAR_PLAY_MEDIA] =  "PossiblePlaybackStorageMedia";
    (*transportVariableNames)[TRANSPORT_VAR_REC_MEDIA] =  "PossibleRecordStorageMedia";
    (*transportVariableNames)[TRANSPORT_VAR_CUR_PLAY_MODE] =  "CurrentPlayMode";
    (*transportVariableNames)[TRANSPORT_VAR_TRANSPORT_PLAY_SPEED] =  "TransportPlaySpeed";
    (*transportVariableNames)[TRANSPORT_VAR_REC_MEDIUM_WR_STATUS] =  "RecordMediumWriteStatus";
    (*transportVariableNames)[TRANSPORT_VAR_CUR_REC_QUAL_MODE] =  "CurrentRecordQualityMode";
    (*transportVariableNames)[TRANSPORT_VAR_POS_REC_QUAL_MODE] =  "PossibleRecordQualityModes";
    (*transportVariableNames)[TRANSPORT_VAR_NR_TRACKS] =  "NumberOfTracks";
    (*transportVariableNames)[TRANSPORT_VAR_CUR_TRACK] =  "CurrentTrack";
    (*transportVariableNames)[TRANSPORT_VAR_CUR_TRACK_DUR] =  "CurrentTrackDuration";
    (*transportVariableNames)[TRANSPORT_VAR_CUR_MEDIA_DUR] =  "CurrentMediaDuration";
    (*transportVariableNames)[TRANSPORT_VAR_CUR_TRACK_META] =  "CurrentTrackMetaData";
    (*transportVariableNames)[TRANSPORT_VAR_CUR_TRACK_URI] =  "CurrentTrackURI";
    (*transportVariableNames)[TRANSPORT_VAR_AV_URI] =  "AVTransportURI";
    (*transportVariableNames)[TRANSPORT_VAR_AV_URI_META] =  "AVTransportURIMetaData";
    (*transportVariableNames)[TRANSPORT_VAR_NEXT_AV_URI] =  "NextAVTransportURI";
    (*transportVariableNames)[TRANSPORT_VAR_NEXT_AV_URI_META] =  "NextAVTransportURIMetaData";
    (*transportVariableNames)[TRANSPORT_VAR_REL_TIME_POS] =  "RelativeTimePosition";
    (*transportVariableNames)[TRANSPORT_VAR_ABS_TIME_POS] =  "AbsoluteTimePosition";
    (*transportVariableNames)[TRANSPORT_VAR_REL_CTR_POS] =  "RelativeCounterPosition";
    (*transportVariableNames)[TRANSPORT_VAR_ABS_CTR_POS] =  "AbsoluteCounterPosition";
    (*transportVariableNames)[TRANSPORT_VAR_LAST_CHANGE] =  "LastChange";
    (*transportVariableNames)[TRANSPORT_VAR_AAT_SEEK_MODE] =  "A_ARG_TYPE_SeekMode";
    (*transportVariableNames)[TRANSPORT_VAR_AAT_SEEK_TARGET] =  "A_ARG_TYPE_SeekTarget";
    (*transportVariableNames)[TRANSPORT_VAR_AAT_INSTANCE_ID] =  "A_ARG_TYPE_InstanceID";
    (*transportVariableNames)[TRANSPORT_VAR_CUR_TRANSPORT_ACTIONS] =  "CurrentTransportActions";
    (*transportVariableNames)[TRANSPORT_VAR_UNKNOWN] =  "";

    transportVariableValues = new std::map<transportVariable, const char *>;
    (*transportVariableValues)[TRANSPORT_VAR_TRANSPORT_STATE] =  "STOPPED";
    (*transportVariableValues)[TRANSPORT_VAR_TRANSPORT_STATUS] =  "OK";
    (*transportVariableValues)[TRANSPORT_VAR_PLAY_MEDIUM] =  "UNKNOWN";
    (*transportVariableValues)[TRANSPORT_VAR_REC_MEDIUM] =  "NOT_IMPLEMENTED";
    (*transportVariableValues)[TRANSPORT_VAR_PLAY_MEDIA] =  "NETWORK,UNKNOWN";
    (*transportVariableValues)[TRANSPORT_VAR_REC_MEDIA] =  "NOT_IMPLEMENTED";
    (*transportVariableValues)[TRANSPORT_VAR_CUR_PLAY_MODE] =  "NORMAL";
    (*transportVariableValues)[TRANSPORT_VAR_TRANSPORT_PLAY_SPEED] =  "1";
    (*transportVariableValues)[TRANSPORT_VAR_REC_MEDIUM_WR_STATUS] =  "NOT_IMPLEMENTED";
    (*transportVariableValues)[TRANSPORT_VAR_CUR_REC_QUAL_MODE] =  "NOT_IMPLEMENTED";
    (*transportVariableValues)[TRANSPORT_VAR_POS_REC_QUAL_MODE] =  "NOT_IMPLEMENTED";
    (*transportVariableValues)[TRANSPORT_VAR_NR_TRACKS] =  "0";
    (*transportVariableValues)[TRANSPORT_VAR_CUR_TRACK] =  "0";
    (*transportVariableValues)[TRANSPORT_VAR_CUR_TRACK_DUR] =  "00:00:00";
    (*transportVariableValues)[TRANSPORT_VAR_CUR_MEDIA_DUR] =  "";
    (*transportVariableValues)[TRANSPORT_VAR_CUR_TRACK_META] =  "";
    (*transportVariableValues)[TRANSPORT_VAR_CUR_TRACK_URI] =  "";
    (*transportVariableValues)[TRANSPORT_VAR_AV_URI] =  "";
    (*transportVariableValues)[TRANSPORT_VAR_AV_URI_META] =  "";
    (*transportVariableValues)[TRANSPORT_VAR_NEXT_AV_URI] =  "";
    (*transportVariableValues)[TRANSPORT_VAR_NEXT_AV_URI_META] =  "";
    (*transportVariableValues)[TRANSPORT_VAR_REL_TIME_POS] =  "00:00:00";
    (*transportVariableValues)[TRANSPORT_VAR_ABS_TIME_POS] =  "NOT_IMPLEMENTED";
    (*transportVariableValues)[TRANSPORT_VAR_REL_CTR_POS] =  "2147483647";
    (*transportVariableValues)[TRANSPORT_VAR_ABS_CTR_POS] =  "2147483647";
    (*transportVariableValues)[TRANSPORT_VAR_LAST_CHANGE] =  "<Event xmlns\"urn:schemas-upnp-org:metadata-1-0/AVT/\"/>";
    (*transportVariableValues)[TRANSPORT_VAR_AAT_SEEK_MODE] =  "TRACK_NR";
    (*transportVariableValues)[TRANSPORT_VAR_AAT_SEEK_TARGET] =  "";
    (*transportVariableValues)[TRANSPORT_VAR_AAT_INSTANCE_ID] =  "0";
    (*transportVariableValues)[TRANSPORT_VAR_CUR_TRANSPORT_ACTIONS] =  "PLAY";
    (*transportVariableValues)[TRANSPORT_VAR_UNKNOWN] =  "";

    transportStateValues = new std::map<transportState, const char *>;
    (*transportStateValues)[TRANSPORT_STOPPED] = "STOPPED";
    (*transportStateValues)[TRANSPORT_PLAYING] = "PLAYING";
    (*transportStateValues)[TRANSPORT_TRANSITIONING] = "TRANSITIONING";
    (*transportStateValues)[TRANSPORT_PAUSED_PLAYBACK] = "PAUSED_PLAYBACK";
    (*transportStateValues)[TRANSPORT_PAUSED_RECORDING] = "PAUSED_RECORDING";
    (*transportStateValues)[TRANSPORT_RECORDING] = "RECORDING";
    (*transportStateValues)[TRANSPORT_NO_MEDIA_PRESENT] = "NO_MEDIA_PRESENT";

}

//---------------------------------------------------------------------------------------------------------------
Transport::Transport(Player *pPlayer) : m_pPlayer(pPlayer) {
    initMaps();
    ID = "urn:schemas-upnp-org:service:AVTransport";
    state = TRANSPORT_STOPPED;
    std::map<std::string, std::string>::iterator it = StateVariables->begin();
    it = StateVariables->insert(it, std::pair<std::string, std::string>("TransportState", (*transportStateValues)[state]));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("TransportStatus", "OK"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("CurrentMediaCategory", "TRACK_AWARE"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("PlaybackStorageMedium", "NETWORK,UNKNOWN"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("RecordStorageMedium", "NONE"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("PossiblePlaybackStorageMedia", "NETWORK,UNKNOWN"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("PossibleRecordStorageMedia", "NONE"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("CurrentPlayMode", "NORMAL"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("TransportPlaySpeed", "1"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("RecordMediumWriteStatus", "NOT_WRITABLE"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("CurrentRecordQualityMode", "NOT_IMPLEMENTED"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("PossibleRecordQualityModes", "NOT_IMPLEMENTED"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("NumberOfTracks", "1"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("CurrentTrack", "0"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("CurrentTrackDuration", "0:00:00"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("CurrentMediaDuration", "0:00:00"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("CurrentTrackMetaData", ""));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("CurrentTrackURI", ""));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("AVTransportURI", ""));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("AVTransportURIMetaData", ""));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("NextAVTransportURI", ""));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("NextAVTransportURIMetaData", ""));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("RelativeTimePosition", "0:00:00"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("AbsoluteTimePosition", "0:00:00"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("RelativeCounterPosition", "0"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("AbsoluteCounterPosition", "0"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("LastChange", Xml::escape("<Event xmlns\"urn:schemas-upnp-org:metadata-1-0/AVT/\"/>", 0)));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("A_ARG_TYPE_SeekMode", "REL_TIME"));
    it = StateVariables->insert(it, std::pair<std::string, std::string>("A_ARG_TYPE_SeekTarget", "REL_TIME"));
    StateVariables->insert(it, std::pair<std::string, std::string>("A_ARG_TYPE_InstanceID", "0"));
    pPlayer->init();
}
//---------------------------------------------------------------------------------------------------------------
Transport::~Transport() {
    delete transportCmdValues;
    delete transportVariableNames;
    delete transportVariableValues;
    delete transportStateValues;
}
//---------------------------------------------------------------------------------------------------------------
/*
    bool Transport::notify(void)
    {
    char *buf;

    asprintf(&buf,
    "<Event xmlns = \"urn:schemas-upnp-org:metadata-1-0/RCS/ \"><InstanceID val=\"0\"><%s val=\"%s\"/></InstanceID></Event>",
    varName.c_str(), varVal.c_str());

    notifyResponse->push( new VarElement( "LastChange", Xml::escape(buf , 0) ));
    if( buf )free(buf);

    (*StateVariables)[varName] = varVal;
    return true;
    }*/
//---------------------------------------------------------------------------------------------------------------
bool Transport::action(struct Upnp_Action_Request *event) {
    std::string sw = event->ActionName;
    //clog<<"event->ActionName: "<<event->ActionName<<endl;

    switch ((*transportCmdValues)[sw]) {
        case TRANSPORT_CMD_PAUSE:
            Pause();
            break;

        case TRANSPORT_CMD_NEXT:
            Next();
            break;

        case TRANSPORT_CMD_PLAY:
            Play();
            break;

        case TRANSPORT_CMD_PREVIOUS:
            Previos();
            break;

        case TRANSPORT_CMD_SEEK:
            Seek(event);
            break;

        case TRANSPORT_CMD_SETAVTRANSPORTURI:
            SetAVTransportUri(event);
            break;

        case TRANSPORT_CMD_STOP:
            Stop();
            break;

        case TRANSPORT_CMD_SETNEXTAVTRANSPORTURI:
            SetNextAVTransportUri(event);
            break;

        case TRANSPORT_CMD_GETPOSITIONINFO:
            return GetPositionInfo();

        case TRANSPORT_CMD_GETTRANSPORTINFO:
            return GetTransportInfo();

        case TRANSPORT_CMD_GETDEVICECAPABILITIES :
            return GetDeviceCapabilities();

        case TRANSPORT_CMD_GETTRANSPORTSETTINGS :
            return GetTransportSettings();

        default:
            return false;
    }

    (*StateVariables)["TransportState"] = (*transportStateValues)[state];

    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Transport::Play() {
    if (state != TRANSPORT_PAUSED_PLAYBACK) {
        if (state == TRANSPORT_TRANSITIONING) {
            m_pPlayer->Play();
            state = TRANSPORT_PLAYING;
            notify(2,
                   "TransportState", (*transportStateValues)[state],
                   "TransportPlaySpeed", "1");
        }
        else {
            state = TRANSPORT_NO_MEDIA_PRESENT;
            notify(1, "TransportState", (*transportStateValues)[state]);
        }
    }
    else {
        m_pPlayer->Pause(false);
        state = TRANSPORT_PLAYING;
        notify(2,
               "TransportState", (*transportStateValues)[state],
               "TransportPlaySpeed", "1");
    }

    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Transport::Stop() {
    if (state == TRANSPORT_PLAYING ||
            state == TRANSPORT_PAUSED_PLAYBACK) {
        m_pPlayer->Stop();
    }

    state = TRANSPORT_STOPPED;
    notify(1, "TransportState", (*transportStateValues)[state]);
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Transport::Pause() {
    if (state != TRANSPORT_PAUSED_PLAYBACK) {
        state = TRANSPORT_PAUSED_PLAYBACK;
        m_pPlayer->Pause(true);
    }
    else {
        state = TRANSPORT_PLAYING;
        m_pPlayer->Pause(false);
    }

    notify(1, "TransportState", (*transportStateValues)[state]);
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Transport::Next() {
    //state = TRANSPORT_PLAYING;
    m_pPlayer->Next();
    state = TRANSPORT_TRANSITIONING;
    notify(1, "TransportState", (*transportStateValues)[state]);
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Transport::Previos() {
    //state = TRANSPORT_PLAYING;
    m_pPlayer->Previos();
    state = TRANSPORT_TRANSITIONING;
    notify(1, "TransportState", (*transportStateValues)[state]);
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Transport::Seek(struct Upnp_Action_Request *event) {
    //state = TRANSPORT_PLAYING;
    //clog<< "Transport::Seek\n";
    std::string Unit = upnpGetString(event, "Unit");//REL_TIME
    std::string Target = upnpGetString(event, "Target");
    state = TRANSPORT_TRANSITIONING;
    notify(1, "TransportState", (*transportStateValues)[state]);
    m_pPlayer->Seek(Unit, Target);
    state = TRANSPORT_PLAYING;
    notify(1, "TransportState", (*transportStateValues)[state]);
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Transport::SetAVTransportUri(struct Upnp_Action_Request *event) {
    Stop();
    uri = upnpGetString(event, "CurrentURI");
    TrackMetaData = upnpGetString(event, "CurrentURIMetaData");

    if (m_pPlayer->SetAVTransportUri(uri)) {
        state = TRANSPORT_TRANSITIONING;
        notify(2, "NumberOfTracks", "1",
               "TransportState", (*transportStateValues)[state]);
    }

    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Transport::SetNextAVTransportUri(struct Upnp_Action_Request *event) {
    //state = TRANSPORT_PLAYING;
    m_pPlayer->SetNextAVTransportUri(uri);
    return true;
}

bool Transport::GetPositionInfo() {
    actionResponse->push(new VarElement("Track", "1"));
    actionResponse->push(new VarElement("TrackDuration", m_pPlayer->trackDuration()));
    actionResponse->push(new VarElement("TrackMetaData", Xml::escape(TrackMetaData.c_str(), 0)));
    actionResponse->push(new VarElement("TrackURI", uri));
    actionResponse->push(new VarElement("RelTime", m_pPlayer->curPostion()));
    actionResponse->push(new VarElement("AbsTime", m_pPlayer->curPostion()));
    actionResponse->push(new VarElement("RelCount", m_pPlayer->curPostionCount()));
    actionResponse->push(new VarElement("AbsCount", m_pPlayer->curPostionCount()));
    return true;
}


bool Transport::GetTransportInfo() {
    actionResponse->push(new VarElement("CurrentTransportState", (*transportStateValues)[state]));
    actionResponse->push(new VarElement("CurrentTransportStatus", "OK"));   //OK || ERROR_OCCURRED
    actionResponse->push(new VarElement("CurrentSpeed", "1"));
    return true;
}

#define BUFLEN 8192

bool Transport::notify(int count, ...) {
    char *buf;
    int n;
    va_list va;
    buf = (char *)malloc(BUFLEN);
    memset(buf, 0, BUFLEN);
    sprintf(buf, "<Event xmlns = \"urn:schemas-upnp-org:metadata-1-0/AVT/\"><InstanceID val=\"0\">");
    va_start(va, count);

    for (n = 0; n < count; n++) {
        char *arg_name, *arg_val;
        arg_name = va_arg(va, char *);
        arg_val  = va_arg(va, char *);
        char buf1[4096];
        sprintf(buf1, "<%s val=\"%s\"/>", arg_name, arg_val);
        buf = strcat(buf, buf1);
    }

    va_end(va);
    sprintf(buf, "%s</InstanceID></Event>", buf);
    notifyResponse->push(new VarElement("LastChange", buf));

    if (buf)free((void *)buf);

    return true;
}

bool Transport::loop() {
    //clog<<"TrackDuration: "<<trackDuration()<<" curPostion: "<< curPostion() <<endl;
    if (state == TRANSPORT_PLAYING && !m_pPlayer->isPlaying()) {
        state = TRANSPORT_STOPPED;
        m_pPlayer->Stop();
    }

    //if(state == TRANSPORT_PLAYING )   GetPositionInfo();
    return true;
}

bool Transport::GetDeviceCapabilities() {
    actionResponse->push(new VarElement("PlayMedia", "NETWORK,UNKNOWN"));
    actionResponse->push(new VarElement("RecMedia", "NOT_IMPLEMENTED"));
    actionResponse->push(new VarElement("RecQualityModes", "NOT_IMPLEMENTED"));
    return true;
}

bool Transport::GetTransportSettings() {
    actionResponse->push(new VarElement("PlayMode", "NORMAL"));
    actionResponse->push(new VarElement("RecQualityMode", "NOT_IMPLEMENTED"));
    return true;
}


