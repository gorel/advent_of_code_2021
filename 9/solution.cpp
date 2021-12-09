#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <utility>
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

class Graph {
 public:
  Graph(std::vector<std::string>& lines)
      : rows_{static_cast<int64_t>(lines.size())},
        cols_{static_cast<int64_t>(lines.at(0).size())},
        data_{lines} {}

  int64_t rowCount() const { return rows_; }

  int64_t colCount() const { return cols_; }

  bool inBounds(int64_t row, int64_t col) const {
    return row >= 0 && row < rows_ && col >= 0 && col < cols_;
  }

  int64_t at(int64_t row, int64_t col) const {
    if (!inBounds(row, col)) {
      return std::numeric_limits<int64_t>::max();
    }
    return data_.at(row).at(col);
  }

  int64_t riskLevel(int64_t row, int64_t col) const {
    return 1 + (at(row, col) - '0');
  }

  bool isMinimum(int64_t row, int64_t col) const {
    std::vector<std::pair<int64_t, int64_t>> dirs{
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    auto thisValue = at(row, col);
    for (auto [rDelta, cDelta] : dirs) {
      if (at(row + rDelta, col + cDelta) <= thisValue) {
        return false;
      }
    }
    return true;
  }

  int64_t flood(int64_t row, int64_t col) {
    int64_t count = 0;
    data_[row][col] = '9';
    std::vector<std::pair<int64_t, int64_t>> dirs{
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    std::queue<std::pair<int64_t, int64_t>> q;
    q.push(std::make_pair(row, col));
    while (!q.empty()) {
      ++count;
      auto [r, c] = q.front();
      q.pop();
      for (const auto& [rDelta, cDelta] : dirs) {
        auto newR = r + rDelta;
        auto newC = c + cDelta;
        if (inBounds(newR, newC) && at(newR, newC) != '9') {
          data_[newR][newC] = '9';
          q.push(std::make_pair(newR, newC));
        }
      }
    }

    return count;
  }

 private:
  int64_t rows_;
  int64_t cols_;
  std::vector<std::string>& data_;
};

int64_t solve1(std::vector<std::string> lines) {
  int64_t risk = 0;
  Graph g{lines};
  for (int64_t r = 0; r < g.rowCount(); ++r) {
    for (int64_t c = 0; c < g.colCount(); ++c) {
      if (g.isMinimum(r, c)) {
        risk += g.riskLevel(r, c);
      }
    }
  }
  return risk;
}

int64_t solve2(std::vector<std::string> lines) {
  std::vector<std::pair<int64_t, int64_t>> basins;
  std::vector<int64_t> basinSizes;
  Graph g{lines};
  for (int64_t r = 0; r < g.rowCount(); ++r) {
    for (int64_t c = 0; c < g.colCount(); ++c) {
      if (g.isMinimum(r, c)) {
        basins.emplace_back(r, c);
      }
    }
  }

  for (const auto& [row, col] : basins) {
    basinSizes.push_back(g.flood(row, col));
  }

  std::sort(basinSizes.begin(), basinSizes.end());
  int64_t res = 1;
  for (auto i = basinSizes.size() - 3; i < basinSizes.size(); ++i) {
    res *= basinSizes.at(i);
  }

  return res;
}

int main(int argc, char** argv) {
  auto sampleLines = readLines("sample.txt");
  expect(solve1(sampleLines), 15);
  std::cout << "Passed sample 1\n";

  expect(solve2(sampleLines), 1134);
  std::cout << "Passed sample 2\n";

  auto lines = readLines("input.txt");
  std::cout << "Problem 1: " << solve1(lines) << '\n';
  std::cout << "Problem 2: " << solve2(lines) << '\n';

  return 0;
}
