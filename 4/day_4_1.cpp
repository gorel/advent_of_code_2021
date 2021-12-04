#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

constexpr int64_t kBoardSize = 5;

std::vector<int64_t> splitToInts(const std::string& s, char delim) {
  std::vector<int64_t> res;
  std::stringstream ss{s};

  int64_t val;
  while (ss >> val) {
    res.push_back(val);
    if (ss.peek() == delim) {
      ss.ignore();
    }
  }

  return res;
}

class Board {
 public:
  static bool readFromStdin(Board& b) {
    for (int64_t row = 0; row < kBoardSize; ++row) {
      std::string line;
      getline(std::cin, line);
      if (line.empty()) {
        return false; // failed to read a line, out of boards
      }
      auto nums = splitToInts(line, ' ');
      for (int64_t col = 0; col < kBoardSize; ++col) {
        b.nums_[row*kBoardSize + col] = nums[col];
      }
    }
    return true;
  }

  /*
   * Marks a board but does not check for bingo.
   * This should be called before 5 nums have been called for minor efficiency
   * gains since we can skip the costly didWin() call.
   */
  void markFast(int64_t num) {
    for (int64_t row = 0; row < kBoardSize; ++row) {
      for (int64_t col = 0; col < kBoardSize; ++col) {
        if (nums_[row*kBoardSize + col] == num) {
          nums_[row*kBoardSize + col] = 0;
          // Assume there won't be duplicate numbers in a bingo board
          break;
        }
      }
    }
  }

  /*
   * Marks a number *and* checks if this board has bingo yet.
   */
  void mark(int64_t num) {
    markFast(num);
    if (didWin()) {
      score_ = num * sumUnmarked();
    }
  }

  int64_t getScore() const {
    return score_;
  }

 private:
  bool didWin() const {
    for (int64_t row = 0; row < kBoardSize; ++row) {
      int64_t rowSum = 0;
      for (int64_t col = 0; col < kBoardSize; ++col) {
        rowSum += nums_[row*kBoardSize + col];
      }
      if (rowSum == 0) {
        return true;
      }
    }
    for (int64_t col = 0; col < kBoardSize; ++col) {
      int64_t colSum = 0;
      for (int64_t row = 0; row < kBoardSize; ++row) {
        colSum += nums_[row*kBoardSize + col];
      }
      if (colSum == 0) {
        return true;
      }
    }
    return false;
  }

  int64_t sumUnmarked() const {
    int64_t sum = 0;
    for (int64_t row = 0; row < kBoardSize; ++row) {
      for (int64_t col = 0; col < kBoardSize; ++col) {
        sum += nums_[row*kBoardSize + col];
      }
    }
    return sum;
  }


  // We're storing the board as a 1D array for memory efficiency
  std::array<int64_t, kBoardSize * kBoardSize> nums_;
  int64_t score_ = -1;
};

int main(int argc, char** argv) {
  std::string line;
  getline(std::cin, line);
  auto calledNums = splitToInts(line, ',');

  std::vector<Board> boards;
  Board b;
  // Read next empty newline
  getline(std::cin, line);
  while (Board::readFromStdin(b)) {
    boards.push_back(b);
    // Read next empty newline
    getline(std::cin, line);
  }

  // A bingo board can't win before calling at least 5 numbers
  for (size_t i = 0; i < 5; ++i) {
    for (auto& b : boards) {
      b.markFast(calledNums.at(i));
    }
  }

  int64_t maxScore = -1;
  for (size_t i = 5; i < calledNums.size(); ++i) {
    for (size_t b = 0; b < boards.size(); ++b) {
      boards.at(b).mark(calledNums.at(i));
      auto score = boards.at(b).getScore();
      if (score > 0) {
        std::cout << "Board[" << b << "] found with score = " << score << '\n';
        maxScore = score > maxScore ? score : maxScore;
      }
    }

    if (maxScore > 0) {
      std::cout << "Max score: " << maxScore << '\n';
      break;
    }
  }
}
