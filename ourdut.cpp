#include "ourdut.h"
#include <QDebug>

OurDut::OurDut(QObject *parent) : QObject(parent),
    numberOfPointsDut(0)
{
    //prilikom inicijalizacije objekta se pocinje sa ucitavanjem
    // podataka, istovremeno da bi smo zadrzali konzistenciju i jednostavnost
    // moracemo da da postavimo da vektori paramera interpoliranih i neinterpoliranih
    // sadrze iste podatke.

    vectorSparama = paramRead.getExtractedData();

    //Ovo smo cisto dodali da bi smo mogli da radimo
    // sa setom podataka sa kojim smo i mogli da radimo
    //    freqStart = 1000;       //radi sa MHz
    //    freqStop = 1400;
    //    numberOfPointsDut = vectorSparama.count();
    //    deltaF = (freqStop-freqStart)/(numberOfPointsDut - 1);

    //    interpolateMeasure(2000);

    //    int tempDebugVar = 0;
    //    tempDebugVar++;


    //Hmmm kada smo izvukli podatke red je da znamo
    // zbog kasnije interpolacije zato sto iako prosledjujemo
    // podatke vezane za interpolaciju, mi ipak u njoj samoj
    // kalkulisemo i sa podacima za ceo opseg OurDut objekta
    // pogotovo sto interpolaciju radi jednostavnosti radimo
    // na celokupnom opsegu
    // Ovo smo morali da zamenimo da bi normalizovali na mhz
    //    freqStart = vectorSparama.at(0).getFrequency() *1000;
    //    numberOfPointsDut = vectorSparama.count();
    //    freqStop = vectorSparama.at(numberOfPointsDut - 1).getFrequency() * 1000;
    //    deltaF = (freqStop - freqStart)/(numberOfPointsDut - 1);
    freqStart = vectorSparama.at(0).getFrequency();
    numberOfPointsDut = vectorSparama.count();
    freqStop = vectorSparama.at(numberOfPointsDut - 1).getFrequency();
    deltaF = (freqStop - freqStart)/(numberOfPointsDut - 1);



}

//Sluzi da nam generise lazne podatke(za testiranje
// i razvijanje ovog objekta
//void OurDut::generateFakeData()
//{
//    for (int var = 0; var < 10; ++var) {
//        FreqSparPlain tempFreq(10*var+1,var*3/10,var*7/10);
//        vectorSparama << tempFreq;
//        ++numberOfPointsDut;
//    }
//}

int OurDut::getNumberOfPointsDut() const
{
    return numberOfPointsDut;
}

void OurDut::setNumberOfPointsDut(int value)
{
    //Hmmm treba prvo proracunati tacke pa eventualno
    //    setovati
    numberOfPointsDutInterpolate = value;
    //ovde cemo zvati interpolaciju
    interpolate(value);             //odmah racunamo interpolaciju

    //Verovatno cemo trebati da kada se ovo desi da emitujemo
    // neki signal(ovo verovatno, videcemo kako cemo da implenentiramo)
}

// jednostavna metoda za raport u debugu
QString OurDut::raportInterpolatedData()
{
    QString raportString;
    foreach (FreqSparPlain tacka, vectorSparamaInterpolated) {
        raportString += QString("Na frekvenciji %1 imamo parametar: %2,i%3 \n")
                .arg(tacka.getFrequency())
                .arg(tacka.getS11().real())
                .arg(tacka.getS11().imag());
    }
    return raportString;
}

QVector<FreqSparPlain> OurDut::getVectorSparama() const
{
    return vectorSparama;
}

//Hmmm nesto ne valja, proveriti sta je u pitanje
void OurDut::interpolate(int newNumOfPoints)
{
    //Ovo je metoda za interpolaciju
    // da bi bilo jednostavnije interpoliracemo za sve ucestanosti
    // dut ce biti definisan za celokupnu ucestanost po linearnoj raspodeli
    // dakle od 300 kHz do 3GHz na 1601 tacki

    freqStart = vectorSparama.at(0).getFrequency();
    freqStop = vectorSparama.at(numberOfPointsDut -1).getFrequency();

    deltaF = (freqStop - freqStart)/(numberOfPointsDut - 1);
    deltaFInterpolate = (freqStop - freqStart)/(newNumOfPoints -1);

    //hmmm aj da nadjemo izmedju koje dve treba da se nadju dve tacke
    // ovo gledamo samo za slucaj da smo zadali manji broj tacakaa

    FreqSparPlain lowerPoint;        //Ovo je donja tacka
    FreqSparPlain upperPoint;        //gornja tacka

    FreqSparPlain interpolatedPoint; //kada zavrsimo za proracunom smesticemo ovde novu tacku

    //aj da je nadjemo
    qreal distanceFromLower;
    qreal distanceFromUpper;
    int k = 0;                      //Ovo nam je tacka brojaca za trazenje novih frekvencija
    int s = 0;                      //Ovo nam je tacka brojaca za trazenje po starim frekvencijama
    qreal currentFrequency = freqStart + k*deltaFInterpolate;
    qreal seekFreq = freqStart + s*deltaF;
    //Nastaviti posle crtanja

    //
    //Postupak je dat u svesci, mada ovde cemo ga na brzinu opisati
    // Da bi smo pronasli tacku u kojoj vrsimo interpolaciju
    // potrebno je pronaci izmedju koje dve tacke se nalazi nova
    // frekvencija. Ako znamo frekvenciju koju trazimo (freq = fstart * k*deltafInterpolate)
    // treba da primetimo da se ona u odnosu na odgovarajuce tacke nalazi na najmanjoj deltaf razdaljini.
    // Ukoliko se gornja i donja tacka stare raspodele nalaze na frekv. rastojanju manjeg od tog,
    // nasli smo trazene tacke i mozemo da nadjemo interpoliranu vrednost izmedju njih.


    //Nadjene vrednosti cemo staviti u
    FreqSparPlain foundedUpperPoint;
    FreqSparPlain foundedLowerPoint;

    while (k < newNumOfPoints) {
        while (s <= numberOfPointsDut) {

            //Moramo da proverimo da li se ipak neke frekvencije poklapaju
            if (currentFrequency == seekFreq) {
                vectorSparamaInterpolated << vectorSparama.at(s);
                ++s;
            }

            // razdaljine od frekvencija moraju da budu apsolutne, posto ako nisu
            // razdaljina kada predje prvu tacku ce otici u minus i uvek ce je dodavati
            // na vektor interpolisanih tacaka
            // hmm kako smo ga ovako suntavo uradili moramo da dodamo deltaf
            // da bi smo nasli poziciju gornje tacke.
            distanceFromUpper = qAbs(currentFrequency - seekFreq + deltaF);
            distanceFromLower = qAbs(currentFrequency - seekFreq);

            //hmmm pokazuje se da u principu moramo da nadjemo samo gornju tacku
            // dakle, dovoljno je da pronadjemo samo jednu tacku
            // Kada je pronadjemo znaci nasli smo ih,
            // inace kako imamo maksimum tacaka prvu tacku cemo naci
            // sigurno na tackama dva i tri pa necemo imati problem sa s-1 i s-2

            //Napomena: za sada u FreqSparPlain imamo jedan parametar,
            // mada mislim da nece biti problem da posle promenimo da posle racuna
            // za sva cetiti parametra.
            if((distanceFromUpper < deltaF) && (distanceFromLower < deltaF)){
                FreqSparPlain foundedUpperPoint = vectorSparama.at(s-1);    //at() pocinje od nule indekse
                FreqSparPlain foundedLowerPoint = vectorSparama.at(s-2);

                FreqSparPlain calculatedPoint;                              //U ovoj cemo smestiti rezultate

                //Sada pristupamo interpolaciji, samo je pitanje kako da izaberemo
                //Koristicemo gotov izraz(za linearnu interpolaciju)
                // mada ne treba zaboraviti da radimo sa kompleksnim brojevima

                // #Odavde koristimo za sledece tacke
                qreal y1real = foundedUpperPoint.getS11().real();
                qreal y0real = foundedLowerPoint.getS11().real();
                qreal y1imag = foundedUpperPoint.getS11().imag();
                qreal y0imag = foundedLowerPoint.getS11().imag();

                //tacka x0 ce predstavljati frekvencije pa ce biti i za realni i za imaginarni
                qreal x1 = foundedUpperPoint.getFrequency();
                qreal x0 = foundedLowerPoint.getFrequency();

                // x nam je u stvari razlika frekvencija one koje se nalazi nova tacka
                // i frekvencija donje tacke
                qreal x = currentFrequency - foundedLowerPoint.getFrequency();

                //sada racunamo vrednosti iz koje cemo da konstruisemo tacku
                qreal yreal;
                qreal yimag;
                yreal = (y0real*(x1 - x) + y1real * (x - x0))/(x1 - x0);
                yimag = (y0imag *(x1 - x) + y1imag * (x - x0))/(x1 - x0);

                //Sada mozemo da konstruisemo trazenu tacku
                calculatedPoint.setFrequency(currentFrequency);
                SparamComplexPlain complPoint(yreal,yimag);
                calculatedPoint.setS11(complPoint);

                //sada kada smo konstruisali tacku mozemo da je stavimo na vektor
                // koji sadrzi interpolirane vrednosti
                vectorSparamaInterpolated << calculatedPoint;
            }

            //Ako nije veca povecaj za 1(trazi dalje)
            // ne zaboraviti da se podesi i frekvencija
            // (inace pomera s ali ne i frekvenciju)
            ++s;
            seekFreq = freqStart + s* deltaF;
        }

        //Idemo na sledecu tacku
        // naravno moramo i ovu da podesimo frekvenciju
        ++k;
        currentFrequency = freqStart + k*deltaFInterpolate;

        //Da bi ponovo trcao po starim frekvencijama moramo da postavimo s na nulu
        s = 0;
    }
    qDebug() << "Ok nasao je " << vectorSparamaInterpolated.count() << "tacaka";
    qDebug() << "Tacke su sledece: " << raportInterpolatedData();
}

qreal OurDut::getDeltaFMeasure() const
{
    return deltaFMeasure;
}

int OurDut::getNumberOfPointsDutMeasure() const
{
    return numberOfPointsDutMeasure;
}

qreal OurDut::getFreqStopMeasure() const
{
    return freqStopMeasure;
}

qreal OurDut::getFreqStartMeasure() const
{
    return freqStartMeasure;
}

void OurDut::setMeasureParameters(qreal fstmstartmeasure, qreal fsmstopmeasure, int nopmeasure)
{
    freqStartMeasure = fstmstartmeasure;
    freqStopMeasure = fsmstopmeasure;
    numberOfPointsDutMeasure = nopmeasure;
    deltaFMeasure = (freqStopMeasure - freqStartMeasure)/(numberOfPointsDutMeasure - 1);

    //Automatski zapocinjemo interpolaciju na celom opsegu
    // ukoliko se razlikuje zadati broj tacaka od trenutnog
    // uradi interpolaciju.
    //    if (nopmeasure != numberOfPointsDutMeasure) {
    //        interpolateMeasure(numberOfPointsDutMeasure);
    //    }

    //Hmmm da li je ovo promenljiv broj
    // posto interpolateMeasure() zna da promeni numberOfPointsDutMeasure
    interpolateMeasure(numberOfPointsDutMeasure);
    //    interpolateMeasure(nopmeasure);

    //kada se ovo zavrsi mozemo da pitamo OurDut za
    // objekte na odredjenim frekvencijama
}

void OurDut::interpolateMeasure(int changedNumOfPoints)
{
    //Ako je vec jedanput radjena interpolacija izbrisati te tacke
    if (vectorSparamaInterpolated.count() > 0) {
        vectorSparamaInterpolated.clear();
    }

    //Napomena iako smo vec izracunali deltaFmeasure
    //    deltaFMeasure = (freqStop - freqStart)/(changedNumOfPoints-1);
    deltaFMeasure = (freqStopMeasure - freqStartMeasure)/(changedNumOfPoints - 1);
    //za razliku od prethodne sada cemo po frekvenciji imati po
    // cetiri parametra sto znaci da kada nadje frekvenciju morace za sva
    // cetiri da racuna...
    //Hmmm naravno ovo nam pravi problem

    //Prve dve linije su nam je bio pocetan izbor
    // medjutim kada smo zamenili njih sa sledecimlinijama
    // koje su aktivne poceli smo da dobijamo pravi broj
    // tacaka za interpolaciju
    //    qreal sweepFreq = freqStart;
    //    qreal currentPointFreq = freqStart;
    qreal sweepFreq = freqStartMeasure;
    qreal currentPointFreq = freqStartMeasure;
    /*qreal nextPointFreq = freqStart + deltaF;*/           //Pozicija sledece tacke(ova ne valja)
    qreal nextPointFreq = freqStart + deltaF;



    int counterCurrentPoint = 0;                        //pocinje od nule
    int counterNextPoint = 1;                           // sledeca je logicno 1

    //Naravno Potrebne su nam i vrednosti tacaka
    FreqSparPlain currentPoint = vectorSparama.at(counterCurrentPoint);
    FreqSparPlain nextPoint = vectorSparama.at(counterNextPoint);

    //Posto znamo da je sledeca tacka na freqStart + deltaF
    // kazemo ovako

    //Treba nam i nesto da smestamo izracunati parametar
    FreqSparPlain calculatedPoint;

    //Ovu smo samo postavili da vidimo
    // sta nije u redu na kraju while petlje
    //    int debugVar = 1;

    //neka nam sweep ide do poslednje frekcencije
    while (sweepFreq <= freqStopMeasure) {
        // do sledece tacke
        //        if(debugVar == 400){
        //            debugVar++;
        //        }
        while (sweepFreq <= nextPointFreq) {
            //znamo tacke koje imamo sad je vreme da nadjemo nedostajuce
            // do ove frekvencije (za svaki parametar racunamo
            for (int var = 0; var <= currentPoint.getSparameters().count() -1; ++var) {
                //Sada iz bazne interpolate metode za svaku tacku
                qreal y1real = nextPoint.getSparameters().at(var).real();
                qreal y0real = currentPoint.getSparameters().at(var).real();
                qreal y1imag = nextPoint.getSparameters().at(var).imag();
                qreal y0imag = currentPoint.getSparameters().at(var).imag();

                //                qreal x1 = nextPoint.getFrequency() * 1000;         //radimo sa mhz
                //                qreal x0 = currentPoint.getFrequency() * 1000;

                //                qreal x = sweepFreq - currentPoint.getFrequency() * 1000;

                qreal x1 = nextPoint.getFrequency();         //radimo sa mhz
                qreal x0 = currentPoint.getFrequency();

                //U sledecoj liniji se krila greska
                // ako izracunamo ovako razliku dobijacemo (x - x0) sto
                // ce uvek biti negativna vrednost
                //                qreal x = sweepFreq - currentPoint.getFrequency();
                //moracemo ovako
                qreal x = sweepFreq;

                //Sada racunamo
                qreal yreal;
                qreal yimag;
                yreal = (y0real*(x1 - x) + y1real * (x - x0))/(x1 - x0);
                yimag = (y0imag *(x1 - x) + y1imag * (x - x0))/(x1 - x0);


                SparamComplexPlain complPoint(yreal,yimag);

                calculatedPoint.setFrequency(sweepFreq);
                calculatedPoint.setOneParameter(complPoint);        // dodaje prosledjeni parametar

                //kada zavrsi sa ovim izracunao je jedan parametar,
                // kada zavrsi sa svim ovim izracunao je sve parametri na jednoj
                // frekvenciji
            }
            //Kada izadje iz for petlje ima izracunata sva 4 parametra na odredjenoj
            // frekvenciji, pa ga smestamo na interpolirani vektor
            vectorSparamaInterpolated << calculatedPoint;

            //Sada je red da povecamo sweepFrekvenciju
            sweepFreq = sweepFreq + deltaFMeasure;
        }
        //kada zavrsi sa ovim parom frekvencija red je da se predje na sledeci
        counterCurrentPoint++;
        counterNextPoint++;
        //        if (counterCurrentPoint == 399) {
        //            debugVar++;
        //        }
        //moramo dodati ovo da posto na kraju puca
        // jel pokusava da dohvati parametar na nepostojecoj
        // poziciji, za sada ne mogu da smislim bolje resenje
        if (counterNextPoint < vectorSparama.count()) {
            // Ovde je bila greska mi kao da smo pravili lokalnu varijablu
            // u okviru if-a sa deklaracijom bez obavestenja sa overshadowom
            // gore uvedenih... One su zapravo ostajale 10 i 20, i zato smo dobijali
            // onakav rezultat. Dakle nikako
            //            FreqSparPlain currentPoint = vectorSparama.at(counterCurrentPoint);
            //            FreqSparPlain nextPoint = vectorSparama.at(counterNextPoint);

            currentPoint = vectorSparama.at(counterCurrentPoint);
            nextPoint = vectorSparama.at(counterNextPoint);
        }else {
            //Aj da dodamo ovde za parametre merenja
            //Ovo je nepotrebno i netacno
            //            freqStartMeasure = freqStart;
            //            freqStopMeasure = calculatedPoint.getFrequency();
            //            numberOfPointsDutMeasure = vectorSparamaInterpolated.count();
        }

        //kada dodje do kraja

        //        sweepFreq = sweepFreq + deltaFMeasure;
        nextPointFreq = nextPointFreq + deltaF;
        //        debugVar++;
    }

    //hmmm ispostavlja se da ne radi vectorSparamaInterpolated kako treba
    // posto ukljucuje i tacke pocev od prve

    //TODO ovo su samo provere da li radi odsecanje kako treba
    // Dakle problem je bio sto ovaj vektor sadrzi i stvari koje su
    // pocinju na primer od 10MHz iako je merenje definisano da krece od 1000 Mhz
    // pa ih na ovaj nacin odsecamo
    //auto testNumPointVectorInterpolatedBefore= vectorSparamaInterpolated.count();



    foreach (FreqSparPlain elemTest, vectorSparamaInterpolated) {
        if (elemTest.getFrequency() < freqStartMeasure) {
            //Ako je manji onda izbaci ga
            vectorSparamaInterpolated.pop_front();
        }
        if (elemTest.getFrequency() > freqStopMeasure) {
            // ako je veca frekvencija izbaci ga
            vectorSparamaInterpolated.pop_back();
        }

    }

    //TODO drugi deo provere nakon odsecanja
    //    auto testNumPointVectorInterpolatedAfter = vectorSparamaInterpolated.count();

    //TODO ovde mozda hoce da pukne...
    //    numberOfPointsDutMeasure = vectorSparamaInterpolated.count();
    if (numberOfPointsDutMeasure > vectorSparamaInterpolated.count()) {
        //dodajemo zadnji element ne interpoliranog posto se raslikuju
        // najvise u jednom elementu
        // mada moze se desiti da nije poslednji vec pretposlednji
        //        vectorSparamaInterpolated.append(vectorSparama.last());

        //dakle uzecemo tacku na counterNextPoint
        vectorSparamaInterpolated.append(vectorSparama.at(counterNextPoint));

    }


}

QVector<FreqSparPlain> OurDut::getVectorSparamaInterpolated() const
{
    return vectorSparamaInterpolated;
}

QVector<FreqSparPlain> OurDut::getVectorSparamaPointer() const
{
    //Za dohvatanje reference za fiktivni dut
    return vectorSparama;
}

FreqSparPlain OurDut::getDataFromFreq(qreal frequency)
{
    //Ovaj nam je zahtevao namodel po opisanom nacinu rada

    //Potreban nam je FreqSparPlain objekat kada ne nadjemo
    // sa ovim mozemo da vratimo nesto, mada to je gotovo
    // nemoguca situacija
    FreqSparPlain tempFreqObj;
    //    tempFreqObj.setOneParameter();

    foreach (FreqSparPlain freqObj, vectorSparamaInterpolated) {
        if (freqObj.getFrequency() == frequency)
            return freqObj;
        else
            return tempFreqObj;
    }
}

//Ovo je varijanta kada treba da izvadimo podatak iz interpoliranih
// podataka (ovde je inace bio problem )
//TODO verovatno cemo morati da ga menjamo posto ce za velike skupove
// non stop prolaziti od pocetka.Mada za sada ne pravi problem...
FreqSparPlain OurDut::getMeasuredDataFromFreq(qreal frequency)
{
    //Ovaj nam sluzi cisto da suzbijemo warning
    FreqSparPlain tempFreqObj;
    //Aj da mu dodamo i nedostajuce sparametre
    tempFreqObj.setOneParameter({0,0});
    tempFreqObj.setOneParameter({0,0});
    tempFreqObj.setOneParameter({0,0});
    tempFreqObj.setOneParameter({0,0});
    //    tempFreqObj.setFrequency(frequency);
    bool isItFind = false;


    foreach (FreqSparPlain freqObj, vectorSparamaInterpolated) {
        //    for (FreqSparPlain freqObj : vectorSparamaInterpolated) {

        // razlikaFrekv moramo da ubacimo zbog operacija sa sabiranjem double
        // tipova, posto cpp ne mozemo nikada verovati sa ovim
        // razlika od 0.1 Mhz je cini mi se dovoljna da pronadje sve vrednosti
        // nama od interesa
        // desilo se da nije upala nikakva frekvencija, zato cemo da dodamo deltaFMeasure/2
        // interval.. valjda ce biti dovoljan...
        double razlikaFrekv = qAbs(freqObj.getFrequency() - frequency);
        if (razlikaFrekv < (deltaFMeasure/2)) {
            //Setuje da ga je nasao
            isItFind = true;
            return freqObj;
        }
    }

    //Ako ga nije nasao
    if (isItFind == false) {
        tempFreqObj.setFrequency(frequency);
        return tempFreqObj;
    }

}
//Na prosledenu frekvcenciju vraca spar objekat...hmmm
// sta da se radi ako nema zadate frekvencije, mozda da vrati bazican 0 + i0 spar
//FreqSpar OurDut::getSparObject(qreal frequency)
//{
//    foreach (FreqSpar var, vectorSparama) {
//        if (var.) {

//        }
//    }
//}
