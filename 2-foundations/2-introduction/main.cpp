#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

enum class State {kEmpty, kObstacle};

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

string CellString(const State cell) {
  return cell == State::kObstacle ? "⛰️   " : "0   ";
}

void PrintBoard(const vector<vector<State>>& board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << CellString( board[i][j] );
    }
    cout << std::endl;
  }
}

int main() {
  auto board = ReadBoardFile("1.board");
  PrintBoard(board);
}