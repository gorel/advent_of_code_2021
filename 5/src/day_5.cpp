#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Point {
  int64_t x;
  int64_t y;

  Point() : x{0}, y{0} {}
  Point(int64_t xx, int64_t yy) : x{xx}, y{yy} {}
  Point& operator=(const Point& p) {
    x = p.x;
    y = p.y;
    return *this;
  }

  static std::pair<Point, Point> readFrom(const std::string& s) {
    std::stringstream ss{s};
    Point p1, p2;
    ss >> p1.x;
    ss.ignore();  // ,
    ss >> p1.y;
    ss.ignore();  // -
    ss.ignore();  // >
    ss.ignore();  // ' '
    ss >> p2.x;
    ss.ignore();  // ,
    ss >> p2.y;
    return std::make_pair(std::move(p1), std::move(p2));
  }

  friend bool operator==(const Point& p1, const Point& p2) {
    return p1.x == p2.x && p1.y == p2.y;
  }

  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << '[' << p.x << ", " << p.y << ']';
    return os;
  }
};

namespace std {
template <>
struct hash<Point> {
  size_t operator()(const Point& p) const {
    return static_cast<size_t>(p.x << 4 | p.y);
  }
};
}  // namespace std

class Line {
 public:
  Line(Point& a, Point& b) {
    isStandard_ = a.x == b.x || a.y == b.y;
    if (a.x == b.x) {
      auto start = std::min(a.y, b.y);
      auto end = std::max(a.y, b.y);
      for (int64_t i = start; i <= end; ++i) {
        points_.emplace_back(a.x, i);
      }
    } else if (a.y == b.y) {
      auto start = std::min(a.x, b.x);
      auto end = std::max(a.x, b.x);
      for (int64_t i = start; i <= end; ++i) {
        points_.emplace_back(i, a.y);
      }
    } else {
      if (a.x > b.x) {
        Point tmp = a;
        a = b;
        b = tmp;
      }

      if (a.y < b.y) {
        // top down
        // We're guaranteed a perfectly diagonal line, so we only need to check
        // one condition for termination in our loop
        for (int64_t x = a.x, y = a.y; x <= b.x; ++x, ++y) {
          points_.emplace_back(x, y);
        }
      } else {
        // bottom up
        // We're guaranteed a perfectly diagonal line, so we only need to check
        // one condition for termination in our loop
        for (int64_t x = a.x, y = a.y; x <= b.x; ++x, --y) {
          points_.emplace_back(x, y);
        }
      }
    }
  }

  const std::vector<Point> getPoints() const { return points_; }

  bool isStandard() const { return isStandard_; }

  friend std::ostream& operator<<(std::ostream& os, const Line& line) {
    os << line.points_[0] << " -> " << line.points_[line.points_.size() - 1];
    return os;
  }

 private:
  std::vector<Point> points_;
  bool isStandard_;
};

class Graph {
 public:
  void addLine(const Line& line) {
    for (const auto& p : line.getPoints()) {
      if (line.isStandard()) {
        ++pCounts_[p];
      }
      ++pCounts2_[p];
    }
  }

  int64_t solve1() const {
    int64_t res = 0;
    for (const auto& [k, v] : pCounts_) {
      if (v > 1) {
        ++res;
      }
    }
    return res;
  }

  int64_t solve2() const {
    int64_t res = 0;
    for (const auto& [k, v] : pCounts2_) {
      if (v > 1) {
        ++res;
      }
    }
    return res;
  }

 private:
  std::unordered_map<Point, int64_t> pCounts_;
  std::unordered_map<Point, int64_t> pCounts2_;
};

extern "C" void doEverything() {
  Graph g;
  std::string line;
  int64_t lines = 0;
  while (getline(std::cin, line)) {
    auto [p1, p2] = Point::readFrom(line);

    g.addLine(Line(p1, p2));
    ++lines;
    if (lines % 100 == 0) {
    }
  }

  std::cout << "Problem 1: " << g.solve1() << '\n';
  std::cout << "Problem 2: " << g.solve2() << '\n';
}
