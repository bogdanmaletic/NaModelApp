#ifndef FREQSPAR_H
#define FREQSPAR_H

#include <QObject>
#include <QVector>

#include <complex>

typedef std::complex<double> SparamComplex;

/*
 * predstavlja kompoziju frekvencije i pripadajucih S parametara na toj ucestanosti,
 * za sada jednostavna realizacija ima jedna frekvencija i jedan kompleksan parametar
 * (kasnije ce biti prosiren)
 * Izgleda da ne moze qobject i kontejner klase
 * aj da napravimo realizaciju jednostavnih objekata
 *
 * Ali moze da se radi sa pokazivacem na te objekte ukoliko nam
 * treba, postupak je sledeci:
 * QVector<FreqSpar*> points;
    FreqSpar *pointerToPoint1 = new FreqSpar(0.1,1.0,1.0);
    FreqSpar *pointerToPoint2 = new FreqSpar(1,2.2,15);
    FreqSpar *pointerToPoint3 = new FreqSpar(3,4.0,7);

    points << pointerToPoint1 << pointerToPoint2 << pointerToPoint3;

    na ovaj nacin radi...ako bude potrebe lako cemo ovo da vratimo
    (za sada cemo koristiti FreqSparPlain objekte)
*/

class FreqSpar : public QObject
{
    Q_OBJECT
public:
    explicit FreqSpar(QObject *parent = nullptr);
    explicit FreqSpar(qreal freq,
                      qreal realPart,
                      qreal imagPart,QObject *parent = nullptr);


    //Ovo smo cisto generisali funkcije da bi smo mogli da im pristupimo
    qreal getFrequency() const;
    void setFrequency(const qreal &value);

    SparamComplex getSparam11() const;
    void setSparam11(const SparamComplex &value);

    QString simpleReport();

signals:

public slots:

private:
    qreal frequency;        //mora da bude realan posto je u Mhz
    SparamComplex sparam11;        //Kompleksni parametar

};

#endif // FREQSPAR_H
