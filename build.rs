extern crate cmake;
use cmake::Config;

fn main()
{
    let dst = Config::new("zlmediakit_lib").build();       

    println!("cargo:rustc-link-search=native={}", dst.display());
    println!("cargo:rustc-link-lib=static=zlmediakit_cpp_interface"); 
    println!("cargo:rustc-link-lib=dylib=stdc++"); 
}