/*
 * IJA 2018/19 - Testovaci trida pro ukol 2.
 */

package ija.ija2018.homework2;

import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;
import ija.ija2018.homework2.common.Game;
import ija.ija2018.homework2.game.Board;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

/**
 *
 * @author koci
 */
public class Homework2Test {

    public Homework2Test() {
    }

    @Before
    public void setUp() {
    }

    /**
     * Testovaci metoda. Testuje vytvoreni hry sachy a zakladni operace.
     */
    @Test
    public void testChessGame() {
        Board board = new Board(8);
        Game game = GameFactory.createChessGame(board);

        Assert.assertEquals("Na [1,1] je vez.", "V[W]1:1", board.getField(1, 1).get().getState());
        Assert.assertEquals("Na [8,1] je vez.", "V[W]8:1", board.getField(8, 1).get().getState());
        for (int i = 2; i <= 7; i++) {
            Assert.assertTrue("[" + i + ",1] je prazdne.", board.getField(i, 1).isEmpty());
        }
        for (int i = 1; i <= 8; i++) {
            Assert.assertEquals("Na [" + i + ",2] je pesec.", "P[W]"+i+":2", board.getField(i, 2).get().getState());
        }

        Assert.assertEquals("Na [1,8] je vez.", "V[B]1:8", board.getField(1, 8).get().getState());
        Assert.assertEquals("Na [8,8] je vez.", "V[B]8:8", board.getField(8, 8).get().getState());
        for (int i = 2; i <= 7; i++) {
            Assert.assertTrue("[" + i + ",1] je prazdne.", board.getField(i, 1).isEmpty());
        }

        Figure figure;
        Field field;

        // cerne figury
        figure = board.getField(3, 7).get();
        field = board.getField(3, 8);
        Assert.assertFalse("Presun pesce na [3,8]", game.move(figure, field));
        field = board.getField(3, 6);
        Assert.assertTrue("Presun pesce na [3,6]", game.move(figure, field));
        Assert.assertEquals("Na [3,6] je pesec.", "P[B]3:6", board.getField(3, 6).get().getState());

        // bile figury
        figure = board.getField(1, 1).get();
        field = board.getField(1, 3);
        Assert.assertFalse("Presun veze na [1,3]", game.move(figure, field));

        figure = board.getField(1, 2).get();
        field = board.getField(1, 3);
        Assert.assertTrue("Presun pesce na [1,3]", game.move(figure, field));
        field = board.getField(1, 4);
        Assert.assertTrue("Presun pesce na [1,4]", game.move(figure, field));

        figure = board.getField(1, 1).get();
        field = board.getField(1, 3);
        Assert.assertTrue("Presun veze na [1,3]", game.move(figure, field));
        field = board.getField(3, 3);
        Assert.assertTrue("Presun veze na [3,3]", game.move(figure, field));
        field = board.getField(3, 6);
        Assert.assertTrue("Presun veze na [3,6] (sebrani cerneho pesce)", game.move(figure, field));

        Assert.assertEquals("Na [3,6] je vez.", "V[W]3:6", board.getField(3, 6).get().getState());

        // jeden krok zpet
        game.undo();
        Assert.assertEquals("Na [3,6] je pesec.", "P[B]3:6", board.getField(3, 6).get().getState());
        Assert.assertEquals("Na [3,3] je vez.", "V[W]3:3", board.getField(3, 3).get().getState());

        // druhy krok zpet
        game.undo();
        Assert.assertEquals("Na [1,3] je vez.", "V[W]1:3", board.getField(1, 3).get().getState());
    }

    /**
     * Testovaci metoda. Testuje vytvoreni hry dama a zakladni operace.
     */
    @Test
    public void testCheckersGame() {
        Board board = new Board(8);
        Game game = GameFactory.createCheckersGame(board);

        for (int i = 1; i <= 8; i=i+2) {
            Assert.assertEquals("Na [" + i + ",1] je pesec.", "P[W]"+i+":1", board.getField(i, 1).get().getState());
        }
        for (int i = 2; i <= 8; i=i+2) {
            Assert.assertEquals("Na [" + i + ",2] je pesec.", "P[W]"+i+":2", board.getField(i, 2).get().getState());
        }

        Figure figure;
        Field field;

        figure = board.getField(3, 1).get();
        field = board.getField(2, 2);
        Assert.assertFalse("Presun pesce na [3,2]", game.move(figure, field));

        figure = board.getField(4, 2).get();
        field = board.getField(3, 3);
        Assert.assertTrue("Presun pesce na [3,3]", game.move(figure, field));
        field = board.getField(5, 3);
        Assert.assertFalse("Presun pesce na [5,3]", game.move(figure, field));

        // jeden krok zpet
        game.undo();

        field = board.getField(5, 3);
        Assert.assertTrue("Presun pesce na [5,3]", game.move(figure, field));

    }

}