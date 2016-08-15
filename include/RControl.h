#ifndef RCONTROL_H
#define RCONTROL_H

#include "Service.h"
#include "Player.h"

class RControl : public Service {
    public:
        bool mute;

        RControl(Player *pPlayer);
        virtual ~RControl();
        bool action(struct Upnp_Action_Request *event);

        bool notify(const std::string &, const std::string &);

        bool SetMute(struct Upnp_Action_Request *event);
        bool GetMute();
        bool GetVol();
        bool SetVol(struct Upnp_Action_Request *event);
        bool SetVol(const std::string &dvol);

    protected:
        Player                         *m_pPlayer;
};

#endif // RCONTROL_H
