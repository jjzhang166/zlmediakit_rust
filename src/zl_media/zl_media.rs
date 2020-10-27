use crate::interface::interface::*;
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
