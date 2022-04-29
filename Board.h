#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
class Board {

    private:
        const static int BOARD_HEIGHT = 8, BOARD_WIDTH = 8;

        struct Piece {
            std::vector<std::string> availableMoves;
            std::vector<std::string> protectiveMoves;
            std::string type;
            int row, col;
            bool endOfOppositeSide = false;
            bool hasMoved = false;
            Piece(std::string inputCoords, std::string inType): type(inType) {
                std::pair<int, int> rowCol = parseStringToCoords(inputCoords);
                row = rowCol.first;
                col = rowCol.second;
            }
        };

        struct Pawn : Piece {
            Pawn(std::string input): Piece(input, "Pawn") {}
        };

        struct Rook : Piece {
            Rook(std::string input): Piece(input, "Rook") {}
        };

        struct Bishop : Piece {
            Bishop(std::string input): Piece(input, "Bishop") {}
        };

        struct Knight : Piece {
            Knight(std::string input): Piece(input, "Knight") {}
        };

        struct Queen : Piece {
            Queen(std::string input): Piece(input, "Queen") {}
        };

        struct King : Piece {
            King(std::string input): Piece(input, "King") {}
        };


        std::string getLetterOfPiece(int row, int col) const;
        void findAvailableMoves(Piece * piece);
        void update();
        void updatePieces(std::vector<Piece *> &vec);
        bool containsPiece(std::vector<Piece *> &pieces, Piece * piece) const;
        bool containsMove(std::vector<Piece *> &pieces, std::string move) const;
        bool containsMove(Piece * piece, std::string move) const;
        void removePiece(Piece * piece);

        std::vector<Piece *> whitePieces;
        std::vector<Piece *> blackPieces;

        void rookBishopHelper(std::vector<Piece *> *currPlayerPtr, std::vector<Piece *> *enemyPtr,
                        Piece *currPiece, int rowInc, int colInc);

        bool validCoord(std::string input);

        Piece * getKing(std::vector<Piece *> &vec) {
            for (auto &piece : vec) {
                if (piece->type == "King") return piece;
            }
            return nullptr;
        }

        bool checkCheckMate(std::vector<Piece *> &currPlayer) {
            std::vector<Piece *> *enemyPtr = &blackPieces;
            if (currPlayer == blackPieces) enemyPtr = &whitePieces;
            Piece * king = getKing(currPlayer);
            std::string kingCoord = parseCoordsToString(king->row, king->col);
            for (auto &piece : *enemyPtr) {
                for (std::string move : piece->availableMoves) {
                    if (move == kingCoord) return true;
                }
            }
            return false;
        }
        

    public:

        void test() {
            for (auto &piece : whitePieces) {
                std::cout << getLetterOfPiece(piece->row, piece->col) << " Moves: ";
                for (std::string move : piece->availableMoves) {
                    std::cout << move << " ";
                }
                std::cout << std::endl;
            }
            move(whitePieces, "E6", "F5");
        }

        bool move(std::vector<Piece *> &currPlayer, std::string start, std::string end) {
            std::vector<Piece *> *enemyPtr = &blackPieces;
            if (currPlayer == blackPieces) enemyPtr = &whitePieces;
            
            if (!validCoord(start) || !validCoord(end)) {
                std::cout << "Invalid coords." << std::endl;
                return false;
            } //INVALID COORDS
            Piece *pieceToMove = getPieceAt(start);
            if (!containsPiece(currPlayer, pieceToMove)) {
                std::cout << "Start coord isn't one of your pieces." << std::endl;
                return false;
            }//Start coordinate doesn't point to one of your pieces
            if (!containsMove(pieceToMove, end)) {
                std::cout << "Invalid move." << std::endl;
                return false;
            }//Not a valid move

            int startRow = pieceToMove->row, startCol = pieceToMove->col;
            std::pair<int, int> coords = parseStringToCoords(end);

            pieceToMove->row = coords.first;
            pieceToMove->col = coords.second;
            update();
            if (checkCheckMate(currPlayer)) {
                pieceToMove->row = startRow;
                pieceToMove->col = startCol;
                std::cout << "Invalid move." << std::endl;
                return false; //Moving will still put you in checkmate
            }
            pieceToMove->row = startRow;
            pieceToMove->col = startCol;

            Piece *attackedPiece = getPieceAt(end);
            if (attackedPiece != nullptr) {
                removePiece(attackedPiece);
            }
            pieceToMove->row = coords.first;
            pieceToMove->col = coords.second;
            update();
            return true;
        }
        
        Board() {
            /*for (int col = 0; col < BOARD_WIDTH; col++) {
                whitePieces.push_back(new Pawn(parseCoordsToString(6, col)));
                blackPieces.push_back(new Pawn(parseCoordsToString(1, col)));
            } */

            whitePieces.push_back(new King("D7"));
            blackPieces.push_back(new King("H1"));
            whitePieces.push_back(new Pawn("E6"));
            blackPieces.push_back(new Queen("E5"));
            blackPieces.push_back(new Rook("F5"));

            update();
        }

        ~Board() {
            for (Piece * piece : whitePieces) {
                delete piece;
            }

            for (Piece * piece : blackPieces) {
                delete piece;
            }
        } 

        Piece * getPieceAt(int row, int col) const {
            if (row < 0 || row >= BOARD_HEIGHT
             || col < 0 || col >= BOARD_WIDTH) return nullptr;
            for (Piece * piece : whitePieces) {
                if (piece->row == row && piece->col == col) return piece;
            }
            for (Piece * piece : blackPieces) {
                if (piece->row == row && piece->col == col) return piece;
            }
            return nullptr;
        }

        Piece * getPieceAt(std::string input) {
            std::pair<int, int> coords = parseStringToCoords(input);
            return getPieceAt(coords.first, coords.second);
        }

        static std::pair<int, int> parseStringToCoords(std::string input) {
            std::pair<int, int> coord (-1, -1);
            if (input.length() != 2) return coord;
            if (input.at(0) < 'A' || input.at(0) > 'H') return coord;
            if (input.at(1) < '1' || input.at(1) > '8') return coord;
            coord.first = input.at(1) - '1';
            coord.second = input.at(0) - 'A';
            return coord;
        }

        static std::string parseCoordsToString(int row, int col) {
            std::string output = "";
            output += col + 'A';
            output += std::to_string(row + 1);
            return output;
        }


        friend std::ostream &operator<<(std::ostream &os, const Board &board);
};

std::ostream &operator<<(std::ostream &os, const Board &board);
