extern crate cmake;
use cmake::Config;

fn main()
{
    let dst = Config::new("zlmediakit_lib").define("ENABLE_OPENSSL", "OFF").build();       

    println!("cargo:rustc-link-search=native={}", dst.display());
    println!("cargo:rustc-link-lib=static=zlmediakit_cpp_interface"); 
    println!("cargo:rustc-link-lib=static=zlmediakit");
    println!("cargo:rustc-link-lib=static=zltoolkit");
    println!("cargo:rustc-link-lib=static=mov");
    println!("cargo:rustc-link-lib=static=mpeg");
    println!("cargo:rustc-link-lib=static=flv");
    println!("cargo:rustc-link-lib=dylib=stdc++"); 
}