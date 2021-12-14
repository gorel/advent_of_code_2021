#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
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

template <typename T>
T into(const std::string& s) {
  std::stringstream ss{s};
  T val;
  ss >> val;
  return val;
}

struct Fold {
  char axis;
  int64_t idx;
};

struct Point {
  int64_t x;
  int64_t y;

  Point fold(const Fold& f) const {
    if (f.axis == 'x' && x > f.idx) {
      // Fold left
      return Point{2 * f.idx - x, y};
    } else if (f.axis == 'y' && y > f.idx) {
      // Fold up
      return Point{x, 2 * f.idx - y};
    } else {
      return Point{x, y};
    }
  }

  friend bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
  }
};

namespace std {
template <>
struct hash<Point> {
  size_t operator()(const Point& p) const {
    return p.x << 4 | p.y;
  }
};
} // namespace std

int64_t solve1(std::vector<std::string> lines) {
  std::unordered_set<Point> points;
  std::vector<Fold> folds;

  size_t idx;
  for (idx = 0; idx < lines.size(); ++idx) {
    if (lines[idx].find(',') != std::string::npos) {
      auto parts = split(lines[idx], ',');
      auto x = into<int64_t>(parts[0]);
      auto y = into<int64_t>(parts[1]);
      points.insert(Point{x, y});
    } else {
      break;
    }
  }

  // Advance idx one past the newline
  for (idx += 1; idx < lines.size(); ++idx) {
    auto parts = split(lines[idx], ' ');
    auto fold = split(parts[2], '=');
    char axis = fold[0][0];
    int64_t val = into<int64_t>(fold[1]);
    folds.push_back(Fold{axis, val});
  }

  std::unordered_set<Point> newPoints;
  for (auto& p : points) {
    newPoints.insert(p.fold(folds[0]));
  }
  points = std::move(newPoints);
  return points.size();
}

void solve2(std::vector<std::string> lines) {
  std::unordered_set<Point> points;
  std::vector<Fold> folds;

  size_t idx;
  for (idx = 0; idx < lines.size(); ++idx) {
    if (lines[idx].find(',') != std::string::npos) {
      auto parts = split(lines[idx], ',');
      auto x = into<int64_t>(parts[0]);
      auto y = into<int64_t>(parts[1]);
      points.insert(Point{x, y});
    } else {
      break;
    }
  }

  // Advance idx one past the newline
  for (idx += 1; idx < lines.size(); ++idx) {
    auto parts = split(lines[idx], ' ');
    auto fold = split(parts[2], '=');
    char axis = fold[0][0];
    int64_t val = into<int64_t>(fold[1]);
    folds.push_back(Fold{axis, val});
  }

  for (const auto& f : folds) {
    std::unordered_set<Point> newPoints;
    for (auto& p : points) {
      newPoints.insert(p.fold(f));
    }
    points = std::move(newPoints);
  }

  size_t maxX = 0;
  size_t maxY = 0;
  for (const auto& p : points) {
    maxX = p.x > maxX ? p.x : maxX;
    maxY = p.y > maxY ? p.y : maxY;
  }

  for (size_t y = 0; y <= maxY; ++y) {
    for (size_t x = 0; x <= maxX; ++x) {
      Point p{static_cast<int64_t>(x), static_cast<int64_t>(y)};
      if (points.find(p) == points.end()) {
        std::cout << ' ';
      } else {
        std::cout << '.';
      }
    }
    std::cout << '\n';
  }
}

int main(int argc, char** argv) {
  auto sampleLines = readLines("sample.txt");
  expect(solve1(sampleLines), 17);
  std::cout << "Passed sample 1\n";

  auto lines = readLines("input.txt");
  std::cout << "Problem 1: " << solve1(lines) << '\n';
  solve2(lines);

  return 0;
}
