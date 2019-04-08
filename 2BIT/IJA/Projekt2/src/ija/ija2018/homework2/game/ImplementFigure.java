
package ija.ija2018.homework2.game;

import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;

public class ImplementFigure implements Figure {

    private boolean isWhite;
    private Field currentField;
    final private Type type;
//    private int row, col;


    /**
     * Constructor for Board object
     *
     * @param isWhite dimension of side of the board
     */
    public ImplementFigure(boolean isWhite, Type type) {
        this.isWhite = isWhite;
        this.type = type;
    }


    @Override
    public boolean isWhite() {
        return this.isWhite;
    }

    @Override
    public Type getType() {
        return this.type;
    }

    @Override
    public void setCurrentField(Field field) {
        this.currentField = field;
    }

    @Override
    public Field getCurrentField() {
        return this.currentField;
    }

    @Override
    public int getRow() {
        return this.currentField.getRow();
    }

    @Override
    public int getCol() {
        return this.currentField.getCol();
    }


    @Override
    public String getState()
    {
        String colour;
        if (isWhite())
            colour = "W";
        else
            colour = "B";

        String type;
        if (this.type.equals(Type.Rook))
            type = "V";
        else if (this.type.equals(Type.Pawn))
            type = "P";
        else
            type = "Unknown";


        return type + "[" + colour +"]" + this.getCurrentField().getPosition();
    }


    @Override
    public boolean equals(Object obj) {
        if ((obj != null) && (obj instanceof ImplementFigure) && (this.getClass() == obj.getClass())) {
            final ImplementFigure other = (ImplementFigure) obj;
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