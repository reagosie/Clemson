package cpsc2150.ConnectX;

import org.junit.Test;
import static org.junit.Assert.*;

public class TestIGameBoard {

    private IGameBoard factory(int r, int c, int numtowin){
        return new GameBoardFast(r, c, numtowin);
    }

    private String arrayToString(char[][] board){
        String toprow = "";
        String str = "";
        for(int i = 0; i < board[0].length; i++) {     // loop through columns to print the column labels
            if(i < 10) {                    // for single digit columns, add a space
                toprow = toprow + "| " + i;
            }
            else{
                toprow = toprow + "|" + i;  // for double digit columns, no space needed
            }
        }
        toprow = toprow + "|\n";

        for(int r = board.length-1; r >= 0; r--){   // loop through rows
            for(int c = 0; c < board[0].length; c++) { // loop through columns
                str = str + "| " + board[r][c]; // print what's at index [r][c]
            }
            str = str + "|\n";
        }
        return toprow + str;                // return the concatenated string
    }

    @Test
    public void constructorTest_Small(){
        IGameBoard gb = factory(3, 3, 3);
        char [][] empty = new char[3][3];
        for(int i = 0; i < gb.getNumRows(); i++){
            for(int j = 0; j < gb.getNumColumns(); j++){
                empty[i][j] = ' ';
            }
        }
        assertEquals(gb.getNumRows(), 3);
        assertEquals(gb.getNumColumns(), 3);
        assertEquals(gb.getNumToWin(), 3);
        assertEquals(gb.toString(), arrayToString(empty));
    }

    @Test
    public void constructorTest_Large(){
        IGameBoard gb = factory(100, 100, 25);
        char [][] empty = new char[100][100];
        for(int i = 0; i < gb.getNumRows(); i++){
            for(int j = 0; j < gb.getNumColumns(); j++){
                empty[i][j] = ' ';
            }
        }
        assertEquals(gb.getNumRows(), 100);
        assertEquals(gb.getNumColumns(), 100);
        assertEquals(gb.getNumToWin(), 25);
        assertEquals(gb.toString(), arrayToString(empty));
    }

    @Test
    public void constructorTest_Med(){
        IGameBoard gb = factory(50, 50, 10);
        char [][] empty = new char[50][50];
        for(int i = 0; i < gb.getNumRows(); i++){
            for(int j = 0; j < gb.getNumColumns(); j++){
                empty[i][j] = ' ';
            }
        }
        assertEquals(gb.getNumRows(), 50);
        assertEquals(gb.getNumColumns(), 50);
        assertEquals(gb.getNumToWin(), 10);
        assertEquals(gb.toString(), arrayToString(empty));
    }

    @Test
    public void checkIfFreeTest_ColumnFull(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 0); //column should be full at this point

        assertTrue(!act.checkIfFree(0));
    }

    @Test
    public void checkIfFreeTest_NoneEmptyNoneFull(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 2);
        act.placeToken('X', 3);
        act.placeToken('O', 4);

        assertTrue(act.checkIfFree(0));
    }


    @Test
    public void checkIfFreeTest_OnlyEmptyIs0(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('X', 1);
        act.placeToken('O', 2);
        act.placeToken('X', 3);
        act.placeToken('O', 4);

        assertTrue(act.checkIfFree(0));
    }

    @Test
    public void checkHorizWinTest_OWinBottomRow(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 1);
        act.placeToken('X', 0);
        act.placeToken('O', 2); //should signal a horizontal win for O here

        assertTrue(act.checkHorizWin(0, 2, 'O'));
    }

    @Test
    public void checkHorizWinTest_XWinMiddleRow(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 2);
        act.placeToken('X', 3);
        act.placeToken('O', 4);
        act.placeToken('X', 0);
        act.placeToken('O', 1);
        act.placeToken('X', 2);
        act.placeToken('O', 3);
        act.placeToken('X', 4);
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 4);
        act.placeToken('X', 2);
        act.placeToken('O', 1);
        act.placeToken('X', 3); //should signal a horizontal win for X here

        assertTrue(act.checkHorizWin(2, 3, 'X'));
    }

    @Test
    public void checkHorizWinTest_NoWin(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 1);
        act.placeToken('X', 0);

        assertTrue(!act.checkHorizWin(2, 3, 'X'));
    }

    @Test
    public void checkHorizWinTest_NoTokenNoWin(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 1);
        act.placeToken('X', 0);

        assertTrue(!act.checkHorizWin(4, 4, 'X'));
    }

    @Test
    public void checkHorizWinTest_OWinFromNotLastPlacedToken(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 1);
        act.placeToken('X', 0);
        act.placeToken('O', 2); //should signal a horizontal win for O here

        assertTrue(act.checkHorizWin(0, 0, 'O'));
    }

    @Test
    public void checkVertWinTest_OWinLeftSide(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 0); //should signal a vertical win for O here

        assertTrue(act.checkVertWin(2,0,'O'));
    }

    @Test
    public void checkVertWinTest_OWinRightSide(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 4);
        act.placeToken('X', 1);
        act.placeToken('O', 4);
        act.placeToken('X', 1);
        act.placeToken('O', 4); //should signal a vertical win for O here

        assertTrue(act.checkVertWin(2,4,'O'));
    }

    @Test
    public void checkVertWinTest_XWinMiddleCol(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 2);
        act.placeToken('O', 1);
        act.placeToken('X', 2);
        act.placeToken('O', 3);
        act.placeToken('X', 2); //should signal a vertical win for X here

        assertTrue(act.checkVertWin(2,2,'X'));
    }

    @Test
    public void checkVertWinTest_NoWin(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 0);
        act.placeToken('X', 1);

        assertTrue(!act.checkVertWin(1,0,'O')); //there is a token here but it is not a winning one
    }

    @Test
    public void checkVertWinTest_OWinFromNotLastPlacedToken(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 0); //should signal a vertical win for O here

        //this is a vertical win but we check from a token that was not the last one placed
        assertTrue(act.checkVertWin(0,0,'O'));
    }

    @Test
    public void checkDiagWinTest_OWinFromBottomLeft_CheckLast(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 1);
        act.placeToken('X', 2);
        act.placeToken('O', 2);
        act.placeToken('X', 0);
        act.placeToken('O', 2); //should signal a diagonal win for O here

        assertTrue(act.checkDiagWin(2,2,'O'));
    }

    @Test
    public void checkDiagWinTest_OWinFromBottomRight_CheckLast(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 4);
        act.placeToken('X', 3);
        act.placeToken('O', 3);
        act.placeToken('X', 2);
        act.placeToken('O', 2);
        act.placeToken('X', 4);
        act.placeToken('O', 2); //should signal a diagonal win for O here

        assertTrue(act.checkDiagWin(2,2,'O'));
    }

    @Test
    public void checkDiagWinTest_OWinFromBottomLeft_CheckFirst(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 1);
        act.placeToken('X', 2);
        act.placeToken('O', 2);
        act.placeToken('X', 0);
        act.placeToken('O', 2); //should signal a diagonal win for O here

        assertTrue(act.checkDiagWin(0,0,'O'));
    }

    @Test
    public void checkDiagWinTest_OWinFromBottomRight_CheckFirst(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 4);
        act.placeToken('X', 3);
        act.placeToken('O', 3);
        act.placeToken('X', 2);
        act.placeToken('O', 2);
        act.placeToken('X', 4);
        act.placeToken('O', 2); //should signal a diagonal win for O here

        assertTrue(act.checkDiagWin(0,4,'O'));
    }

    @Test
    public void checkDiagWinTest_NoWin(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 1);
        act.placeToken('X', 2);
        act.placeToken('O', 2);
        act.placeToken('X', 0);

        //there is a token here but not a winning one
        assertTrue(!act.checkDiagWin(1,2,'O'));
    }

    @Test
    public void checkDiagWinTest_NoToken(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 1);
        act.placeToken('X', 2);
        act.placeToken('O', 2);
        act.placeToken('X', 0);

        //there is not even a token here so no win is found
        assertTrue(!act.checkDiagWin(0,4,'O'));
    }

    @Test
    public void checkDiagWinTest_ThisIsAVertWin(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 0); //should signal a vertical win for O here

        //there should be no Diag win here because it is a Vert win
        assertTrue(!act.checkDiagWin(2,0,'O'));
    }

    @Test
    public void checkDiagWinTest_ThisIsAHorizWin(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 1);
        act.placeToken('X', 0);
        act.placeToken('O', 2); //should signal a horizontal win for O here

        //there should be no Diag win here because it is a Horiz win
        assertTrue(!act.checkDiagWin(0, 2, 'O'));
    }

    @Test
    public void checkTieTest_BoardFull(){
        IGameBoard act = factory(3,3,3);
        //board is 3 by 3, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 0);
        act.placeToken('X', 2);
        act.placeToken('O', 2);
        act.placeToken('X', 2);
        act.placeToken('O', 1);
        act.placeToken('X', 1);
        act.placeToken('O', 1); //the 3x3 board should be full now

        assertTrue(act.checkTie());
    }

    @Test
    public void checkTieTest_BoardNotEmptyNotFull(){
        IGameBoard act = factory(3,3,3);
        //board is 3 by 3, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 0);
        act.placeToken('X', 2);

        assertTrue(!act.checkTie());
    }

    @Test
    public void checkTieTest_BoardEmpty(){
        IGameBoard act = factory(3,3,3);
        assertTrue(!act.checkTie());
    }

    @Test
    public void checkTieTest_VertWinNotATie(){
        IGameBoard act = factory(3,3,3);
        //board is 3 by 3, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 0);
        act.placeToken('X', 1);
        act.placeToken('O', 0); //should signal a vertical win for O here

        //there should not be a tie because there is a Vert win
        assertTrue(!act.checkTie());
    }

    @Test
    public void whatsAtPosTest_CheckBottomLeftForO(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 1);

        assertEquals(act.whatsAtPos(0, 0), 'O');
    }

    @Test
    public void whatsAtPosTest_CheckBottomRightForO(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 4);
        act.placeToken('X', 4);
        act.placeToken('O', 3);

        assertEquals(act.whatsAtPos(0, 4), 'O');
    }

    @Test
    public void whatsAtPosTest_CheckTopLeftForO(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 0);

        assertEquals(act.whatsAtPos(4, 0), 'O');
    }

    @Test
    public void whatsAtPosTest_CheckTopRightForO(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 4);
        act.placeToken('X', 4);
        act.placeToken('O', 4);
        act.placeToken('X', 4);
        act.placeToken('O', 4);

        assertEquals(act.whatsAtPos(4, 4), 'O');
    }

    @Test
    public void whatsAtPosTest_CheckMiddleForX(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 4);
        act.placeToken('X', 3);
        act.placeToken('O', 2);
        act.placeToken('X', 1);
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 1);
        act.placeToken('X', 2);
        act.placeToken('O', 3);
        act.placeToken('X', 4);
        act.placeToken('X', 2);

        assertEquals(act.whatsAtPos(2, 2), 'X');
    }

    @Test
    public void whatsAtPosTest_CheckMiddleForBlank(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 4);
        act.placeToken('X', 3);
        act.placeToken('O', 2);
        act.placeToken('X', 1);
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 1);
        act.placeToken('X', 2);
        act.placeToken('O', 3);
        act.placeToken('X', 4);

        assertEquals(act.whatsAtPos(2, 2), ' ');
    }

    @Test
    public void whatsAtPosTest_CheckMiddleTopForBlank(){
        IGameBoard act = factory(5,5,3);
        //board is 5 by 5, numtowin is 3
        act.placeToken('O', 4);
        act.placeToken('X', 3);
        act.placeToken('O', 2);
        act.placeToken('X', 1);
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 1);
        act.placeToken('X', 2);
        act.placeToken('O', 3);
        act.placeToken('X', 4);

        assertEquals(act.whatsAtPos(4, 2), ' ');
    }

    @Test
    public void placeTokenTest_PlaceBottomLeftAndCheck(){
        IGameBoard act = factory(5, 5, 3);
        char [][] empty = new char[5][5];
        for(int i = 0; i < act.getNumRows(); i++){
            for(int j = 0; j < act.getNumColumns(); j++){
                empty[i][j] = ' ';
            }
        }

        act.placeToken('O', 0);
        empty[0][0] = 'O';

        assertEquals(act.toString(), arrayToString(empty));
    }

    @Test
    public void placeTokenTest_PlaceBottomRightAndCheck(){
        IGameBoard act = factory(5, 5, 3);
        char [][] empty = new char[5][5];
        for(int i = 0; i < act.getNumRows(); i++){
            for(int j = 0; j < act.getNumColumns(); j++){
                empty[i][j] = ' ';
            }
        }

        act.placeToken('O', 4);
        empty[0][4] = 'O';

        assertEquals(act.toString(), arrayToString(empty));
    }

    @Test
    public void placeTokenTest_PlaceTopLeftAndCheck(){
        IGameBoard act = factory(5, 5, 3);
        char [][] empty = new char[5][5];
        for(int i = 0; i < act.getNumRows(); i++){
            for(int j = 0; j < act.getNumColumns(); j++){
                empty[i][j] = ' ';
            }
        }

        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 0);
        act.placeToken('X', 0);
        act.placeToken('O', 0);

        empty[0][0] = 'O';
        empty[1][0] = 'X';
        empty[2][0] = 'O';
        empty[3][0] = 'X';
        empty[4][0] = 'O';

        assertEquals(act.toString(), arrayToString(empty));
    }

    @Test
    public void placeTokenTest_PlaceTopRightAndCheck(){
        IGameBoard act = factory(5, 5, 3);
        char [][] empty = new char[5][5];
        for(int i = 0; i < act.getNumRows(); i++){
            for(int j = 0; j < act.getNumColumns(); j++){
                empty[i][j] = ' ';
            }
        }

        act.placeToken('O', 4);
        act.placeToken('X', 4);
        act.placeToken('O', 4);
        act.placeToken('X', 4);
        act.placeToken('O', 4);

        empty[0][4] = 'O';
        empty[1][4] = 'X';
        empty[2][4] = 'O';
        empty[3][4] = 'X';
        empty[4][4] = 'O';

        assertEquals(act.toString(), arrayToString(empty));
    }

    @Test
    public void placeTokenTest_PlaceMiddleAndCheck(){
        IGameBoard act = factory(5, 5, 3);
        char [][] empty = new char[5][5];
        for(int i = 0; i < act.getNumRows(); i++){
            for(int j = 0; j < act.getNumColumns(); j++){
                empty[i][j] = ' ';
            }
        }

        act.placeToken('O', 2);
        act.placeToken('X', 2);
        act.placeToken('O', 2);
        act.placeToken('X', 2);
        act.placeToken('O', 2);

        empty[0][2] = 'O';
        empty[1][2] = 'X';
        empty[2][2] = 'O';
        empty[3][2] = 'X';
        empty[4][2] = 'O';

        assertEquals(act.toString(), arrayToString(empty));
    }

}
