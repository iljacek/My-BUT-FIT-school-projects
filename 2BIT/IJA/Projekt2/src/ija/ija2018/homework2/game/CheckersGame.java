package ija.ija2018.homework2.game;

import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;
import ija.ija2018.homework2.common.Game;

import java.util.LinkedList;

public class CheckersGame implements Game {

    final private LinkedList<HistoryRecord> moves = new LinkedList<>();
    private Board gameBoard;

    boolean PawnCheckBetween(Figure figure, Field field) {
        if (figure.isWhite()) {
            if ((field.get() == null) && (figure.getRow() == field.getRow()-1)) {
                figure.getCurrentField().remove(figure);
                field.put(figure);
                return true;
            }
            else
            {
                undo();
                return false;
            }

        } else {
            if ((field.get() == null) && (figure.getRow() == field.getRow()+1)) {
                figure.getCurrentField().remove(figure);
                field.put(figure);
                return true;
            }
            else
            {
                undo();
                return false;
            }
        }
    }

    public CheckersGame(Board gameBoard) {
        this.gameBoard = gameBoard;
    }

    /**
     * Moves the stone to specified field
     *
     * @param figure figure to be moved
     * @param field field that represents final destination of the stone
     * @return true if action was succesfull
     */
    @Override
    public boolean move(Figure figure, Field field) {
        Field controlledField = figure.getCurrentField();
        Figure endFigure = field.get();

        moves.add(new HistoryRecord(figure,endFigure,figure.getCurrentField(),field));

        switch (figure.getType())
        {
                //bile mozu ist up , cerne down
            case Pawn:
                if (figure.getCol() == field.getCol()-1 || figure.getCol() == field.getCol()+1) {
                    return PawnCheckBetween(figure, field);
                }
                else
                {
                    undo();
                    return false;
                }
            default:
                undo();
                return false;
        }
    }

    @Override
    public void undo() {
        HistoryRecord lastMove;
        lastMove = moves.removeLast();
        lastMove.getEndField().remove(lastMove.getStartFigure());
        lastMove.getStartField().put(lastMove.getStartFigure());
        if (lastMove.getEndFigure() != null)
            lastMove.getEndField().put(lastMove.getEndFigure());
    }
}
