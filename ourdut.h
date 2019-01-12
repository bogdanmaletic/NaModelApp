#ifndef OURDUT_H
#define OURDUT_H

#include <QObject>
#include <QVector>
#include <QString>
#include "freqsparplain.h"
#include "parameterreader.h"



/*
 * Ovo je model Dut-a koji je karakterizovan listom
 * frekvencija i vrednosti koji ih karakterisu.
 * Radi jednostavnosti modela sve rekalkulacije vezane
 * za interpolaciju koja je povezana sa zadanim brojem tacaka.
 * Umesto FreqSpar koristimo FreqSparPlain zbog jednostavnosti rada
 * sa QVector objektima.
 * Za citanje i parsiranje parametara cemo iz callout projekta gde smo
 * definisali pogled izvucicemo objekte koji sluze bas za to.
 *
 * Nedostaje jedino deo za dohvatanje podataka(koji cemo dodati naknadno)
 * Hmmm izgleda da nam interpolator(measured dakle na setu podataka) pravi probleme...
*/

class OurDut : public QObject
{
    Q_OBJECT
public:
    explicit OurDut(QObject *parent = nullptr);
    //    void generateFakeData();

    //    FreqSpar getSparObject(qreal frequency);

    // ! Ovi su odredjeni za brisanje
    // Tacnije ove metode nam nisu potrebne za dalji rad
    int getNumberOfPointsDut() const;
    void setNumberOfPointsDut(int value);       //Ako menjamo broj tacaka moramo da uradimo interpolaciju
    //medjutim zbog jednostavnosti nju cemo izdvojiti u posebnu private
    //metodu

    QString raportInterpolatedData();           //Ovo nam cisto treba za raportovanje

    //Ove nam trebaju da bi smo dohvatili podatke
    QVector<FreqSparPlain> getVectorSparama() const;
    QVector<FreqSparPlain> getVectorSparamaInterpolated() const;

    //Ovaj nam treba za fiktivni dut
    // Posle cemo da vidimo kako ce nam proslediti pointer
    QVector<FreqSparPlain> getVectorSparamaPointer() const;




    //Nama treba podatak samo za izabrane frekvencije
    // zapravo za komunikaciju sa NaModelom nama treba samo ovaj
    FreqSparPlain getDataFromFreq(qreal frequency);

    //Treba nam i varijanta koja operise sa interpoliranim
    // podacima getDataFromFreq(qreal freq) metode

    FreqSparPlain getMeasuredDataFromFreq(qreal frequency);

    //kada se promeni broj tacaka zovemo interpolate metodu koja
    // je u privatnom delu, iz prostog razloga sto je povezana sa
    // setNumberOfPointsDut(int value);

    //Ovde dodajemo metode koje su nam bitne za rad sa NA modelom

    //Sve cetiri metode nam trebaju kada se NAmodel povezuje
    // na odredjeni Dut da bi mogao da dohvati parametre
    qreal getFreqStartMeasure() const;
    qreal getFreqStopMeasure() const;
    int getNumberOfPointsDutMeasure() const;
    qreal getDeltaFMeasure() const;

    //za pocetak faze komunikacije sa NA modelom
    // treba da prilagodimo dut merenju
    // smatramo da su podaci vec dohvaceni
    // vise o ovome na strani 30 sveske
    void setMeasureParameters(qreal fstmstartmeasure,
                              qreal fsmstopmeasure,
                              int nopmeasure);

signals:

public slots:
private:

    //Za ove parametre nam se odnosi na svih 1601 tacka
    // na frekvencijma od 0.3 MHz do 3000 MHz

    QVector<FreqSparPlain> vectorSparama;      //U vector su nam smesteni podaci o freq i pripadajucim parametrima,
    // Inace ovde se smestaju podaci nakon citanja parametara iz fajla

    int numberOfPointsDut;                     //Ovo je broj tacaka definisan za dut

    void interpolate(int newNumOfPoints);      //Kada se promeni broj tacaka zovemo ovu metodu

    QVector<FreqSparPlain> vectorSparamaInterpolated;   //kada proracunamo(interpolaciju),
    //moramo negde da smestimo

    //Sledece ce nam biti privremene varijable koje
    // cemo ostaviti, prvo smo ih stavili da razvijamo i testiramo
    // objekat, medjutim ostavicemo ih posle u radu( mada ukoliko bude potrebe
    // interpolaciju cemo izvuci u poseban objekat ukoliko nam bude bila potrebna i na
    // raznim drugim mestima(to na prvi pogled da ce trebati medjutim, mozda nam nece ni trebati.
    // Eventualno cemo morati da interpoliramo i vektore koji nam se odnose na kalibrisane podatke

    //Ovo su nam parametri vezani za ceo Ne
    qreal freqStart, freqStop, deltaF, deltaFInterpolate;
    int numberOfPointsDutInterpolate;

    //parametri koji nam se odnose na samo merenje dakle ne na cele parametre
    //mozda cemo morati da menjamo celokupnu postavku duta-a
    // za sada cemo da se zadovoljimo sa dodavanjem
    qreal freqStartMeasure;
    qreal freqStopMeasure;
    int numberOfPointsDutMeasure;
    qreal deltaFMeasure;



    //sada kada smo setovali parametre (u okviru setMeasureParameters(...);)
    //treba nam interpolacija
    // mi bi smo trebali da vrsimo interpolaciju samo za opseg
    // od interesa (medjutim da ne bi smo komplikovali uradicemo
    // za celokupan set podataka.
    void interpolateMeasure(int changedNumOfPoints);

    //Aj da dodamo i parametarReader;
    ParameterReader paramRead;



};

#endif // OURDUT_H
