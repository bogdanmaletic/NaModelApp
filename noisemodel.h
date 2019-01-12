#ifndef NOISEMODEL_H
#define NOISEMODEL_H

#include <QObject>
#include <random>

//Ovaj ubacujemo zbog tipa u njemu...mozda bi
// mogli to da izdvojimo u poseban
#include "freqsparplain.h"

/*
 * Ovo nam je model suma koji cemo koristiti da bi smo dobili sum
 * u mreznom analizatoru.
 * Razmisliti kako prosiriti model da ukljucuje vrednost IF BW filtra.
 * Kao rezultat poziva mora da vrati kompleksnu vrednost koja se u stvari
 * moze interpretirati kao signal suma pa se moze sabirati sa signalima
 * predajnika.
 */

class NoiseModel : public QObject
{
    Q_OBJECT
public:
    explicit NoiseModel(QObject *parent = nullptr);

    void generate100random();                       //za razvijanje objekta
    void simplerGeneratorUniform();                 //za razvijanje objekta
    void simpleNormalDistribution();                //za razvijanje objekta

    double getNextRandom();                         //Ova nam treba za generisanje magnitude
    int getNextInt();                               //Ovo nam treba za fazu

    //Ova nam jedino zaista treba
    // sve ostale mogu
    SparamComplexPlain getComplexRandom();          //cisto generisanje bez obzira na nivo suma
    SparamComplexPlain getComplexRandomWithNoisePower();// generisanje sa obzirom na snagu suma

    qreal calculateInDbm(qreal parametarToCalculate);   //verovatno nam nece trebati (calc u dbm)


    //pored setovanja ifBwFreq menja i power of noise
    qreal getIfbwfreq() const;
    void setIfbwfreq(const qreal &value);               //ne mora nista spec da se radi automatski preracunava



signals:

public slots:

private:    
    //Aj da ubacimo i IF Bw
    // morali smo da ga premestimo na pocetak radi kalkulacije
    qreal ifbwfreq;                               //racunacemo ga u khz
    const qreal boltzmanConstant {1.38e-23};    //cisto da imamo i ovu konstantu
    const int temperature{300};                 // temperatura u kelvinima
    qreal powerOfNoise =  boltzmanConstant*
            temperature*ifbwfreq * 1e3;    //snaga suma, varirace oko ove vrednosti

    //Ovo je bitno za parametre raspodele(verovatno
    //moraju da budu zavisni od snage suma)
    // Ovo za sada su nam test vrednosti
    qreal standard_deviation = 0.5;             //Ovo je standardna vrednost po Gausu
    qreal srednja_vrednost = 1;                   // srednja vrednost ce biti snaga suma

    std::default_random_engine generator;
    //    std::normal_distribution<double> distribution(upperLimit,lowerLimit);
    qreal generisaniBroj;

    //Ovi nam zapravo trebaju za ceo posao
    std::random_device rd;
    std::mt19937 gen{rd()};

    std::normal_distribution<> dist{srednja_vrednost,standard_deviation};
    std::uniform_int_distribution<> uniformDist{-1,1};

    //Za rekalkulaciju powerofNoise kada menjamo ifbwFreq
    void recalculatePowerOfNoise();
};

#endif // NOISEMODEL_H
