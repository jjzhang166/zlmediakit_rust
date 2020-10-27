use crate::interface::interface::*;
use std::ffi::CString;

pub struct ZLMedia {
    zl_media: *mut ZLInstance,
    url: String
}

impl ZLMedia {
    pub fn new(url: &str) -> ZLMedia {
        let c_url = CString::new(url).expect("CString::new failed");
        ZLMedia {
            zl_media: unsafe{zlmedia_new(c_url.into_raw())},
            url: url.to_string(),
        }
    }
}