#include <iostream>



int main(int argc, char** argv) {
  int64_t prev, next, n;
  int64_t count = 0;

  int64_t l1, l2, l3;

  // Get first window
  std::cin >> n;
  l3 = n;
  std::cin >> n;
  l2 = n;
  std::cin >> n;
  l1 = n;
  prev = l1 + l2 + l3;

  while (std::cin >> n) {
    int64_t next = prev - l3 + n;
    if (next > prev) {
      ++count;
    }
    prev = next;
    l3 = l2;
    l2 = l1;
    l1 = n;
  }

  std::cout << count << '\n';
  return 0;
}
