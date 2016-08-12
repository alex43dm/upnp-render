#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "Service.h"
#include "Player.h"
#include "Config.h"

enum transportState {
    TRANSPORT_STOPPED,
    TRANSPORT_PLAYING,
    TRANSPORT_TRANSITIONING,
    TRANSPORT_PAUSED_PLAYBACK,
    TRANSPORT_PAUSED_RECORDING,
    TRANSPORT_RECORDING,
    TRANSPORT_NO_MEDIA_PRESENT
};

class Transport : public Service {
    public:
        Transport(Config *, Player *);
        virtual ~Transport();
        bool action(struct Upnp_Action_Request *_event);

        bool Play();
        bool Stop();
        bool Pause();
        bool Next();
        bool Previos();
        bool Seek(struct Upnp_Action_Request *event);
        bool SetAVTransportUri(struct Upnp_Action_Request *event);
        bool SetNextAVTransportUri(struct Upnp_Action_Request *event);
        bool GetPositionInfo();
        bool GetTransportInfo();
        bool GetDeviceCapabilities();
        bool GetTransportSettings();
        bool loop();

    protected:
    private:
        transportState state;
        bool notify(void);
        bool notify(int count, ...);
        string TrackMetaData;
        Player                     *m_pPlayer;
        string                      uri;
};

#endif // TRANSPORT_H
