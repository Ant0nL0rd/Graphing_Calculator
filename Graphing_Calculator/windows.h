#include "calc.h"
#include <utility>
#include <iostream>
#include <vector>
#include <list>

#include <QtWidgets/QMainWindow>
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QCloseEvent>

#include <QLabel>
#include <QlineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>

#include <QHBoxLayout>
#include <QVBoxLayout>


class GraphBuilder : public QWidget
{
    Q_OBJECT

public:
    GraphBuilder(int _numID, QWidget *parent = NULL);

    ~GraphBuilder();

    std::list<solver *> *solvers;

    std::list<QString *> *Slines;

    std::list<QString *> *SlinesOld;
private:
    bool LMB;
    double scale, offx, offy;
    int startx, starty, offbegx, offbegy;
    std::vector<std::pair<int, int>> *dots;

    void build(std::vector<std::pair<int, int>> *dots, solver* solv);

protected:
    void closeEvent(QCloseEvent *event);

    void resizeEvent(QResizeEvent *event);

    void paintEvent(QPaintEvent *event);

    void wheelEvent(QWheelEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

public slots:
	void formatUpdate();
signals:
    void sendClosed();

};

struct GraphLayout : public QWidget {
    Q_OBJECT
public:
    QLabel *lbl;
    std::list<QLineEdit*> lines;
    QPushButton *ok, *close, *plus, *minus;
    QHBoxLayout *mainHor, *linesHor;
    QSpinBox *sb;
    QSlider *sl;
    GraphBuilder *GS;

    GraphLayout(int _numID, QWidget *parent = NULL);
    ~GraphLayout();

private slots:
    void deleteGraphClicked();
    void okClicked();
    void setTransparency(int optlvl);
    void plusClicked();
    void minusClicked();


signals:
    void sendForm();
    void sendDeleteGraph(GraphLayout *abc);

};

class MyWindow : public QWidget {
	Q_OBJECT

public:
	MyWindow(QWidget *parent = 0);
private:
    QPushButton *addGraph;
    QHBoxLayout *menueHor;
    QVBoxLayout *mainVer;
    std::list<GraphLayout*> graphLines;
    int linesNum;

    void addGraphLine();
    void closeEvent(QCloseEvent *event);
private slots:
    void addGraphClicked();
    void deleteGraphLine(GraphLayout *curLay);
};

