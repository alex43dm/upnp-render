#include <iostream>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Player.h"
#include "Config.h"
//#include "stools.h"

#ifdef _FF_PLAYER_
FFPlayer *player;
#endif

void Player::init(string xml) {
    #ifdef _FF_PLAYER_
    player = new FFPlayer();
    #endif
    eqSet(xml);
}

//---------------------------------------------------------------------------------------------------------------
bool Player::Play() {
    clog << "Player::Play" << endl;

    #ifdef _FF_PLAYER_
    if (player) {
        if (player->cmd == State::STOP) {
            pthread_create(&thread, NULL, threadLoop, (void *)player);
        }
        else {
            player->cmd = State::PLAY;
        }
    }
    #endif
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::Stop() {
    clog << "Player::Stop" << endl;

    #ifdef _FF_PLAYER_
    if (player) {
        player->cmd = State::STOP;

        if (thread) {
            pthread_join(thread, NULL);
            thread = 0;
        }
    }
    #endif
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::Pause(bool onOff) {
    clog << "Player::Pause: " << onOff << endl;

    #ifdef _FF_PLAYER_
    if (onOff)
        player->cmd = State::PAUSE;
    else
        player->cmd = State::PLAY;
    #endif
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::Next() {
    clog << "Player::Next" << endl;
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::Previos() {
    clog << "Player::Previos" << endl;
    return true;
}
//---------------------------------------------------------------------------------------------------------------
int Player::seekTime(string Unit, string Target) {
    int res = 0;
    if (Unit == "REL_TIME") {
        istringstream ss(Target);
        string token;
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

bool Player::Seek(string Unit, string Target) {
    clog << "Player::Seek: Target:" << Target << " Unit: " << Unit << endl;

    #ifdef _FF_PLAYER_
    int res = seekTime(Unit, Target);
    player->seekTo = res * 1000000LL;
    player->cmd = State::SEEK;
    #else
    seekTime(Unit, Target);
    #endif
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::SetAVTransportUri(const string _uri) {
    clog << "Player::SetAVTransportUri: url: " << _uri << endl;
    Stop();
    #ifdef _FF_PLAYER_
    return player->open(_uri);
    #else
    return false;
    #endif
}
//---------------------------------------------------------------------------------------------------------------
bool Player::SetNextAVTransportUri(std::string _uri) {
    clog << "Player::SetNextAVTransportUri: " << _uri << endl;

    #ifdef _FF_PLAYER_
    if (player) {
        //      player->nextUri = _uri;
    }
    #endif
    return true;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::Mute(bool val) {
    clog << "Player::Mute: " << val << endl;

    #ifdef _FF_PLAYER_
    if (player) {
        if (val)
            player->setVol(0);
        else
            player->setVol(vol);
    }
    #endif
    return true;
}
//---------------------------------------------------------------------------------------------------------------
string Player::GetVol() {
    std::clog << "Player::GetVol" << std::endl;
    return std::string(0);
}
//---------------------------------------------------------------------------------------------------------------
bool Player::SetVol(int val) {
    std::clog << "Player::SetVol: " << val << std::endl;

    #ifdef _FF_PLAYER_
    if (player) {
        player->setVol(val);
    }
    #endif
    return true;
}

string Player::trackDuration() {
    #ifdef _FF_PLAYER_
    return time2string(player->duration / 1000000LL);
    #else
    return "";
    #endif
}

string Player::curPostion() {
    #ifdef _FF_PLAYER_
    return time2string(player->position * player->time_base.num / player->time_base.den);
    #else
    return "0";
    #endif
}
//---------------------------------------------------------------------------------------------------------------
string Player::time2string(long ns) {
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
    #ifdef _WIN32
    _snprintf_s(tmp, sizeof(tmp), sizeof(tmp), "%d:%02d:%02d", hh, mm, ss);
    #else
    snprintf(tmp, sizeof(tmp), "%d:%02d:%02d", hh, mm, ss);
    #endif
    return string(tmp);
}
//---------------------------------------------------------------------------------------------------------------
string Player::curPostionCount() {
    #ifdef _FF_PLAYER_
    char tmp2[10];
    snprintf(tmp2, sizeof(tmp2), "%ld", player->position);
    return string(tmp2);
    #else
    return "0";
    #endif
}
//---------------------------------------------------------------------------------------------------------------
bool Player::isPlaying() {
    #ifdef _FF_PLAYER_
    if (player->cmd == State::PLAY) {
        //pthread_join(thread,NULL);
        return true;
    }
    #endif
    return false;
}
//---------------------------------------------------------------------------------------------------------------
bool Player::eqSet(string xml) {

    IXML_Document *doc = ixmlParseBuffer(xml.c_str());

    if (!doc) {
        clog << "does not found parse: " << xml << endl;
        return false;
    }

    IXML_NodeList *lst = ixmlDocument_getElementsByTagName(doc, "gain");

    if (lst) {
        for (size_t i = 0; i < ixmlNodeList_length(lst); i++) {
            #ifdef _FF_PLAYER_
            IXML_Node *n = ixmlNodeList_item(lst, i);
            string freq = ixmlElement_getAttribute((IXML_Element *)n,  "freq");
            IXML_Node *textNode = ixmlNode_getFirstChild(n);
            double val =  strtod(ixmlNode_getNodeValue(textNode), NULL);
            player->eq->gains[eqBands[freq]] = val;
            #endif
            //          clog<<"count: "<< i << " freq: "<< freq <<" val: "<< player->eq->gains[i]<<endl;
        }
    }
    else {
        clog << "does not found equalizer section: " << xml << endl;
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------------------------------------------
string Player::eqGet() {

    //  char buf[1024];
    string ret = "<equalizer>\n";
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
