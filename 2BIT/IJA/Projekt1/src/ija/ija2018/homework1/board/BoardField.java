
package ija.ija2018.homework1.board;


public class BoardField implements Field {

    private int col;
    private int row;
    private Disk disk;
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
        this.disk = null;
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
    public Disk get() {
        return this.disk;
    }

    @Override
    public boolean isEmpty() {
        return this.disk == null;
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
    public boolean put(Disk disk) {
        if (isEmpty()) {
            this.disk = disk;
            this.disk.getField(this);
            return true;
        } else
            return false;
    }

    @Override
    public boolean remove(Disk disk) {
        if (isEmpty())
            return false;
        else if (this.disk != disk)
            return false;
        else {
            this.disk = null;
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