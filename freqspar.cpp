#include "freqspar.h"

FreqSpar::FreqSpar(QObject *parent) : QObject(parent)
{
    //Ovo su zadate vrednosti
    frequency = 0.1;
    sparam11.real(1.0);
    sparam11.imag(1.0);

}

FreqSpar::FreqSpar(qreal freq, qreal realPart, qreal imagPart, QObject *parent) : QObject(parent)
{
    frequency = freq;
    sparam11.real(realPart);
    sparam11.imag(imagPart);

}

qreal FreqSpar::getFrequency() const
{
    return frequency;
}

void FreqSpar::setFrequency(const qreal &value)
{
    frequency = value;
}

SparamComplex FreqSpar::getSparam11() const
{
    return sparam11;
}

void FreqSpar::setSparam11(const SparamComplex &value)
{
    sparam11 = value;
}

//Cista raport metoda, za debug
QString FreqSpar::simpleReport()
{
    return QString("Imamo frekvenciju %1 i parametar{%2,%3} ")
            .arg(frequency)
            .arg(sparam11.real())
            .arg(sparam11.imag());
}
