#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
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

/*
   template <typename T>
struct Entry {
  T value;
  size_t count;
};

template <typename T>
class DenseVec {
 public:
  DenseVec() {}

  std::pair<size_t, Entry<T>&> entryWithIndexAt(size_t i) {
    size_t dIndex = 0;
    while (i != 0) {
      auto& entry = data_.at(dIndex);
      if (entry.count >= i) {
        return std::make_pair(dIndex, entry);
      }
    }
  }

  const Entry<T>& entryAt(size_t i) const {
    return entryWithIndexAt(i).second;
  }

  Entry<T>& entryAt(size_t i) {
    return const_cast<Entry<T>&>(const_cast<const
DenseVec<T>&>(*this).entryAt(i));
  }

 private:
  std::vector<Entry<T>> data_;
};

class Polymer {
 public:
  Polymer(std::string s) {
    auto seed = s[0];
    densePack_.push_back(std::make_pair(seed, 1));
    for (size_t i = 1; i < s.size(); ++i) {
      insertAfter(s.at(i), i-1);
    }
  }

  size_t size() const {
    return size_;
  }

  char at(size_t i) const { return entry(i).first; }

  char at(size_t i) { return const_cast<const Polymer&>(*this).at(i); }

  void insertAfter(char c, size_t i) {
    auto [packIdx, p] = entry(i);
    if (p.first == c) {
      ++p.second;
    } else if (i < numEntries()) {
      auto& p2 = densePack_.at(packIdx + 1);
      if (p2.first == c) {
        ++p2.second;
      } else {
        // Add new pack
        densePack_.insert(densePack_.begin() + packIdx + 1, std::make_pair(c,
1));
      }
    } else {
      // Add new pack entry
      densePack_.insert(densePack_.begin() + packIdx + 1, std::make_pair(c, 1));
    }
    ++size_;
  }

 private:
  std::pair<int64_t, std::pair<char, int64_t>> entry(size_t i) const {
    size_t packIdx = 0;
    while (i > 0) {
      auto& entry = densePack_.at(packIdx);
      if (entry.second > i) {
        return std::make_pair(packIdx, entry);
      }
      i -= entry.second;
      ++packIdx;
    }
    throw std::out_of_range{"entry"};
  }

  size_t numEntries() { return densePack_.size(); }

  std::pair<int64_t, std::pair<char, int64_t>> entry(size_t i) {
    return const_cast<const Polymer&>(*this).entry(i);
  }
  std::vector<std::pair<char, int64_t>> densePack_;
  size_t size_ = 0;
};

*/

class Rule {
 public:
  Rule(const std::string& line): input_{line.at(0), line.at(1)}, output_{line.at(line.size() - 1)} {}

  const std::pair<char, char>& getInput() const {
    return input_;
  }

  char getOutput() const {
    return output_;
  }

 private:
  std::pair<char, char> input_;
  char output_;
};

class Polymer {
 public:
  Polymer() {}
  Polymer(const std::string& s) {
    for (size_t i = 0; i < s.size(); ++i) {
      char thisChar = s.at(i);
      if (i + 1 < s.size()) {
        char nextChar = s.at(i + 1);
        ++pairCounts_[std::make_pair(thisChar, nextChar)];
      }
      ++charCounts_[thisChar];
    }
  }

  void applyRules(const std::vector<Rule>& rules) {
    // Make a copy of our map as we apply all rules
    std::map<std::pair<char, char>, size_t> pairCopy = pairCounts_;

    for (const auto& r : rules) {
      auto [first, second] = r.getInput();
      char out = r.getOutput();
      // Find how many times the rule's pair occurs
      auto numMatching = pairCounts_[r.getInput()];

      // Replace [ab] with [acb] which gives us new pairs [ac] and [cb]
      pairCopy[std::make_pair(first, out)] += numMatching;
      pairCopy[std::make_pair(out, second)] += numMatching;
      // All of the input pairs are also gone since we broke them up
      // NOTE: Can't set = 0 because another rule may be concurrently adding
      // this pair *back* into the set
      pairCopy[r.getInput()] -= numMatching;

      // We added the output `numMatching` times
      // It's fine to update this in-place since it doesn't affect pairs
      charCounts_[out] += numMatching;
    }

    // Set our pairCounts_ to the copied + updated pairCopy
    pairCounts_ = std::move(pairCopy);
  }

  std::pair<char, size_t> mostCommon() const {
    std::pair<char, size_t> entry{'?', 0};
    for (const auto& [c, count] : charCounts_) {
      if (count > entry.second) {
        entry.first = c;
        entry.second = count;
      }
    }
    return entry;
  }

  std::pair<char, size_t> leastCommon() const {
    std::pair<char, size_t> entry{'?', std::numeric_limits<size_t>::max()};
    for (const auto& [c, count] : charCounts_) {
      if (count < entry.second) {
        entry.first = c;
        entry.second = count;
      }
    }
    return entry;
  }

 private:
  std::map<char, size_t> charCounts_;
  std::map<std::pair<char, char>, size_t> pairCounts_;
};


int64_t solve1(std::vector<std::string> lines) {
  Polymer p{lines.at(0)};
  std::vector<Rule> rules;
  for (size_t i = 2; i < lines.size(); ++i) {
    rules.emplace_back(lines.at(i));
  }
  for (int64_t i = 0; i < 10; ++i) {
    p.applyRules(rules);
  }

  auto mostCommon = p.mostCommon();
  auto leastCommon = p.leastCommon();
  return mostCommon.second - leastCommon.second;
}

int64_t solve2(std::vector<std::string> lines) {
  Polymer p{lines.at(0)};
  std::vector<Rule> rules;
  for (size_t i = 2; i < lines.size(); ++i) {
    rules.emplace_back(lines.at(i));
  }
  for (int64_t i = 0; i < 40; ++i) {
    p.applyRules(rules);
  }

  auto mostCommon = p.mostCommon();
  auto leastCommon = p.leastCommon();
  return mostCommon.second - leastCommon.second;
}

int main(int argc, char** argv) {
  auto sampleLines = readLines("sample.txt");
  expect(solve1(sampleLines), 1588);
  std::cout << "Passed sample 1\n";

  expect(solve2(sampleLines), 2188189693529);
  std::cout << "Passed sample 2\n";

  auto lines = readLines("input.txt");
  std::cout << "Problem 1: " << solve1(lines) << '\n';
  std::cout << "Problem 2: " << solve2(lines) << '\n';

  return 0;
}
