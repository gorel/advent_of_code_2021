#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
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

class Node {
 public:
  Node(std::string name) : name_{name} {}

  const std::string& getName() const { return name_; }

  const std::unordered_set<std::string>& getNeighbors() const {
    return neighbors_;
  }

  void addNeighbor(const std::string& neighborName) {
    neighbors_.insert(neighborName);
  }

 private:
  std::string name_;
  std::unordered_set<std::string> neighbors_;
};

class Path {
 public:
  Path() {}
  Path(const Path& other) : path_{other.path_} {}

  void push(const std::string& node) { path_.push_back(node); }

  std::string pop() {
    std::string res = path_.back();
    path_.pop_back();
    return res;
  }

  const std::vector<std::string> getPath() const { return path_; }

  friend bool operator==(const Path& a, const Path& b) {
    if (a.getPath().size() != b.getPath().size()) {
      return false;
    }

    for (size_t i = 0; i < a.getPath().size(); ++i) {
      if (a.getPath().at(i) != b.getPath().at(i)) {
        return false;
      }
    }
    return true;
  }

  friend std::ostream& operator<<(std::ostream& os, const Path& p) {
    bool first = true;
    for (const auto& node : p.getPath()) {
      if (!first) {
        // os << " -> ";
        os << ",";
      }
      os << node;
      first = false;
    }
    return os;
  }

 private:
  std::vector<std::string> path_;
};

class Graph {
 public:
  Graph(std::vector<std::string>& lines) {
    for (const auto& line : lines) {
      auto parts = split(line, '-');
      addEdge(parts[0], parts[1]);
    }
  }

  std::vector<Path> dfs() const {
    std::vector<Path> paths;
    std::unordered_set<std::string> visited;
    Path curPath;
    dfsHelper("start", paths, curPath, visited);
    return paths;
  }

  std::vector<Path> dfs2() const {
    std::vector<Path> paths;
    std::unordered_map<std::string, int8_t> visited;
    Path curPath;
    dfsHelper2("start", paths, curPath, visited, false);
    return paths;
  }

 private:
  void addEdge(const std::string& src, const std::string& dst) {
    if (nodes_.find(src) == nodes_.end()) {
      nodes_[src] = std::make_unique<Node>(src);
    }
    if (nodes_.find(dst) == nodes_.end()) {
      nodes_[dst] = std::make_unique<Node>(dst);
    }

    nodes_[src]->addNeighbor(nodes_[dst]->getName());
    nodes_[dst]->addNeighbor(nodes_[src]->getName());
  }

  void dfsHelper(const std::string& thisNode, std::vector<Path>& paths,
                 Path& curPath,
                 std::unordered_set<std::string>& visited) const {
    // Only add to visited list if it's a small cave
    if (!std::isupper(thisNode.at(0))) {
      visited.insert(thisNode);
    }
    curPath.push(thisNode);

    if (thisNode == "end") {
      Path copy{curPath};
      paths.push_back(copy);
    } else {
      for (const auto& neighbor : nodes_.at(thisNode)->getNeighbors()) {
        if (visited.find(neighbor) == visited.end()) {
          dfsHelper(neighbor, paths, curPath, visited);
        }
      }
    }
    // Allow this node to be visited again
    curPath.pop();
    visited.erase(thisNode);
  }

  void dfsHelper2(const std::string& thisNode, std::vector<Path>& paths,
                  Path& curPath,
                  std::unordered_map<std::string, int8_t>& visited,
                  bool exploredSmallTwice) const {
    // Only add to visited list if it's a small cave
    if (!std::isupper(thisNode.at(0))) {
      ++visited[thisNode];
    }
    curPath.push(thisNode);

    if (thisNode == "end") {
      Path copy{curPath};
      paths.push_back(copy);
    } else {
      for (const auto& neighbor : nodes_.at(thisNode)->getNeighbors()) {
        if (neighbor == "start") {
          continue;
        }
        if (visited[neighbor] > 0) {
          if (exploredSmallTwice) {
            continue;
          } else {
            // This is a node we've seen before, choose to take it as our
            // second visit to a small cavern
            dfsHelper2(neighbor, paths, curPath, visited, true);
          }
        } else {
          dfsHelper2(neighbor, paths, curPath, visited, exploredSmallTwice);
        }
      }
    }

    // Allow this node to be visited again
    curPath.pop();
    // Once again, we only care for small caves
    if (!std::isupper(thisNode.at(0))) {
      --visited[thisNode];
    }
  }

  std::unordered_map<std::string, std::unique_ptr<Node>> nodes_;
  std::vector<Path> paths_;
};

int64_t solve1(std::vector<std::string> lines) {
  Graph g{lines};
  auto paths = g.dfs();
  return paths.size();
}

int64_t solve2(std::vector<std::string> lines) {
  Graph g{lines};
  auto paths = g.dfs2();
  return paths.size();
}

int main(int argc, char** argv) {
  auto sampleLines = readLines("sample.txt");
  expect(solve1(sampleLines), 10);
  std::cout << "Passed sample 1\n";

  expect(solve2(sampleLines), 36);
  std::cout << "Passed sample 2\n";

  auto lines = readLines("input.txt");
  std::cout << "Problem 1: " << solve1(lines) << '\n';
  std::cout << "Problem 2: " << solve2(lines) << '\n';

  return 0;
}
