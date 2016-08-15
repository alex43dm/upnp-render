#include <iostream>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Player.h"
#include "Config.h"
#include "Log.h"

void Player::init() {
    //eqSet(xml);
}

//---------------------------------------------------------------------------------------------------------------
bool Player::Play() {
    Log::debug("%s", __func__);
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::Stop() {
    Log::debug("%s", __func__);
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::Pause(bool onOff) {
    Log::debug("%s", __func__);
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::Next() {
    Log::debug("%s", __func__);
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::Previos() {
    Log::debug("%s", __func__);
    return true;
}
//---------------------------------------------------------------------------------------------------------------
int Player::seekTime(const std::string &Unit, const std::string &Target) {
    Log::debug("%s", __func__);
    int res = 0;
    if (Unit == "REL_TIME") {
        std::istringstream ss(Target);
        std::string token;
        int pos = 0;

        while (getline(ss, token, ':')) {
            switch (pos) {
                case 0:
                    res += atoi(token.c_str()) * 3600;
                    break;

                case 1:
                    res += atoi(token.c_str()) * 60;
                    break;

                case 2:
                    res += atoi(token.c_str());
                    break;
            }

            pos++;
        }
    }
    return res;
}

bool Player::Seek(const std::string &Unit, const std::string &Target) {
    Log::debug("%s, target: %s unit: %s", __func__, Target.c_str(), Unit.c_str());
    seekTime(Unit, Target);
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::SetAVTransportUri(const std::string &_uri) {
    Log::debug("%s url: ", __func__,_uri.c_str());
   Stop();
    return false;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::SetNextAVTransportUri(const std::string &_uri) {
    Log::debug("%s url: ", __func__,_uri.c_str());
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::Mute(bool val) {
    Log::debug("%s", __func__);
    return true;
}
//---------------------------------------------------------------------------------------------------------------
std::string Player::GetVol() {
    Log::debug("%s", __func__);
    return std::string(0);
}
//---------------------------------------------------------------------------------------------------------------
bool Player::SetVol(int val) {
    Log::debug("%s", __func__);
    return true;
}

std::string Player::trackDuration() {
    Log::debug("%s", __func__);
    return "";
}

std::string Player::curPostion() {
    Log::debug("%s", __func__);
    return "0";
}
//---------------------------------------------------------------------------------------------------------------
std::string Player::time2string(long ns) {
    char tmp[10];
    int hh, mm, ss;
    //init val
    hh = 0;
    mm = 0;
    ss = 0;
    hh   = ns / 3600;
    mm   = (ns % 3600) / 60;
    ss   = (ns % 60);
    memset(tmp, 0, sizeof(tmp));
    snprintf(tmp, sizeof(tmp), "%d:%02d:%02d", hh, mm, ss);
    return std::string(tmp);
}
//---------------------------------------------------------------------------------------------------------------
std::string Player::curPostionCount() {
    Log::debug("%s", __func__);
    return "0";
}
//---------------------------------------------------------------------------------------------------------------
bool Player::isPlaying() {
    Log::debug("%s", __func__);
    return false;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::eqSet(const std::string &xml) {

    IXML_Document *doc = ixmlParseBuffer(xml.c_str());

    if (!doc) {
        Log::err("does not found parse: %s", xml.c_str());
        return false;
    }

    IXML_NodeList *lst = ixmlDocument_getElementsByTagName(doc, "gain");

    if (lst) {
        for (size_t i = 0; i < ixmlNodeList_length(lst); i++) {
            //          clog<<"count: "<< i << " freq: "<< freq <<" val: "<< player->eq->gains[i]<<endl;
        }
    }
    else {
        Log::err("does not found equalizer section: %s",xml.c_str());
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------------------------------------------
std::string Player::eqGet() {

    //  char buf[1024];
    std::string ret = "<equalizer>\n";
    /*
        for( int i =0; i < EQLEN; i++)
        {
        snprintf(buf, sizeof(buf),"<gain freq=\"%s\">%.2f</gain>\n",eqBandsR[i].c_str(),player->eq->gains[i]);
        ret += buf;
        }*/
    /*
        for (map<string,int>::iterator it=eqBands.begin(); it!=eqBands.end(); ++it)
        {
            snprintf(buf, sizeof(buf),"<gain freq=\"%s\">%.2f</gain>\n",it->first.c_str(),player->eq->gains[it->second]);
            ret += buf;
        }
    */
    ret += "</equalizer>";
    return ret;
}
