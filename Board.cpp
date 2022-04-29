#include "Board.h"

void Board::updatePieces(std::vector<Piece *> &vec) {
    for (Piece * piece : vec) {
        findAvailableMoves(piece);
    }
}

void Board::update() {
    updatePieces(whitePieces);
    updatePieces(blackPieces);
    updatePieces(whitePieces);
}

void Board::removePiece(Piece * piece) {
    delete piece;
    auto it = std::find(whitePieces.begin(), whitePieces.end(), piece);
    if (it != whitePieces.end()) {
        whitePieces.erase(it);
        return;
    }
    it = std::find(blackPieces.begin(), blackPieces.end(), piece);
    if (it != blackPieces.end()) {
        blackPieces.erase(it);
        return;
    }
}

bool Board::containsMove(Piece * piece, std::string move) const {
    for (std::string currMove : piece->availableMoves) {
        if (currMove == move) return true;
    }
    return false;
}

bool Board::containsPiece(std::vector<Piece *> &pieces, Piece * piece) const {
    if (std::count(pieces.begin(), pieces.end(), piece)) return true;
    return false;
}

bool Board::containsMove(std::vector<Piece *> &pieces, std::string move) const {
    for (auto &piece : pieces) {
        if (std::count(piece->availableMoves.begin(), piece->availableMoves.end(), move)) return true;
        if (std::count(piece->protectiveMoves.begin(), piece->protectiveMoves.end(), move)) return true;
    }
    return false;
}

std::string Board::getLetterOfPiece(int row, int col) const {
    Piece * foundPiece = getPieceAt(row, col);
    if (!foundPiece) return "  ";
    std::string s;
    if (std::count(whitePieces.begin(), whitePieces.end(), foundPiece)) s.push_back('W');
    else s.push_back('B');
    s.push_back(foundPiece->type.at(0));
    return s;
}

void Board::findAvailableMoves(Piece * piece) {
    int direction = 1;
    std::vector<Piece *> *currPlayerPtr = &whitePieces;
    std::vector<Piece *> *enemyPtr = &blackPieces;
    if (containsPiece(blackPieces, piece)) {
        direction *= -1;
        enemyPtr = &whitePieces;
        currPlayerPtr = &blackPieces;
    }
    piece->availableMoves.clear();
    piece->protectiveMoves.clear();
    int row = piece->row, col = piece->col;
    if (piece->type == "Pawn") {
        if (row - direction >= 0 && row - direction < BOARD_HEIGHT) {
            if (getPieceAt(row - direction, col) == nullptr) piece->availableMoves.push_back(parseCoordsToString(row - direction, col));
        }
        if (row - 2 * direction >= 0 && row - 2 * direction < BOARD_HEIGHT 
            && !piece->hasMoved && getPieceAt(row - 2 * direction, col) == nullptr) piece->availableMoves.push_back(parseCoordsToString(row - 2 * direction, col));
        if (containsPiece(*enemyPtr, getPieceAt(row - direction, col - 1))) piece->availableMoves.push_back(parseCoordsToString(row - direction, col - 1));
        if (containsPiece(*enemyPtr, getPieceAt(row - direction, col + 1))) piece->availableMoves.push_back(parseCoordsToString(row - direction, col + 1));
        if (containsPiece(*currPlayerPtr, getPieceAt(row - direction, col - 1))) piece->protectiveMoves.push_back(parseCoordsToString(row - direction, col - 1));
        if (containsPiece(*currPlayerPtr, getPieceAt(row - direction, col + 1))) piece->protectiveMoves.push_back(parseCoordsToString(row - direction, col + 1));
    }

    if (piece->type == "Rook" || piece->type == "Queen") {
        rookBishopHelper(currPlayerPtr, enemyPtr, piece, 0, 1);
        rookBishopHelper(currPlayerPtr, enemyPtr, piece, 0, -1);
        rookBishopHelper(currPlayerPtr, enemyPtr, piece, 1, 0);
        rookBishopHelper(currPlayerPtr, enemyPtr, piece, -1, 0);
    }

    if (piece->type == "Bishop" || piece->type == "Queen") {
        rookBishopHelper(currPlayerPtr, enemyPtr, piece, 1, 1);
        rookBishopHelper(currPlayerPtr, enemyPtr, piece, 1, -1);
        rookBishopHelper(currPlayerPtr, enemyPtr, piece, -1, 1);
        rookBishopHelper(currPlayerPtr, enemyPtr, piece, -1, -1);
    }

    if (piece->type == "Knight") {
        for (int checkColInc = -2; checkColInc <= 2; checkColInc += 4) {
            for (int checkRowInc = -1; checkRowInc <= 1; checkRowInc += 2) {
                int checkRow = row + checkRowInc, checkCol = col + checkColInc;
                if (checkRow >= 0 && checkRow < BOARD_HEIGHT && checkCol >= 0 && checkCol < BOARD_WIDTH) {
                    if (containsPiece(*currPlayerPtr, getPieceAt(checkRow, checkCol))) {
                        piece->protectiveMoves.push_back(parseCoordsToString(checkRow, checkCol));
                        continue;
                    }
                    if (containsPiece(*enemyPtr, getPieceAt(checkRow, checkCol))) {
                        piece->availableMoves.push_back(parseCoordsToString(checkRow, checkCol)); 
                        continue;
                    }
                    piece->availableMoves.push_back(parseCoordsToString(checkRow, checkCol));
                }
                
            }
        }

        for (int checkRowInc = -2; checkRowInc <= 2; checkRowInc += 4) {
            for (int checkColInc = -1; checkColInc <= 1; checkColInc += 2) {
                int checkRow = row + checkRowInc, checkCol = col + checkColInc;
                if (checkRow >= 0 && checkRow < BOARD_HEIGHT && checkCol >= 0 && checkCol < BOARD_WIDTH) {
                    if (containsPiece(*currPlayerPtr, getPieceAt(checkRow, checkCol))) {
                        piece->protectiveMoves.push_back(parseCoordsToString(checkRow, checkCol));
                        continue;
                    }
                    if (containsPiece(*enemyPtr, getPieceAt(checkRow, checkCol))) {
                        piece->availableMoves.push_back(parseCoordsToString(checkRow, checkCol)); 
                        continue;
                    }
                    piece->availableMoves.push_back(parseCoordsToString(checkRow, checkCol));
                }
                
            }
        }
    }

    if (piece->type == "King") {
        for (int checkRowInc = -1; checkRowInc <= 1; checkRowInc++) {
            for (int checkColInc = -1; checkColInc <= 1; checkColInc++) {
                if (checkRowInc == 0 && checkColInc == 0) continue;
                int checkRow = row + checkRowInc, checkCol = col + checkColInc;
                
                if (checkRow >= 0 && checkRow < BOARD_HEIGHT && checkCol >= 0 && checkCol < BOARD_WIDTH) {
                    std::string checkSpot = parseCoordsToString(checkRow, checkCol);
                    Piece * checkPiece = getPieceAt(checkRow, checkCol);
                    if (containsPiece(*currPlayerPtr, checkPiece)) {
                        piece->protectiveMoves.push_back(checkSpot);
                        continue;
                    }
                    if (!containsMove(*enemyPtr, checkSpot)) piece->availableMoves.push_back(checkSpot);  
                } 
            }
        }
    } 
}

void Board::rookBishopHelper(std::vector<Piece *> *currPlayerPtr, std::vector<Piece *> *enemyPtr,
                        Piece *currPiece, int rowInc, int colInc) {
    int row = currPiece->row, col = currPiece->col;
    row += rowInc; col += colInc;
    while (row >= 0 && row < BOARD_HEIGHT && col >= 0 && col < BOARD_WIDTH) {
        if (containsPiece(*currPlayerPtr, getPieceAt(row, col))) {
            currPiece->protectiveMoves.push_back(parseCoordsToString(row, col));
            break;
        }
        if (containsPiece(*enemyPtr, getPieceAt(row, col))) {
            currPiece->availableMoves.push_back(parseCoordsToString(row, col)); 
            break;
        }
        currPiece->availableMoves.push_back(parseCoordsToString(row, col));
        row += rowInc;
        col += colInc;
    }
}

bool Board::validCoord(std::string input) {
    if (input.length() != 2) return false;
    if (input.at(0) < 'A' || input.at(0) > 'H') return false;
    if (input.at(1) < '1' || input.at(1) > '8') return false;
    return true;
}

std::ostream &operator<<(std::ostream &os, const Board &board) {
    std::cout << "   ---------------------------------------------------------" << std::endl;
    for (int row = 0; row < board.BOARD_HEIGHT; row++) {
        std::cout << "   |      |      |      |      |      |      |      |      |" << std::endl;
        std::cout << row + 1 << "  |";
        for (int col = 0; col < board.BOARD_WIDTH; col++) {
            std::cout << "  " << board.getLetterOfPiece(row, col) << "  |";
        }
        std::cout << std::endl;
        std::cout << "   |      |      |      |      |      |      |      |      |" << std::endl;
        std::cout << "   ---------------------------------------------------------" << std::endl;
        
    }
    std::cout << "   ----A------B------C------D------E------F------G------H---" << std::endl;
    return os;
}


