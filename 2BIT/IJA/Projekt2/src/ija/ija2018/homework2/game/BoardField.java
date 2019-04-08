
package ija.ija2018.homework2.game;


import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;

public class BoardField implements Field {

    private int col;
    private int row;
    private Figure figure;
    private Field[] adjacent;

    /**
     * Constructor for Board object
     *
     * @param col index of column
     * @param row index of row
     */
    public BoardField(int col, int row) {
        this.col = col;
        this.row = row;
        this.adjacent = new Field[8];
        this.figure = null;
    }

    @Override
    public void addNextField(Direction dirs, Field field) {
        int index = 0;
        for (Direction dir : Direction.values()) {
            if (dirs != dir)
                index++;
            else
                break;
        }
        this.adjacent[index] = field;
    }

    @Override
    public Figure get() {
        return this.figure;
    }

    @Override
    public boolean isEmpty() {
        return this.figure == null;
    }

    @Override
    public Field nextField(Direction dirs) {
        int index = 0;
        for (Direction dir : Direction.values()) {
            if (dirs != dir)
                index++;
            else
                break;
        }
        return this.adjacent[index];
    }

    @Override
    public boolean put(Figure figure) {
        if (isEmpty()) {
            this.figure = figure;
            this.figure.setCurrentField(this);
            return true;
        } else
            return false;
    }

    @Override
    public boolean remove(Figure figure) {
        if (isEmpty())
            return false;
        else if (this.figure != figure)
            return false;
        else {
            this.figure = null;
            return true;
        }
    }

    @Override
    public int getRow() {
        return this.row;
    }

    @Override
    public int getCol() {
        return this.col;
    }

    @Override
    public String getPosition() { return (this.getCol() + ":" + this.getRow());}


    @Override
    public boolean equals(Object obj) {
        if ((obj != null) && (obj instanceof BoardField) && (this.getClass() == obj.getClass())) {
            final BoardField other = (BoardField) obj;
            if ((other.row == this.row) && (other.col == this.col)) {
                return (true);
            }
        }
        return (false);
    }

    @Override
    public int hashCode() {
        return super.hashCode();
    }


}