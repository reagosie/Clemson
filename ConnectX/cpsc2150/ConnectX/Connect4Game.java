/*  Reagan Leonard
    CPSC 2150
    HW 3
    10/28/18
 */
package cpsc2150.ConnectX;

import java.util.*;
import java.lang.*;

/**
 * @inv turn >= 0
 * @inv turnCount >= 0
 */
public class Connect4Game {
    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        int cols = 0;
        int rows = 0;
        int numtowin = 0;
        int numplayers = 0;
        //char players[i = 'X';  // start as player X's turn
        char[] players = new char[10];
        char gametype;
        int c = 0;
        boolean gameover = false;  // boolean to control main while loop
        //boolean placed = false;            // boolean to check if we //placed a token or not
        System.out.println("How many players?"); //ask for user input
        numplayers = scan.nextInt();
        while (numplayers < 2 || numplayers > 10) {  //check for valid input
            if (numplayers < 2) {
                System.out.println("Must be at least 2 players.");  //error messages for invalid input
                System.out.println("How many players?");
                numplayers = scan.nextInt();
            } else {
                System.out.println("Must be 10 players or fewer.");
                System.out.println("How many players?");
                numplayers = scan.nextInt();
            }
        }
        for (int i = 0; i < numplayers; i++) {
            System.out.print("Enter the character to represent player ");
            System.out.println(i+1);
            char q = scan.next().charAt(0);
            q = Character.toUpperCase(q);
            scan.nextLine();
            for (int j = 0; j < numplayers; j++) {
                if(q == players[j]) {
                    System.out.print(players[j]);
                    System.out.println(" is already taken as a player token!");
                    System.out.print("Enter the character to represent player ");
                    System.out.println(j+1);
                    q = scan.next().charAt(0);
                    scan.nextLine();
                    j = -1;
                }
            }
            players[i] = q;
        }
        System.out.println("How many rows should be on the board?"); //ask for user input
        rows = scan.nextInt();
        while (rows < 3 || rows > 100) {  //check for valid input
            if (rows < 3) {
                System.out.println("Must have at least 3 rows.");  //error messages for invalid input
                rows = scan.nextInt();
            } else {
                System.out.println("Can have at most 100 rows.");
                rows = scan.nextInt();
            }
        }

        System.out.println("How many columns should be on the board?"); //ask for user input
        cols = scan.nextInt();
        while (cols < 3 || cols > 100) {   //check for valid input
            if (cols < 3) {
                System.out.println("Must have at least 3 columns.");  //error messages for invalid input
                cols = scan.nextInt();
            } else {
                System.out.println("Can have at most 100 columns.");
                cols = scan.nextInt();
            }
        }

        System.out.println("How many in a row to win?");   //ask for user input
        numtowin = scan.nextInt();
        while (numtowin < 3 || numtowin > 25) {   //check for valid input
            if (numtowin < 3) {
                System.out.println("Must have at least 3 in a row to win.");  //error messages for invalid input
                numtowin = scan.nextInt();
            } else {
                System.out.println("Can have at most 25 in a row to win.");
                numtowin = scan.nextInt();
            }
        }

        System.out.println("Would you like a Fast Game (F/f) or a Memory Efficient Game (M/m)?");
        gametype = scan.next().charAt(0);
        scan.nextLine();
        while(gametype != 'F' && gametype != 'f' && gametype != 'M' && gametype != 'm') {
            System.out.println("Please enter F or M");
            gametype = scan.next().charAt(0);
            scan.nextLine();
        }
        IGameBoard board;
        if (gametype == 'F' || gametype == 'f') {
            //FAST IMPLEMENTATION
            board = new GameBoardFast(cols, rows, numtowin);  // construct game board for fast implementation
        }
        else {
            //MEMORY IMPLEMENTATION
            board = new GameBoardMem(cols, rows, numtowin);  // construct game board for memory implementation
            }

        System.out.println(board.toString());                   //print the blank game board

        while (!gameover) {                                     // loop through players until gameover is true (when there's a win or a tie)
            for (int i = 0; i < numplayers;) {

                System.out.println("Player " + players[i] + ", what column do you want to place your marker in?"); //ask for user input
                int choice = scan.nextInt();
                while (choice < 0 || choice > board.getNumColumns()-1) {   //check for valid input
                    if (choice < 0) {
                        System.out.println("Column cannot be less than 0");  //error messages for invalid input
                        choice = scan.nextInt();
                    } else if (choice > board.getNumColumns()-1) {
                        System.out.println("Column cannot be greater than " + (board.getNumColumns()-1));  //error messages for invalid input
                        choice = scan.nextInt();
                    }
                }
                if (board.checkIfFree(choice)) {                    // check if the column they selected is full or not
                    board.placeToken(players[i], choice);        // if not full, place the token
                    //placed = true;                                  // change //placed to true because we //placed a token
                    i++;
                } else {                                            // if column is full,
                    while (!board.checkIfFree(choice)) {
                        System.out.println("Column is full");       // print error message
                        choice = scan.nextInt();
                        if (board.checkIfFree(choice)) {            // try to place token again
                            board.placeToken(players[i], choice);
                            //placed = true;
                            i++;
                        }
                    }
                }

                c = choice;                                         // reset c to choice

                if (board.checkForWin(c)) {                         // check for win
                    gameover = true;
                    System.out.println(board.toString());           // print board
                    //i--;
                    players[i] = players[i - 1];                    //switch players back to whoever actually won
                    System.out.println("Player " + players[i] + " won!"); // print win message
                    playAgain();                                             // call function to play again
                    players[i] = players[i - 1];                                     // switch players
                    System.out.println(board.toString());
                    //placed = false;
                }
                if (board.checkTie()) {                                 // check for tie
                    gameover = true;
                    System.out.println(board.toString());               // print board
                    System.out.println("It's a tie!");                  // print tie message
                    playAgain();                                        // call function to play again
                    //players[i] = players[0];                                // switch players
                }
                System.out.println(board.toString());
            }
        }
    }

    private static void playAgain() {  // this function asks if the user wants to play again; restarts main if yes, ends program if no
        System.out.println("Would you like to play again? Y/N");  // ask for user input
        Scanner scan = new Scanner(System.in);
        String input = scan.next();
        while (!input.equals("Y") && !input.equals("N") && !input.equals("y") && !input.equals("n")) {  // check for valid input
            System.out.println("Would you like to play again? Y/N");
            input = scan.next();
        }
        if (input.equals("N") || input.equals("n")) {   // if "no", exit program
            System.exit(0);
        } else if (input.equals("Y") || input.equals("y")) { // if "yes", start main over again
            main(null);
        } else {
            playAgain();                                // call this function again until we get valid input
        }
    }
}