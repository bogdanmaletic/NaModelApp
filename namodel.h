#ifndef NAMODEL_H
#define NAMODEL_H

#include <QObject>
#include <QDebug>
#include <QVector>
#include <QTimer>

#include <complex>

//Aj da vidimo kako se ponasa sa OurDut objektima
#include "ourdut.h"
#include "noisemodel.h"
//#include "freqsparplain.h"

//Aj da isprobamo sa freqspar elementima,zbog pripadajucih
// frekvencija
/*
 * Uz dosadasnje parametre
 *
 * Komunikacije izmedju NAmodela i OurDut objekta se odvija
 * preko parametara.
 *
 * Za merenje NAmodel mora da posalje OurDutu :
 * (smatracemo da salje validne podatke)
 *
 * freqStartMeasure -> frekvencija od koje pocinje merenje
 * freqStopMeasure -> frekvencija do koje se meri
 * numOfPointsMeasure -> broj tacaka koje zelimo da proracunamo
 *
 * Ovi parametri nam diktiraju:
 * deltaFMeasure -> frekvencijski razmak izmedju tacaka koje merimo
 *
*/

typedef std::complex<double> ComplexDouble;

class NAmodel : public QObject
{
    Q_OBJECT
public:
    explicit NAmodel(QObject *parent = nullptr);

    void initSparams();                             //metoda za debug i testiranje(posle cemo do sa ourdutom)

    void calculateOnOneFreq(qreal freq);            //Ovde su proracuni na jednoj frekvenciji
    void calculateOnAllFreq();

    //Aj da krenemo da dohvatimo podatke na jednoj frekvenciji
    //mada pre nego sto krenemo da dohvatamo podatke
    // moramo da znamo koje mozemo da dohvatimo
    // mada mozda cemo morati i da zahtevamo promenu od dut-a
    // da ponovo interpolira podatke
    void getMeasureParameters();
    void getDataFromOneFreq(qreal freqOfData);



    //Ove smo morati da dodamo zbog sinhronizacije sa pogledom
    qreal getFStartMeasure() const;

    qreal getFStopMeasure() const;

    qreal getDeltaFMeasure() const;

    //Preko ovog parametra biramo S11 S12 S21 ili S22
    int getSelektovaniParametar() const;
    void setSelektovaniParametar(int value);

    //MagCalc i phaseCalc nam trebaju ukoliko
    // hocemo da omogucimo kalkulaciju magnitude i faze
    int getMagCalc() const;
    void setMagCalc(int value);

    int getPhaseCalc() const;
    void setPhaseCalc(int value);

    //Preko ovih funkcija setujemo koju seriju vrednosti saljemo preko signal slota
    //Dakle ovo su guardovi kalkulacija
    bool getLinMagGuard() const;
    void setLinMagGuard(bool value);

    bool getLogMagGuard() const;
    void setLogMagGuard(bool value);

    bool getPhaseGuard() const;
    void setPhaseGuard(bool value);

    bool getLinPhaseGuard() const;
    void setLinPhaseGuard(bool value);

    bool getLogPhaseGuard() const;
    void setLogPhaseGuard(bool value);

    bool getRealGuard() const;
    void setRealGuard(bool value);

    bool getImagGuard() const;
    void setImagGuard(bool value);

    bool getSwrGuard() const;
    void setSwrGuard(bool value);

    bool getZParamGuard() const;
    void setZParamGuard(bool value);

    bool getYParamGuard() const;
    void setYParamGuard(bool value);

    bool getSmithChartGuard() const;
    void setSmithChartGuard(bool value);

    //Zbog potrebe da menjamo format podataka
    // uvodimo metodu koja resetuje sve guardove
    void resetAllFormatGuards();



    //Ovi seteri nam trebaju da ih setujemo
    // kada ih preko akcija menjamo iz MainWindowa
    // Koji se ponasa kao kontroler
    void setFStartMeasure(const qreal &value);

    void setFStopMeasure(const qreal &value);

    int getNumOfPointsMeasure() const;
    void setNumOfPointsMeasure(int value);

    void setDeltaFMeasure(const qreal &value);

    //    int getNumOfPointsMeasure() const;
    //    void setNumOfPointsMeasure(int value);


    //Ove metode su nadlezne za menjanje max i min
    // tacaka koje koristimo za autoscale
    qreal getMaxPoint() const;
    void setMaxPoint(const qreal &value);

    qreal getMinPoint() const;
    void setMinPoint(const qreal &value);

    void maxMinCalc(qreal &pointToCompare);

    //Ovo je za setovanje guarda za nalazenje
    // polazne tacke min i max(ne moze 0,0 da se trazi
    // zbog kvadratne funkcije
    bool getMaxMinPointGuard() const;
    void setMaxMinPointGuard(bool value);

    //Ovo nam treba za setovanje snage izvora
    qreal getGenP() const;
    void setGenP(const qreal &value);

    void setChangedNumberOfPoints(int changedNumberOfPoins);            //menja broj tacaka uz novu interpolaciju





    //Ovo su guardovi koji otljucavaju punjene average bafera
    bool getAverageGuard() const;
    void setAverageGuard(bool value);

    //geter i seter average ciklusa(koristi se u proracunu)
    int getAverageCycles() const;
    void setAverageCycles(int value);

    //ovo koristimo u proracunu
    int getAveragePointPos() const;
    void setAveragePointPos(int value);

    //Ovaj koristimo samo na setovanju i na kraju
    // da bi smo znali sa koliko da podelimo
    int getAverageCyclesNumber() const;
    void setAverageCyclesNumber(int value);

    //Svaki put kada menjamo pogled resetovacemo i sweepFreq
    void resetSweepFreq();

    bool getCancelCalculationGuard() const;
    void setCancelCalculationGuard(bool value);

    //Ova metoda nam treba za inicijalizaciju calDut-a
    void loadCalDut();
    QVector<QPointF> getCalData();

    qreal getSweepFreq() const;
    void setSweepFreq(const qreal &value);

    // Za setovanje izbora kalibracionih i nekalibrisanih
    // setova podataka S parametara
    bool getCalGuard() const;
    void setCalGuard(bool value);

    //Za pristup calculationGuard
    // koji po potrebi stopira performCalculation();
    bool getCalculationGuard() const;
    void setCalculationGuard(bool value);

    //Kada ukljucujemo i iskljucujemo i restartujemo average
    // potrebno je da ispraznimo average bafer
    void clearAverageBuffer();

    //Ovo nam treba za promenu IFBW vremena
    int getIFBWtime() const;
    void setIFBWtime(int value);

    //razdvojeno je zbog jednostavnosti
    // kada menjamo ifbw menjamo i ovo(u mainwindowu ga setujemo)
    qreal getIfbwFreqModel() const;
    void setIfbwFreqModel(const qreal &value);

public slots:
    //Aj da dodamo timer koji ce izvrsavati proracune na sveke dve sekunde
    void performCalculation();
    void testTimer();
    void processPoint(const FreqSparPlain &calcPoint);                             //Ovo samo testiramo kako bi moglo da komunicira(Za brisanje)
    void ourSimpleSlot();                                                          //Za test slot
    void drugiSlot(int sig);                                                       //Za test slot
    void doWork();                                                                 //Ovaj nam je za WorkerThread (dakle slot nam je ovaj a signal pointIsCalculated)


signals:
    void pointIsCalulated(const FreqSparPlain &calcPoint);                         //Kada se tacka izracuna ovo se emituje
    void ourSimpleSignal();                                                         //test signali
    void drugiSignal(int sig);                                                      //test signali
    void simplePointIsCalculated(const QPointF &simplePoint);
    void cancelNaModelViewSig();                                                  //Signal koji prekida qued conn
    void calcIsFinished();                                                        //Signal koji se emituje na kraju kalkul.
    void eraseViewBuffer();                 //signal koji signalizira view-u da izbrise svoje bafere
    void inspectTimer();                    //signal da ispitamo timer u parentu
    void modelRequesToSleep();              // nakon kalkulacija model trazi sleep

    //Kada menjamo formate treba nam nesto da startuje
    // kalkulacijiju
    void sigRestartCalculation();

private:
    const qreal fStart{0.3};                    //start frekvencija NA (300 kHz = 0.3 MHz)
    const qreal fStop{3000};                    //stop frekvencija NA  (3GHz = 3000 MHz)
    qreal genP{1};                        // snaga generatora u mW (milivatima, default vrednost je 1mW)
    qreal sweepFreq = 0;                            //koristimo mHz i DBm za generator ( trenutna frekvencija za sweep)
    const int numOfPoints {1601};               // Default broj tacaka (za nas tip analizatora 1601)
    const qreal opseg{fStop - fStart};          //opseg koristimo u interpolacionom algoritmu
    const qreal deltaF{opseg/numOfPoints - 1};  //medjufrekvencijsko rastojanje NA ( (fstop - fstart)/ numOfPoints)   )

    //Aj da definisemo parametre koje
    // cemo predavati OurDutu
    //za razliku od prethodnih ovi su promenjlive kategori
    qreal fStartMeasure;
    qreal fStopMeasure;
    qreal deltaFMeasure;
    int numOfPointsMeasure;


    ComplexDouble complexValue;             //za test
    QVector<ComplexDouble> sParams;         //fiktivni S(i,j) vektor (za test)

    OurDut dut;                             //model duta (za sada ce biti member ovog modela)
    NoiseModel noiseModel;                  // noise model
    OurDut *calDut;

    //Ovaj nam je jedan od najbitnijih
    // za smestanje podataka( bice potrebno za kalibracije,
    // averaging i osnova za komunikaciju sa pogledom
    QVector<FreqSparPlain> measuredData;    //Ovde cemo smestati proracunate podatke

    //Treba nam i timer da za zadato vreme izvrsi rekalkulacije
    QTimer *timer;

    int testSignalPoint{0};

    //Treba nam jedan selektor da vidimo koje cemo podatke da prosledimo
    int selektovaniParametar;

    //Trebaju nam serije vrednosti da mozemo da znamo koju tacku da emitujemo
    bool linMagGuard = false;
    bool logMagGuard = false;
    bool phaseGuard = false;

    bool linPhaseGuard = false;
    bool logPhaseGuard = false;

    bool realGuard = false;
    bool imagGuard = false;

    bool swrGuard = false;

    //naravno treba nam i za Z i Y parametre...
    bool zParamGuard = false;
    bool yParamGuard = false;

    // Poseban guard nam treba za smitov dijagram
    //Ovo je posebna verzija guarda i vezuje se uz phaseguardove zbog istog racunanja
    // parametara. Jedino ako je true umesto faze i mag na polarni prosledjuje tacke
    // tipa (real,imag)
    bool smithChartGuard = false;

    //Ova sledeca dva parametra nam trebaju ukoliko trazimo magnitutu ili fazu
    int magCalc = 0;
    int phaseCalc = 0;

    //Trebaju nam tacke za minimum i maksimum
    // za autoscale
    qreal maxPoint = 0.0;
    qreal minPoint = 0.0;
    bool maxMinPointGuard = true;       //Ovo je guard za nalazenje min i max

    //Treba nam i vektor za Average(average radimo
    // ako je averageGuard true i obavlja se nakon sto
    // se izracuna tacka u getDataFromOneFreq(qreal);
    QVector<FreqSparPlain> averageVector;               //average bafer
    bool averageGuard = false;
    int averageCycles = 0;              //broj average ciklusa
    int averagePointPos = 0;            //broj podatka u average baferu
    int averageCyclesNumber = 0;        // posto koristimo averageCycles za proracun treba nam koliko ih je bilo

    //Kada racunamo treba nam
    // nesto da prekinemo racunanje perform calculation
    bool cancelCalculationGuard = false;

    //Treba nam nesto da vrati podatke za calDut
    QVector<FreqSparPlain> emptyVector;

    //Potreban nam je guard za biranje
    // pravog ili kalibracionog seta Sparametara
    // ako cita nekalibrisane podatke onda je true
    // ako hocemo da cita kalibrisane onda je false
    bool calGuard = true;

    //Potreban nam je calculation guard
    // sve dok je on true, performCalculation(); ima da
    // zove samog sebe sa zadrskom
    bool calculationGuard = true;

    //kada menjamo IFBW filtar menjace se vreme sweep-a
    //razdvojen je u dve varijable zbog jednostavnosti
    int IFBWtime = 0;
    qreal ifbwFreqModel = 30;

};

#endif // NAMODEL_H
