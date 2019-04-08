
package ija.ija2018.homework1.board;

public class Disk {

    private boolean isWhite;
    private Field currentField;


    /**
     * Constructor for Board object
     *
     * @param isWhite dimension of side of the board
     */
    public Disk(boolean isWhite) {
        this.isWhite = isWhite;
    }


    /**
     * Tests whether the stone is white
     *
     * @return true if the stone is white
     */
    public boolean isWhite() {
        return this.isWhite;
    }

    /**
     * Get field on which the disk is placed
     *
     * @param field field to be gained
     */
    public void getField(Field field) {
        this.currentField = field;
    }

    /**
     * Moves the stone to specified field
     *
     * @param moveTo field that represents final destination of the stone
     * @return true if action was succesfull
     */
    public boolean move(Field moveTo) {
        Field controlledField = moveTo;
        // for Tower
        if (this.currentField.getCol() == moveTo.getCol()) {
            if (this.currentField.getRow() < moveTo.getRow()) {
                while (!this.currentField.equals(controlledField)) {
                    if (!controlledField.isEmpty())
                        return false;
                    controlledField = controlledField.nextField(Field.Direction.D);
                }
                this.currentField.remove(this);
                moveTo.put(this);
            } else if (this.currentField.getRow() > moveTo.getRow()) {
                while (!this.currentField.equals(controlledField)) {
                    if (!controlledField.isEmpty())
                        return false;
                    controlledField = controlledField.nextField(Field.Direction.U);
                }
                this.currentField.remove(this);
                moveTo.put(this);
            }
            return true;
        } else if (this.currentField.getRow() == moveTo.getRow()) {
            if (this.currentField.getCol() < moveTo.getCol()) {
                while (!this.currentField.equals(controlledField)) {
                    if (!controlledField.isEmpty())
                        return false;
                    controlledField = controlledField.nextField(Field.Direction.L);
                }
                this.currentField.remove(this);
                moveTo.put(this);
            } else if (this.currentField.getCol() > moveTo.getCol()) {
                while (!this.currentField.equals(controlledField)) {
                    if (!controlledField.isEmpty())
                        return false;
                    controlledField = controlledField.nextField(Field.Direction.R);
                }
                this.currentField.remove(this);
                moveTo.put(this);
            }
            return true;
        }
        return false;
    }


    @Override
    public boolean equals(Object obj) {
        if ((obj != null) && (obj instanceof Disk) && (this.getClass() == obj.getClass())) {
            final Disk other = (Disk) obj;
            if (other.isWhite == this.isWhite) {
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