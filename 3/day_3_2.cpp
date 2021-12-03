#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int64_t getIntFromBitVec(const std::vector<int64_t>& v) {
  int64_t res = 0;
  for (auto val : v) {
    auto bit = val > 0 ? 1 : 0;
    res = res << 1 | bit;
  }
  return res;
}

int main(int argc, char** argv) {
  std::vector<std::string> linesSaved;
  std::vector<int64_t> gammaRate;

  std::string line;
  // Read first line to setup pairs
  while (std::cin >> line) {
    linesSaved.push_back(line);
  }

  std::vector<std::pair<std::string, std::function<bool(int, int)>>>
      criteria{
          {std::string{"Oxygen"}, [](auto a, auto b) { return a == b; }},
          {std::string{"CO2"}, [](auto a, auto b) { return a != b; }}};

  for (const auto& [key, func] : criteria) {
    std::vector<std::string> lines = linesSaved;
    for (std::size_t bitIdx = 0;
         lines.size() > 1 && bitIdx < lines.at(0).size(); ++bitIdx) {
      std::cout << "bitIdx[" << bitIdx << "], size=" << lines.size() << '\n';
      int64_t value = 0;
      for (std::size_t lineIdx = 0; lineIdx < lines.size(); ++lineIdx) {
        if (lines.at(lineIdx).at(bitIdx) == '1') {
          ++value;
        } else {
          --value;
        }
      }

      int64_t mostCommonBit;
      if (value > 0) {
        mostCommonBit = 1;
      } else if (value < 0) {
        mostCommonBit = 0;
      } else {
        // Hacky, but since we do a comparison with `func` below, we *always*
        // compare against 1 (this gives us zero for the CO2 check)
        mostCommonBit = 1;
      }

      std::vector<std::string> newLines;
      for (const auto& line : lines) {
        if (func(line.at(bitIdx) - '0', mostCommonBit)) {
          newLines.push_back(line);
        }
      }
      lines = std::move(newLines);
    }

    auto resStr = lines.at(0);
    std::vector<int64_t> resVec;
    for (auto c : resStr) {
      resVec.push_back(c - '0');
    }

    int64_t asInt = getIntFromBitVec(resVec);
    std::cout << "Calculated " << key << " => " << resStr << " (" << asInt
              << ")\n";
  }

  return 0;
}
