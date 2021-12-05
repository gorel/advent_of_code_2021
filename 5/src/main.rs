extern {
    fn doEverything();
}

fn main() {
    unsafe { doEverything(); }
    println!("Wow, Rust *is* easy!");
}
