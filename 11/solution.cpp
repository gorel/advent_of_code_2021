#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
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

  int64_t getTotalFlashes() const { return flashed_; }

  int64_t getFirstSync() const { return firstSync_; }

  bool flashing(int64_t row, int64_t col) const {
    return data_.at(row).at(col) > '9';
  }

  void boom(int64_t row, int64_t col,
            std::set<std::pair<int64_t, int64_t>>& flashed) {
    ++flashed_;
    flashed.insert({row, col});
    for (auto [newR, newC] : neighbors(row, col)) {
      ++data_.at(newR).at(newC);
      if (flashed.find({newR, newC}) == flashed.end() && flashing(newR, newC)) {
        boom(newR, newC, flashed);
      }
    }
  }

  void flash() {
    std::set<std::pair<int64_t, int64_t>> flashed;
    for (int64_t i = 0; i < rowCount(); ++i) {
      for (int64_t j = 0; j < colCount(); ++j) {
        ++data_.at(i).at(j);
        if (flashed.find({i, j}) == flashed.end() && flashing(i, j)) {
          boom(i, j, flashed);
        }
      }
    }
    for (int64_t i = 0; i < rowCount(); ++i) {
      for (int64_t j = 0; j < colCount(); ++j) {
        // Now this means "did flash"
        if (flashing(i, j)) {
          data_.at(i).at(j) = '0';
        }
      }
    }
    if (flashes_ == 0) {
    }
    ++flashes_;
    ++iteration_;
    if (flashed.size() == rowCount() * colCount()) {
      firstSync_ = firstSync_ > 0 ? firstSync_ : iteration_;
    }
  }

  std::vector<std::pair<int64_t, int64_t>> neighbors(int64_t row, int64_t col) {
    std::vector<std::pair<int64_t, int64_t>> res;
    std::vector<std::pair<int64_t, int64_t>> dirs{
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
    for (auto [r, c] : dirs) {
      if (inBounds(row + r, col + c)) {
        res.push_back({row + r, col + c});
      }
    }
    return res;
  }

 private:
  int64_t rows_;
  int64_t cols_;
  std::vector<std::string>& data_;
  int64_t flashes_ = 0;
  int64_t flashed_ = 0;
  int64_t firstSync_ = 0;
  int64_t iteration_ = 0;
};

int64_t solve1(std::vector<std::string> lines) {
  Graph g{lines};
  for (int64_t i = 0; i < 100; ++i) {
    g.flash();
  }
  return g.getTotalFlashes();
}

int64_t solve2(std::vector<std::string> lines) {
  Graph g{lines};
  for (int64_t i = 0; i < 1000; ++i) {
    g.flash();
  }
  return g.getFirstSync();
}

int main(int argc, char** argv) {
  auto sampleLines = readLines("sample.txt");
  expect(solve1(sampleLines), 1656);
  std::cout << "Passed sample 1\n";

  expect(solve2(sampleLines), 195);
  std::cout << "Passed sample 2\n";

  auto lines = readLines("input.txt");
  std::cout << "Problem 1: " << solve1(lines) << '\n';
  std::cout << "Problem 2: " << solve2(lines) << '\n';

  return 0;
}
