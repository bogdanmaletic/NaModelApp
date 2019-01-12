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

#ifndef VIEW_H
#define VIEW_H
#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QMouseEvent;
class QResizeEvent;

//dodacemo i liniju cisto da bi
// mogli da je setamo po ekranu
// pogledati napomenu kasnije ocistiti
class QGraphicsLineItem;
class QGraphicsPixmapItem;
//jos da dodamo i nas window sa kontrolama (ovo ne cistiti)
//class SampleControls;

//Ovo smo dodali da imamo nas Callout(ourDataMarker)
class Callout;
//class ReaderObject;

//Ovaj nam treba za dodavanje tacaka
class FreqSparPlain;
//class QtCharts::QLineSeries;
class QElapsedTimer;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
//Ovo mora ovde da se doda izgleda
class QLineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

class Callout;

QT_CHARTS_USE_NAMESPACE

class View: public QGraphicsView
{
    Q_OBJECT

public:
    View(QWidget *parent = 0);
    void simpleDbCalc(double &valToCalculate);

    void setOurXAxisRange(int ourNewRange);                     //Ovaj definise od nula do ourNewRange
    void setOurXAxisMin(qreal ourMinX);                         //postavlja minimum x ose
    void setOurXAxisMax(qreal ourMaxX);                         //postavlja maksimum x ose

    void setOurYAxisMin(qreal ourMinY);                         //postovalja minimum y ose
    void setOurYAxisMax(qreal ourMaxY);                         //postavlja maksimum y ose

    void setOurYMinAxisRange(qreal ourMinYRange);
    void setOurYMaxRange(qreal ourMaxYRange);

    //TODO mozda bolje da se prosledjuje kao objekat
    void setMeasureParameters(qreal measureFrequencyStart,
                              qreal measureFrequencyStop,
                              qreal measureDeltaFreq,
                              int measureNumberOfPoints);          //Ovo nam sluzi da povezemo View i Namodel


    //Ove smo morali da dodamo zbog sinhronizacije sa NA modelom
    void setFreqStartMeasure(const qreal &value);

    void setFreqStopMeasure(const qreal &value);

    void setFreqDeltaFMeasure(const qreal &value);

    void initializeSeriesData();                                  //metoda ovo moramo da bi mogli posle replace

    int getNumberOfPoints() const;
    void setNumberOfPoints(int value);

    //Ovu metodu smo iz polarChart primera
    void switchChartType();

    //kada menjamo chartove iz mainwindow-a koji
    // je kontroler treba da znamo o kom se tipu charta radi
    int getChartType();

    //Za slucaj smita treba da iskljucimo iscrtavanje osa
    // Ako je true ukljucice ih, obrnuto ih iskljucuje
    void setGridVisible(bool isGridVisible);

    //Treba nam metoda za iscrtavanje smitovog dijagrama
    // automatski ce zvati metodu setGridVisible(); koja ce
    // verovatno biti prebacena u private
    void drawSmithGrid();
    //Posto pravi probleme aj da probamo da napravimo
    // item sa slikom
    void drawSmithGrid2();
    //Treca verzija
    void drawSmithGrid3();

    //Treba nam i metoda koja sklanja smith grid
    void removeSmithGrid();

    //Posto kada imamo Smitove chartove treba da ga menjamo u obicnog
    // u smitov i obrnut, da bi smo izbegli probleme ovde cemo da ga menjamo
    // Ove metode zovemo iz akcija u MainWindow-u
    void ourCartesianToSmithChart();
    void ourSmithToCartesianChart();

    //Prilikom vracanja u dekartov sistem trebamo
    // da vratimo vrednosti osa
    // jednostavno imamo stare koordinate kada se
    // to desi da bi mogli da ih vratimo
    void cartToCartesianToSmithMinMaxSave();
    void cartToSmithhToCartesianMinMaxLoad();

    qreal getOldYmin() const;
    void setOldYmin(const qreal &value);

    qreal getOldYmax() const;

    void setOldYmax(const qreal &value);

    qreal getOldXmin() const;
    void setOldXmin(const qreal &value);

    qreal getOldXmax() const;
    void setOldXmax(const qreal &value);

    //Kada menjamo chartove treba nam informacija
    // da li je prethodni chart smitov
    // korisno je i kod prelaza sa smitov na polarni
    bool isSmithChart = false;
    bool getIsSmithChart() const;
    void setIsSmithChart(bool value);

    //Aj da pokusamo da kada god menjamo pogled da klirujemo
    // pogled
    void clearViewSeriesBuffer();

    //Treba nam metode da view propusta iscrtavanje tacaka dok
    // se ne pokrene novi ciklus tacaka
    // koristi se uz namodel resetSweepFreq
    bool getIsPausedDraw() const;
    void setIsPausedDraw(bool value);


    //Ove metode nam trebaju kod izmene dijagrama
    // tj switchChartType();
    QGraphicsSimpleTextItem *getCoordX() const;
    void setCoordX(QGraphicsSimpleTextItem *coordX);

    QGraphicsSimpleTextItem *getCoordY() const;
    void setCoordY(QGraphicsSimpleTextItem *coordY);


    //Za dodavanje iscrtavanja CalDuta
    void drawCallDut(QVector<QPointF> callDutSeries);


    //Ovo nam treba da iz mainwindowa notifikujemo kada je serija
    // izbrisana
    QLineSeries *getSeries1() const;
    void setSeries1(QLineSeries *value);

    //Ovo nam sluzi za blokiranje crtanja tacaka
    bool getDrawGuard() const;
    void setDrawGuard(bool value);

    //Ovo je potrebno da podesimo kada menjamo broj
    // tacaka
    int getPointPos() const;
    void setPointPos(int value);

protected:
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void keepCallout();
    void tooltip(QPointF point, bool state);

    void moveOurTooltip(int indexOfDial);


    //Ovo je za promenu serije(ne treba nam za sada)
    void changeSeries();

    //Aj da dodamo za povezivanje sa modelom
    //Iscrtavacemo samo magnitudu 1 za pocetak
    void getDataToDrawFromNAModel(const FreqSparPlain &dataToDraw);

    //Aj da vidimo ovu malo bolju verziju koju cemo moci da koristimo
    void drawPoint(const QPointF &calculatedPoint);
    void drawPointBuffered(const QPointF &calculatedPoint);

    //Za brisanje bafera
    void eraseBuffers();
    //Za polagano iscrtavanje zove je singleShot timer za delay u iscrtavanju
    void replacePointsDelayed();
    //Treca implementacija iscrtavanja tacaka
    void drawPoint2(const QPointF &calculatedPoint);

signals:
    void sigDrawIsDone();       //Signal koji se emituje kada se zavrsi sa crtanjem



private:
    QGraphicsSimpleTextItem *m_coordX;
    QGraphicsSimpleTextItem *m_coordY;
    QChart *m_chart;
    Callout *m_tooltip;
    QList<Callout *> m_callouts;

    // Ovo se odnosi na liniju koju smo dodali da bi mogli
    // da se igramo sa linijom, kasnije da bi mogli da se
    // igramo sa prikazom grafickih objekata(sada vec nepotrebno)

    //SampleControls *sampleControl;

    //Ovo cemo dodati da bi smo znali koliko ovo da pomeramo
    qreal ourWidth;
    qreal ourHeight;
    qreal ourDialStepSize;

    //Ovo dodajemo da bi mogli da pomeramo nas pointer (ourDataMarker)
    Callout *ourDataMarker;
    QLineSeries *ourSeries;

    //Sada da dodamo za selektovanje serije
    int seriesSelected = 1;
    int maxNumberOfSeries = 2;      //kasnije cemo ovo malo
    QList<QLineSeries *> ourSeriesInList;

    //ReaderObj test
    //    ReaderObject *testReadObj;

    QLineSeries *series1;

    //Aj da dodamo i ostale
    QLineSeries *series2;
    QLineSeries *series3;
    QLineSeries *series4;

    QList<qreal> maxOfSeries;
    //Ovo moramo da dodamo cisto da bi smo bili sigurni u koordinate
    //Resili smo ali smo ostavili za svaki slucaj
    //    QGraphicsSimpleTextItem *ourX;
    //    QGraphicsSimpleTextItem *ourY;

    QPointF findMaxOfSeries(QLineSeries *series);
    QPointF findMinOfSeries(QLineSeries *series);

    int pointPos = 0;
    QVector<QPointF> *our_points;

    //Aj da dodamo sada ovde sve parametre koje mozemo
    // da menjamo
    QValueAxis *axisX;
    QValueAxis *axisY;

    //Ove nam trebaju da znamo odakle dokle prikazujemo podatke
    qreal freqStartMeasure;
    qreal freqStopMeasure;
    qreal freqDeltaFMeasure;
    int numberOfPoints;

    int timeToPopulateSeries = 0;
    QElapsedTimer *elapTimer;

    //Aj da probamo i sa ovim
    QVector<QPointF> testSeriesVector;              // Ovo je test vektor...
    QVector<QPointF> bufferDrawVector;              //Ovaj vektor nam treba za slot drawPointBuffered();

    //Test za razne elemente;
    QGraphicsRectItem *rectOnChart;
    QGraphicsEllipseItem *circleOnChart;
    QGraphicsEllipseItem *secondCircle;

    QVector<QGraphicsItem *> OurSmithItems;           //Ovde cemo da stavimo razne iteme

    //Aj da vidimo da li je problem do ovoga;
    QGraphicsPixmapItem *samplePixmap;
    QGraphicsPixmapItem *pixItem;

    //Trebaju nam i stare vrednosti osa
    // kada menjamo iz dekartovog u smit i obrnuto
    qreal oldYmin;
    qreal oldYmax;
    qreal oldXmin;
    qreal oldXmax;

    //Ova promenljiva je guard kojim preskacemo tacke koje dobijamo
    // i ne iscrtavamo ih
    bool isPausedDraw = false;

    //Ovo nam sluzi za pomoc u replace baferu
    QVector<QPointF> drawHelpBuffer;

    //treba nam nesto da signalizira da je prvobitno popunjavanje
    // tacaka gotovo, ako jeste 1 prvo je, a ako je razlicito od 1 ide dalje
    // za drawPoint2(); slot je
    int firstPopulateSeries = 1;
    //Aj da vidimo ovako
    QPointF currentPoint;

    //Hmm u metodi drawPoint imamo problem sa zaostalim tackama
    //Ako je true nece nista crtati
    bool drawGuard = false;
};

#endif
