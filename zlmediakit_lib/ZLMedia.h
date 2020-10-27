#ifndef LIBZLMEDIA_RUST_INTERFACE_H
#define LIBZLMEDIA_RUST_INTERFACE_H
#include "Player/MediaPlayer.h"
#include <stdint.h>

struct ZLMediaInstance;
typedef void (*OnProduce)(ZLMediaInstance *, uint8_t *, size_t);

enum Transport
{
    RTP_OVER_UDP,
    RTP_OVER_TCP,
    RTP_OVER_HTTP,
    RTP_OVER_UDP_MULTICAST
};

class ZLMedia
{
public:
    ZLMedia(std::string uri)
    {
        this->uri = uri;
        //this->zl_media_instance = zl_media_instance;
        player = std::make_shared<MediaPlayer>();
    }

    void set_transport(Transport transport)
    {
        this->transport = transport;
    }

    void set_on_produce(OnProduce on_produce)
    {
        this->on_produce = on_produce;
    }

    int init();

    void run()
    {
        if (transport == RTP_OVER_TCP)
        {
            (*player)[Client::kRtpType] = Rtsp::RTP_TCP;
            player->play(uri);
        }
        else if (transport == RTP_OVER_HTTP)
        {
            //(*player)[Client::kRtpType] = Rtsp::RTP_HTTP;
            //player->play(uri);
            std::cout << "unsupported transport" << std::endl;
            std::exit(1);
        }
        else if (transport == RTP_OVER_UDP)
        {
            (*player)[Client::kRtpType] = Rtsp::RTP_UDP;
            player->play(uri);
        }
        else if (transport == RTP_OVER_UDP_MULTICAST)
        {
            (*player)[Client::kRtpType] = Rtsp::RTP_MULTICAST;
            player->play(uri);
        }
        else
        {
            std::cout << "UNRECOGNIZED PLAY TYPE FOR RTSP" << std::endl;
        }
        //std::cout << "running for " << uri << std::endl;
    };

    int receivePacket()
    {
        return 0;
    };

private:
    //ZLMediaInstance *zl_media_instance;
    MediaPlayer::Ptr player;
    Transport transport;
    OnProduce on_produce;
    std::string uri;
};
#endif //LIBZLMEDIA_RUST_INTERFACE_H