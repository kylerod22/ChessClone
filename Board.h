#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
class Board {
    friend class Game;
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
            Piece(int inRow, int inCol, std::string inType): Piece(parseCoordsToString(inRow, inCol), inType) {}
                
    
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

        bool checkCheckMate(std::string currPlayer) {
            std::vector<Piece *> *currPlayerPiecesPtr = &whitePieces;
            if (currPlayer == "Black") currPlayerPiecesPtr = &blackPieces;
            return checkCheckMate(*currPlayerPiecesPtr);
        }

        bool checkNoPossibleMoves(std::vector<Piece *> &currPlayer) {
            std::vector<Piece *> *enemyPtr = &blackPieces;
            if (currPlayer == blackPieces) enemyPtr = &whitePieces;
            bool foundPossibleMove = false;
            for (auto & piece : currPlayer) {
                for (std::string move : piece->availableMoves) {
                    int startRow = piece->row, startCol = piece->col;
                    int enemyPieceRow = -1, enemyPieceCol = -1;
                    std::pair<int, int> moveCoords = parseStringToCoords(move);
                    Piece * enemyPieceAtPos = getPieceAt(move);
                    piece->row = moveCoords.first;
                    piece->col = moveCoords.second;

                    if (enemyPieceAtPos != nullptr) {
                        enemyPieceRow = enemyPieceAtPos->row;
                        enemyPieceCol = enemyPieceAtPos->col;
                        enemyPieceAtPos->row = -1;
                        enemyPieceAtPos->col = -1;
                    }
                    

                    update();
                    if (!checkCheckMate(currPlayer)) foundPossibleMove = true;
                    piece->row = startRow;
                    piece->col = startCol;

                    if (enemyPieceAtPos != nullptr) {
                        enemyPieceAtPos->row = enemyPieceRow;
                        enemyPieceAtPos->col = enemyPieceCol;
                    }
                    update();
                }
            }
            return !foundPossibleMove;
        }

        bool checkStalemate(std::string currPlayer) {
            std::vector<Piece *> *currPlayerPiecesPtr = &whitePieces;
            if (currPlayer == "Black") currPlayerPiecesPtr = &blackPieces;
            if (!checkCheckMate(*currPlayerPiecesPtr) && checkNoPossibleMoves(*currPlayerPiecesPtr)) return true;
            return false;
        }

        bool checkLose(std::string currPlayer) {
            std::vector<Piece *> *currPlayerPiecesPtr = &whitePieces;
            if (currPlayer == "Black") currPlayerPiecesPtr = &blackPieces;
            if (checkCheckMate(*currPlayerPiecesPtr) && checkNoPossibleMoves(*currPlayerPiecesPtr)) return true;
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
            //move(whitePieces, "E6", "F5");
        } 

        bool move(std::vector<Piece *> &currPlayer, std::string start, std::string end) {
            std::vector<Piece *> *enemyPtr = &blackPieces;
            int endOppositeRow = 0;
            if (currPlayer == blackPieces) {
                enemyPtr = &whitePieces;
                endOppositeRow = 7;
            }
            
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

            Piece *attackedPiece = getPieceAt(end);
            int startRow = pieceToMove->row, startCol = pieceToMove->col;
            int attackedPieceRow = -1, attackedPieceCol = -1;
            std::pair<int, int> coords = parseStringToCoords(end);

            pieceToMove->row = coords.first;
            pieceToMove->col = coords.second;
            if (attackedPiece != nullptr) {
                attackedPieceRow = attackedPiece->row;
                attackedPieceCol = attackedPiece->col;
                attackedPiece->row = -1;
                attackedPiece->col = -1;
            }
            update();
            if (checkCheckMate(currPlayer)) {
                pieceToMove->row = startRow;
                pieceToMove->col = startCol;
                if (attackedPiece != nullptr) {
                    attackedPiece->row = attackedPieceRow;
                    attackedPiece->col = attackedPieceCol;
                }
                std::cout << "Invalid move." << std::endl;
                update();
                return false; //Moving will still put you in checkmate
            }

            if (attackedPiece != nullptr) {
                removePiece(attackedPiece);
            }
            pieceToMove->hasMoved = true;

            update();
            return true;
        }
        
        Board() {
            std::vector<std::string> backOrder = {"Rook", "Knight", "Bishop", "Queen", "King", "Bishop", "Knight", "Rook"};
            for (int col = 0; col < BOARD_WIDTH; col++) {
                whitePieces.push_back(new Piece(6, col, "Pawn"));
                blackPieces.push_back(new Piece(1, col, "Pawn"));
            } 

            for (int col = 0; col < backOrder.size(); col++) {
                std::string pieceType = backOrder.at(col);
                whitePieces.push_back(new Piece(7, col, pieceType));
                blackPieces.push_back(new Piece(0, col, pieceType));
            }

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
