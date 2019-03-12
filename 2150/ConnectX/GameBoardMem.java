package cpsc2150.ConnectX;

import java.util.*;

/**
 * @pre ROWS >= 3 and <= 100
 * @pre COLS >= 3 and <= 100
 * @inv c > 0, c <= number of columns
 * @inv r > 0, r <= number of rows
 * @inv p = any character in the player array
 * Correspondence: ROWS = rows
 * Correspondence: COLS = cols
 * Correspondence: NUMTOWIN = numtowin
 */
public class GameBoardMem implements IGameBoard{
    //this class will use a map with the key being an integer representing the column
    //and the value being a list of characters that are NOT initialized to empty spaces

    private final static int MAX_ROWS = 100;
    private final static int MAX_COLS = 100;    //these variables are for the maximum number of rows and columns the user is allowed to have
    private static Map<Integer, List<Character>> board = new HashMap<Integer, List<Character>>();
    Character[] players = new Character[10];
    private int ROWS;
    private int COLS;
    private int NUMTOWIN;

    /**
     * @pre p == any character in the player array
     * @pre c > 0 and c <= number of columns
     * @param p the player's token
     * @param c the column the user selected to place their token in
     */
    public void placeToken(char p, int c) {        // this function places a token
        board.get(c).add(p);                       // add token to the list "column"
    }

    /**
     * @pre r > 0 and r <= rows in board
     * @pre c > 0 and c <= columns in board
     * @pre p == any character in the player array
     * @param r the row of the board
     * @param c the column of the board
     * @param p the player's token
     * @return true if the character at board.get(c).get(r) matches p
     */
    public boolean checkHorizWin(int r, int c, char p) { // this function checks horizontally from our current position for a win
        int counter = 0;                                 // count variable
        for (int i = c; i < COLS; i++) {      // check to the right for matching tokens
            if (whatsAtPos(r, i) == p) counter++;             // increment counter if we find a matching token
            else break;
        }

        counter--;
        for (int i = c; i >= 0; i--) {                   // check to the left for matching tokens
            if (whatsAtPos(r, i) == p) counter++;             // increment counter if we find a matching token
            else break;
        }
        if (counter == getNumToWin())                    // if counter reaches the numtowin, return true
            return true;
        return false;
    }

    /**
     * @pre r > 0 and r <= rows in board
     * @pre c > 0 and c <= columns in board
     * @pre p == any character in the player array
     * @param r the row of the board
     * @param c the column of the board
     * @param p the player's token
     * @return true if the character at board.get(c).get(r) matches p
     */
    public boolean checkVertWin(int r, int c, char p){  // this function checks vertically from our current position for a win
        int counter = 0;                                // count variable
        for (int i = r; i >= 0; i--) {                  // check below for matching tokens
            if (whatsAtPos(i, c) == p) counter++;            // increment counter if we find a matching token
            else break;
        }
        if (counter == getNumToWin())                   // if counter reaches the numtowin, return true
            return true;
        return false;
    }

    /**
     * @pre r > 0 and r <= rows in board
     * @pre c > 0 and c <= columns in board
     * @pre p == any character in the player array
     * @param r the row of the board
     * @param c the column of the board
     * @param p the player's token
     * @return true if the character at board.get(c).get(r) matches p
     */
    public boolean checkDiagWin(int r, int c, char p) { // this function checks diagonally from our current position for a win
        int counter = 1;                                // count variable
        int rTemp = r;                                  // temporary variables that represent row and col that we will change in our while loops while checking
        int cTemp = c;
        while(rTemp > 0 && cTemp > 0) {                 // checks down and to the left of our current position for a matching token
            rTemp--;
            cTemp--;
            if(p == whatsAtPos(rTemp, cTemp)) counter++;     // increment counter if we find a matching token
        }
        rTemp = r;
        cTemp = c;
        while(rTemp < getNumRows()-1 && cTemp < getNumColumns()-1) { // checks up and to the right of our current position for a matching token
            rTemp++;
            cTemp++;
            if(p == whatsAtPos(rTemp, cTemp)) counter++;     // increment counter if we find a matching token
        }
        if (counter == getNumToWin()) return true;      // if counter reaches the numtowin, return true

        rTemp = r;
        cTemp = c;
        counter = 1;
        while(rTemp > 0 && cTemp < getNumColumns()-1) { // checks down and to the right of our current position for a matching token
            rTemp--;
            cTemp++;
            if(p == whatsAtPos(rTemp, cTemp)) counter++;     // increment counter if we find a matching token
        }
        rTemp = r;
        cTemp = c;
        while(rTemp < getNumRows()-1 && cTemp > 0) {   // checks up and to the left of our current position for a matching token
            rTemp++;
            cTemp--;
            if(p == whatsAtPos(rTemp, cTemp)) counter++;    // increment counter if we find a matching token
        }
        if (counter == getNumToWin()) return true;     // if counter reaches the numtowin, return true
        return false;
    }

    /**
     * @pre r > 0 and r <= number of rows
     * @pre c > 0 and c <= number of columns
     * @param r the row of the index
     * @param c the column of the index
     * @post pos == a index of players[] or empty
     * @return the character at position [r][c] that was passed in
     */
    public char whatsAtPos(int r, int c){ // this function returns the character that's at board[r][c]
        if(r < board.get(c).size()){
            return board.get(c).get(r);
        }
        else{
            return ' ';
        }
    }

    /**
     * @Override toString
     * @post each index should be one of our player tokens or empty
     * @return the board in string format
     */
    public String toString(){ // this function prints the game board
        String toprow = "";
        String str = "";
        for(int i = 0; i < COLS; i++) {     // loop through columns to print the column labels
            if(i < 10) {                    // for single digit columns, add a space
                toprow = toprow + "| " + i;
            }
            else{
                toprow = toprow + "|" + i;  // for double digit columns, no space needed
            }
        }
        toprow = toprow + "|\n";

        for(int r = ROWS-1; r >= 0; r--){   // loop through rows
            for(int c = 0; c < COLS; c++) { // loop through columns
                str = str + "| " + whatsAtPos(r, c); // print what's at index [r][c]
            }
            str = str + "|\n";
        }
        return toprow + str;                // return the concatenated string
    }

    /**
     * @pre cols => 3 and <= 100
     * @pre rows => 3 and <= 100
     * @pre numtowin => 3 and <= 25
     * @param cols the number of columns in the board
     * @param rows the number of rows in the board
     * @param numtowin the number of tokens needed to win
     * @post board with ROWS number of rows and COLS number of columns
     */
    public GameBoardMem(int cols, int rows, int numtowin){ // constructor for game board
        ROWS = rows;
        COLS = cols;
        NUMTOWIN = numtowin;
        for(int i = 0; i < ROWS; i++){
            board.put(i, new ArrayList<Character>());
        }
    }

    /**
     * @return number of rows in our 2D array
     * @post getNumRows = ROWS
     */
    public int getNumRows(){ // this function returns the number of rows
        return ROWS;
    }

    /**
     * @return number of columns in our 2D array
     * @post getNumColumns = COLS
     */
    public int getNumColumns(){ // this function returns the number of columns
        return COLS;
    }

    /**
     * @return number of tokens in a row required to win
     * @post getNumToWin = number of tokens in a row required to win
     */
    public int getNumToWin(){  // this function returns the number in a row needed to win
        return NUMTOWIN;
    }

}
