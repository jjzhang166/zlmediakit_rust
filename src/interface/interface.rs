use libc::{c_char, c_int, c_uint, size_t};
use super::super::zl_media::ZLMedia;
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

pub type OnProduce = extern "C" fn(*mut ZLMedia, *const u8, size_t);
pub type AllocateFunction = extern "C" fn (size_t) -> *const u8;

extern "C" {
    pub fn zlmedia_new(uri: *const c_char) -> *mut ZLInstance;
    //TODO: pin!!!!!!!!!!!
    pub fn zlmedia_set_parent(zl_media: *mut ZLInstance, parent: Box<ZLMedia>);
    pub fn zlmedia_set_on_produce(zl_media: *mut ZLInstance, on_produce: OnProduce);
    pub fn zlmedia_set_transport(zl_media: *mut ZLInstance, transport: c_int);
    pub fn zlmedia_init(zl_media: *mut ZLInstance);
    pub fn zlmedia_run(zl_media: *mut ZLInstance);
    pub fn zlmedia_destroy(zl_media: *mut ZLInstance);
}

//callbacks:
#[no_mangle]
pub fn zlmedia_socket_receive(zl_media: *mut ZLMedia, data: *mut u8, size: *mut size_t) -> u8 {
    unsafe{<*mut ZLMedia>::as_ref(zl_media)}.unwrap().socket_receive(data, size)
}
#[no_mangle]
pub fn zlmedia_socket_receive_allocate(zl_media: *mut ZLMedia, data: *mut u8, size: *mut size_t, allocate_function: AllocateFunction) -> u8 {
    unsafe{<*mut ZLMedia>::as_ref(zl_media)}.unwrap().socket_receive_allocate(data, size, allocate_function)
}
#[no_mangle]
pub fn zlmedia_socket_send(zl_media: *mut ZLMedia, data: *mut u8, size: size_t) -> u8 {
    unsafe{<*mut ZLMedia>::as_ref(zl_media)}.unwrap().socket_send(data, size)
}
#[no_mangle]
pub fn zlmedia_socket_connect(zl_media: *mut ZLMedia, address: *mut u8, local_port: u16, port: u16) -> u8 {
    unsafe{<*mut ZLMedia>::as_ref(zl_media)}.unwrap().socket_connect(address, local_port, port)
}
#[no_mangle]
pub fn zlmedia_socket_may_send(zl_media: *mut ZLMedia) -> bool {
    unsafe{<*mut ZLMedia>::as_ref(zl_media)}.unwrap().socket_may_send()
}

/*
    u8 socket_receive(ZLInstance *socketInstance, u8 *data, size_t *size);
    u8 *cpp_allocate_buffer_zero_terminated(size_t size);
    u8 socket_receive_allocate(ZLInstance *socketInstance, u8 *data, size_t *size, AllocateFunction allocateFunction);
    u8 socket_send(ZLInstance *socketInstance, u8 *data, size_t size);
    u8 socket_connect(ZLInstance *socketInstance, const u8 *address, u16 localPort, u16 port);
    bool socket_may_send(ZLInstance *socketInstance);
*/