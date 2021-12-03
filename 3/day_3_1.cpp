#include <iostream>
#include <string>
#include <vector>

std::vector<int64_t> invert(const std::vector<int64_t>& v) {
  std::vector<int64_t> res;
  for (auto val : v) {
    res.push_back(!val);
  }
  return res;
}

int64_t getIntFromBitVec(const std::vector<int64_t>& v) {
  int64_t res = 0;
  for (auto val : v) {
    auto bit = val > 0 ? 1 : 0;
    res = res << 1 | bit;
  }
  return res;
}

int main(int argc, char** argv) {
  std::vector<int64_t> gammaRate;

  std::string line;
  // Read first line to setup pairs
  while (std::cin >> line) {
    for (std::size_t i = 0; i < line.size(); ++i) {
      if (i >= gammaRate.size()) {
        gammaRate.push_back(0);
      }

      int64_t value = line.at(i) - '0';
      if (value == 0) {
        ++gammaRate.at(i);
      } else {
        --gammaRate.at(i);
      }
    }
  }

  int64_t gamma = getIntFromBitVec(gammaRate);
  int64_t epsilon = getIntFromBitVec(invert(gammaRate));
  std::cout << "gamma = " << gamma << ", epsilon = " << epsilon << '\n';
  std::cout << gamma * epsilon << '\n';

  return 0;
}
