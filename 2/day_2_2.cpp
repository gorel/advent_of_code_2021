#include <iostream>
#include <string>

int main(int argc, char** argv) {
  int64_t xpos = 0;
  int64_t depth = 0;
  int64_t aim = 0;

  std::string dir;
  int64_t n;
  while (std::cin >> dir >> n) {
    if (dir == "forward") {
      xpos += n;
      depth += aim * n;
    } else if (dir == "down") {
      aim += n;
    } else { // up
      aim -= n;
    }
  }

  std::cout << "xpos: " << xpos << ", depth: " << depth << '\n';
  std::cout << xpos * depth << '\n';
  return 0;
}
