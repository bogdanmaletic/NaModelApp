#include "noisemodel.h"
#include <QDebug>
#include <QtMath>

using namespace std;

NoiseModel::NoiseModel(QObject *parent) : QObject(parent)
{
    //    generate100random();
    //    simplerGeneratorUniform();
    //    simpleNormalDistribution();

    //Ok radi nam za gausovu distribuciju,
    //    kao i za ostale distribucije
    //    for (int var = 0; var < 10; ++var) {
    //        qDebug() << "Dohvatili smo : " << getNextRandom();
    //    }

    //    //Radi lepo
    //    qDebug() << "Sada uniformnu raspodelu: ";
    //    //Za fazu koristimo uniformnu raspodelu: Kao rezultat daje -1 0 i 1
    //    for (int var = 0; var < 10; ++var) {
    //        qDebug() << getNextInt();
    //    }

    //    auto varijabla = getComplexRandom();

    //    qDebug() << "Aj sada full kompleksni";
    //    for (int var = 0; var < 10; ++var) {
    //        varijabla = getComplexRandom();
    //        qDebug() << varijabla.real() << ", i" << varijabla.imag();
    //    }

    //aj uzecemo da imamo filter od 30kHz, ovo cisto da bi smo
    // verifikovali rezultate... i da vidimo sta mozemo da radimo sa njima
    ifbwfreq = 30;
    //    powerOfNoise = boltzmanConstant*temperature*ifbwfreq * 1e3 * 1e+14 ;         // Ovo smo samo ubacili na kraju da vidimo kako se menja mozda cemo morati jos da ga doradjujemo
    recalculatePowerOfNoise();                                                     // Ovo je izmenjeno u odnosu na gornju linuju da bi mogao da uhvati promenu ifbwfreq
    qDebug() << "(NoiseModel)Ok snaga suma nam je " << powerOfNoise;
    qDebug() << "(NoiseModel)U dBm nam je " << 10*(qLn(powerOfNoise)/qLn(10));
    qDebug() << "(NoiseModel)Ovo preko metode " << calculateInDbm(powerOfNoise);

    //    srednja_vrednost = powerOfNoise;
    //    standard_deviation = standard_deviation/powerOfNoise;
    SparamComplexPlain testCompl = getComplexRandom() * powerOfNoise;
    qDebug() << "(NoiseModel) Ok generisani broj je " << testCompl.real()
             << testCompl.imag();

    // ne zaboraviti da ovo zove zapravo dva puta
    for (int var = 0; var < 10; ++var) {
        testCompl = getComplexRandomWithNoisePower();
        qDebug() << "(NoiseModel) Testiramo generaciju broja sa snagom suma " << testCompl.real()
                 << testCompl.imag();
    }


}


void NoiseModel::generate100random()
{
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(srednja_vrednost,standard_deviation);
    double broj;

    for (int var = 0; var < 100; ++var) {
        broj =  distribution(generator);
        qDebug() << "(NoiseModel)generisani broj " << broj;
    }
}

void NoiseModel::simplerGeneratorUniform()
{
    random_device rd;   // non-deterministic generator
    mt19937 gen(rd());  // to seed mersenne twister.
    uniform_int_distribution<> dist(1,6); // distribute results between 1 and 6 inclusive.

    //    cout << "Hmmm: " << dist(gen);

    qDebug() << "(NoiseModel)Ok imamo sledece brojeve...";

    for (int var = 0; var < 10; ++var) {
        qDebug() << dist(gen);
    }

    //    dist(gen());
    //    auto bla= dist(gen());
    //        qDebug() << "(NoiseModel)Ok ovo je iz microsofta";
    //        qDebug() << "(NoiseModel)Ok generisao je broj" << dist(gen());


}

void NoiseModel::simpleNormalDistribution()
{
    //    random_device rd;
    //    mt19937 gen(rd());

    //    normal_distribution<> dist(5,2);

    for (int var = 0; var < 10; ++var) {
        qDebug() << "(NoiseModel)Ok normalna distribucija" << dist(gen);
    }

}

//Generise nam slucajni broj
double NoiseModel::getNextRandom()
{
    return dist(gen);
}

//Generise -1 0 i 1 slucajno
int NoiseModel::getNextInt()
{
    return uniformDist(gen);
}

//Vraca slucajno generisani kompleksni broj
SparamComplexPlain NoiseModel::getComplexRandom()
{
    //Sluzi za konstrukciju kompleksnog broja
    SparamComplexPlain generatedComplex;
    int cosTheta = getNextInt();
    int sinTheta;
    //Hmmm sin i cos pomereni ako je jedan cosTheta 0 onda
    // je sinTheta -1
    // ako je cosTheta -1 tada je sinTheta -1;
    // ako je coTheta 1 onda je sin theta 0;
    // ovo preko switcha
    switch (cosTheta) {
    case 0:
        sinTheta = -1;
        break;
    case -1:
        sinTheta = -1;
        break;
    case 1:
        sinTheta = 0;
        break;
    default:
        //mada ovaj se zaista nece nikada dogoditi
        sinTheta = 1;
        break;
    }

    double realPart = getNextRandom()*cosTheta;
    double imagPart = getNextRandom()*sinTheta;
    generatedComplex.real(realPart);
    generatedComplex.imag(imagPart);

    return generatedComplex;
}

SparamComplexPlain NoiseModel::getComplexRandomWithNoisePower()
{
    SparamComplexPlain complexWithNoisePower = getComplexRandom();
    complexWithNoisePower.real(complexWithNoisePower.real()*powerOfNoise);
    complexWithNoisePower.imag(complexWithNoisePower.imag()*powerOfNoise);

    return complexWithNoisePower;
}


qreal NoiseModel::calculateInDbm(qreal parametarToCalculate)
{
    return  10*(qLn(parametarToCalculate)/qLn(10));
}

qreal NoiseModel::getIfbwfreq() const
{
    return ifbwfreq;
}

void NoiseModel::setIfbwfreq(const qreal &value)
{
    ifbwfreq = value;
    //Da bi doslo do promene kada preko mainwinwowa preko NaModela
    // iniciramo promenu suma
    recalculatePowerOfNoise();
}

void NoiseModel::recalculatePowerOfNoise()
{
    powerOfNoise = boltzmanConstant*temperature*ifbwfreq * 1e3 * 1e+14 ;
}




