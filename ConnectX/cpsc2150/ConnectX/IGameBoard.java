package cpsc2150.ConnectX;

/**
 This interface creates a board with the number of rows and columns the user has chosen.
 The board is constructed and initialized to be blank spaces. Then, it adds both players'
 tokens to the board. The game ends if a player wins or if there is a tie. A player wins
 if they have the amount of tokens they chose in a row horizontally, vertically, or diagonally.

 Defines:  Rows: r
 Columns: c
 Number of tokens to win: numtowin

 Constraints: Rows >= 3 and <= 100
 Cols >= 3 and <= 100
 Number of tokens to win >= 3 and <= 25

 Initialization Ensures: board has MAX_ROWS rows and MAX_COLS columns and they are filled with blank spaces
 */
public interface IGameBoard {
    int getNumRows();
    int getNumColumns();
    int getNumToWin();
    void placeToken(char p, int c);
    boolean checkHorizWin(int r, int c, char p);
    boolean checkVertWin(int r, int c, char p);
    boolean checkDiagWin(int r, int c, char p);
    char whatsAtPos(int r, int c);

    /**
     * @pre c > 0, c <= number of columns
     * @param c is the column the user selected on the most recent turn
     * @return true if the top of the column is free
     */
    default boolean checkIfFree(int c){                     // this function checks the top of each row
        if (whatsAtPos(getNumRows()-1, c) == ' ')        // if the top of the row is blank, return true (because there is space in the column)
            return true;
        else
            return false;
    }

    /**
     * @pre c > 0, c <= number of columns
     * @param c is the column the user selected on the most recent turn
     * @return true if either player won horizontally, vertically, or diagonally
     */
    default boolean checkForWin(int c){                     // this function checks for a win horizontally, vertically, and diagonally
        int row = 0;
        for(int i = 0; i < getNumRows(); i++) {             // this loop finds the last placed token in the last column chosen
            if (whatsAtPos(i, c) == ' ') {
                row = i - 1;
                break;
            }
        }
        char player = whatsAtPos(row, c);                   // set temporary variable player to the last placed token

        if(checkHorizWin(row, c, player)){                  // check for win horizontally
            return true;
        }
        if(checkVertWin(row, c, player)){                   // check for win vertically
            return true;
        }
        else if(checkDiagWin(row, c, player)){              // check for win diagonally
            return true;
        }
        else{return false;}                                 // return false if none of these are true
    }

    /**
     * @pre whatsAtPos == 'X', 'O', or ' '
     * @return true if board is full
     */
    default boolean checkTie() {                            // this function checks for a tie (if the board is full)
        int count = 0;
        for(int c = 0; c < getNumColumns(); c++) {        // check the top of each column
            if (!checkIfFree(c)){                           // if it is full, increment count
                count++;
            }
        }
        if(count == getNumColumns()){                       // if count reaches the number of columns, all columns are full; it's tie
            return true;                                    // return true
        }
        else{return false;}                                 // if any columns are not full, then it is not a tie
    }
}
