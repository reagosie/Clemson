package cpsc2150.ConnectX;

/**
 * The controller class will handle communication between our View and our Model (IGameBoard)
 *
 * This is where you will write code
 *
 * You will need to include your IGameBoard interface
 * and both of the IGameBoard implementations from Homework 3
 * If your code was correct you will not need to make any changes to your IGameBoard implementation class
 */

public class ConnectXController {
    //our current game that is being played
    private IGameBoard curGame;


    //The screen that provides our view
    private ConnectXView screen;



    public static final int MAX_PLAYERS = 10;
    //our play tokens are hard coded. We could make a screen to get those from the user, but
    //I want to keep this example simple
    private char[] players = {'X', 'O', 'Y', 'Z', 'A', 'K', 'E', 'J', 'N', 'H'};

    private int numPlayers;
    private int playerindex = 0;
    private char player;
    boolean gameover = false;


    /**
     *
     * @param model the board implementation
     * @param view the screen that is shown
     * @post the controller will respond to actions on the view using the model.
     */
    ConnectXController(IGameBoard model, ConnectXView view, int np){
        this.curGame = model;
        this.screen = view;
        numPlayers = np;

    }

    /**
     *
     *
     * @param col the column of the activated button
     * @post will allow the player to place a token in the column if it is not full, otherwise it will display an error
     * and allow them to pick again. Will check for a win as well. If a player wins it will allow for them to play another
     * game hitting any button
     */
    public void processButtonClick(int col) {
        //initialize row to 0 because the first token in every column will be in row 0
        int row = 0;
        //set current player equal to whatever index of players[] we are on
        player = players[playerindex];
        //boolean to control for loop that searches for last placed token so we can place above that
        boolean foundtoken = false;

        //if statement that keeps the game going until we have either found a win or a tie
        if(!gameover) {
            //if column is full, print error message
            //otherwise, run the rest of the function
            if(!curGame.checkIfFree(col)){
                screen.setMessage("Column is full! Try again");
            }
            else {
                //to find row, use for loop that calls whatAtPos until it gets to something that isn't a space and then go 1 up
                for (int j = curGame.getNumRows() - 1; j >= 0 && !foundtoken; j--) {
                    if (curGame.whatsAtPos(j, col) != ' ') {
                        row = j + 1;
                        foundtoken = true;
                    }
                }
                //placeToken affects model (our copy of the board)
                curGame.placeToken(player, col);
                //setMarker affects view/screen (their copy of the board)
                screen.setMarker(row, col, player);

                //if there is a tie, set gameover variable to true so that the next button click will call newGame()
                if (curGame.checkTie()) {
                    gameover = true;
                    //print error message
                    screen.setMessage("It's a tie! Press any button to play again");
                }
                //if a win is found, set gameover variable to true so that the next button click will call newGame()
                else if (curGame.checkForWin(col)) {
                    gameover = true;
                    //print error message
                    screen.setMessage("Player " + players[playerindex] + " won! Press any button to play again");
                }
                //if neither a win or tie is found, keep playing by incrementing player index
                else if (!gameover) {
                    playerindex++;
                    //if the index is greater than or equal to numPlayers, start the index back at the beginning of the array
                    if (playerindex >= numPlayers) {
                        playerindex = 0;
                    }
                    screen.setMessage("It is " + players[playerindex] + "'s turn.");
                }
            }
        }
        //if game is over, start a new game the next time processButtonClick is called
        else {
            newGame();
        }
    }

    /**
     * This method will start a new game by returning to the setup screen and controller
     */
    private void newGame()
    {
        //close the current screen
        screen.dispose();
        //start back at the set up menu
        SetupView screen = new SetupView();
        SetupController controller = new SetupController(screen);
        screen.registerObserver(controller);
    }
}
