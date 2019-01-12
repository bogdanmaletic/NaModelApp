#include "freqsparplain.h"

FreqSparPlain::FreqSparPlain()
{
    frequency = 0.1;
    s11.real(0);            //Iskoristicemo ovo za setovanje parametara
    s11.imag(0);

    //Ovo cisto da napravimo blank objekat
    // sada nema vise problema sa setOne parameter
    // frekvencija 0.1 sa nultim parametrima rasejanja
    // mada moguce je da ce praviti problem
    // Potrebno zbog FreqSparPlain OurDut::getDataFromFreq(qreal frequency)
    //Hmmm pravi probleme ovaj pristup
    //    setOneParameter(s11);
    //    setOneParameter(s11);
    //    setOneParameter(s11);
    //    setOneParameter(s11);


}

double FreqSparPlain::getFrequency() const
{
    return frequency;
}

void FreqSparPlain::setFrequency(double value)
{
    frequency = value;
}

SparamComplexPlain FreqSparPlain::getS11() const
{
    return s11;
}

void FreqSparPlain::setS11(const SparamComplexPlain &value)
{
    s11 = value;
}

QVector<SparamComplexPlain> FreqSparPlain::getSparameters() const
{
    return Sparameters;
}

void FreqSparPlain::setSparameters(const QVector<SparamComplexPlain> &value)
{
    Sparameters = value;
}

void FreqSparPlain::setOneParameter(const SparamComplexPlain &value)
{
    //Hmmm kako budemo setovali parametar on ce se automatski dodavati u objekat
    // moramo da malo uslozimo ponasanje ovoga
    // Ako nije popunjen do kraja onda ovo uradi ako jeste
    // prebrisi ga
    if (Sparameters.count() < 4) {
        Sparameters << value;
    }else {
        //kada mu dimenzija predje 4 neka obrise sadrzaj i neka krene ponovo da pise
        Sparameters.clear();
        Sparameters << value;
    }

}


void FreqSparPlain::operator+=(const FreqSparPlain &otherObj)
{
    //Implementiramo ovaj operator da bi mogli da ga koristimo u averaging
    //Verujem da su frekvencije odgovarajuce
    frequency = otherObj.getFrequency();
    s11 += otherObj.getS11();
    for (int var = 0; var < Sparameters.count(); ++var) {
        Sparameters[var] = Sparameters.at(var) + otherObj.getSparameters().at(var);
    }
    //I ovim bi trebalo da sabere i iskopira elemente

}

void FreqSparPlain::divideSparams(qreal delilac)
{
    // Deli sve Sparametar elemente sa deliocem
    for (int var = 0; var < Sparameters.count(); ++var) {
        Sparameters[var] = Sparameters.at(var) / delilac;
    }
}

