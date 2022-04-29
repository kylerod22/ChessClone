#include <iostream>
#include <utility>
#include "Board.h"
using namespace std;

int main() {
    Board board = Board();
    std::string inputCoord;
    cout << board << endl;
    board.test();
    cout << board << endl;
    return 0;
}

