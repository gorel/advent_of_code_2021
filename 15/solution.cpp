#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using pii = std::pair<int64_t, int64_t>;

namespace std {
template <>
struct hash<pii> {
  size_t operator()(const pii& v) const {
    return v.first * 31 + v.second;
  }
};
} // namespace std

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

std::vector<std::string> split(const std::string& s, char delim) {
  std::vector<std::string> res;
  std::stringstream ss{s};
  while (ss.good()) {
    std::string val;
    getline(ss, val, delim);
    res.push_back(std::move(val));
  }
  return res;
}

std::vector<std::string> wrap(const std::vector<std::string>& lines) {
  // Repeat 5 times
  std::vector<std::string> res;
  // rows
  size_t outerBumps = 0;
  for (size_t i = 0; i < 5; ++i) {
    // col repeats
    std::string lineOut;
    for (const auto& line : lines) {
      size_t innerBumps = 0;
      for (size_t j = 0; j < 5; ++j) {
        for (char c : line) {
          char out = c + outerBumps + innerBumps;
          while (out > '9') {
            out -= '9' - '0';
          }
          lineOut += out;
        }
        ++innerBumps;
      }
      res.push_back(std::move(lineOut));
    }
    ++outerBumps;
  }
  return res;
}

class Graph {
 public:
  Graph(const std::vector<std::string>& lines)
      : rowC_{lines.size()}, colC_{lines.at(0).size()} {
    for (const auto& line : lines) {
      for (char c : line) {
        riskLevels_.push_back(c - '0');
      }
    }
  }

  friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
    std::cout << "rows = " << g.rows() << ", cols = " << g.cols() << '\n';
    for (size_t r = 0; r < g.rows(); ++r) {
      for (size_t c = 0; c < g.cols(); ++c) {
        os << static_cast<char>('0' + g.at(std::make_pair(r, c)));
      }
      os << '\n';
    }
    return os;
  }

  size_t rows() const { return rowC_; }

  size_t cols() const { return colC_; }

  bool valid(const pii& p) const {
    const auto& [r, c] = p;
    return r >= 0 && r < rowC_ && c >= 0 && c < colC_;
  }

  int8_t at(const pii& p) const {
    auto& [r, c] = p;
    return riskLevels_.at(r * colC_ + c);
  }

  int64_t dijkstra(const pii& start, const pii& end) const {
    std::vector<pii> dirs{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    std::map<pii, int64_t> dist;
    std::priority_queue<std::pair<int64_t, pii>> q;

    for (size_t i = 0; i < rowC_; ++i) {
      for (size_t j = 0; j < colC_; ++j) {
        auto p = std::make_pair(i, j);
        dist[p] = std::numeric_limits<int64_t>::max();
      }
    }

    q.push(std::make_pair(0, start));
    dist[start] = 0;

    while (!q.empty()) {
      auto [distance, p] = q.top();
      auto& [thisR, thisC] = p;
      q.pop();

      for (const auto& [r, c] : dirs) {
        auto newPoint = std::make_pair(thisR + r, thisC + c);
        if (valid(newPoint)) {
          auto alt = dist[p] + at(newPoint);
          if (alt < dist[newPoint]) {
            dist[newPoint] = alt;
            // Why negative? We're lazy and using default priority_queue but we
            // need to invert the check (could also use std::greater in ctr)
            q.push(std::make_pair(-alt, newPoint));
          }
        }
      }
    }

    return dist[end];
  }

 private:
  size_t rowC_;
  size_t colC_;
  std::vector<int8_t> riskLevels_;
};

int64_t solve1(std::vector<std::string> lines) {
  Graph g{lines};
  auto start = std::make_pair(0, 0);
  auto end = std::make_pair(g.rows() - 1, g.cols() - 1);
  return g.dijkstra(start, end);
}

int64_t solve2(std::vector<std::string> lines) {
  Graph g{wrap(lines)};
  auto start = std::make_pair(0, 0);
  auto end = std::make_pair(g.rows() - 1, g.cols() - 1);
  return g.dijkstra(start, end);
}

int main(int argc, char** argv) {
  auto sampleLines = readLines("sample.txt");
  expect(solve1(sampleLines), 40);
  std::cout << "Passed sample 1\n";

  expect(solve2(sampleLines), 315);
  std::cout << "Passed sample 2\n";

  auto lines = readLines("input.txt");
  std::cout << "Problem 1: " << solve1(lines) << '\n';
  std::cout << "Problem 2: " << solve2(lines) << '\n';

  return 0;
}
