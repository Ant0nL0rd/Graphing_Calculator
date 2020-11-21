#pragma once
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

/// <summary>
/// widget on which graph is being drawn.
/// </summary>
class GraphBuilder : public QWidget
{
    Q_OBJECT

public:
    GraphBuilder(int _numID, QWidget *parent = NULL);

    ~GraphBuilder();

    std::list<solver *> *solvers;  /**< calculating classes for strings */ 

    std::list<QString *> *Slines;  /**< strings for calculation */ 

    std::list<QString *> *SlinesOld;  /**< strings for chacking if strings used for calculation were changed */ 
private:
    bool LMB; /**< signifies if left mouse button is currently pressed */ 
    double scale; /**< variable for scale of depicted graphics */
    double offx, offy; /**< variable for current location */
    int startx, starty, offbegx, offbegy; /**< current location offset */
    std::vector<std::pair<int, int>> *dots;  /**< dots for graphic drawing. have size >= widget width*/

    /// <summary>
    /// takes dots locations for current graphic drawing
    /// </summary>
    /// <param name="solv">equation on which current graph is being built</param>
    void build(solver* solv);

protected:
    /// <summary>
    /// deletes graphic layout of this graphing widget if widget is closed
    /// </summary>
    /// <param name="event">Qt event variable</param>
    void closeEvent(QCloseEvent *event);
    /// <summary>
    /// changes number of dots for graphic building
    /// </summary>
    /// <param name="event">Qt event variable</param>
    void resizeEvent(QResizeEvent *event);
    /// <summary>
    /// updates all graphics in widget
    /// </summary>
    /// <param name="event">Qt event variable</param>
    void paintEvent(QPaintEvent *event);
    /// <summary>
    /// captures mouse wheel movement. changes scope of graphics.
    /// </summary>
    /// <param name="event">Qt event variable</param>
    void wheelEvent(QWheelEvent *event);
    /// <summary>
    /// captures mouse press button events.
    /// </summary>
    /// <param name="event">Qt event variable</param>
    void mousePressEvent(QMouseEvent *event);
    /// <summary>
    /// captures release events.
    /// </summary>
    /// <param name="event">Qt event variable</param>
    void mouseReleaseEvent(QMouseEvent *event);
    /// <summary>
    /// captures movement of mouse.
    /// </summary>
    /// <param name="event">Qt event variable</param>
    void mouseMoveEvent(QMouseEvent *event);

public slots:
	/// <summary>
	/// updates equations for graph builing
	/// </summary>
	void formatUpdate();
signals:
    /// <summary>
    /// deletes graphing line if it's widget is closed.
    /// </summary>
    void sendClosed();

};

/// <summary>
/// Command line for graphing widget.
/// </summary>
struct GraphLayout : public QWidget {
    Q_OBJECT
public:
    QLabel *lbl; /**< Name for graphing line. */ 
    std::list<QLineEdit*> lines; /**< Editable lines for entering equations on which graph will be built*/ 
    QPushButton *ok, *close, *plus, *minus; 
    QHBoxLayout *mainHor, *linesHor;
    QSpinBox *sb; /**< spinning box for transparancy changing*/ 
    QSlider *sl;  /**< slider for transparancy changing*/ 
    GraphBuilder *GS;  /**< Graphing widget*/ 

    /// <summary>
    /// creates body for graphing line
    /// </summary>
    /// <param name="_numID">number of this graphing line</param>
    /// <param name="parent">parantual widget</param>
    GraphLayout(int _numID, QWidget *parent = NULL);
    ~GraphLayout();

private slots:
    /// <summary>
    /// deletes current graphing line on button click
    /// </summary>
    void deleteGraphClicked();
    /// <summary>
    /// takes strings from editable lines to use in GS for graph drowing
    /// </summary>
    void okClicked();
    /// <summary>
    /// sets transparency
    /// </summary>
    /// <param name="optlvl">opticality level</param>
    void setTransparency(int optlvl);
    /// <summary>
    /// adds a new editable line
    /// </summary>
    void plusClicked();
    /// <summary>
    /// removes last editable line if number of lines more then 1
    /// </summary>
    void minusClicked();


signals:
    /// <summary>
    /// signal connected to formUpdate() in GS. Initialte updating of parsing changed strings.
    /// </summary>
    void sendForm();
    /// <summary>
    /// signal connected to deleteGraphClicked() from parantual widget. Deletes this graphing line.
    /// </summary>
    /// <param name="abc">main layout of this graphing line</param>
    void sendDeleteGraph(GraphLayout *abc);

};

/// <summary>
/// main window of application.
/// </summary>
class MyWindow : public QWidget {
	Q_OBJECT

public:
	MyWindow(QWidget *parent = 0);
private:
    QPushButton *addGraphingLine; /**< button will add Graphing line on click. */ 
    QPushButton *viewInfo; /**< button will show info window. */
    QLabel *InfoWidget;
    QHBoxLayout *menueHor; /**< Layout contains top line widgets. */ 
    QVBoxLayout *mainVer;  /**< Layout contains menue line and Graphing lines. */ 
    std::list<GraphLayout*> graphLines;  /**< list for graphing line layouts. */ 
    int linesNum; //**< How many graphing lines already were created */

    /// <summary>
    /// adds a new graphing line to main vertical layout
    /// </summary>
    void addGraphLine();

    /// <summary>
    /// On Qt event: close all grphing widgets if main window closed
    /// </summary>
    /// <param name="event">Qt event variable</param>
    void closeEvent(QCloseEvent *event);
private slots:
    /// <summary>
    /// slot connected to addGraphLine();
    /// </summary>
    void addGraphClicked();
    /// <summary>
    /// slot connected to ViewInfo();
    /// </summary>
    void viewInfoClicked();

    /// <summary>
    /// deletes last in list graphing line and removes it from main vertical layout
    /// </summary>
    /// <param name="curLay">graphing line layout is being removed</param>
    void deleteGraphLine(GraphLayout *curLay);
};

