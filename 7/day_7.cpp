#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct CrabInfo {
  int64_t pos;
  int64_t count;

  int64_t totalDistanceTo(int64_t newPos) {
    auto n = std::abs(newPos - pos);
    return count * n;
  }

  int64_t totalDistanceTwo(int64_t newPos) {
    auto n = std::abs(newPos - pos);
    auto cost = n * (n + 1) / 2;
    return count * cost;
  }
};

template <typename T>
std::vector<T> split(const std::string& s) {
  std::vector<T> res;
  std::stringstream ss{s};
  T val;
  while (ss >> val) {
    res.push_back(val);
    ss.ignore();
  }
  return res;
}

int main(int argc, char** argv) {
  std::string line;
  getline(std::cin, line);
  std::unordered_map<int64_t, CrabInfo> m;

  int64_t maxPos = 0;
  for (auto val : split<int64_t>(line)) {
    if (m.count(val) == 0) {
      m[val] = CrabInfo{val, 0};
    }
    ++m[val].count;
    maxPos = val > maxPos ? val : maxPos;
  }

  int64_t problem1 = std::numeric_limits<int64_t>::max();
  int64_t problem2 = std::numeric_limits<int64_t>::max();
  for (auto i = 0; i <= maxPos; ++i) {
    // Move everything to position i
    int64_t thisMovement = 0;
    int64_t thisMovement2 = 0;
    for (auto j = 0; j <= maxPos; ++j) {
      if (m.count(j) == 0) {
        m[j] = CrabInfo{j, 0};
      }
      thisMovement += m[j].totalDistanceTo(i);
      thisMovement2 += m[j].totalDistanceTwo(i);
    }
    problem1 = thisMovement < problem1 ? thisMovement : problem1;
    problem2 = thisMovement2 < problem2 ? thisMovement2 : problem2;
  }

  std::cout << "Problem 1: " << problem1 << '\n';
  std::cout << "Problem 2: " << problem2 << '\n';

  return 0;
}
