#ifndef FREQSPARPLAIN_H
#define FREQSPARPLAIN_H

/*
 * Kako izgleda Qvector ne moze da sadrzi QObjekte aj da probamo sa ovim
*/
#include <QVector>
#include <complex>

typedef std::complex<double> SparamComplexPlain;


class FreqSparPlain
{
public:
    FreqSparPlain();

    double getFrequency() const;
    void setFrequency(double value);

    SparamComplexPlain getS11() const;
    void setS11(const SparamComplexPlain &value);



    //metode za operacije nad svim parametrima
    QVector<SparamComplexPlain> getSparameters() const;
    void setSparameters(const QVector<SparamComplexPlain> &value);

    //Ovo za postavljanje parametara(treba nam kada citamo vrednosti
    // formiramo objekte ovog tipa
    void setOneParameter(const SparamComplexPlain &value);

    //Ovaj operator nam treba za kada sabiramo ove objekte
    // u usrednjavanju
    void operator+=(const FreqSparPlain &otherObj);
    //Ovaj metoda nam treba da podelimo sve sparametre
    // sa brojem u usrednjavanju
    void divideSparams(qreal delilac);


private:
    double frequency;
    SparamComplexPlain s11;             //Ovaj za brisanje

    //Aj da mu postepeno dodamo parametre za dvoportnu mrezu
    // znaci maksimum u ovome je da imamo 4 parametra po jednoj frekvenciji
    QVector<SparamComplexPlain> Sparameters;


};

#endif // FREQSPARPLAIN_H
