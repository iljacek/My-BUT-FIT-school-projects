
package ija.ija2018.homework2.game;

import ija.ija2018.homework2.common.Field;

public class Board {

    private int size;
    private BoardField[][] boardFields;


    /**
     * Constructor for Board object
     *
     * @param size dimension of side of the board
     */
    public Board(int size) {
        this.size = size;
        this.boardFields = new BoardField[this.size][this.size];
        Field adjacentField = null;

        //int col, row, index = 1;
        for (int row = 1; row <= size; row++) {
            for (int col = 1; col <= size; col++) {
                this.boardFields[col - 1][row - 1] = new BoardField(col, row);
            }
        }

        for (int row = 1; row <= size; row++) {
            for (int col = 1; col <= size; col++) {
                for (Field.Direction dir : Field.Direction.values()) {
                    switch (dir) {
                        case D:
                            adjacentField = getField(col, row - 1);
                            break;
                        case L:
                            adjacentField = getField(col - 1, row);
                            break;
                        case LD:
                            adjacentField = getField(col - 1, row - 1);
                            break;
                        case LU:
                            adjacentField = getField(col - 1, row + 1);
                            break;
                        case R:
                            adjacentField = getField(col + 1, row);
                            break;
                        case RD:
                            adjacentField = getField(col + 1, row - 1);
                            break;
                        case RU:
                            adjacentField = getField(col + 1, row + 1);
                            break;
                        case U:
                            adjacentField = getField(col, row + 1);
                            break;
                    }
                    if (adjacentField != null) {
                        this.boardFields[col - 1][row - 1].addNextField(dir, adjacentField);
                    }
                }
            }
        }
    }

    /**
     * Get field specified by indexes col and row.
     *
     * @param col number of collumn
     * @param row number of row
     * @return indexed field
     */
    public Field getField(int col, int row) {
        if (col < 1 || col > this.size || row < 1 || row > this.size)
            return null;
        else
            return this.boardFields[col - 1][row - 1];
    }


    /**
     * Get the dimensions of board.
     *
     * @return dimension of board
     */
    public int getSize() {
        return this.size;
    }

}