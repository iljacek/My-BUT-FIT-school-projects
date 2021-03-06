
package ija.ija2018.homework1.board;

public interface Field {

    public static enum Direction {
        D,
        L,
        LD,
        LU,
        R,
        RD,
        RU,
        U;
    }

    /**
     * Add adjacent field in specified direction
     *
     * @param dirs direction in which the field is added
     * @param field the field which is being added
     */
    public void addNextField(Field.Direction dirs, Field field);

    /**
     * Get disk from active field
     *
     * @return disk which lays on the field, or null, if the field is empty
     */
    public Disk get();

    /**
     * Check if active field has disk on it
     *
     * @return true, if the field is empty
     */
    public boolean isEmpty();

    /**
     * Get adjacent field in specified direction
     *
     * @param dirs direction in which the adjacent field is searched
     * @return specified field, or null in case the field does not exist
     */
    public Field nextField(Field.Direction dirs);

    /**
     * Put selected disk to active field
     *
     * @param disk disk to be put on the field
     * @return true if action was succesfull
     */
    public boolean put(Disk disk);

    /**
     * Remove selected disk from active field
     *
     * @param disk disk to be removed
     * @return true if action was succesfull
     */
    public boolean remove(Disk disk);

    /**
     * Get row index of the field
     *
     * @return row index
     */
    public int getRow();

    /**
     * Get column index of the field
     *
     * @return column index
     */
    public int getCol();

}