#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
    public:
        void init(const std::string &);
        std::string time2string(long ns = 0);
        bool eqSet(const std::string &str);
        std::string eqGet();
        static int seekTime(const std::string &Unit, const std::string &Target);
    public:
        virtual bool Play();
        virtual bool Stop();
        virtual bool Pause(bool);
        virtual bool Next();
        virtual bool Previos();
        virtual bool Seek(const std::string &, const std::string &);
        virtual bool SetAVTransportUri(const std::string &);
        virtual bool SetNextAVTransportUri(const std::string &);
        virtual bool Mute(bool);
        virtual std::string GetVol();
        virtual bool SetVol(int val);
        virtual std::string trackDuration();
        virtual std::string curPostion();
        virtual std::string curPostionCount();
        virtual bool isPlaying();
};

#endif // PLAYER_H
