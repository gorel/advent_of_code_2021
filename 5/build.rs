extern crate cc;

fn main() {
    cc::Build::new()
        .file("src/day_5.cpp")
        .cpp(true)
        .compile("libsolution.a");
}
