use crate::interface::interface::*;
use libc::size_t;
use std::ffi::CString;

pub struct ZLMedia {
    zl_media: *mut ZLInstance,
    url: String,
}

impl ZLMedia {
    pub fn new(url: &str) -> ZLMedia {
        let c_url = CString::new(url).expect("CString::new failed");
        ZLMedia {
            zl_media: unsafe { zlmedia_new(c_url.into_raw()) },
            url: url.to_string(),
        }
    }

    pub fn set_on_produce(&self, on_produce: OnProduce) {
        unsafe { zlmedia_set_on_produce(self.zl_media, on_produce) }
    }

    pub fn set_transport(&self, transport: Transport) {
        unsafe { zlmedia_set_transport(self.zl_media, transport as libc::c_int) }
    }

    pub fn socket_send(&self, data: *mut u8, size: size_t) -> u8 {
        0
    }

    pub fn socket_connect(&self, address: *mut u8, local_port: u16, port: u16) -> u8 {
        0
    }

    pub fn socket_may_send(&self) -> bool {
        true
    }

    pub fn socket_receive(&self, data: *mut u8, size: *mut size_t) -> u8 {
        0
    }

    pub fn socket_receive_allocate(
        &self, 
        data: *mut u8,
        size: *mut size_t,
        allocate_function: AllocateFunction,
    ) -> u8 {
        0
    }

    pub fn init(&self) {
        unsafe { zlmedia_init(self.zl_media) }
    }

    pub fn run(&self) {
        unsafe { zlmedia_run(self.zl_media) }
    }
}

impl Drop for ZLMedia {
    fn drop(&mut self) {
        unsafe { zlmedia_destroy(self.zl_media) }
    }
}
