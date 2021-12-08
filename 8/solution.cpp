#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
  aaaa
 b    c
 b    c
  dddd
 e    f
 e    f
  gggg

  aaaa
 b
 b

 e    f
 e    f
  gggg
*/

std::unordered_map<char, std::unordered_set<char>> kDigits{
    {0, {'a', 'b', 'c', 'e', 'f', 'g'}},
    {1, {'c', 'f'}},
    {2, {'a', 'c', 'd', 'e', 'g'}},
    {3, {'a', 'c', 'd', 'f', 'g'}},
    {4, {'b', 'c', 'd', 'f'}},
    {5, {'a', 'b', 'd', 'f', 'g'}},
    {6, {'a', 'b', 'd', 'e', 'f', 'g'}},
    {7, {'a', 'c', 'f'}},
    {8, {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
    {9, {'a', 'b', 'c', 'd', 'f', 'g'}}};

// Trim functions taken from https://stackoverflow.com/a/217605
// because I'm lazy and don't want to parse properly
// trim from start (in place)
static inline void ltrim(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
  ltrim(s);
  rtrim(s);
}

template <typename T>
std::vector<T> split(const std::string& line, char delim) {
  std::vector<T> res;
  std::stringstream ss{line};
  while (ss.good()) {
    std::string s;
    getline(ss, s, delim);
    trim(s);
    res.emplace_back(s);
  }
  return res;
}

template <typename It>
std::string join(It begin, It end, char delim) {
  std::string res;
  bool first = true;
  while (begin != end) {
    if (!first) {
      res += delim;
    }
    res += *begin++;
    first = false;
  }
  return res;
}

template <typename K, typename V>
std::string mapToString(const std::unordered_map<K, V>& m) {
  std::string res = "{";
  bool first = true;
  for (const auto& [k, v] : m) {
    if (!first) {
      res += ',';
    }
    res += k + ": " + v;
    first = false;
  }
  res += "}";
  return res;
}

std::string anti(const std::string& signal) {
  std::string s;
  for (char c = 'a'; c <= 'g'; ++c) {
    if (signal.find(c) == std::string::npos) {
      s += c;
    }
  }
  return s;
}

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

void markSolved(
    std::unordered_map<char, std::unordered_set<char>>& possibilities, char c,
    std::unordered_set<char>& solved) {
  if (solved.find(c) != solved.end()) {
    return;
  }

  solved.insert(c);
  auto solution = *possibilities[c].begin();
  for (auto& [k, v] : possibilities) {
    if (k != c) {
      v.erase(solution);
      if (v.size() == 1) {
        markSolved(possibilities, k, solved);
      }
    }
  }
}

void intersect(
    std::unordered_map<char, std::unordered_set<char>>& possibilities, char c,
    const std::string& signal, std::unordered_set<char>& solved) {
  std::unordered_set<char> intersect;
  for (auto element : signal) {
    if (possibilities[c].count(element) > 0) {
      intersect.insert(element);
    }
  }

  possibilities[c] = intersect;
  if (possibilities[c].size() == 1 && solved.find(c) == solved.end()) {
    markSolved(possibilities, c, solved);
  }
}

int64_t makeNumber(const std::unordered_map<char, char>& solution,
                   const std::vector<std::string>& parts) {
  int64_t res = 0;
  for (const auto& numStr : parts) {
    res *= 10;

    std::unordered_set<char> realParts;
    for (auto c : numStr) {
      realParts.insert(solution.at(c));
    }

    bool found = false;
    for (const auto& [k, v] : kDigits) {
      if (realParts == v) {
        // Found the right digit
        res += k;
        found = true;
        break;
      }
    }

    if (!found) {
      std::cout << "Could not find solution for " << numStr << '\n';
      std::exit(1);
    }
  }
  return res;
}

int64_t solve1(std::vector<std::string> lines) {
  int64_t count = 0;
  for (const auto& line : lines) {
    auto parts = split<std::string>(line, '|');
    auto signals = split<std::string>(parts[0], ' ');
    auto outputDigits = split<std::string>(parts[1], ' ');
    for (const auto& digit : outputDigits) {
      auto sz = digit.size();
      if (sz == 2 || sz == 4 || sz == 3 || sz == 7) {
        ++count;
      }
    }
  }
  return count;
}

int64_t solve2(std::vector<std::string> lines) {
  /**
    aaaa
   b    c
   b    c
    dddd
   e    f
   e    f
    gggg
  */

  int64_t sum = 0;
  for (const auto& line : lines) {
    std::unordered_map<char, std::unordered_set<char>> possibilities{
        {'a', {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
        {'b', {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
        {'c', {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
        {'d', {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
        {'e', {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
        {'f', {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
        {'g', {'a', 'b', 'c', 'd', 'e', 'f', 'g'}}};
    std::unordered_set<char> solved;

    auto parts = split<std::string>(line, '|');
    auto signals = split<std::string>(parts[0], ' ');
    auto outputDigits = split<std::string>(parts[1], ' ');
    for (const auto& signal : signals) {
      auto sz = signal.size();
      auto antisignal = anti(signal);
      if (sz == 2) {
        // This is a 1
        intersect(possibilities, 'c', signal, solved);
        intersect(possibilities, 'f', signal, solved);
        // Remove these possibilities from other spots
        intersect(possibilities, 'a', antisignal, solved);
        intersect(possibilities, 'b', antisignal, solved);
        intersect(possibilities, 'd', antisignal, solved);
        intersect(possibilities, 'e', antisignal, solved);
        intersect(possibilities, 'g', antisignal, solved);
      } else if (sz == 4) {
        // This is a 4
        intersect(possibilities, 'b', signal, solved);
        intersect(possibilities, 'c', signal, solved);
        intersect(possibilities, 'd', signal, solved);
        intersect(possibilities, 'f', signal, solved);
      } else if (sz == 3) {
        // This is a 7
        intersect(possibilities, 'a', signal, solved);
        intersect(possibilities, 'c', signal, solved);
        intersect(possibilities, 'f', signal, solved);
      } else if (sz == 7) {
        // This is an 8
      } else if (sz == 5) {
        // This is a 2 or 3 or 5 -> common wires: a, d, g
        intersect(possibilities, 'a', signal, solved);
        intersect(possibilities, 'd', signal, solved);
        intersect(possibilities, 'g', signal, solved);
        // No common antiwires
      } else if (sz == 6) {
        // This is a 0 or 6 or 9 -> common wires: a, b, f, g
        intersect(possibilities, 'a', signal, solved);
        intersect(possibilities, 'b', signal, solved);
        intersect(possibilities, 'f', signal, solved);
        intersect(possibilities, 'g', signal, solved);
      }
    }
    std::unordered_map<char, char> solution;
    for (const auto& [k, v] : possibilities) {
      if (v.size() != 1) {
        std::cout << "Ambiguous result at " << line << '\n';
        std::exit(1);
      }

      solution[*v.begin()] = k;
      //solution[k] = *v.begin();
    }

    sum += makeNumber(solution, outputDigits);
  }

  return sum;
}

int main(int argc, char** argv) {
  auto sampleLines = readLines("sample.txt");
  expect(solve1(sampleLines), 26);
  std::cout << "Passed sample 1\n";

  expect(solve2(sampleLines), 61229);
  std::cout << "Passed sample 2\n";

  auto lines = readLines("input.txt");
  std::cout << "Problem 1: " << solve1(lines) << '\n';
  std::cout << "Problem 2: " << solve2(lines) << '\n';

  return 0;
}
