package cpsc2150.homeworks.hw1;


public class BoardPosition {
    private int rowPos;
    private int colPos;
    private char charStored;

    /**
     * @requires (newRowPos >= 0 && newRowPos <= 7) and (newColPos >= 0 && newColPos <= 7)
     * and (newCharStored == X || newCharStored == Y)
     * @ensures (rowPos = newRowPos) and (colPos = newColPos) and (charStored = newCharStored)
     */
    public BoardPosition(int newRowPos, int newColPos, char newCharStored) {
        rowPos = newRowPos;
        colPos = newColPos;
        charStored = newCharStored;
    }

    /**
     *@ensures [getRow returns rowPos]
     */
    public int getRow() {
        return rowPos;
    }

    /**
     *@ensures [getColumn returns colPos]
     */
    public int getColumn() {
        return colPos;
    }

    /**
     *@ensures [getPlayer returns charStored]
     */
    public char getPlayer() {
        return charStored;
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof  BoardPosition)) {
            return false;
        }

        BoardPosition bp = (BoardPosition)  o;

        if ((bp.getRow() == rowPos) && (bp.getColumn() == colPos)
                && (bp.getPlayer() == charStored)) {
            return true;
        }

        return false;
    }

    @Override
    public String toString() {
        String str = "Player " + charStored + " at position " + rowPos + ", " + colPos;
        return str;
    }

}
