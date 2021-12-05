extern crate cc;

fn main() {
    cc::Build::new()
        .file("src/day_5.cpp")
        .cpp(true)
        .flag("-std=c++17")
        .compile("libsolution.a");
}
