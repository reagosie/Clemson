package cpsc2150.homeworks.hw1;

import java.util.*;

//This class contains only a main function and serves as the driver for the game, creating an gameboard
//for each new game and asking players for input, checking space validity and checking for wins or draws.

/**
 * @invariant (whoseTurn, representing which players turn it is, is either X or Y]
 */
public class GameScreen {

    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);

        boolean newGame = true;

        while (newGame) {

            char whoseTurn = 'X';

            GameBoard myBoard = new GameBoard();
            System.out.println(myBoard.toString());
            System.out.println();

            boolean didWin = false;

            while (!didWin) {

                System.out.println("Player " + whoseTurn + " Please enter your ROW");
                int inputRow = scanner.nextInt();
                System.out.println("Player " + whoseTurn + " Please enter your COLUMN");
                int inputCol = scanner.nextInt();
                BoardPosition newBoardPosition = new BoardPosition(inputRow, inputCol, whoseTurn);

                boolean isValid = false;

                while (!isValid) {
                    if (!myBoard.checkSpace(newBoardPosition)) {
                        isValid = false;
                        System.out.println(myBoard.toString());
                        System.out.println();
                        System.out.println("That space is unavailable, please pick again");
                        System.out.println("Player " + whoseTurn + " Please enter your ROW");
                        inputRow = scanner.nextInt();
                        System.out.println("Player " + whoseTurn + " Please enter your COLUMN");
                        inputCol = scanner.nextInt();
                        newBoardPosition = new BoardPosition(inputRow, inputCol, whoseTurn);
                    } else
                        isValid = true;
                }

                myBoard.placeMarker(newBoardPosition);

                if (!myBoard.checkForWinner(newBoardPosition)) {
                    if(myBoard.checkForDraw()) {
                        System.out.println("The game ended in a draw!");
                        didWin = true;
                    }
                    System.out.println(myBoard.toString());
                    System.out.println();

                    if (whoseTurn == 'X') {
                        whoseTurn = 'O';
                    } else {
                        whoseTurn = 'X';
                    }
                }
                else
                    didWin = true;
            }

            System.out.println("Player " + whoseTurn + " wins!\nWould you like to play again? Y/N");

            String restart = scanner.next();
            if (!restart.equals("Y") && !restart.equals("y")) newGame = false;
        }
    }
}

