package ija.ija2018.homework2.game;

import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;

public class HistoryRecord {
    final private Figure startFigure;
    final private Figure endFigure;
    final private Field startField;
    final private Field endField;

    public HistoryRecord(Figure startFigure, Figure endFigure, Field startField, Field endField) {
        this.startFigure = startFigure;
        this.endFigure = endFigure;
        this.startField = startField;
        this.endField = endField;
    }

    public Figure getStartFigure() {
        return startFigure;
    }

    public Figure getEndFigure() {
        return endFigure;
    }

    public Field getStartField() {
        return startField;
    }

    public Field getEndField() {
        return endField;
    }
}
