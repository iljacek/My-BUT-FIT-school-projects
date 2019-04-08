package ija.ija2018.homework1;/*
 * IJA 2018/2019
 * Ukol 1
 */

import ija.ija2018.homework1.board.Board;
import ija.ija2018.homework1.board.BoardField;
import ija.ija2018.homework1.board.Disk;
import ija.ija2018.homework1.board.Field;


/**
 *
 * @author koci
 */
public class Homework1 {

    /**
     * Spusti testy.
     */
    public static void main(String[] args) {
        boolean assertOn = false;
        assert assertOn = true;

        if (! assertOn) {
            System.out.println(">! Spustte s prepinacem -ea");
            return ;
        }

        testBoard();
        testDisks1();
        testDisks2();
        System.out.println("OK");
    }

    /**
     * Test vytvoreni a prace s hraci deskou (vcetne poli).
     */
    private static void testBoard() {
        Board board = new Board(8);

        assert board.getSize() == 8 : "Test: Velikost desky je 8";

        Field f1 = board.getField(2, 5);
        Field f2 = board.getField(1, 4);

        Field n2 = f1.nextField(Field.Direction.LD);
        assert n2.equals(f2)    : "Test: Spravne okoli pole [2,5] ve smeru LD je pole [1,4]";
    }

    /**
     * Test vytvoreni a prace s kameny.
     */
    private static void testDisks1() {
        Board board = new Board(8);

        assert board.getSize() == 8 : "Test: Velikost desky je 8";

        Field f1 = board.getField(4, 7);
        Field f2 = board.getField(5, 7);

        Disk d1 = new Disk(true);
        f1.put(d1);
        Disk d2 = new Disk(false);
        f2.put(d2);

        assert f1.nextField(Field.Direction.R).get().equals(d2) 
            : "Test: Kamen vlozeny vpravo od [4,7] je shodny s kamenem d2";
        assert ! f2.get().isWhite()   
            : "Test: Kamen na pozici [5,7] ma byt cerny";
        assert f1.nextField(Field.Direction.R).get().equals(f2.get())
            : "Test: Kamen vlozeny vpravo od [4,7] je shodny s kamenem na poli [5,7]";

        assert ! f2.remove(d1) : "Test: Kamen d1 nelze odebrat";
        assert ! f2.isEmpty()  : "Test: Pole f2 neni prazdne";
        assert f2.remove(d2)   : "Test: Kamen d2 lze odebrat";
        assert f2.isEmpty()    : "Test: Pole f2 je prazdne";
    }

    /**
     * Test vytvoreni a prace s kameny.
     */
    private static void testDisks2() {
        Board board = new Board(8);

        assert board.getSize() == 8 : "Test: Velikost desky je 8";

        Field f1 = board.getField(4, 3);
        Field f2 = board.getField(4, 5);
        Field f3 = board.getField(4, 8);
        Field f4 = board.getField(8, 3);

        Disk d1 = new Disk(true);
        f1.put(d1);
        Disk d2 = new Disk(false);
        f2.put(d2);

        assert ! d1.move(f3)   : "Test: Kamen nelze presunout (v ceste je jiny kamen)";
        assert ! f1.isEmpty()  : "Test: Pole f1 zustava obsazeno";
        assert f3.isEmpty()    : "Test: Pole f3 zustava prazdne";

        assert f4.isEmpty()    : "Test: Pole f4 je prazdne";
        assert d1.move(f4)     : "Test: Kamen lze presunout";
        assert f1.isEmpty()    : "Test: Pole f1 je prazdne";
        assert ! f4.isEmpty()  : "Test: Pole f4 je obsazeno";

        assert f4.get().equals(d1) : "Test: Kamen d1 je na f4";
    }

}
