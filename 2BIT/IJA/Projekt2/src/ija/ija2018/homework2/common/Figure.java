package ija.ija2018.homework2.common;

public interface Figure {

    public static enum Type {
        Pawn,
        Rook
    }

    /**
     * Tests whether the stone is white
     *
     * @return true if the stone is white
     */
    public boolean isWhite();

    public Type getType();

    public void setCurrentField(Field field);

    public Field getCurrentField();

    public int getRow();

    public int getCol();

    /**
     * Get colour of current disk
     *
     */
    public String getState();
}
