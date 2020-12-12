#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::abs;
using std::get;
using std::sort;

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

// directional deltas
constexpr const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

vector<State> ParseLine(const string& line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row{};
    while (sline >> n >> c && c == ',') {
      row.push_back( n ? State::kObstacle : State::kEmpty );
    }
    return row;
}

vector<vector<State>> ReadBoardFile(const string& path) {
  ifstream myfile (path);
  vector<vector<State>> board{};
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      board.push_back( std::move(ParseLine(line)) );
    }
  }
  return board;
}

bool Compare(const vector<int>& node1, const vector<int>& node2) {
  return node1[2] + node1[3] > node2[2] + node2[3];
}

// Sort the two-dimensional vector of ints in descending order.
void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}

int Heuristic(const int x1, const int y1, const int x2, const int y2) {
  return std::abs(x2 - x1) + std::abs(y2 - y1);
}

bool CheckValidCell(int x, int y, vector<vector<State>>& board) {
  return x >= 0 && board.size() > x && y >= 0 && board[x].size() > y && board[x][y] == State::kEmpty;
}

void AddToOpen(const int x, const int y, const int g, const int h, vector<vector<int>>& openNodes, vector<vector<State>>& board) {
  openNodes.push_back( {x, y, g, h} );
  board[x][y] = State::kClosed;
}

void ExpandNeighbors(const vector<int>& current, const int goal[2], vector<vector<int>>& open, vector<vector<State>>& board) {
  const int x = current[0];
  const int y = current[1];
  const int g = current[2];
  // const int h = current[3];

  for (const auto& d : delta) {
    int x2 = x + d[0];
    int y2 = y + d[1];

    if ( CheckValidCell(x2, y2, board) ) {
      AddToOpen( x2, y2, g + 1, Heuristic(x2, y2, goal[0], goal[1]), open, board );
    }
  }
}

// Implementation of A* search algorithm
vector<vector<State>> Search(vector<vector<State>>& board, int start[2], int goal[2]) {
  vector<vector<int>> open {};
  int x = start[0];
  int y = start[1];
  int g = 0;
  int h = Heuristic(x, y, goal[0], goal[1]);

  AddToOpen(x, y, g, h, open, board);

  while ( open.size() ) {
    CellSort( &open );
    auto current = open.back();
    open.pop_back();

    x = current[0];
    y = current[1];

    board[x][y] = State::kPath;

    if (goal[0] == x && goal[1] == y) {
      board[ start[0] ][ start[1] ] = State::kStart;
      board[ goal[0] ][ goal[1] ] = State::kFinish;
      return board;
    }

    // If we're not done, expand search to current node's neighbors.
    ExpandNeighbors(current, goal, open, board);
  }

  std::cout << "No path found!" << std::endl;

  return {};
}

string CellString(const State cell) {
  switch(cell) {
    case State::kObstacle:
      return "⛰️   ";
    case State::kPath:
      return "🚗   ";

    case State::kStart:
      return "🚦   ";

    case State::kFinish:
      return "🏁   ";

    default:
      break;
  }

  return "0   ";
}

void PrintBoard(const vector<vector<State>>& board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      std::cout << CellString( board[i][j] );
    }
    std::cout << std::endl;
  }
}

int main() {
  int start[2]{0, 0};
  int goal[2]{4, 5};

  auto board = ReadBoardFile("1.board");

  auto solution = Search(board, start, goal);

  PrintBoard(solution);
}
