#ifndef LIBZLMEDIAKIT_RUST_INTERFACE
#define LIBZLMEDIAKIT_RUST_INTERFACE
#include <iostream>
#include <memory>
#include <map>
#include "ZLMedia.h"

//Function to send produced packets: onProduce(ZL_ID id, uint8_t* buffer, size_t size)

extern "C"
{
    //Returns number >=0 if created sucessfully, <0 if problem. Returned number is id
    ZLMediaInstance *zlmedia_new(const char *url)
    {
        return ((ZLMediaInstance *)new ZLMedia(url));
    }
    void zlmedia_set_on_produce(ZLMediaInstance *zl_media, OnProduce on_produce)
    {
        ((ZLMedia *)zl_media)->set_on_produce(on_produce);
    }
    void zlmedia_set_transport(ZLMediaInstance *zl_media, int transport)
    {
        ((ZLMedia *)zl_media)->set_transport((Transport)transport);
    }
    void zlmedia_destroy(ZLMediaInstance* zl_media) {
        delete ((ZLMedia *)zl_media);
    }
    void zlmedia_init(ZLMediaInstance* zl_media) {
        ((ZLMedia *)zl_media)->init();
    }
    void cppDeleteArray(uint8_t *data)
    {
        delete[] data;
    }
    void cppDeletePointer(uint8_t *data)
    {
        delete data;
    }
}
#endif //LIBZLMEDIAKIT_RUST_INTERFACE