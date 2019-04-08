package ija.ija2018.homework2.game;

import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;
import ija.ija2018.homework2.common.Game;

import java.util.LinkedList;

public class ChessGame implements Game {

//    private Figure currentFigure;
    final private LinkedList<HistoryRecord> moves = new LinkedList<>();
    private Board gameBoard;

    public ChessGame(Board gameBoard) {
        this.gameBoard = gameBoard;
    }


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

    boolean rookCheckBetween(Field.Direction direction, Figure figure, Field field)
    {
        Field controlledField = figure.getCurrentField();
        while (!controlledField.equals(field)) {
            controlledField = controlledField.nextField(direction);
            if (!controlledField.equals(field) && !controlledField.isEmpty())
            {
                undo();
                return false;
            }
        }
        if (!controlledField.isEmpty())
        {
            if (controlledField.get().isWhite() != figure.isWhite())
                field.remove(field.get());
            else
            {
                undo();
                return false;
            }
        }
        figure.getCurrentField().remove(figure);
        field.put(figure);
        return true;
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
            case Rook:
                if (figure.getCol() == field.getCol()) {
                    if (figure.getRow() < field.getRow()) {
                        return rookCheckBetween(Field.Direction.U, figure, field);
                    } else if (figure.getRow() > field.getRow()) {
                        return rookCheckBetween(Field.Direction.D, figure, field);
                    }
                    return true;
                } else if (figure.getRow() == field.getRow()) {
                    if (figure.getCol() < field.getCol()) {
                        return rookCheckBetween(Field.Direction.R, figure, field);
                    } else if (figure.getCol() > field.getCol()) {
                        return rookCheckBetween(Field.Direction.L, figure, field);
                    }
                    return true;
                }

                //bile mozu ist up , cerne down
            case Pawn:
                if (figure.getCol() == field.getCol()) {
                    return PawnCheckBetween(figure, field);
                }
        }
        undo();
        return false;
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
