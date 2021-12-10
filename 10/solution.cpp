#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
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

template <typename T>
bool in(const T& needle, const std::vector<T> haystack) {
  for (const auto& val : haystack) {
    if (val == needle) {
      return true;
    }
  }
  return false;
}

int64_t solve1(std::vector<std::string> lines) {
  std::vector<char> openers{'(', '[', '{', '<'};
  std::vector<char> closers{')', ']', '}', '>'};
  std::vector<int64_t> scores{3, 57, 1197, 25137};
  int64_t score = 0;

  for (const auto& line : lines) {
    std::stack<char> s;
    for (char c : line) {
      if (in(c, openers)) {
        s.push(c);
      } else if (in(c, closers)) {
        char opener = s.top();
        s.pop();
        auto oIdx =
            std::find(openers.begin(), openers.end(), opener) - openers.begin();
        auto cIdx =
            std::find(closers.begin(), closers.end(), c) - closers.begin();
        if (oIdx != cIdx) {
          score += scores.at(cIdx);
          break;
        }
      }
    }
  }

  return score;
}

int64_t solve2(std::vector<std::string> lines) {
  std::vector<char> openers{'(', '[', '{', '<'};
  std::vector<char> closers{')', ']', '}', '>'};
  std::vector<int64_t> scores{1, 2, 3, 4};
  std::vector<int64_t> allScores;

  for (const auto& line : lines) {
    std::stack<char> s;
    bool corrupted = false;
    for (char c : line) {
      if (in(c, openers)) {
        s.push(c);
      } else if (in(c, closers)) {
        char opener = s.top();
        s.pop();
        auto oIdx =
            std::find(openers.begin(), openers.end(), opener) - openers.begin();
        auto cIdx =
            std::find(closers.begin(), closers.end(), c) - closers.begin();
        if (oIdx != cIdx) {
          corrupted = true;
          break;
        }
      }
    }

    if (!corrupted) {
      int64_t thisScore = 0;
      std::string complete;
      while (!s.empty()) {
        thisScore *= 5;
        char opener = s.top();
        s.pop();
        auto oIdx =
            std::find(openers.begin(), openers.end(), opener) - openers.begin();
        complete += closers[oIdx];
        thisScore += scores[oIdx];
      }
      allScores.push_back(thisScore);
    }
  }

  std::sort(allScores.begin(), allScores.end());
  return allScores.at(allScores.size() / 2);
}

int main(int argc, char** argv) {
  auto sampleLines = readLines("sample.txt");
  expect(solve1(sampleLines), 26397);
  std::cout << "Passed sample 1\n";

  expect(solve2(sampleLines), 288957);
  std::cout << "Passed sample 2\n";

  auto lines = readLines("input.txt");
  std::cout << "Problem 1: " << solve1(lines) << '\n';
  std::cout << "Problem 2: " << solve2(lines) << '\n';

  return 0;
}
