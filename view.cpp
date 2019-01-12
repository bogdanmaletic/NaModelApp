/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "view.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include "callout.h"
#include <QtGui/QMouseEvent>
#include <QGraphicsLineItem>

//#include "samplecontrols.h"
//Ovo ubacujemo da testiramo nas reader objekat
//#include "readerobject.h"

//umesto njega treba da dodamo da bi mogli
// da ga koristimo u slotu
#include "freqsparplain.h"

#include <QDebug>
#include <QDial>
#include <QPushButton>

#include <QtCharts/QValueAxis>
#include <QtMath>
#include <QLogValueAxis>
#include <QtCharts/QPolarChart>

#include <QElapsedTimer>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QApplication>


View::View(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_coordX(0),
      m_coordY(0),
      m_chart(0),
      m_tooltip(0),
      ourDataMarker(nullptr),
      ourSeries(nullptr),
      our_points(nullptr)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // chart
    m_chart = new QChart;

    //    this->adjustSize();
    //    this->sizePolicy()
    //za jednake width i heihgt ne dolazi do iskrivljenja grafika, pogotovo smita
    //    m_chart->setMinimumSize(640, 480);
    m_chart->setMinimumSize(640,640);
    //    m_chart->setMinimumSize(480,480);
    //    m_chart->setMinimumSize(480,480);
    //Hmmm ovo bi smo morali da dodamo da resimo problem sa pointerima
    //    m_chart->setMaximumSize(640,480);
    //    fitInView(m_chart->plotArea());
    m_chart->setMaximumSize(640,640);
    //    m_chart->setMaximumSize(480,480);
    //    fitInView(m_chart);

    //    setFixedSize(640,640);
    //    setMinimumSize(640,800);
    //    adjustSize();
    //    parent->adjustSize();

    //Title i legenenda nam ne treba
    //    m_chart->setTitle("Hover the line to show callout. Click the line to make it stay");
    //    m_chart->legend()->hide();
    m_chart->legend()->hide();

    //    Ove serije cemo za sada morati da ukinemo da bi smo iscrtali kako treba
    //    QLineSeries *series = new QLineSeries;
    //    series->append(1, 3);
    //    series->append(4, 5);
    //    series->append(5, 4.5);
    //    series->append(7, 1);
    //    series->append(11, 2);
    //    m_chart->addSeries(series);

    //    QSplineSeries *series2 = new QSplineSeries;
    //    series2->append(1.6, 1.4);
    //    series2->append(2.4, 3.5);
    //    series2->append(3.7, 2.5);
    //    series2->append(7, 4);
    //    series2->append(10, 2);
    //    m_chart->addSeries(series2);


    //Zapravo ovo pravi problem sa osama
    //    m_chart->createDefaultAxes();
    //    QValueAxis *axisX = new QValueAxis;
    axisX = new QValueAxis;
    //    QValueAxis *axisY = new QValueAxis;
    axisY = new QValueAxis;
    m_chart->setAcceptHoverEvents(true);


    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(m_chart);

    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height());
    m_coordX->setText("X: ");
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height());
    m_coordY->setText("Y: ");

    //Nazalost i ovo cemo morati da ukinemo
    //    connect(series, &QLineSeries::clicked, this, &View::keepCallout);
    //    connect(series, &QLineSeries::hovered, this, &View::tooltip);

    //    connect(series2, &QSplineSeries::clicked, this, &View::keepCallout);
    //    connect(series2, &QSplineSeries::hovered, this, &View::tooltip);


    this->setMouseTracking(true);

    //Jos da vidimo da dodamo liniju
    //Ok radi...Aj samo da vidimo i kako da je pomerimo,verovatno cemo morati
    // da je pomerimo ne iz glavnog prozora vec iz programa...
    // !Ocistiti
    //    ourLine = new QGraphicsLineItem(10,0,10,m_chart->size().height(),m_chart);

    //Sada da dodamo nas prozor sa kontrolama
    // Ovaj ako se izbaci pravi problem

    //TODO Ovo izbaciti... Preko NA Modela uctitavamo podatke

    //    sampleControl = new SampleControls(parent);
    //    sampleControl->show();

    //sada da ih ispovezujemo...samo da vidimo kako

    //Aj da probamo ovako
    //TODO QDial nam za sada ne treba
    //    QDial* ourDial = sampleControl->getDial();

    //    qDebug() << "Ok sada imamo pointer" << ourDial;
    //Ovo sada radi...
    // ! I ovo ocistiti
    //    connect(ourDial,SIGNAL(valueChanged(int)),this,SLOT(moveLine(int)));

    //hmm zapravo imamo problem sa marginama i sa celim coordinatama
    ourHeight = m_chart->size().height();
    ourWidth = m_chart->size().width();
    //    qDebug() << "Sirina: " << ourWidth << ", a visina: " << ourHeight;
    //    ourDialStepSize = ourWidth/100;

    //Malo ce biti zajebano da se sljaka sa chartovima i markerima
    // posto problem je u modelu koji imamo a to je da moramo da vidimo
    // na koji nacin da vidimo da li ima preseka sa nasom linijom
    // Kasnije cemo je zameniti sa markerom
    //    m_chart->scene()->addItem(ourLine);
    //    m_chart->scene()->collidingItems(ourLine)

    //Ne treba ovako vec moramo da dodamo postojeci callout
    //OVO smo ukinuli samo da vidimo za hover elemente kako radi...
    //Morali smo da ga dodamo na kraj posto tek moramo da definisemo serije
    //    ourDataMarker = new Callout(m_chart);
    //    //Ovo je tacka samo da vidimo da li marker radi
    //    // Pogledati dobro vrednosti tacke...
    //    QPointF *ourTestPoint = new QPointF(m_chart->mapToValue(QPointF(0,0),series1));
    //    ourDataMarker->setText(QString("Ovaj je nas X: %1 \nY: %2 ").arg(ourTestPoint->x()).arg(ourTestPoint->y()));
    //    ourDataMarker->setAnchor(*ourTestPoint);
    //    qDebug() << "Da proverimo nasu tacku na adresi" << ourTestPoint << ", cija je vrednost " << *ourTestPoint;
    //    ourDataMarker->setZValue(12);
    //    ourDataMarker->updateGeometry();
    //    ourDataMarker->show();
    //    qDebug() << ourDataMarker;

    //Aj da probamo da povezemo tooltip sa ovim
    //Sledeca linija nam treba za marker povezan sa serijom, pa mora da bude dole definisana!!!
    //    ourSeries = series1;
    //    qDebug() << "Definisana serija je " << series;
    //    qDebug() << "A nova serija je " << ourSeries;
    //    ourDial->setMaximum(ourSeries->count() - 1);

    //Ovo radi...
    //Dole ponovo definisano da izbegnemo probleme posto smo serije isto dole definisali !!!
    //    connect(ourDial,SIGNAL(valueChanged(int)),this,SLOT(moveOurTooltip(int)));

    //    qDebug() << "Maksimum od naseg diala je sada " << ourDial->maximum();

    //sada da implementiramo ovo za izbor serije
    //    ourSeriesInList.append(series);
    //    ourSeriesInList.append(series2);

    //    qDebug() << ourSeriesInList;
    //    maxNumberOfSeries = ourSeriesInList.count();
    //    qDebug() << maxNumberOfSeries;

    //Ovo da dobijemo handler za button u drugom prozoru kao sto smo dial
    //Ponovo smo kontrole povezali na kraju da, posle definisanja serija

    //TODO ne treba za sada izbaciti
    //    QPushButton *ourBtnChangeSeries = sampleControl->getButtonChangeSeries();

    //    connect(ourBtnChangeSeries,SIGNAL(pressed()),this,SLOT(changeSeries()));

    //Test naseg reader objekta
    //TODO izbaciti
    //    testReadObj = new ReaderObject(this);
    //    testReadObj->loadFile();

    //Ok cita ga kako treba
    //    testReadObj->simpleWholeSRaport();

    //Aj da pokusamo da dodamo seriju
    //    QLineSeries *series3 = new QLineSeries;
    series1 = new QLineSeries;

    //Ove ostale serije nam ne trebaju
    //Dvojke nam trebaju trenutno za calData
    series2 = new QLineSeries;
    //    series3 = new QLineSeries;
    //    series4 = new QLineSeries;

    //    qreal maxXOfSeries(0);
    //    qreal minXOfSeries(0);
    //    qreal maxYOfSeries(0);
    //    qreal minYOfSeries(0);

    //    QPointF pointOfMax(0,0);
    //    QPointF pointOfMin(0,0);

    //Ovde cemo da dodajemo procitane podatke u seriju.
    //Kasnije cemo da vidimo kako cemo ovo izmenimo da bude citljivije

    //I ovo nam ne treba izbaciti
    //    for (int var = 0; var < testReadObj->getNumberOfPoints(); ++var) {
    //        QPointF samplePoint = QPointF(testReadObj->getFormatedData()->getOneDataColumn(var).getFrequency(),
    //                                      testReadObj->getFormatedData()->getOneDataColumn(var).getMagnitudeS().at(0));


    //        if (pointOfMax.y() < samplePoint.y()) {
    //            pointOfMax = samplePoint;
    //        }else if (pointOfMin.x() > samplePoint.y()) {
    //            pointOfMin = samplePoint;
    //        }


    //TODO izbaciti

    //        series1->append(samplePoint);


    //        //Ovo je za drugu seriju
    //        samplePoint = QPointF(testReadObj->getFormatedData()->getOneDataColumn(var).getFrequency(),
    //                              testReadObj->getFormatedData()->getOneDataColumn(var).getMagnitudeS().at(1));

    //        series2->append(samplePoint);

    //        samplePoint = QPointF(testReadObj->getFormatedData()->getOneDataColumn(var).getFrequency(),
    //                              testReadObj->getFormatedData()->getOneDataColumn(var).getMagnitudeS().at(2));

    //        series3->append(samplePoint);

    //        samplePoint = QPointF(testReadObj->getFormatedData()->getOneDataColumn(var).getFrequency(),
    //                              testReadObj->getFormatedData()->getOneDataColumn(var).getMagnitudeS().at(3));

    //        series4->append(samplePoint);


    //        //Aj sada da vidimo minimume i maximume
    //    }


    //    // Ok radi ovo
    //    series1->setColor(Qt::red);
    //    series2->setColor(Qt::yellow);
    //    qDebug() << "Maksimum od serije 1(crvena)" << findMaxOfSeries(series1);
    //    qDebug() << "Maksimum od serije 2(zuta)" << findMaxOfSeries(series2);


    //    //Aj da probamo ovo
    //    QPen pen(QRgb(0xfdb157));
    //    pen.setWidth(2);
    //    series1->setPen(pen);

    //    maxOfSeries << pointOfMax.y() << pointOfMin.y();
    //    qDebug() << maxOfSeries;

    //    series1->pointsVector().

    //    qDebug() << "Serija 1: " << series1->count();
    //    qDebug() << "Serija 2: " << series2->count();
    //    qDebug() << "Serija 3: " << series3->count();
    //    qDebug() << "Serija 4: " << series4->count();


    //Sada da dodamo seriju
    m_chart->addSeries(series1);
    //    series1->hide();
    // Ne treba da dodajemo ostale serije
    m_chart->addSeries(series2);
    //    m_chart->addSeries(series3);
    //    m_chart->addSeries(series4);

    //    m_chart->adj
    connect(series1, &QLineSeries::hovered, this, &View::tooltip);

    //aj probamo i za ostale(ne treba)
    connect(series2,&QLineSeries::hovered,this, &View::tooltip);
    //    connect(series2, &QLineSeries::hovered, this, &View::tooltip);
    //    series->hide();
    //    series2->hide();

    //    series->clear();
    //    series2->clear();

    //    m_chart->updateGeometry();
    //Hmmm malo nekorektno radi, verovatno problem u razlikama za prva dva niza
    //pravi problem posto je Y vrednost veoma mala
    //    m_chart->axisX()->setRange(0,20);
    //    m_chart->axisY()->setRange(-100,20);



    //    axisX->setRange(0,400);
    //    axisX->setRange(1000,1500);
    axisX->setRange(500,1500);
    //    axisX->setRange(1000,20000);
    axisY->setRange(-25,5);


    //Ove nam setovanje Range-a sluzi da vidimo kako mozemo da
    // se igramo sa osom
    //    axisX->setRange(107,127);
    //    axisY->setRange(-25,0);

    //Podesili smo podelu mozda cemo morati to jos
    // da podesavamo
    axisX->setTickCount(10);
    axisY->setTickCount(10);

    //    m_chart->setAxisX(axisX,series1);
    //Izgleda da je problem bio  ovome, za svaku seriju
    // moramo da podesimo ose za SVAKU seriju!!!
    // kao u primeru na https://doc.qt.io/Qt-5/qtcharts-lineandbar-example.html
    //ISTO ce trebati i za x osu da se radi, kada budemo radili opsege,mada i sa pointerima je pravio probleme
    // TODO samo za seriju 1
    //    m_chart->setAxisY(axisY,series3);
    m_chart->setAxisY(axisY,series2);
    m_chart->setAxisY(axisY,series1);

    //    m_chart->setAxisY(axisY,series4);

    m_chart->setAxisX(axisX,series1);
    m_chart->setAxisX(axisX,series2);
    //    m_chart->setAxisX(axisX,series3);
    //    m_chart->setAxisX(axisX,series4);

    //    qDebug() << "Hmmm velicina je : " << m_chart->size();

    //    m_chart->axisY()->setRange(series3->);
    //    m_chart->setAxisX(m_chart->axisX(series3),series3);
    //    m_chart->setAxisY(m_chart->axisY(series3),series3);
    //    qDebug() << "Series " << series1->at(0);

    //Ovo dodajemo da vidimo kakve su zaista koordinate
    //Ovo nam vise ne treba kako smo resili problem sa prikazom
    //    ourX = new QGraphicsSimpleTextItem(m_chart);
    //    ourY = new QGraphicsSimpleTextItem(m_chart);

    //    ourX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height() +50);
    //    ourY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height() + 50);
    //    ourX->setText("PravoX:");
    //    ourY->setText("Pravo Y:");

    //Sada da resimo i pointer na seriju (ukinut)
    //Posle cemo napraviti da genericki radimo
    //TODO ovaj cemo za ukinuti
    //    ourDataMarker = new Callout(m_chart);
    //Ovo je tacka samo da vidimo da li marker radi
    // Pogledati dobro vrednosti tacke...(nema frekvencije 0 zato je i razdesen
    //    QPointF *ourTestPoint = new QPointF(m_chart->mapToValue(QPointF(0.1,0),series1));
    //    ourDataMarker->setText(QString("Ovaj je nas X: %1 \nY: %2 ").arg(ourTestPoint->x()).arg(ourTestPoint->y()));
    //    ourDataMarker->setAnchor(*ourTestPoint);
    //    ourSeries = series1;

    //TODO ovo isto izbaciti
    //    ourDial->setMaximum(ourSeries->count() -1);
    //    connect(ourDial,SIGNAL(valueChanged(int)),this,SLOT(moveOurTooltip(int)));

    //Aj da dodamo i za promenu serije
    //TODO ovo za sada ne treba
    //    ourSeriesInList.append(series1);
    //    ourSeriesInList.append(series2);
    //    ourSeriesInList.append(series3);
    //    ourSeriesInList.append(series4);
    //    connect(ourBtnChangeSeries,SIGNAL(pressed()),this,SLOT(changeSeries()));

    //    series3->hide();
    //    series4->hide();
    //    series2->hide();
    //    axisY->setTickCount(10);
    //    axisX->setTickCount(10);

    //izgleda da moramo da dodamo nesto poput bafera
    //our_points = new QVector<QPointF>();
    //aj napusticemo ideju bafera...ali uzecemo i kreirati tacke u seriji

    //Aj da inicijalizujemo ovo (Treba da se poklapa broj tacaka da ne bi setao levo desno)
    //Zamenjeno sa  initializeSeriesData();
    //    for (int var = 0; var < 401; ++var) {
    //        QPointF samplePoint(var,0);
    //        series1->append(samplePoint);
    //        pointPos++;
    //    }

    //pri tom ne mozemo da ga zovemo odavde
    // posto nam treba sinhronizacija sa modelom
    // tek se zove nakon sto podesimo parametre merenja
    //    initializeSeriesData();

    //Ovo smo koristili na debug
    //    auto vecPoints = series1->pointsVector();

    //da vratimo na staro(Ovo mora da ostane... mada mozda treba promeniti na var
    pointPos = 0;
    //Ovo poboljsava perfomanse mnogo
    series1->setUseOpenGL(true);

    //Ovo smo cisto dodali radi stilizovanja,
    // mada bice od koristi posto se na slican nacin crta i
    // smitov dijagram
    series1->setColor(QColor(QRgb(0x990000)));

    //Ovo je dok ne resimo da li cemo smita kao background
    //    QLinearGradient plotAreaGradient;
    //       plotAreaGradient.setStart(QPointF(0, 1));
    //       plotAreaGradient.setFinalStop(QPointF(1, 0));
    //       plotAreaGradient.setColorAt(0.0, QRgb(0xFF9999));
    //       plotAreaGradient.setColorAt(1.0, QRgb(0xFFE5CC));
    //       plotAreaGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    //    m_chart->setBackgroundBrush(plotAreaGradient);

    //Ovaj nam treba da vidimo koliko se trosi vremena za ovu operaciju
    elapTimer = new QElapsedTimer;
    elapTimer->start();



    //Na kraju konstrukcije view objekta startujemo racunanje
    emit sigDrawIsDone();
}

void View::simpleDbCalc(double &valToCalculate)
{
    //Sluzi nam za kalkulaciju u dBm (prvi je zbog mog glupiranja
    // sa osom dakle ide prvo manja vrednost nikako od 0 do -20 tipa (0,-20)
    // vec ide (-20,0)
    //    valToCalculate = qFabs( 10 * log10(valToCalculate));
    valToCalculate = 10 * log10(valToCalculate);

}

void View::setOurXAxisRange(int ourNewRange)
{
    //Aj da pokusamo da promenimo parametre
    //    axis
    axisX->setRange(0,ourNewRange);

}

void View::setOurXAxisMin(qreal ourMinX)
{
    axisX->setMin(ourMinX);
}

void View::setOurXAxisMax(qreal ourMaxX)
{
    axisX->setMax(ourMaxX);
}

void View::setOurYAxisMin(qreal ourMinY)
{
    axisY->setMin(ourMinY);
}

void View::setOurYAxisMax(qreal ourMaxY)
{
    axisY->setMax(ourMaxY);
}

void View::setOurYMinAxisRange(qreal ourMinYRange)
{
    //Ovo je malo komplikovanije od setovanja za x range
    //Aj da vidimo koliko je ovo pametno
    //    qreal minRange = axisY->min();
    axisY->setMin(qFloor(ourMinYRange));
}

void View::setOurYMaxRange(qreal ourMaxYRange)
{
    //Setuje maksimum na y Osi TODO
    axisY->setMax(qCeil(ourMaxYRange));
}

void View::setMeasureParameters(qreal measureFrequencyStart, qreal measureFrequencyStop, qreal measureDeltaFreq, int measureNumberOfPoints)
{
    //Sluzi za pocetno setovanje View-a sa NAmodelom
    freqStartMeasure = measureFrequencyStart;
    freqStopMeasure = measureFrequencyStop;
    freqDeltaFMeasure = measureDeltaFreq;
    numberOfPoints = measureNumberOfPoints;

    //Ovo dodajemo da automatski pomeri osu
    // na pocetku
    axisX->setMin(freqStartMeasure);
    axisX->setMax(freqStopMeasure);
}

void View::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        m_chart->resize(event->size());
        m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height() - 20);
        m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height() - 20);
        const auto callouts = m_callouts;
        for (Callout *callout : callouts)
            callout->updateGeometry();
    }
    QGraphicsView::resizeEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    //Ovo je bio qDebug da bi smo mogli da proverimo
    //    qDebug() << "Ovo je mouse move event " << event;
    m_coordX->setText(QString("X: %1").arg(m_chart->mapToValue(event->pos()).x()));
    m_coordY->setText(QString("Y: %1").arg(m_chart->mapToValue(event->pos()).y()));
    //    m_coordX->setText(QString("X: %1").arg(m_chart->mapToPosition(event->pos()).x()));
    //    m_coordY->setText(QString("Y: %1").arg(m_chart->mapToPosition(event->pos()).y()));

    //Ovo dodajemo da vidimo prave koordinate
    //Vise nema potrebe za ovim
    //    ourX->setText(QString("Pravo X:%1").arg(m_chart->mapToPosition(event->pos()).x()));
    //    ourY->setText(QString("Pravo X:%1").arg(m_chart->mapToPosition(event->pos()).y()));

    QGraphicsView::mouseMoveEvent(event);
}

void View::keepCallout()
{
    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(m_chart);
}

void View::tooltip(QPointF point, bool state)
{
    //Ovo da vidimo samo kakvu on to tacku predaje
    // Ovde u ovom grmu lezi zec
    qDebug() << "Tacka je " << point;

    //TODO kada se menjaju dijagrami nastaje
    // problem da nam ovde ostaje tooltip objekat
    // koji je u okviru starog charta
    if (m_tooltip == 0)
        m_tooltip = new Callout(m_chart);

    //Resenje je da za kada hoverujemo uvek pravimo
    // novi tooltip
    //    m_tooltip = new Callout(m_chart);

    if (state) {
        m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}

void View::moveOurTooltip(int indexOfDial)
{
    //Ovo je dodato da bi mogli da pomeramo nas pokazivac
    //Ovo je slot

    //xValue i yValue su potrebni da bi smo imali marker koji nam pokazuje na tacne koordinate
    qreal yValue = ourSeries->at(indexOfDial).y();
    qreal xValue = ourSeries->at(indexOfDial).x();
    QPointF ourNewPoint = QPointF(xValue,yValue);
    ourDataMarker->setText(QString("BlaX: %1 \n trtY: %2").arg(ourNewPoint.x()).arg(ourNewPoint.y()));
    ourDataMarker->setAnchor(ourNewPoint);
    ourDataMarker->updateGeometry();

    //Aj samo da proverimo (posto radi verovatno nece trebati, mada ostavimo za svaki slucaj)
    //    qDebug() << "yValue: " << yValue;
    //    qDebug() << "ourNewPoint: " << ourNewPoint;
    //    qDebug() << "ourDataMarker " << ourDataMarker;
    //    qDebug() << "Izvestaj o tacki serije " << ourSeries->at(indexOfDial);
}

void View::changeSeries()
{
    //Ovo je za promenu serije
    //Popraviti posle za vise markera
    //    if (seriesSelected == 1) {
    //        ourSeries = ourSeriesInList.at(1);
    //        seriesSelected = 2;
    //    }else {
    // //        ourSeries = ourSeriesInList.at(0);
    // //        seriesSelected = 1;
    //    }
    //Verovatno ce biti potrebno
    //    ourDataMarker->updateGeometry();
    //Da ovo ce biti potrebno...radi

    //moracemo u okviru switch statementa
    //    switch (seriesSelected) {
    //    case 2:
    // //        qDebug() << "SeriesSelected " << seriesSelected;
    //        seriesSelected = 2;
    //        ourSeries = ourSeriesInList.at(1);
    //        moveOurTooltip(sampleControl->getDial()->value());
    //        seriesSelected++;
    //        break;
    //    case 3:
    // //        qDebug() << "SeriesSelected " << seriesSelected;
    //        seriesSelected = 3;
    //        ourSeries = ourSeriesInList.at(2);
    //        moveOurTooltip(sampleControl->getDial()->value());
    //        seriesSelected++;
    //        break;
    //    case 4:
    // //        qDebug() << "SeriesSelected " << seriesSelected;
    //        seriesSelected = 4;
    //        ourSeries = ourSeriesInList.at(3);
    //        moveOurTooltip(sampleControl->getDial()->value());
    //        seriesSelected++;
    //        break;
    //    default:
    // //        qDebug() << "SeriesSelected " << seriesSelected;
    //        seriesSelected = 1;
    //        ourSeries = ourSeriesInList.at(0);
    //        moveOurTooltip(sampleControl->getDial()->value());
    //        seriesSelected++;
    //        break;
    //    }


    //    moveOurTooltip(sampleControl->getDial()->value());
}

void View::getDataToDrawFromNAModel(const FreqSparPlain &dataToDraw)
{
    //TODO Ovo je prva verzija slota
    // Sada radi dobro ovakav kakav je ne beze mu podaci nikako
    // Verovatno cemo mu ovde mozda dodati mogucnost da proracunava
    // razne podatke ali razmisliti jos o ovome....
    qreal realValue = dataToDraw.getSparameters().at(0).real();
    qreal imagValue = dataToDraw.getSparameters().at(0).imag();

    qreal magnitude = sqrt(pow(realValue,2) + pow(imagValue,2));
    simpleDbCalc(magnitude);
    QPointF dataPoint(pointPos,magnitude);
    //treba nam i stara tacka za replace
    QPointF oldDataPoint = series1->at(pointPos);
    //    series1->append(pointPos,magnitude);
    //    qDebug() << "Sada je series count " << series1->count();
    int debugVar = 0;
    debugVar++;

    //Aj da pokusamo preko replace-a da ga uradimo kako treba
    //Ovo cemo posle da vezemo za broj tacaka
    if (pointPos == 400) {
        pointPos = -1;                              //izgleda zbog ovoga jebao (zbog donjeg inkrementiranja)
        auto vecPoints = series1->pointsVector();
        auto pointDiff = dataPoint.y() - oldDataPoint.y();

    }
    series1->replace(oldDataPoint,dataPoint);

    //i to bi trebalo da bude to...

    //Aj da probamo bez direktnog dodavanja vec preko
    // vektora tacaka

    //prvo ga punimo sa svim tackama

    //    while(our_points->size() <= 400) {
    //        //dakle sve dok nije puno
    //        our_points->append(dataPoint);
    //        series1->replace(*our_points);
    //        pointPos++;
    //    }

    //Aj treca sreca da odma iscrtava
    //    if (pointPos <= 400) {
    //        our_points->append(dataPoint);
    //    }else {
    //        our_points->clear();
    //        pointPos = 0;
    //        //kada se resetuje moramo da postavimo novu tacku
    //        dataPoint.setX(pointPos);
    //        dataPoint.setY(magnitude);

    //        //Naravno ne zaboraviti da se doda ova tacka
    //        our_points->append(dataPoint);
    //        //        our_points->pop_front();
    //        //        our_points->append(dataPoint);
    //        //pazi redosled je veoma bitan!!!
    //    }

    //ma treba na pocetku da inicijalizujemo celu seriju
    // sa nulama i onda samo replace point... da bi smo izbegli append


    //Ovde je bila po koji put ideja bafera koja se pokazala
    // neodgavarujucom
    //    if (pointPos <= 400) {
    //        //        series1->append(dataPoint);
    //        our_points->at(pointPos);
    //        series1->replace(*our_points);
    //    }else {
    //        series1->clear();
    //        pointPos = 0;
    //        dataPoint.setX(pointPos);
    //        dataPoint.setY(magnitude);
    //        //        series1->replace(1,pointPos,magnitude);
    //    }


    //    our_points->append(dataPoint);
    //    series1->replace(*our_points);
    pointPos++;

    //    qDebug() << "View-- " << series1->count();

    //    our_points->pop_front();


    //Kada se napuni krecemo da ga ponovo punimo od nule
    //    our_points->pop_front();
    //    our_points->append(dataPoint);
    //    series1->replace(*our_points);


    //    if (series1->count() < 402) {
    //        series1->append(dataPoint);
    //        pointPos++;

    //    }else{
    //        pointPos = 0;
    //        QVector<QPointF> ourPoints;
    //        ourPoints = series1->pointsVector();
    //        ourPoints.pop_front();
    //        series1->replace(ourPoints);
    //        qDebug() << "Velicina serije je " << series1->count();
    //    }



    //    if(pointPos <= 400){
    //    if(series1->count() < 401) {

    //        series1->append(dataPoint);

    //        pointPos++;
    // //        qDebug() << "(SLOT)Tacka nam je " << dataPoint;
    //        //Ovo da dodamo da vidimo zasto nam seta
    //        if(pointPos == 401){
    // //            int debugVar = 0;
    //            debugVar = series1->count();
    //        }
    //    }else {
    //        series1->remove(400);
    //        qDebug() << "(View) series 1 je " << series1->count();
    //        pointPos = 0;
    //        series1->clear();

    //    }
}


// Ovaj slot je kada izracunamo tacku i iscrtavamo je na grafiku
// Hmmm pitanje je zasto ovo nece da se poveze sa pogledom
void View::drawPoint(const QPointF &calculatedPoint)
{

    if(m_chart->chartType() == QChart::ChartTypeCartesian) {
        //Ovaj slot je poboljsana verzija slota
        // View::getDataToDrawFromNAModel(const FreqSparPlain &dataToDraw)

        // kao parametar mu se predaje QpointF tacka koja sadrzi ili
        //  frekvenciju i magnitudu, ili magnitudu i fazu...

        //Aj prvo da iscrtava izracunati parametar naspram frekvencije
        // tj onako kako je prethodni slot radio, smatramo da je su frekvencije
        // vec inicijalizovane da bi mogli da koristimo replace

        //Ipak nam treba stara tacka
        //Aj da je nadjemo preko indexa
        // Ako znamo startnu frekvenciju i deltaF ostaje samo pitanje
        // proracuna stare tacke.

        //Bilo je problema sa preskakanjem tacaka sada je reseno...

        qreal currentFreq = calculatedPoint.x();
        qreal tempFreq = currentFreq - freqStartMeasure;

        // Hmmm imamo problem ako pocnu da stizu tacke koje su van opsega
        // na primer ako se desi da su zaostale sa polarnog dijagrama
        // Aj da probamo to ovako
        //TODO verovatno problem lezi u switchChartType();
        // ako i dalje upucava stare tacke nece nista raditi..
        if (tempFreq > 0) {
            auto chartSwitch = series1->pointsVector();

            //Aj da probamo sa qCeil funkcijom, cini mi se da ne menja kako treba
            //    int ourIndex = qCeil(tempFreq/freqDeltaFMeasure);
            //    int ourIndex = tempFreq/freqDeltaFMeasure;

            // moramo da proverimo kako da zaokruzimo da ne bi preskakali tacke
            int ourIndex;
            int ourIndexForCheck = tempFreq/freqDeltaFMeasure;
            qreal realIndex = tempFreq/freqDeltaFMeasure;

            //Ovo moramo da dodamo da ne bi preskakali tacke
            qreal testPointDiff = realIndex - ourIndexForCheck;

            if(testPointDiff >= 0.5){
                //aAbs moramo kada menjamo tacke da nam ne ode u negativan
                // index
                ourIndex = qAbs(qCeil(realIndex));
            }else {
                ourIndex = qAbs(qFloor(realIndex));
            }


            //Uvek promasi 84 frekvenciju na ovakav nacin...
            if (ourIndex == 79) {
                int debugVar = 0;
                debugVar++;
            }

            //Hmmm mozda bi smo mogli ovo na drugi nacin
            // da resimo
            // Radi samo u prvom prolazu pa onda ocekivano puca
            //    int betterIndex = series1->pointsVector().indexOf(QPointF(currentFreq,0));

            //Ovde pravi problem kada se radi setovanje
            // start i stop frekvencija jer se desava kada
            // promenimo start i stop menja se i deltaF kao i broj tacaka
            //TODO ovo je sve osim oldPointa i replace-a za debug
            if (series1->count() > 401) {
                QVector<QPointF> samplePointVector;
                foreach (auto point, series1->pointsVector()) {
                    samplePointVector.append(point);
                }
            }

            //Aj da dodamo guard da nam ne puca
            // desava se da ovde napravi problem

            if(ourIndex <= numberOfPoints) {
                auto debugPoints = series1->pointsVector();
                QPointF oldPoint = series1->at(ourIndex);
                //    QPointF oldPoint = series1->at(betterIndex);

                //TODO kada zavrsimo sa donjom test metodom mozda cemo
                // da vratimo ovu liniju
                series1->replace(oldPoint,calculatedPoint);

            }else {
                //Samo da vidimo da li i dalje puca...
                int debugVar;
                debugVar++;
            }


            //Aj da vidimo sta se ovde desava
            //Aj da vidimo sta se desava sa ovim
            // TODO obrisati ovo, pravio je problem na ovim
            // frekvencijama, vise ne
            //    if((oldPoint.x() > 4790) && (oldPoint.x() < 5000)) {
            //        int debugVar = 0;
            //        debugVar++;

            //        QVector<QPointF> samplePointVector;
            //        foreach (auto point, series1->pointsVector()) {
            //            samplePointVector.append(point);
            //        }
            //    }

            //    } else {
            //        //Ovaj deo se odnosi ukoliko imamo ne kvadratni chart
            //        //Eventualno cemo ga primeniti i gore...
            //        if (series1->count() <= numberOfPoints) {
            //            series1->append(calculatedPoint);
            //        }else {
            //            // ma samo ovde izbrisemo sve i ponovo dodajemo tacke
            //            series1->clear();
            //        }

        }  // kraj provere za stare tacke tempFreq < 0
    } //kraj provere da li je u pitanju kartesian chart

    //Aj da ovde dodamo deo koji se odnosi na polarne
    // moramo da iskorstimo i pointPos
    //        pointPos = 0;
    if (m_chart->chartType() == QChart::ChartTypePolar) {

        //Za njega cemo da koristimo pointPos posto
        // ne mozemo da mu inicijalizujemo i menjamo tacke kao kod obicnog
        if (series1->count() <= numberOfPoints) {
            //smatramo da je klirovan u metodi switchChartType();
            pointPos = 0;                       //Trebalo bi ovde da ga setujemo
            series1->append(calculatedPoint);
        }
        if (pointPos <= numberOfPoints) {
            //Ovde ubacujemo nove tacke na postojece
            //                QPointF polarOldPoint = series1->at(pointPos);
            series1->replace(pointPos,calculatedPoint);
            pointPos++;
        } else {
            pointPos = 0;
        }
    }

    timeToPopulateSeries++;
    //Popunjavamo ovo sve dok ne dodje
    //TODO ovo samo za Cartesian chartove...

    /*
      Ideja je da imamo bafer koji popunjavamo sve do trenutka
      dok se ne napuni do potrebnog broja tacaka.
      Kada se napuni tek onda ih zamenjuje
      sto bi eventualno trebalo da bude mnogo brze
      nego da menjamo tacku po tacku...

    Treba nastaviti sa pokusajima

    if (testSeriesVector.count() <= numberOfPoints)
    {
        testSeriesVector.append(calculatedPoint);
    }

    if (testSeriesVector.count() > numberOfPoints)
    {
        //Ne moramo da klirujemo
        series1->replace(testSeriesVector);
        //kada ih zamenimo mozemo da klirujemo ovaj bafer vektor
        testSeriesVector.clear();
        //auto testSer = series1->pointsVector();
        //series1->clear();
    }
    //series1->replace(testSeriesVector);
    */
    //TODO ovo je samo da izmerimo koliko nam treba
    if (timeToPopulateSeries == numberOfPoints) {
        timeToPopulateSeries = 0;
        qDebug() << "Za iscrtavanje svih tacaka nam treba " << elapTimer->elapsed();
        elapTimer->restart();
    }

}

void View::drawPointBuffered(const QPointF &calculatedPoint)
{
    //Za debug pointPos
    //    pointPos++;
    //Ovo je baferovana varijanta slota...
    // bafer nam je bufferDrawVector...

    //TODO izbrisati ili pod komentar testVar
    // kada zavrsis sa debugom
    auto testVar = series1->count();
    auto testSerContent = series1->pointsVector();

    //Aj da probamo sa ovim
    // ako je bafer (bufferDrawVector prazan)
    // i ako prva tacka nije na prvoj startnoj frekvenciji
    // merenja onda da nista ne radi, naravno ovo nece vaziti
    // za polarne i za smitove dijagrame
    //    if ((bufferDrawVector.count() == 0) && (calculatedPoint.x() != freqStartMeasure)) {
    //        return;
    //    }

    //Izgleda da problem pravi initializeSeriesData
    // koju smo ranije koristili...
    //Ovo bi trebalo da resi to(posto nekim cudom ima 402 clana
    // za  numberOfPoints 401
    //Ovo dole ne radi kako treba
    //    if (series1->count() > numberOfPoints) {
    //        series1->clear();
    //    }

    //Da bi smo mogli da dodamo da se puni bafer od pocetka
    // preko ovoga cemo da pauziramo iscrtavanje tacaka
    if (getIsPausedDraw() == true) {
        return;
    }

    //Treba nam i provera ako je bafer
    // veci od broja tacaka obrisati ga
    if (bufferDrawVector.count() > numberOfPoints) {
        bufferDrawVector.clear();
    }

    //Ovo parce zapravo preuzima ulogu initializeSeriesData();
    // i samim tim se ukoliko je inicijalizovana serija
    // ovo nikada ne izvrsava
    if (series1->count() == 0) {
        //Ovo je za slucaj da je serija za iscrtavanje prazna
        for (int var = 0; var < numberOfPoints; ++var) {
            bufferDrawVector.append(QPointF(0,0));
            //hmmm serija ne podrzava da se odjedanput doda

            series1->append(bufferDrawVector.at(var));

        }
        //sada mozemo da ga ispraznimo
        bufferDrawVector.clear();

    }


    //    Ako nije prazan prvo punimo baffer sa tackama
    // Hmmm interesanto kako nesto promeni broj tacaka
    // sa 401 na 422 popraviti to...inace radi...
    //TODO sada radi...
    if (bufferDrawVector.count() <= (numberOfPoints)) {
        auto testVar1 = bufferDrawVector.count();
        auto testSerVec = series1->pointsVector();
        bufferDrawVector.append(calculatedPoint);
    }
    //Ovo bilo potrebno za debug
    //    if(pointPos == (numberOfPoints-1)){
    //        int debugVar = 0;
    //    }

    // Kada se napuni onda
    if (bufferDrawVector.count() == (numberOfPoints)) {
        auto testVar2 = bufferDrawVector.count();
        auto bftPoint = series1->pointsVector();

        //        //Ovako nece ici
        //        //Aj da pokusamo delay sa pomocnim baferom

        //        //Prvo napunimo help baffer sa serijom
        //        drawHelpBuffer = series1->pointsVector();
        //        pointPos = 0;
        //        //Ovo koristimo da odredimo velicinu bafera
        //        int bafferLength = 100;
        //        int var = 0;
        // //        for (int var = 0; var < numberOfPoints; ++var) {
        //        while (var <= numberOfPoints) {
        //            if (pointPos < (bafferLength-1)) {
        //                //menjamo tacke i ova konstrukcija
        //                // bi trebalo da obezbedi da koliko god tacaka imamo
        //                drawHelpBuffer.replace(var,bufferDrawVector.at(var));
        //                pointPos++;
        //            }else {
        //                //Da se ne zameni i ova tacka
        // //                drawHelpBuffer.replace(var,bufferDrawVector.at(var));
        //                //Da bi prosirili bafer za menjanje
        // //                bafferLength += 0;
        //                pointPos = 0;
        //                //Ovde iniciramo zamenu
        //                QTimer::singleShot(1000,this, &View::replacePointsDelayed);
        //            }
        //            var++;
        //        }
        //        //Kada zavrsi vracamo bafer
        // //        bafferLength = 100;

        //Ova metoda samo ovako radi
        series1->replace(bufferDrawVector);
        bftPoint = series1->pointsVector();
        bufferDrawVector.clear();
        //Dakle kada se dogodi ovo kliruje bafer

        //Emitovacemo signal da je crtanje gotovo
        emit sigDrawIsDone();
        //Ostaje jos samo pitanje ostatka tacaka QpointF(0,0)
        series1->remove(QPointF(0,0));
        qDebug() << "(View::drawPointBuffered();) Trenutni broj tacaka je " << series1->count();
        qDebug() << "(View::drawPointBuffered();) Broj (0,0) tacaka je " << series1->pointsVector().indexOf(QPointF(0,0));
    }

    //I to bi trebalo da bude to, i polarni i kartesian dijagram radi sa ovim


}

void View::eraseBuffers()
{
    series1->clear();
    pointPos = 0;
    //ne treba i ovo zaboraviti
    firstPopulateSeries = 1;
    //Bafer ne koristimo sa drawPoint2(); slotom
    //    bufferDrawVector.clear();

    //Ne pomaze drawGuard
    //    setDrawGuard(true);
    qDebug() << "Obrisali smo series 1!!!!";

}

void View::replacePointsDelayed()
{
    //Ovo je nakon signleShot timera polagana zamena
    // serija
    //    series1->replace(drawHelpBuffer);
    series1->replace(pointPos,currentPoint);
    pointPos++;

}

void View::drawPoint2(const QPointF &calculatedPoint)
{
    //Aj probamo ovako da eliminisemo problematicne
    // tacke
    // Ok za ovo radi...(ne pravi vise probleme spajanjem
    // prve i zadnje tacke
    if ((calculatedPoint.x() > freqStopMeasure) &&
            (firstPopulateSeries == 0) &&
            (pointPos == 0)) {
        return;
    }

    //Ako necemo da nam crta pitacemo drawGuard
    if (getDrawGuard()==true) {
        return;
    }
    bufferDrawVector = series1->pointsVector();
    //Ovo za debug
    if (bufferDrawVector.count() == (numberOfPoints-1)) {
        int debugVar = 0;
        debugVar++;
    }

    //Ovo koristi iscrtavanje tacke po tacku, ali ovaj
    // put u naModelu imamo pauze pa valjda nece preterano
    // nam praviti problem


    //Ovo je prvobitno punjenje
    if (series1->count() == 0) {
        //        QVector<QPointF> initialSeriesData(numberOfPoints,QPointF(0,0));
        //        foreach (QPointF zeroPoint, initialSeriesData) {
        //            series1->append(zeroPoint);
        //        }
        //        series1->append(calculatedPoint);
    }


    //Prvo da inicijalno popunimo seriju
    if ((series1->count() <= (numberOfPoints-1)) &&( firstPopulateSeries == 1)) {
        series1->append(calculatedPoint);
        auto debugSer = series1->pointsVector();
        //        bufferDrawVector = debugSer;
    }else {
        //        auto serie = series1->pointsVector();
        //        firstPopulateSeries = 0;
        //        //moramo da zovemo posle inicijalnog crtanja
        //        emit sigDrawIsDone();
    }

    //Kada je serija vec popunjena
    if (series1->count() == (numberOfPoints) && (firstPopulateSeries == 0)) {
        auto ser = series1->pointsVector();
        //        series1->removePoints(pointPos,1);
        //        series1->append(calculatedPoint);
        //        series1->replace(pointPos,calculatedPoint);

        if (pointPos < numberOfPoints) {
            series1->replace(pointPos,calculatedPoint);
            pointPos++;
        }
        //Ovde redovno puca
        //        pointPos++;

        //Ovo samo da mozemo debugDa radimo
        //        bufferDrawVector = series1->pointsVector();
    }
    if ((series1->count() == (numberOfPoints)) && (pointPos == (numberOfPoints)) &&(firstPopulateSeries != 1) ) {
        //Ovo je cisto da pokrenemo ponovo kalkulaciju

        //Ovo je najednostavnija korekcija za poslednju tacku
        //Posto cesto pravi problem, zamenicemo je sa pretposlednjom
        bufferDrawVector = series1->pointsVector();
        series1->replace(bufferDrawVector.last(),bufferDrawVector.at(numberOfPoints-2));

        pointPos = 0;
        emit sigDrawIsDone();
        return;
    }

    //zbog prvog prolaza moramo dole da ne bi se radilo dva
    // puta...
    if((series1->count() == numberOfPoints) && (firstPopulateSeries == 1)) {
        auto debSer = series1->pointsVector();

        //Ovaj pravi problem
        firstPopulateSeries = 0;
        //Aj da probamo na ovakav nacin da izbegnemo neke stvari
        // umesto direktno da ga posaljemo imamo da ga zakasnimo
        //        QTimer delaySigDrawIsOver;
        emit sigDrawIsDone();
        return;
    }

    //    if ((series1->count() == (numberOfPoints)) && (pointPos == (numberOfPoints)) &&(firstPopulateSeries != 1) ) {
    //        //Ovo je cisto da pokrenemo ponovo kalkulaciju
    //        pointPos = 0;
    //        emit sigDrawIsDone();
    //    }
    //    if (pointPos < numberOfPoints) {
    //        series1->replace(pointPos,calculatedPoint);
    //        pointPos++;
    //        currentPoint = calculatedPoint;
    //        QTimer::singleShot(4,this,&View::replacePointsDelayed);
    //    }
    //Kada prodje sve tacke resetujemo point pos
    // i zapocinjemo ponovo racunanje
    //    if (pointPos == numberOfPoints) {
    //        pointPos = 0;
    //        emit sigDrawIsDone();
    //        QApplication::processEvents();
    //    }
    //    auto bla = QCoreApplication::instance();
    //    bla->processEvents();
    //    QCoreApplication::processEvents();
    //    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    //za debug
    //    bufferDrawVector = series1->pointsVector();
    //    if (bufferDrawVector.count() > (numberOfPoints-1)) {
    //        int debugVar = 0;
    //        debugVar++;
    //    }
}

QLineSeries *View::getSeries1() const
{
    return series1;
}

void View::setSeries1(QLineSeries *value)
{
    series1 = value;
}

QGraphicsSimpleTextItem *View::getCoordY() const
{
    return m_coordY;
}

void View::setCoordY(QGraphicsSimpleTextItem *coordY)
{
    m_coordY = coordY;
}

void View::drawCallDut(QVector<QPointF> callDutSeries)
{
    //Iscrtava realan deo neobradjenih podataka
    if (series2->count() == 0) {
        for (int var = 0; var < callDutSeries.count() - 1; ++var) {
            series2->append(QPointF(0,0));
        }
    }
    //Sada dodajemo seriju
    series2->replace(callDutSeries);
}

QGraphicsSimpleTextItem *View::getCoordX() const
{
    return m_coordX;
}

void View::setCoordX(QGraphicsSimpleTextItem *coordX)
{
    m_coordX = coordX;
}

//Ovo nam je pomocna metoda da nadjemo maximume od serije
// hmmm mozda bi mogli da prepravimo da vraca min i max ali tada bi
// morali dodatno da se jebavamo i da non stop pristupamo
QPointF View::findMaxOfSeries(QLineSeries *series)
{
    QPointF maxPoint(0,0);
    QPointF minPoint(0,0);


    for (int var = 0; var < series->count(); ++var) {
        if (series->at(var).y() < minPoint.y()) {
            minPoint = series->at(var);
        }
        if (series->at(var).y() > maxPoint.y()) {
            maxPoint = series->at(var);
        }
    }
    return maxPoint;
}

//Obe funkcije imaju duplirani kod, ispraviti to...
QPointF View::findMinOfSeries(QLineSeries *series)
{
    QPointF maxPoint(0,0);
    QPointF minPoint(0,0);
    int bla {0};

    for (int var = 0; var < series->count(); ++var) {
        if (series->at(var).y() < minPoint.y()) {
            minPoint = series->at(var);
        }
        if (series->at(var).y() > maxPoint.y()) {
            maxPoint = series->at(var);
        }
        bla++;
    }
    return minPoint;

}

int View::getPointPos() const
{
    return pointPos;
}

void View::setPointPos(int value)
{
    pointPos = value;
}

bool View::getDrawGuard() const
{
    return drawGuard;
}

void View::setDrawGuard(bool value)
{
    drawGuard = value;
}

bool View::getIsPausedDraw() const
{
    return isPausedDraw;
}

void View::setIsPausedDraw(bool value)
{
    isPausedDraw = value;
}

void View::setOldYmin(const qreal &value)
{
    oldYmin = value;
}

qreal View::getOldXmax() const
{
    return oldXmax;
}

void View::setOldXmax(const qreal &value)
{
    oldXmax = value;
}

bool View::getIsSmithChart() const
{
    return isSmithChart;
}

void View::setIsSmithChart(bool value)
{
    isSmithChart = value;
}

void View::clearViewSeriesBuffer()
{
    //Metoda koja sluzi da isprazni bafer
    // kada menjamo podakte koji nam stizu
    bufferDrawVector.clear();
}

qreal View::getOldXmin() const
{
    return oldXmin;
}

void View::setOldXmin(const qreal &value)
{
    oldXmin = value;
}

void View::setOldYmax(const qreal &value)
{
    oldYmax = value;
}

qreal View::getOldYmax() const
{
    return oldYmax;
}

qreal View::getOldYmin() const
{
    return oldYmin;
}

int View::getNumberOfPoints() const
{
    return numberOfPoints;
}

void View::setNumberOfPoints(int value)
{
    numberOfPoints = value;

    //Broj tacaka utice na deltaF
    freqDeltaFMeasure = (freqStopMeasure - freqStartMeasure)/((qreal) (numberOfPoints - 1) );
    //I na osnovu ovoga menjamo series1 inicijalni vektor
    initializeSeriesData();
}

void View::setFreqDeltaFMeasure(const qreal &value)
{
    freqDeltaFMeasure = value;
    //Ovaj nema sta da se menja
}

void View::initializeSeriesData()
{
    //sluzi za inicijalizaciju serije kada konstruisemo grafik
    // zove se pri konstrukciji i sinhronizaciji sa modelom

    //Ovo ukoliko je posedovao vec neke vrednosti

    //    if (m_chart->type() == QChart::ChartTypeCartesian) {




    series1->clear();

    //Ovaj radi kada crta tacku po tacku
    //        qreal freq = freqStartMeasure;
    //        while (freq <= freqStopMeasure) {
    //            QPointF ourPoint(freq,0);
    //            freq += freqDeltaFMeasure;
    //            series1->append(ourPoint);
    //        }

    //Ovaj radi kada koristimo drawPointBuffered();
    pointPos = 0;
    //numberOfPoints;
    while (pointPos < numberOfPoints) {
        series1->append(QPointF(0,0));
        pointPos++;
    }

    //    }else {
    //Dovoljno je da ispraznimo podatke
    //        series1->clear();
    //Hmmm ovako necemo moci da inicijalizujemo seriju
    // malo cemo da promenimo draw data specijalno za ovu svrhu

    //smatracemo da su ostali polarnog tipa
    //        series1->clear();
    //        qreal freq = freqStartMeasure;
    //        while (freq <= freqStopMeasure) {

    //            QPointF ourPoint(pha,0);
    //            freq += freqDeltaFMeasure;
    //            series1->append(ourPoint);
    //        }
    //    }

    auto chartOurType = m_chart->chartType();
    auto points = series1->pointsVector();
}

void View::setFreqStopMeasure(const qreal &value)
{
    freqStopMeasure = value;
    //I ovde da kada se promeni ovaj parametar
    // automatski pomeri osa prilikom sinhronizacije
    // sa NA modelom
    axisX->setMax(value);

    //Ovo vise ne treba sa drawPoint2(); metodom
    //moramo i ovde da dodamo inicijalizujemo ponovo
    // podatke pri promeni ovog parametra zbog replacePoint-a
    // u drawPoint(); slotu
    //TODO ovde pravi probleme
    //initializeSeriesData();
}

void View::setFreqStartMeasure(const qreal &value)
{
    freqStartMeasure = value;
    //Aj da pokusamo da svaki put kada se dodaje ova
    // tacka automatski se setuje i pogledi na njih
    axisX->setMin(value);

    //Ne moramo sa drawPoint2(); metodom da inicijalizujemo tacke
    //Svaki put kada promenimo nesto u pogledu vezano
    // za tacke moramo da inicijalizujemo ponovo bazni
    // vektor da bi mogli da radimo replace
    //    initializeSeriesData();
}

// Uzeli smo ovu metodu iz polarchart
// primera

void View::switchChartType()
{
    QChart *newChart;
    QChart *oldChart = m_chart;

    //Ovo smo morali da dodamo...
    series1->clear();

    //Proveramo da li je stari chart mozda smitov
    // pa ako jeste sklanjamo mu grid
    if(getIsSmithChart() == true) {
        removeSmithGrid();
    }

    if (oldChart->chartType() == QChart::ChartTypeCartesian)
        newChart = new QPolarChart();
    else
        newChart = new QChart();

    // Move series and axes from old chart to new one
    // dohvata ose i serije iz starog grafika kao i njihove opsege
    const QList<QAbstractSeries *> seriesList = oldChart->series();
    const QList<QAbstractAxis *> axisList = oldChart->axes();
    QList<QPair<qreal, qreal> > axisRanges;

    //prenosi ose
    //    for (QAbstractAxis *axis : axisList) {
    //        QValueAxis *valueAxis = static_cast<QValueAxis *>(axis);
    //        axisRanges.append(QPair<qreal, qreal>(valueAxis->min(), valueAxis->max()));
    //    }

    //malo cemo bolje ove ose da prenesemo bez rangeova

    //Ovu for petlju mozda necemo morati da koristimo
    // posto istovremeno radi za obe ose...
    // TODO eventualno ako bude trebalo
    for(QAbstractAxis *axis : axisList) {
        QValueAxis *valueAxis = static_cast<QValueAxis *>(axis);

        //Sada moramo da prenesemo i range-ve
        if(newChart->chartType() == QChart::ChartTypePolar) {
            axisX->setRange(0,360); //odnosi se na angular osu
            axisY->setRange(0,1);   //odnosi se na radial osu
        }

        //Sa druge strane ako imamo kartesian chart
        // Podesavamo osu na osnovu podataka
        // TODO moguce je kada budemo uvodili smita da cemo
        // morati ovo da menjamo
        if(newChart->chartType() == QChart::ChartTypeCartesian) {
            //idemo redom
            axisX->setMin(freqStartMeasure);
            axisX->setMax(freqStopMeasure);

            //Ovo smo stavili bezveze posto za sada ne
            // znam koje vrednosti da stavim
            axisY->setMin(-25);
            axisY->setMax(5);
        }

    }

    //Necemo prenositi ose izmedju ova dva tipa prikaza
    //podesavamo posebno za jedan i za drugi (pogledati gornji if)
    //    if (oldChart->chartType() == QChart::ChartTypeCartesian) {
    //        //TODO posle cemo ubaciti
    //    }

    //skida serije sa starog charta
    // u nasem slucaju ima ih vise
    for (QAbstractSeries *series : seriesList)
        oldChart->removeSeries(series);

    // skida ose sa starog i dodaje na novi
    for (QAbstractAxis *axis : axisList) {
        oldChart->removeAxis(axis);
        //Dodajemo ose na novi dijagram
        newChart->addAxis(axis, axis->alignment());
    }

    //dodaje serije na novi chart i povezuje sa osama
    // trebalo bi da radi...
    for (QAbstractSeries *series : seriesList) {
        //baratamo sa jednom serijom pa je cistimo...
        //TODO verovatno ce raditi...
        if (newChart->chartType() == QChart::ChartTypePolar) {
            //potrebno ga je obrisati u slucaju da prebacujemo na polarni
            // ne svaki put...
            //            series1->clear();
        }
        //        series1->clear();
        newChart->addSeries(series);

        // TODO ovde pravi problem kada menja sa polarnog na kartesian
        // pa moramo da upucamo podatke ponovo...
        if (newChart->chartType() == QChart::ChartTypeCartesian) {
            initializeSeriesData();
        }

        for (QAbstractAxis *axis : axisList)
            series->attachAxis(axis);
    }

    //Na isti nacin kao i kod ovoga dojajemo coord Iteme
    oldChart->scene()->removeItem(m_coordX);
    oldChart->scene()->removeItem(m_coordY);
    //pa ga dodajemo na novi
    //    newChart->scene()->addItem(m_coordX);
    //    newChart->scene()->addItem(m_coordY);
    m_coordX->setParentItem(newChart);
    m_coordY->setParentItem(newChart);

    //Aj da vidimo sta nam ostaje da ovde
    auto ostatak = oldChart->childItems();
    auto uNovomChartu = newChart->childItems();

    //setuje range za ose na novom
    // TODO za sada nam ne treba
    //    int count = 0;
    //    for (QAbstractAxis *axis : axisList) {
    //        axis->setRange(axisRanges[count].first, axisRanges[count].second);
    //        count++;
    //    }

    //    newChart->setTitle(oldChart->title());

    //    scene()->addItem(newChart);
    //    m_chart = newChart;

    //Ovo nam vise ne treba
    //    QPolarChart *newChart = new QPolarChart();
    //    newChart = new QPolarChart;
    //    m_chart->removeSeries(series1);
    //Aj da pokusamo da uklonimo sve
    //    m_chart->removeAxis(axisX);
    //    m_chart->removeAxis(axisY);
    //    //Ovo dodajemo da nam se ne povecava previse
    newChart->setMinimumSize(640, 640);
    //    //Hmmm ovo bi smo morali da dodamo da resimo problem sa pointerima
    newChart->setMaximumSize(640,640);

    //    //Aj da vidimo o cemu se radi
    //    //Ovo smo dodali cisto da vidimo o cemu se radi...


    //TODO dodavali smo tacke sada da li ovo treba
    // ovde, a trebace negde da se izbrise posto su serije
    // drugacije kada menjamo chart(baratamo sa jednom serijom
    // pa je gore kada prenosimo ose ,,klirujemo''
    //    series1->clear();
    //    series1->append(QPointF(0,1));
    //    series1->append(QPointF(45,2));
    //    series1->append(QPointF(90,3));
    //    series1->append(QPointF(135,4));
    //    series1->append(QPointF(180,5));
    //    series1->append(QPointF(270,6));
    //    series1->append(QPointF(360,6));
    //    series1->append(QPointF(395,6));
    //    series1->append(QPointF(420,7));
    //    series1->append(QPointF(180,6));
    //    series1->append(QPointF(60,4));
    //    series1->append(QPointF(75,5));
    //    series1->append(QPointF(120,5));
    //    series1->append(QPointF(180,6));
    //    series1->append(QPointF(225,7));
    //    series1->append(QPointF(255,9));

    //    series1->append(QPointF(0,1));
    //    series1->append(QPointF(1,1));
    //    series1->append(QPointF(2,2));
    //nastavicemo posle da vidimo blaladsfs
    //    //ne moze ovako da se doda, vec na svenu da se doda
    //    // kako smo ga u liniji gore dodali
    //    // Hmmm ne radi ovo nikako
    // //    setChart(newChart);
    // //    QValueAxis polarAxis;
    //    auto polarPoint newChart->ser
    // //    newChart->addAxis(polarAxis);

    //TODO dodali smo ga vec gore
    //    newChart->addSeries(series1);


    //    auto polarPoint = series1->pointsVector();

    //     newChart->createDefaultAxes();

    //Posto smo preneli ose ovo je vec nepotrebno
    //    QValueAxis *angularAxis = new QValueAxis;
    //    QAbstractAxis *radialAxis = new QValueAxis;


    //TODO ovo je potrebno za svaki slucaj
    m_chart->setAcceptHoverEvents(true);
    setRenderHint(QPainter::Antialiasing);
    //mada mislim da je nepotrebno da se ovo ponovo radi...
    // Polar chart ovo ne podrzava
    series1->setUseOpenGL();

    //Ovo smo u pocetku koristili, mada isti efekat se dobija uz donji aligment
    //    newChart->addAxis(angularAxis,QPolarChart::PolarOrientationAngular);
    //    newChart->addAxis(radialAxis,QPolarChart::PolarOrientationRadial);

    //Ovo je nepotrebno vec smo uradili
    //    auto alignX = axisX->alignment();
    //    auto alignY = axisY->alignment();
    //    newChart->addAxis(angularAxis,axisX->alignment());
    //    newChart->addAxis(radialAxis,axisY->alignment());

    //Ovo redovno zaboravljamo
    // nikako ovo ne zaboraviti da se uradi posto
    // ne prikazuje lepo podatke u odnosu na osu
    //TODO ovo smo vec uradili
    //    series1->attachAxis(angularAxis);
    //    series1->attachAxis(radialAxis);

    //Hmmm prva je faza cija je orijentacija radijalna, pa onda faza angularna
    //    newChart->addAxis(horizontalAxis,QPolarChart::PolarOrientationRadial);
    //    newChart->addAxis(verticalAxis,QPolarChart::PolarOrientationAngular);


    //TODO vec smo uradili kada smo prenosili ose...
    //    radialAxis->setRange(0,1);
    //    angularAxis->setRange(-90,90);
    //    newChart->createDefaultAxes();

    //TODO  Aj da pokusamo jos neku osu da dodamo da bi mogli da mu predajemo
    // samo tacke sa x i y koordinatama
    //    QValueAxis *verticalAxis = new QValueAxis;
    //    QValueAxis *horizontalAxis = new QValueAxis;
    //    newChart->addAxis(verticalAxis,axisY->alignment());
    //    newChart->addAxis(horizontalAxis,axisX->alignment());
    //    verticalAxis->setLabelsVisible(false);          //cisto da nam ne smeta

    //    QLineSeries *seriesTest = new QLineSeries;
    //Ovo cisto da dodamo jednu seriju da bi probali
    //    *seriesTest << QPointF(0,0) << QPointF(45,1) << QPointF(90,2) <<  QPointF(180,3);
    //    verticalAxis->setRange(0,10);                //cisto da mu setujemo range, mora da se poklapa da vidimo kako radi
    //    horizontalAxis->setRange(0,10);
    //    seriesTest->attachAxis(angularAxis);
    //    seriesTest->attachAxis(radialAxis);
    //    newChart->addSeries(seriesTest);

    scene()->addItem(newChart);

    //Problem pravi itemi koji se nalaze u starom
    // chartu i ostanu da vise
    //    QList
    //    for(QGraphicsItem oldChartItem : oldChart) {
    //        oldChart->scene()->removeItem(*oldChartItem);
    //    }
    //    scene()->removeItem(oldChart);

    //da li je ovde problem
    // u legendi pa se ne vide koordinate
    newChart->legend()->hide();

    //TODO U novom chartu treba da podesimo pa tek onda da ih menjamo

    //    QGraphicsSimpleTextItem *our_new_coord_x = new QGraphicsSimpleTextItem(newChart);
    //    QGraphicsSimpleTextItem *our_new_coord_y = new QGraphicsSimpleTextItem(newChart);

    //    setCoordX(new QGraphicsSimpleTextItem(newChart));
    //    setCoordY(new QGraphicsSimpleTextItem(newChart));

    //    getCoordX()->setPos(newChart->size().width()/2-50,m_chart->size().height());
    //    getCoordY()->setPos(m_chart->size().width()/2 + 50,m_chart->size().height());

    //    getCoordX()->setText("Novo x:");
    //    getCoordY()->setText("Novo y:");


    //ovo moramo da dodamo ako nismo hoverovali
    // pa ga i nema
    if (m_tooltip != 0) {
        //        m_tooltip->setParentItem(newChart);
        //Kada menjamo chart najbolje je da ga
        // obrisemo pa da pravimo novi
        delete m_tooltip;
        m_tooltip = nullptr;
    }
    m_chart = newChart;
    //    series1->hide();
    //Naravno pravi problem i sa pripadajucim elementima koje moramo da
    // rekreiramo i clanove koji su pripadali starom

    //prvo eliminisemo stare

    //TODO ovo za debug jer se ne vidi na novom chartu
    //    auto old_coord_x = m_coordX;
    //    auto old_coord_y = m_coordY;

    //    delete m_coordX;
    //    delete m_coordY;

    //TODO end


    //da ne bi bilo problema u prenosu
    // posto hoce da pravi problem
    //    m_callouts.clear();

    // i napravimo nove

    //Mada treba da napravimo nove i da ih pozicioniramo bez brisanja

    //    QGraphicsSimpleTextItem *our_new_coord_x = new QGraphicsSimpleTextItem(m_chart);
    //    QGraphicsSimpleTextItem *our_new_coord_y = new QGraphicsSimpleTextItem(m_chart);

    //U ovom slucaju malo je drugacije
    //Vise ne moramo da ih ponovo konstuisemo samo smo promenuli parenta
    //    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    //    m_coordX = our_new_coord_x;
    //    m_coordY = our_new_coord_y;

    //    m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height());
    //    m_coordX->setPos(0,0);

    //    m_coordX->setText("X: ");
    //    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    //    m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height());
    //    m_coordY->setPos(50,0);
    //    m_coordY->setText("Y: ");





    //    oldChart->scene()->clear();
    delete oldChart;
    //    oldChart->hide();

    //Hmmm i dalje pravi probleme
    // ali ovaj problem je kada zameni dijagrame i onda view
    // pravi probleme
}

int View::getChartType()
{
    return m_chart->chartType();
}

void View::setGridVisible(bool isGridVisible)
{
    //Jednostavna metoda kojom pristupamo vidljivosti grid
    // linija koje su nam nepotrebne kod smitovih dijagrama
    if (isGridVisible == true) {
        axisX->setGridLineVisible();
        axisY->setGridLineVisible();
    }else {
        axisX->setGridLineVisible(false);
        axisY->setGridLineVisible(false);
    }
}

void View::drawSmithGrid()
{
    // metoda koja crta smitov dijagram
    // na kartesian chartu

    //prvo da izbacimo grid
    setGridVisible(false);

    //trebaju nam koordinate za to sto
    // crtamo
    QRectF area = m_chart->plotArea();

    //Hmmm ne moze na ovakav nacin
    //Sada kada znamo gde crtamo aj da probamo
    // da nacrtamo, crtamo na chartu
    //    QPainter smithPaint(this);
    //    smithPaint.setWindow(QRect(50,50,100,100));

    //    smithPaint.drawEllipse(area);
    //    smithPaint.drawLine(QPointF(25,25),QPointF(100,100));

    //Aj da probamo sa Graphic View frameworka
    //Ok ovo radi...
    rectOnChart = new QGraphicsRectItem(m_chart->plotArea());
    circleOnChart = new QGraphicsEllipseItem(m_chart->plotArea());
    rectOnChart->setParentItem(m_chart);
    circleOnChart->setParentItem(m_chart);
    //    rectOnChart->setPos(10,10);
    //    qDebug() << "Ok da vidimo da li pripada uopste " << m_chart->childItems();
    //    rectOnChart->setZValue(11.0);
    //Da li moze lokalno, ne moze mora preko pointera...
    //    QGraphicsRectItem someLocalRectItem(10,10,100,100);
    //    m_chart->scene()->addItem(someLocalRectItem);
    //    someLocalRectItem.setParentItem(m_chart);

    //Aj da pokusamo drugi krug da napravimo
    QRectF smallerPlotArea = m_chart->plotArea();
    smallerPlotArea.moveRight(0.1);
    smallerPlotArea.setHeight(smallerPlotArea.height() - 0.5);
    smallerPlotArea.setWidth(smallerPlotArea.width() - 0.5);

    QRectF blaArea(10,10,100,100);
    //    blaArea.moveCenter(m_chart->plotArea().center());
    //    blaArea.moveRight(1.0);
    auto centerPoint = m_chart->mapFromItem(m_chart,m_chart->plotArea().center());
    auto centerPointOnChart = m_chart->mapToValue(QPointF(1.0,0),series1);
    //Ovako se mapira na parent(u nasem slucaju View)
    auto centerPointOnViewportMapToParent = mapToParent(QPoint(centerPointOnChart.x(),centerPointOnChart.y()));
    auto centerPointOnViewportMapToValue = mapFromScene(QPointF(centerPointOnChart.x(),centerPointOnChart.y()));
    blaArea.moveCenter(area.center());

    blaArea.setHeight(area.height() - 10);
    blaArea.setWidth(area.height() - 10);
    //Tek nakon podesavanja dimenzija mozemo da pomeramo centar
    blaArea.moveCenter(area.center());
    blaArea.moveCenter(QPointF(area.center().x() + 10.0,area.center().y()));
    //    blaArea.moveRight(area.center().x());
    //    blaArea.setHeight(m_chart->plotArea().height() - 10);
    //    blaArea.setWidth(m_chart->plotArea().width() - 10);

    //Aj da probemo da centriramo sa charta
    //    QPointF pointOnChart(1.0,0);
    //Hmmm odredimo tacke prvo na chartu pa onda ih mapiramo na parent
    // ili lokalno kako cemo i raditi

    //Za sada necemo da crtamo ovaj krug na ovaj nacin
    //    secondCircle = new QGraphicsEllipseItem(blaArea);
    //    secondCircle->setParentItem(m_chart);

    //Aj da vidimo kako da dodamo na sam m_chart iteme
    // kako se zadaje moramo da vodimo racuna o centriranju
    // posto kada ga pomerimo uzima u obzir svoj lokalni koordinatni sistem
    QGraphicsRectItem *sampleLocalRect = new QGraphicsRectItem(-5,-5,10,10);
    sampleLocalRect->setParentItem(m_chart);
    OurSmithItems << sampleLocalRect;
    //    m_chart->scene()->addItem(sampleLocalRect);

    //    sampleLocalRect->setPos(m_chart->mapToScene((QPointF(1.0,0))));
    //Dakle ovako ga podesavamo u odnosu na osu
    sampleLocalRect->setPos(m_chart->mapToPosition(QPointF(1.0,0.0),series1));
    //da bi smo izvukli koordinate sa charta koristimo
    // mapToValue, valjda
    qDebug() << sampleLocalRect->parentItem();

    //Ok aj da pokusamo sada prvi krug
    // Ok ovo je nacin za formiranje krugova
    //Trebaju nam zapravo za pravougaonik samo dve tacke
    QPointF firstPoint(m_chart->mapToPosition(QPointF(-0.9,0.9)));
    QPointF secondPoint(1,0.9);
    QPointF thirdPoint(m_chart->mapToPosition(QPointF(1,-0.9)));
    QPointF fourthPoint(m_chart->mapToPosition(QPointF(-0.9,-0.9)));
    QRectF ourRect(firstPoint,thirdPoint);



    //Umesto da ga direktno dodajemo treba nam nesto da ga uhvatimo
    //    m_chart->scene()->addEllipse(ourRect);
    QGraphicsEllipseItem *firstEclipse = new QGraphicsEllipseItem(ourRect);
    m_chart->scene()->addItem(firstEclipse);
    OurSmithItems << firstEclipse;

    //Kraj inicijalnog definisanja koncentricnih krugova...

    //Aj jos jedan
    //Ovako ne
    //    firstPoint = firstPoint + m_chart->mapToPosition(QPointF(0.1,-0.1),series1);
    //    thirdPoint = thirdPoint + m_chart->mapToPosition(QPointF(0.1,0.1),series1);

    //Aj probamo ovako
    QPointF firstPoint2(-0.9,0.9);
    QPointF thirdPoint2(1,-0.9);

    firstPoint2 += QPointF(0.1,-0.1);
    thirdPoint2 += QPointF(0,0.1);

    firstPoint2 = m_chart->mapToPosition(firstPoint2,series1);
    thirdPoint2 = m_chart->mapToPosition(thirdPoint2,series1);
    QRectF ourRect2(firstPoint2,thirdPoint2);

    QGraphicsEllipseItem *secondEclipse = new QGraphicsEllipseItem(ourRect2);
    m_chart->scene()->addItem(secondEclipse);
    OurSmithItems << secondEclipse;

    //Ne moramo da dodajemo centar
    //    ourRect.moveCenter(area.center());

    //Na osnovu ovoga mozemo da crtamo sada krugove rezistanse
    qreal deltaRezistansa = 0;

    //    QPointF firstPoint3(-1.0,1.0);
    //    QPointF thirdPoint3(1,-1);

    //Na isti nacin i admitansne linije samo sto ih imamo sa obe strane

    qreal numberOfSmithLines = 10;
    qreal paintedLines = 0;

    while (paintedLines <= numberOfSmithLines) {

        deltaRezistansa += 1/numberOfSmithLines;

        QPointF firstPoint3(-1.0,1.0);
        QPointF thirdPoint3(1,-1);

        QPointF firstPointAdmitansa1(0,2);
        QPointF thirdPointAdmitansa1(2,0);

        //        QPointF firstPointAdmitansa1(-1,4);
        //        QPointF thirdPointAdmitansa1(3,0);

        QPointF firstPointAdmitansa2(0,0);
        QPointF thirdPointAdmitansa2(2,-2);


        //Za rezistansu
        // Ako je pomeramo za jednake imacemo problem i dobiti
        // pravougaonike,zato za svaku promenu na x osi moramo
        // da pomerimo za pola te podele na y osi, jer samo tako na
        // taj nacin dobijamo kvadrat i krug posle iz njega, a ne
        // pravougaonik i elipsu
        firstPoint3 += QPointF(deltaRezistansa,-1*deltaRezistansa/2);
        thirdPoint3 += QPointF(0,deltaRezistansa/2);

        //Donja treca se ne pomera od 0 y ose (gornja admitansa)
        //Za admitansu ide ista konstrukcija, samo ovaj put se za
        // svaku promenu po y osi ide po pola promene na x osi
        firstPointAdmitansa1 += QPointF(deltaRezistansa/2,-1*deltaRezistansa);
        thirdPointAdmitansa1 += QPointF(-1*deltaRezistansa/2,0);

        //Prva ne treba da se pomera od 0 y ose (donja admitansa)
        //Isto ide i za donju admitansu, za promenu po y osi ide pola podele po
        // x osi
        firstPointAdmitansa2 += QPointF(deltaRezistansa/2,0);
        thirdPointAdmitansa2 += QPointF(-1*deltaRezistansa/2,deltaRezistansa);

        QPointF firstPoint3Rect = m_chart->mapToPosition(firstPoint3,series1);
        QPointF thirdPoint3Rect = m_chart->mapToPosition(thirdPoint3,series1);
        QRectF ourRect3(firstPoint3Rect,thirdPoint3Rect);

        //Definise kvadrat za gornji admitansni krug
        QPointF firstPointAdmitansa1Rect = m_chart->mapToPosition(firstPointAdmitansa1,series1);
        QPointF thirdPointAdmitansa1Rect = m_chart->mapToPosition(thirdPointAdmitansa1,series1);
        QRectF rectAdmitansa1(firstPointAdmitansa1Rect,thirdPointAdmitansa1Rect);

        //Definise kvadrate za donji admitansni krug
        QPointF firstPointAdmitansa2Rect = m_chart->mapToPosition(firstPointAdmitansa2,series1);
        QPointF thirdPointAdmitansa2Rect = m_chart->mapToPosition(thirdPointAdmitansa2,series1);
        QRectF rectAdmitansa2(firstPointAdmitansa2Rect,thirdPointAdmitansa2Rect);

        //Dodaje konstuisani rezistansni krug
        QGraphicsEllipseItem *ellipseRezistansa = new QGraphicsEllipseItem(ourRect3);
        m_chart->scene()->addItem(ellipseRezistansa);
        ellipseRezistansa->setOpacity(0.5);
        OurSmithItems << ellipseRezistansa;

        //Dodaje konstruisani gornji admitansni krug
        QGraphicsEllipseItem *ellipseAdmitansaGornja = new QGraphicsEllipseItem(rectAdmitansa1);

        //hmmm moramo da proracunamo start i span angle
        // mogli bi da iskoristimo da nadjemo tacke preseka (malo teze)

        ellipseAdmitansaGornja->setStartAngle(180*16);
        ellipseAdmitansaGornja->setSpanAngle(90*16);
        //        ellipseAdmitansaGornja->setRotation(90);
        m_chart->scene()->addItem(ellipseAdmitansaGornja);
        ellipseAdmitansaGornja->setOpacity(0.5);
        OurSmithItems << ellipseAdmitansaGornja;

        //Dodaje konstruisani donji admitansni krug
        QGraphicsEllipseItem *ellipseAdmitansaDonja = new QGraphicsEllipseItem(rectAdmitansa2);
        m_chart->scene()->addItem(ellipseAdmitansaDonja);
        OurSmithItems << ellipseAdmitansaDonja;
        //Cisto da nam podaci budu vidljiviji
        ellipseAdmitansaDonja->setOpacity(0.5);

        //Uvecavamo za 1 i ponovo proracunavamo deltaRezistansu
        paintedLines++;

        //TODO eventualno dodati jos rezistansnih linija
    }


    //    foreach(QGraphicsItem *bla, OurSmithItems){
    //        bla->hide();
    //        auto elipInfo = bla->parentItem();
    //    }

    //Aj da pokusamo da nateramo view da samo pokaze onoliko koliko treba
    //Hmmm verovatno cemo razviti drugu metodu

}

//Druga verzija metode drawSmithGrid();
void View::drawSmithGrid2()
{
    //Aj da dodamo sliku kao pixmap
    //    QGraphicsPixmapItem *smithPixmap = new QGraphicsPixmapItem(m_chart);
    //Ovo bi trebalo da ucita sliku
    //    smithPixmap->setPixmap(QPixmap(":/Smith_chart_gen.svg"));
    //    QGraphicsPixmapItem smithPixmap(QPixmap(":/Smith_chart_gen.svg"),m_chart);
    //    qDebug() << "Ok ako je true ucitana je:" << smithPixmap.pixmap().isNull();
    //    m_chart->scene()->addItem(&smithPixmap);

    //    smithPixmap.show();
    //    auto ourSmith =  scene()->addPixmap(QPixmap(":/Smith_chart_gen.svg"));
    QPixmap scaledPix(":/SmithPix");
    //Sledeca linija je samo proba, posto scaledToWidth vraca skaliranu sliku
    //Dakle lancamo sve ovo da bi smo skalirali u odnosu na viewPort charta
    auto scaleWidthPix =  scaledPix.scaledToWidth(m_chart->plotArea().width(),Qt::SmoothTransformation);
    auto fullScalePix = scaleWidthPix.scaledToHeight(m_chart->plotArea().height(),Qt::SmoothTransformation);
    //Ovde dodajemo pixmap i dobijamo pointer na isti objekat
    auto pixItem =  m_chart->scene()->addPixmap(fullScalePix);


    //Ostaje jos da mu podesimo koordinate
    //    pixItem->setY(m_chart->mapToPosition(QPointF(-1,0).y(),series1));
    //    pixItem->setX(m_chart->mapToPosition(QPointF(-1,0).x(),series1));
    pixItem->setPos(m_chart->mapToPosition(QPointF(-1-(1-0.739),1),series1));
    pixItem->setZValue(11.0);
    //    series1->hide();
    axisX->hide();
    axisY->hide();



    //    auto ourSmith = scene()->addPixmap(QPixmap(":/SmithPix"));

    //Ok ovaj nacin radi...problem bio sa resursima
    //    auto testPixmap = scene()->addPixmap(QPixmap(":/qtLogo"));
    //    QRectF isExist = testPixmap->boundingRect();
    //    bool proverea =  QPixmap(":/qtLogo").isNull();
    //    testPixmap->pixmap();

    //    samplePixmap = m_chart->scene()->addPixmap(QPixmap(":/SmithPix"));
    //    samplePixmap->setScale(0.5);
    //    QRectF pixDimensions = samplePixmap->boundingRect();


    //Ovo sa brushom ne valja
    //    qreal width = m_chart->plotArea().width();
    //    qreal height = m_chart->plotArea().height();
    //    QPixmap smithBackground(":/SmithPix");

    //    smithBackground = smithBackground.scaled(QSize(m_chart->plotArea().width(),
    //                                                   m_chart->plotArea().height()),Qt::IgnoreAspectRatio);
    //    m_chart->setPlotAreaBackgroundBrush(QBrush(smithBackground));
    //    m_chart->setPlotAreaBackgroundVisible(true);
    //    m_chart->scene()->setBackgroundBrush(QBrush(QPixmap(":/SmithPix")));
    //    testPixmap->show();
    //    m_chart->hide();
    //auto ourPixDimension = smithPixmap->boundingRect();
    //    samplePixmap = new QGraphicsPixmapItem();
    //    samplePixmap->setPixmap(QPixmap(":/qt_logo_green_rgb_whitespace.png"));
    //    m_chart->scene()->addItem(samplePixmap);

}

void View::drawSmithGrid3()
{
    //Ovo je treca verzija sa slikom koju generisemo kada treba
    // da iscrtamo smitov dijagram
    //Koliko krugova crtamo kontrolisemo preko broja krugova
    int brojKrugova = 10;

    //Za ovo nam treba ipak nesto sto ce da imitira ono
    // sto smo imali kod mainWindowa

    //Ovo inace sto je veci pravougaonik veca je rezolucija
    // pa su finije linije smitovog dijagrama
    QRectF samplePicRect(0,0,640,640);


    QImage sourceImage(QSize(640,640),QImage::Format_ARGB32_Premultiplied);
    QPainter sourcePainter(&sourceImage);
    sourcePainter.setRenderHint(QPainter::Antialiasing);
    QRectF jedinicniKrugOkvir(QPointF(0,0),QPointF(samplePicRect.width(),samplePicRect.height()));
    sourcePainter.fillRect(jedinicniKrugOkvir,Qt::transparent);
    //sada crtamo jedinicni krug(zbog isecanja crtamo ga na kraju sa drugim painterom)
    //petlju pocinjemo od 1 posto bi za nulti bio crtan i jedinicni krug ponovo
    //    sourcePainter.drawEllipse(jedinicniKrugOkvir);
    for (int var = 1; var < brojKrugova ; ++var) {
        QRectF koncentricniOkvir(jedinicniKrugOkvir.topLeft() + QPointF(20*var,10*var),
                                 jedinicniKrugOkvir.bottomRight() + QPointF(0,-10*var));
        sourcePainter.drawEllipse(koncentricniOkvir);
    }
    //Aj da dodamo i jos ove male krugove
    QRectF unutrRezKrug(QPointF(samplePicRect.width()/2,samplePicRect.height()/4),
                        QPointF(samplePicRect.width(),(3.0/4.0)*samplePicRect.height()));
    sourcePainter.drawEllipse(unutrRezKrug);

    //za crtanje gornjih i donjih krugova nam treba neki scaleFactor
    int scaleFactor = 40;

    //Aj da nacrtamo gornjeAdmitansneKrugove
    QPointF gornjaTacka(samplePicRect.width()/2.0,-1*samplePicRect.height());
    QPointF donjaTacka(2*samplePicRect.width(),0);
    QRectF gornjiAdmitansniKrug(QPointF(samplePicRect.width()/2.0,-1*samplePicRect.height()),
                                QPointF((3.0/2.0)*samplePicRect.width(),samplePicRect.height()/2.0));
    sourcePainter.drawEllipse(gornjiAdmitansniKrug);
    for (int var = 0; var < brojKrugova; ++var) {
        QRectF admKoncentricniKrugovi(gornjiAdmitansniKrug.topLeft() + QPointF(-1*scaleFactor*var,-1*(scaleFactor/2)*var),
                                      gornjiAdmitansniKrug.bottomRight() + QPointF(scaleFactor*var,0));
        sourcePainter.drawEllipse(admKoncentricniKrugovi);
    }
    //I ovde dodajemo male krugove
    QRectF maliAdmKrugGornji(QPointF((3.0/4.0)*samplePicRect.width(),0),
                             QPointF(samplePicRect.width()+0.25*samplePicRect.width(),(0.5)*samplePicRect.height()));
    sourcePainter.drawEllipse(maliAdmKrugGornji);

    //Aj sada donje admitansne krugove
    //Ovaj pocetni valja
    QRectF donjiAdmitansniKrug(QPointF(samplePicRect.width()/2.0,samplePicRect.height()/2.0),
                               QPointF((3.0/2.0)*samplePicRect.width(),(3.0/2.0)*samplePicRect.height()));
    sourcePainter.drawEllipse(donjiAdmitansniKrug);

    //sada crtamo i ostale

    for (int var = 0; var < brojKrugova; ++var) {
        QRectF donjiAdmKoncenKrugovi(donjiAdmitansniKrug.topLeft() + QPointF(-1*scaleFactor*var,0),
                                     donjiAdmitansniKrug.bottomRight() + QPointF(scaleFactor*var,(scaleFactor/2)*var));
        sourcePainter.drawEllipse(donjiAdmKoncenKrugovi);
    }
    //Ovo dodajemo donji mali admitansniKrug
    QRectF maliAdmKrugDonji(QPointF((3.0/4.0)*samplePicRect.width(),samplePicRect.height()/2),
                            QPointF(width()+0.25*samplePicRect.width(),samplePicRect.height()));
    sourcePainter.drawEllipse(maliAdmKrugDonji);

    //Ovde smo zavrsili iscrtavanje na source slici
    // sada idemo na destination

    //Koristimo setClipRegion
    QImage destinationImage(QSize(640,640),QImage::Format_ARGB32_Premultiplied);
    //    QImage destImage(500,500);
    QPainter srcPaintClipTest(&destinationImage);
    srcPaintClipTest.fillRect(jedinicniKrugOkvir,Qt::transparent);
    srcPaintClipTest.setRenderHint(QPainter::Antialiasing);
    //Ovo pravi interesantan Efekat
    //    QRegion region(QRect(0,0,401,401),QRegion::Ellipse);
    QRegion region(QRect(0,0,samplePicRect.width(),samplePicRect.height()),QRegion::Ellipse);
    //Sada da klipujemo
    srcPaintClipTest.setClipRegion(region);
    srcPaintClipTest.drawImage(0,0,sourceImage);



    //Aj da docrtamo jos i jedinicni rezistivni krug koji
    // smo morali da izdvojimo zbog klipa
    QImage destImageSecond(QSize(640,640),QImage::Format_ARGB32_Premultiplied);
    QPainter secondPainter(&destImageSecond);
    secondPainter.fillRect(jedinicniKrugOkvir,Qt::transparent);
    secondPainter.drawImage(0,0,destinationImage);
    secondPainter.drawEllipse(jedinicniKrugOkvir);

    //Kraj upotrebe secondPainter-a

    //Ovde zavrsavamo operaciju formiranja slike smitovog
    // dijagrama koja nam je iscrtana na destinationImage

    //Ovaj ce da crta rezultate(ostalo iz drugog projekta gde smo
    // ovo radili

    //    QPainter widgetPainter(this);
    //    widgetPainter.setRenderHint(QPainter::Antialiasing);
    //    widgetPainter.drawImage(0,0,destinationImage);
    //    //tek sada crtamo jedinicni krug
    //    widgetPainter.drawEllipse(jedinicniKrugOkvir);

    //Sada ostaje da napravimo pixmap Objekat

    // Odmah ga pretvaramo u pixmap, preko static metode fromImage
    //    QPixmap smithDiagramPicture(QPixmap::fromImage(destinationImage));
    QPixmap smithDiagramPicture(QPixmap::fromImage(destImageSecond));
    //    smithDiagramPicture.fromImage(&destinationImage);
    //Dalje je isto kao u drawSmithGrid2();

    auto testArea = m_chart->plotArea();
    auto scaleWidthPix =  smithDiagramPicture.scaledToWidth(m_chart->plotArea().width(),Qt::SmoothTransformation);
    auto fullScalePix = scaleWidthPix.scaledToHeight(m_chart->plotArea().height(),Qt::SmoothTransformation);
    //Ovde dodajemo pixmap i dobijamo pointer na isti objekat
    //Imamo da ga izdvojimo u membere pixItem da bi mogli
    // da ga dohvatimo i ne bi morali da ga pretrazujemo
    pixItem =  m_chart->scene()->addPixmap(fullScalePix);
    pixItem->setOpacity(0.2);
    //    pixItem->stackBefore(m_tooltip);


    //Ostaje jos da mu podesimo koordinate
    //    pixItem->setY(m_chart->mapToPosition(QPointF(-1,0).y(),series1));
    //    pixItem->setX(m_chart->mapToPosition(QPointF(-1,0).x(),series1));
    pixItem->setPos(m_chart->mapToPosition(QPointF(-1,1),series1));
    pixItem->setZValue(50);

    //Ovo za debug, resili ovo sa setOpacity(0.2)
    //    auto nasaPixZValue = pixItem->zValue();
    //    auto odTooltipaZValue = m_tooltip->zValue();

    //    pixItem->setZValue(11.0);
    //    series1->hide();
    //    axisX->hide();
    //    axisY->hide();



}

void View::removeSmithGrid()
{
    //Sklanja smitov grid koji
    // je QGraphicsPixmapItem
    // Sluzi kada iz smita idemo u dekartov sistem
    m_chart->scene()->removeItem(pixItem);

}

void View::ourCartesianToSmithChart()
{
    //Ovde menjamo kartezijan(dekart) u smita
    // Ovo zovemo iz akcija u mainwindow-u
    cartToCartesianToSmithMinMaxSave();

    setOurXAxisMin(-1);
    setOurXAxisMax(1);
    setOurYAxisMin(-1);
    setOurYAxisMax(1);

    //setujemo da je presao u smitov Dijagram
    setIsSmithChart(true);
}

void View::ourSmithToCartesianChart()
{
    //Ovde menjamo smitov dijagram u kartezijan(dekart)
    // Ovo zovemo iz akcija u mainwindowu
    // prvo uklonimo smitov grid
    removeSmithGrid();

    //Pa vracamo stare koordinate
    //TODO eventualno videti koje su od ovih
    // metoda kandidati za private
    cartToSmithhToCartesianMinMaxLoad();

    //setujemo da nije smitov dijagram
    setIsSmithChart(false);
}

void View::cartToCartesianToSmithMinMaxSave()
{
    //Ovde cuvamo stare vrednosti osa
    // da bi mogli da ih vratimo
    // da ne bi smo mnogo filozofirali uzecemo sa osa
    setOldXmin(axisX->min());
    setOldXmax(axisX->max());
    setOldYmin(axisY->min());
    setOldYmax(axisY->max());

}

void View::cartToSmithhToCartesianMinMaxLoad()
{
    //Ovu metodu zovemo kada vracamo iz smita
    // u dekartov(Cartesian) chart pa da vratimo stare
    // vrednosti osa
    axisX->setMin(getOldXmin());
    axisX->setMax(getOldXmax());

    axisY->setMin(getOldYmin());
    axisY->setMax(getOldYmax());

}

