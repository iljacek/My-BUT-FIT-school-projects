package ija.ija2018.homework2.common;

public interface Game {


     /**
     * Moves the stone to specified field
     *
     * @param figure
     * @param field
     * @return true if action was succesfull
     */
    public boolean move(Figure figure, Field field);


    /**
     * Undo the last move
     *
     */
    public void undo();

    //undo - pri move si uloz board a vrat sa potom k nemu... alebo ukladaj samotny move
}
