#include "ZLMedia.h"

int ZLMedia::init()
{
    std::cout << "init player" << std::endl;

    weak_ptr<MediaPlayer> weakPlayer = player;
    player->setOnPlayResult([weakPlayer, this](const SockException &ex) {
        //this->LOG << "OnPlayResult: " << ex.what() << " for " << this->uri;
        //onConnection();
        auto strongPlayer = weakPlayer.lock();
        if (ex || !strongPlayer)
        {
            return;
        }

        auto videoTrack = strongPlayer->getTrack(TrackVideo);
        if (!videoTrack)
        {
            //this->LOG << "No video track for " << uri;
            return;
        }
        videoTrack->addDelegate(std::make_shared<FrameWriterInterfaceHelper>([this](const Frame::Ptr &frame) {    
            //onProduce(zLRTSPEncodedPacket);
            this->on_produce((ZLMediaInstance *)this, reinterpret_cast<uint8_t*>(frame->data()), frame->size());
        }));
    });

    player->setOnResume([this]() {

    });

    player->setOnShutdown([this](const SockException &ex) {
        std::cout << "shutdown player" << std::endl;
    });
    return 0;
}