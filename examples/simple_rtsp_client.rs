//extern crate zlmediakit;
use zlmediakit::zl_media::zl_media::ZLMedia;
use zlmediakit::interface::interface::*;

extern "C" fn on_produce(zl_instance: *mut ZLInstance, data: *const libc::c_uchar, size: libc::size_t) {
    println!("packet produced! size: {}", size);
}

fn main() {
    println!("Simple RTSP client");
    let rtsp_client = ZLMedia::new("rtsp://admin:19929394@192.168.1.198:10554/tcp/av0_0");
    rtsp_client.set_transport(Transport::RtpOverTcp);
    rtsp_client.set_on_produce(on_produce);
    rtsp_client.init();
    rtsp_client.run();
    std::thread::sleep(std::time::Duration::from_secs(1000));
}