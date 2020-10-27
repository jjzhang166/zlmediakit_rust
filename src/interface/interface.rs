use libc::{c_char, c_int, c_uchar, c_uint, size_t};
//use std::ffi::CString;

#[repr(C)]
pub struct ZLInstance {
    _private: [u8; 0],
}

pub enum Transport {
    RtpOverUdp,
    RtpOverTcp,
    RtpOverHttp,
    RtpOverUdpMulticast,
}

type OnProduce = extern "C" fn(*mut ZLInstance, *const c_uchar, size_t);

extern "C" {
    pub fn zlmedia_new(uri: *const c_char) -> *mut ZLInstance;
    pub fn zlmedia_set_on_produce(zl_media: *mut ZLInstance, on_produce: OnProduce);
    pub fn zlmedia_set_transport(zl_media: *mut ZLInstance, transport: c_int);
    pub fn zlmedia_init(zl_media: *mut ZLInstance);
    pub fn zlmedia_destroy(zl_media: *mut ZLInstance);
}
