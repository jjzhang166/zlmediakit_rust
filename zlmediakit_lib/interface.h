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
    void zlmedia_set_parent(ZLMediaInstance *zLMediaInstance, ZLMedia *zl_media)
    {
        ((ZLMedia *)zLMediaInstance)->set_parent(zl_media);
    }
    void zlmedia_set_on_produce(ZLMediaInstance *zLMediaInstance, OnProduce on_produce)
    {
        ((ZLMedia *)zLMediaInstance)->set_on_produce(on_produce);
    }
    void zlmedia_set_transport(ZLMediaInstance *zLMediaInstance, int transport)
    {
        ((ZLMedia *)zLMediaInstance)->set_transport((Transport)transport);
    }
    void zlmedia_destroy(ZLMediaInstance *zLMediaInstance)
    {
        delete ((ZLMedia *)zLMediaInstance);
    }
    void zlmedia_init(ZLMediaInstance *zLMediaInstance)
    {
        ((ZLMedia *)zLMediaInstance)->init();
    }
    void zlmedia_run(ZLMediaInstance *zLMediaInstance)
    {
        ((ZLMedia *)zLMediaInstance)->run();
    }
    /*
    void cppDeleteArray(uint8_t *data)
    {
        delete[] data;
    }
    void cppDeletePointer(uint8_t *data)
    {
        delete data;
    }
    */
}
#endif //LIBZLMEDIAKIT_RUST_INTERFACE