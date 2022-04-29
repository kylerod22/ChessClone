class Piece {
    protected:
        int row = 0, col = 0;
    public:
        Piece(int inRow, int inCol): row(inRow), col(inCol) {}

        virtual void canMove() = 0;      
        virtual void move() = 0; 


};

class Pawn : public Piece {
    public:
        Pawn(int inRow, int inCol): Piece(inRow, inCol) {}

}; 