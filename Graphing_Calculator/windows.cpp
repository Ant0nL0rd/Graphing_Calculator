#include "windows.h"

#define INFO "functions:\nsin(x), arcsin(x), sh(x)\ncos(x), arccos(x), ch(x)\ntan(x), arctan(x), th(x)\nexp(x), ln(x), floor(x)\n\
amodb, alogb (lna/lnb)\nconstants:\np=3.14159265358979323846\ne=2.71828182845904523536\ng=1.61803398874989484820\nEncolouring:\
\n\\r\\-rad, \\b\\-blue, \\g\\-green\n\\y\\-yellow, \\v\\-violet\n\\c\\-cyan, \\#ffffff\\-castom"

GraphBuilder::GraphBuilder(int _numID, QWidget *parent) : dots(new std::vector<std::pair<int, int>>(width() + 200)),
Slines(new std::list<QString*>), SlinesOld(new std::list<QString *>), solvers(new std::list<solver *>), scale(60), offx(0), offy(0), LMB(false) {
    Slines->push_back(new QString);
    SlinesOld->push_back(new QString);
    solvers->push_back(new solver("0"));
    setWindowTitle(QString::fromStdString("Graph" + std::to_string(_numID)));
    connect(this, SIGNAL(sendClosed()), parent, SLOT(deleteGraphClicked()));
    resize(600, 600);

    show();
}
GraphBuilder::~GraphBuilder() {
    for (auto it : *solvers) {
        delete it;
    }
    for (auto it : *Slines) {
        delete it;
    }
    for (auto it : *SlinesOld) {
        delete it;
    }
    delete solvers;
    delete dots;
}
void GraphBuilder::build(solver* solv) {
    //builds the points on which the graph will be drawn
    for (long long i = 0; i < width() && i < dots->size(); ++i) {
        double x = double((i - width() / 2) - offx) / scale;
        double y = -solv->count(x);
        (*dots)[i].first = width() / 2 + (x * scale);
        (*dots)[i].second = height() / 2 + (y * scale);
    }
}
void GraphBuilder::closeEvent(QCloseEvent *event) {
    emit sendClosed();//clears slot in main box
}
void GraphBuilder::resizeEvent(QResizeEvent *event) {
    if (width() > dots->size()) {
        dots->resize(width() + 100);
    }
}
void GraphBuilder::paintEvent(QPaintEvent *event) {
    QPainter *painter = new QPainter(this);
    // Background
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(0, 0, width(), height());
    //Coordinat lines
    painter->setPen(Qt::black);
    painter->drawLine(0, height() / 2 + offy, width(), height() / 2 + offy);
    painter->drawLine(width() / 2 + offx, 0, width() / 2 + offx, height());
    // center
    painter->drawLine(width() / 2 - 10, height() / 2 - 10, width() / 2 + 10, height() / 2 + 10);
    painter->drawLine(width() / 2 - 10, height() / 2 + 10, width() / 2 + 10, height() / 2 - 10);
    //coordinat numbers
    int numStep = 60;
    for (int i = (width() / 2) % numStep; i < width(); i += numStep) {//horizontal
        if (abs(i - width() / 2) < numStep) {
            painter->setPen(Qt::red);
        }
        painter->drawText(i, height(), QString::number((i - offx - width() / 2) / scale));
        if (abs(i - width() / 2) < numStep) {
            painter->setPen(Qt::black);
        }
    }
    for (int i = (height() / 2) % numStep; i < height(); i += numStep) {//vertical
        if (abs(i - height() / 2) < numStep) {
            painter->setPen(Qt::red);
        }
        painter->drawText(0, i, QString::number(-(i - offy - height() / 2) / scale));
        if (abs(i - height() / 2) < numStep) {
            painter->setPen(Qt::black);
        }
    }
    for (auto it : *solvers) {//paints graphs
        build(it);
        painter->setPen(Qt::black);
        if (it->seq[0] == '\\') {
            switch (it->seq[1]) {
            case 'v': painter->setPen("#9932cc"); break;
            case 'r': painter->setPen(Qt::red); break;
            case 'g': painter->setPen(Qt::green); break;
            case 'b': painter->setPen(Qt::blue); break;
            case 'y': painter->setPen(Qt::yellow); break;
            case 'c': painter->setPen(Qt::darkCyan); break;
            case '#': char colour[7];
                for (int i = 0; i < 7; ++i) {
                    colour[i] = it->seq[i + 1];
                }
                painter->setPen(colour);
            }
        }
        //paints lines between dots if point not too extreme
        for (int i = 1; i < width() - 1 && i < dots->size(); ++i) {
            if (abs((*dots)[i].second - (*dots)[i + 1].second) < height())
                painter->drawLine((*dots)[i].first + offx, (*dots)[i].second + offy, (*dots)[i + 1].first + offx, (*dots)[i + 1].second + offy);
        }
    }
    delete painter;
}
void GraphBuilder::wheelEvent(QWheelEvent *event) {
    //change scope on wheel movement
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        if (numSteps.y() > 0) {
            scale *= 1 / sqrt(2);
            offx *= 1 / sqrt(2);
            offy *= 1 / sqrt(2);
        } else {
            scale *= sqrt(2);
            offx *= sqrt(2);
            offy *= sqrt(2);
        }
    }
    this->update();
    event->accept();
}
void GraphBuilder::mousePressEvent(QMouseEvent *event) {
    if (event->button() == 1) {
        //Start tracking movement
        LMB = true;
        startx = event->localPos().x();
        starty = event->localPos().y();
        offbegx = offx;
        offbegy = offy;
    } else if (event->button() == 4) {
        //reset position on middle button click
        offx = 0;
        offy = 0;
        this->update();
    }
}
void GraphBuilder::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == 1) {
        LMB = false;
    }
}
void GraphBuilder::mouseMoveEvent(QMouseEvent *event) {
    //needed to track movement on click
    if (LMB) {
        offx = offbegx + (event->localPos().x() - startx);
        offy = offbegy + (event->localPos().y() - starty);
        this->update();
    }
}
void GraphBuilder::formatUpdate() {
    auto ita = Slines->begin();
    auto itb = SlinesOld->begin();
    for (auto it : *solvers) {
        if (std::strcmp((*ita)->toStdString().c_str(), (*itb)->toStdString().c_str())) {
            *(*itb) = *(*ita);
            it->parser((*ita)->toStdString());
        }
        ++ita;
        ++itb;
    }
    this->update();
}

GraphLayout::GraphLayout(int _numID, QWidget *parent) : lbl(new QLabel(QString::fromStdString("G" + std::to_string(_numID)))), ok(new QPushButton("Ok")), close(new QPushButton("Delete")), GS(new GraphBuilder(_numID, this)),
mainHor(new QHBoxLayout), linesHor(new QHBoxLayout), sb(new QSpinBox), sl(new QSlider(Qt::Horizontal)), plus(new QPushButton("+")), minus(new QPushButton("-")) {
    lines.push_back(new QLineEdit);//create body of graph entering line
    ok->setDefault(true);
    sl->setMaximum(100);
    sl->setMaximumWidth(150);
    sl->setMinimumWidth(100);
    sb->setMaximum(100);
    minus->setFixedWidth(28);
    plus->setFixedWidth(28);
    linesHor->addWidget(lines.front());
    mainHor->addWidget(lbl);
    mainHor->addWidget(minus);
    mainHor->addWidget(plus);
    mainHor->addLayout(linesHor);
    mainHor->addWidget(ok);
    mainHor->addWidget(close);
    mainHor->addWidget(sb);
    mainHor->addWidget(sl);
    connect(ok, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(close, SIGNAL(clicked()), this, SLOT(deleteGraphClicked()));
    connect(this, SIGNAL(sendForm()), GS, SLOT(formatUpdate()));
    connect(this, SIGNAL(sendDeleteGraph(GraphLayout *)), parent, SLOT(deleteGraphLine(GraphLayout *)));
    connect(sl, SIGNAL(valueChanged(int)), sb, SLOT(setValue(int)));
    connect(sb, SIGNAL(valueChanged(int)), sl, SLOT(setValue(int)));
    connect(sb, SIGNAL(valueChanged(int)), this, SLOT(setTransparency(int)));
    connect(plus, SIGNAL(clicked()), this, SLOT(plusClicked()));
    connect(minus, SIGNAL(clicked()), this, SLOT(minusClicked()));
    sb->setValue(100);

}
GraphLayout::~GraphLayout() {
    delete mainHor;
    //delete linesHor;
    for (auto it : lines) {
        delete it;
    }
    delete lbl;
    delete ok;
    delete close;
    delete plus;
    delete minus;
    delete sb;
    delete sl;
    delete GS;
}
void GraphLayout::okClicked() {
    auto ita = GS->Slines->begin();
    for (auto it : lines) {
        (*(*ita)) = it->text();
        if ((*ita)->isEmpty()) {
            (*(*ita)) = "0";
        }
        ++ita;
    }
    emit sendForm();
}
void GraphLayout::deleteGraphClicked() {
    emit sendDeleteGraph(this);
}
void GraphLayout::setTransparency(int optlvl) {
    GS->setWindowOpacity(float(optlvl) / 100);
}
void GraphLayout::plusClicked() {
    //add back Graphing line
    lines.push_back(new QLineEdit);
    GS->Slines->push_back(new QString);
    GS->SlinesOld->push_back(new QString);
    GS->solvers->push_back(new solver("0"));
    linesHor->addWidget(lines.back());
}
void GraphLayout::minusClicked() {
    //delete back graphing line
    if (lines.size() > 1) {
        delete lines.back();
        delete GS->Slines->back();
        delete GS->SlinesOld->back();
        delete GS->solvers->back();
        lines.pop_back();
        GS->Slines->pop_back();
        GS->SlinesOld->pop_back();
        GS->solvers->pop_back();
        GS->update();
    }
}

MyWindow::MyWindow(QWidget *parent) : QWidget(parent), linesNum(0), mainVer(new QVBoxLayout), 
addGraphingLine(new QPushButton("Add graphing line")), viewInfo(new QPushButton("View Info")), InfoWidget(new QLabel(INFO))
, menueHor(new QHBoxLayout) {
    setWindowTitle("Enter your Graph");
    InfoWidget->setStyleSheet("font: 18pt;");
    menueHor->addWidget(addGraphingLine);
    menueHor->addWidget(viewInfo);
    mainVer->addItem(menueHor);
    mainVer->addStretch(5);
    mainVer->setAlignment(Qt::AlignTop);
    setLayout(mainVer);
    connect(addGraphingLine, SIGNAL(clicked()), this, SLOT(addGraphClicked()));
    connect(viewInfo, SIGNAL(clicked()), this, SLOT(viewInfoClicked()));
    addGraphLine();
}
void MyWindow::addGraphLine() {
    ++linesNum;
    GraphLayout* setG = new GraphLayout(linesNum, this);
    graphLines.push_back(setG);
    mainVer->addLayout(setG->mainHor);
    setLayout(mainVer);
    this->show();
}
void MyWindow::deleteGraphLine(GraphLayout *curLay) {
    mainVer->removeItem(curLay->mainHor);
    graphLines.remove(curLay);
    delete curLay;
}
void MyWindow::addGraphClicked() {
    addGraphLine();
}
void MyWindow::viewInfoClicked() {
    InfoWidget->show();
}
void MyWindow::closeEvent(QCloseEvent *event) {
    while (!graphLines.empty()) {//terminate all windows.
        GraphLayout *it = graphLines.front();
        graphLines.pop_front();
        delete it;
    }
}
void MyWindow::keyPressEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat() && event->key() == 16777220) {
        for (auto it : graphLines) {
            it->okClicked();
        }
    }
}