#include "Board.h"
class Game {
    private:
        Board gameBoard = Board();
        
        std::string currPlayer = "White";
        bool runGame = true;
        

        std::string trim(const std::string& line) {
            const char* WhiteSpace = " \t\v\r\n";
            std::size_t start = line.find_first_not_of(WhiteSpace);
            std::size_t end = line.find_last_not_of(WhiteSpace);
            return start == end ? std::string() : line.substr(start, end - start + 1);
        }

        bool turn() {
            
            std::cout << currPlayer << " to move." << std::endl;
            std::string inputMove1, inputMove2; std::cin >> inputMove1 >> inputMove2;
            //inputMove = trim(inputMove);


            if (currPlayer == "White") {
                while (!gameBoard.move(gameBoard.whitePieces, inputMove1, inputMove2)) {
                    std::cin >> inputMove1 >> inputMove2;
                }
            } else {
                while (!gameBoard.move(gameBoard.blackPieces, inputMove1, inputMove2)) {
                    std::cin >> inputMove1 >> inputMove2;
                }
            } 

            std::cout << gameBoard << std::endl;
            if (currPlayer == "White") currPlayer = "Black";
            else currPlayer = "White";

            if (gameBoard.checkCheckMate(currPlayer)) std::cout << "Checkmate!" << std::endl;

            if (gameBoard.checkLose(currPlayer)) {
                if (currPlayer == "White") std::cout << "Black wins!" << std::endl;
                else std::cout << "White wins!" << std::endl;
                return false;
            } else if (gameBoard.checkStalemate(currPlayer)) {
                std::cout << "Stalemate!" << std::endl;
                return false;
            }
            return true;
        }

    
    public:
        Game() {
            std::cout << gameBoard << std::endl;
        }   

        void fullGame() {
            while (runGame) {
                runGame = turn();
            }
        } 

};