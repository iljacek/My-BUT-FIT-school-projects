package ija.ija2018.homework2;

import ija.ija2018.homework2.common.Figure;
import ija.ija2018.homework2.common.Game;
import ija.ija2018.homework2.game.Board;
import ija.ija2018.homework2.game.CheckersGame;
import ija.ija2018.homework2.game.ChessGame;
import ija.ija2018.homework2.game.ImplementFigure;

public abstract class GameFactory {

    public static Game createChessGame(Board board) {

        board.getField(1, 2).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(2, 2).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(3, 2).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(4, 2).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(5, 2).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(6, 2).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(7, 2).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(8, 2).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(1, 1).put(new ImplementFigure(true, Figure.Type.Rook));
        board.getField(8, 1).put(new ImplementFigure(true, Figure.Type.Rook));

        board.getField(1, 7).put(new ImplementFigure(false, Figure.Type.Pawn));
        board.getField(2, 7).put(new ImplementFigure(false, Figure.Type.Pawn));
        board.getField(3, 7).put(new ImplementFigure(false, Figure.Type.Pawn));
        board.getField(4, 7).put(new ImplementFigure(false, Figure.Type.Pawn));
        board.getField(5, 7).put(new ImplementFigure(false, Figure.Type.Pawn));
        board.getField(6, 7).put(new ImplementFigure(false, Figure.Type.Pawn));
        board.getField(7, 7).put(new ImplementFigure(false, Figure.Type.Pawn));
        board.getField(8, 7).put(new ImplementFigure(false, Figure.Type.Pawn));
        board.getField(1, 8).put(new ImplementFigure(false, Figure.Type.Rook));
        board.getField(8, 8).put(new ImplementFigure(false, Figure.Type.Rook));

        return new ChessGame(board);
    }

    public static Game createCheckersGame(Board board){
        board.getField(1, 1).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(2, 2).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(3, 1).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(4, 2).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(5, 1).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(6, 2).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(7, 1).put(new ImplementFigure(true, Figure.Type.Pawn));
        board.getField(8, 2).put(new ImplementFigure(true, Figure.Type.Pawn));

        return new CheckersGame(board);
    }
}
