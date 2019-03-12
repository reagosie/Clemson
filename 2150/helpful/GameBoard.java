package cpsc2150.homeworks.hw1;

/**
 * @invariant [board is an 8x8 character array consisting of X's, Y's, or blank spaces]
 */
public class GameBoard {

    private char[][] board;

    public GameBoard() {
        board = new char[8][8];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = ' ';
            }
        }
    }

    /**
     * @ensures [checkSpace return a bool saying whether the position requested is out of the board
     * already taken]
     */
    public boolean checkSpace(BoardPosition pos) {
        if (pos.getRow() < 0 || pos.getRow() > 7 || pos.getColumn() < 0 || pos.getColumn() > 7)
            return false;
        else if (board[pos.getRow()][pos.getColumn()] != ' ')
            return false;
        else
            return true;
   }

    /**
     * @requires [marker is a valid board position]
     * @ensures [board is updated with marker]
     */
    public void placeMarker (BoardPosition marker) {
        board[marker.getRow()][marker.getColumn()] = marker.getPlayer();
    }

    /**
     * @requires [the last position that was updated on the board]
     * @ensures [returns true if last position updated was a winning move, false otherwise]
     */
    public boolean checkForWinner(BoardPosition lastPos) {
        if (checkHorizontalWin(lastPos))
            return true;
        if (checkVerticalWin(lastPos))
            return true;
        if (checkDiagonalWin(lastPos))
            return true;
        else
            return false;
    }

    /**
     * @ensures [returns true if all spaces are not blank]
     */
    public boolean checkForDraw() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] == ' ') return false;
            }
        }
        return true;
    }

    private boolean checkHorizontalWin(BoardPosition lastPos) {
        int row = lastPos.getRow();
        int counter = 0;
        for (int i = 0; i < 8; i++) {
            if (board[row][i] == lastPos.getPlayer()) counter++;
            else counter = 0;
            if (counter == 5) return true;
        }
        return false;
    }

   private boolean checkVerticalWin (BoardPosition lastPos) {
       int col = lastPos.getColumn();
       int counter = 0;
       for (int i = 0; i < 8; i++) {
           if (board[i][col] == lastPos.getPlayer()) counter++;
           else counter = 0;
           if (counter == 5) return true;
       }
       return false;
    }

    private boolean checkDiagonalWin (BoardPosition lastPos) {
        int tRow = lastPos.getRow();
        int tCol = lastPos.getColumn();
        int counter = 1;
        while(tRow > 0 && tCol > 0) {
            tRow--;
            tCol--;
            if(lastPos.getPlayer() == board[tRow][tCol]) counter++;
        }
        tRow = lastPos.getRow();
        tCol = lastPos.getColumn();
        while(tRow < 7 && tCol < 7) {
            tRow++;
            tCol++;
            if(lastPos.getPlayer() == board[tRow][tCol]) counter++;
        }
        if (counter == 5) return true;
        tRow = lastPos.getRow();
        tCol = lastPos.getColumn();
        counter = 1;
        while(tRow > 0 && tCol < 7) {
            tRow--;
            tCol++;
            if(lastPos.getPlayer() == board[tRow][tCol]) counter++;
        }
        tRow = lastPos.getRow();
        tCol = lastPos.getColumn();
        while(tRow < 7 && tCol > 0) {
            tRow++;
            tCol--;
            if(lastPos.getPlayer() == board[tRow][tCol]) counter++;
        }
        if (counter == 5) return true;
        return false;
    }

    @Override
    public String toString() {
        return "  0 1 2 3 4 5 6 7 " + '\n' +
                "0|" + board[0][0] + '|' + board[0][1] + '|' + board[0][2] + '|' + board[0][3] + '|' + board[0][4] + '|' + board[0][5] + '|' + board[0][6] + '|' + board[0][7] + '|' + '\n' +
                "1|" + board[1][0] + '|' + board[1][1] + '|' + board[1][2] + '|' + board[1][3] + '|' + board[1][4] + '|' + board[1][5] + '|' + board[1][6] + '|' + board[1][7] + '|' + '\n' +
                "2|" + board[2][0] + '|' + board[2][1] + '|' + board[2][2] + '|' + board[2][3] + '|' + board[2][4] + '|' + board[2][5] + '|' + board[2][6] + '|' + board[2][7] + '|' + '\n' +
                "3|" + board[3][0] + '|' + board[3][1] + '|' + board[3][2] + '|' + board[3][3] + '|' + board[3][4] + '|' + board[3][5] + '|' + board[3][6] + '|' + board[3][7] + '|' + '\n' +
                "4|" + board[4][0] + '|' + board[4][1] + '|' + board[4][2] + '|' + board[4][3] + '|' + board[4][4] + '|' + board[4][5] + '|' + board[4][6] + '|' + board[4][7] + '|' + '\n' +
                "5|" + board[5][0] + '|' + board[5][1] + '|' + board[5][2] + '|' + board[5][3] + '|' + board[5][4] + '|' + board[5][5] + '|' + board[5][6] + '|' + board[5][7] + '|' + '\n' +
                "6|" + board[6][0] + '|' + board[6][1] + '|' + board[6][2] + '|' + board[6][3] + '|' + board[6][4] + '|' + board[6][5] + '|' + board[6][6] + '|' + board[6][7] + '|' + '\n' +
                "7|" + board[7][0] + '|' + board[7][1] + '|' + board[7][2] + '|' + board[7][3] + '|' + board[7][4] + '|' + board[7][5] + '|' + board[7][6] + '|' + board[7][7] + '|' ;
    }

//    @Override
//    public boolean equals(Object o) {
//        if (!(o instanceof  GameBoard)) {
//            return false;
//        }
//
//        GameBoard gb = (GameBoard)  o;
//
//        o.
//
//        return false;
//    }
}