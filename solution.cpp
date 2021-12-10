#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void expect(int64_t actual, int64_t expected) {
  if (actual != expected) {
    std::cerr << "expect failed; actual = " << actual
              << ", expected = " << expected << '\n';
    std::exit(1);
  }
}

std::vector<std::string> readLines(std::string fileName) {
  std::vector<std::string> lines;
  std::ifstream file{fileName};
  std::string line;
  while (std::getline(file, line)) {
    lines.push_back(std::move(line));
  }
  return lines;
}

int64_t solve1(std::vector<std::string> lines) {
  return 0;
}

int64_t solve2(std::vector<std::string> lines) {
  return 0;
}

int main(int argc, char** argv) {
  auto sampleLines = readLines("sample.txt");
  expect(solve1(sampleLines), 0);
  std::cout << "Passed sample 1\n";

  expect(solve2(sampleLines), 0);
  std::cout << "Passed sample 2\n";

  auto lines = readLines("input.txt");
  std::cout << "Problem 1: " << solve1(lines) << '\n';
  std::cout << "Problem 2: " << solve2(lines) << '\n';

  return 0;
}
