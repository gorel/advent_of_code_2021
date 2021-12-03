#include <iostream>



int main(int argc, char** argv) {
  int64_t n;
  int64_t last;
  int64_t count = 0;

  std::cin >> last;

  while (std::cin >> n) {
    if (n > last) {
      ++count;
    }
    last = n;
  }

  std::cout << count << '\n';
  return 0;
}
