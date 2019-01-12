#include "namodel.h"

//I ovde treba da vidimo koliko nam treba vremena
// za obavljanje nekih operacija
#include <QElapsedTimer>
#include <QTime>
#include <QtMath>

NAmodel::NAmodel(QObject *parent) : QObject(parent)
{
    qDebug() << "Kreiran je ali gde ?";

    // prvo da prikljucimo dut na NaModel
    //za parametre antene
    //    dut.setMeasureParameters(1000,19990,401);
    //    dut.setMeasureParameters(1000,19990,10);
    //    dut.setMeasureParameters(1000,19990,801);

    //Ovo je za ispravljenu verziju
    //    dut.setMeasureParameters(100,20000,401);
    //    dut.setMeasureParameters(0,2000,801);
    //    dut.setMeasureParameters(1000,1400,401);
    //    dut.setMeasureParameters(980,1500,401);
    //za tuki parametre
    //    dut.setMeasureParameters(1000,1390,401);
    // Odavde trazimo povratne informacije o parametrima

    //Aj da vidimo kako se ponasa sa ovim merenjima
    //    dut.setMeasureParameters(10,3000,401);
    //    dut.setMeasureParameters(10,3000,501);
    //    dut.setMeasureParameters(10,3000,801);
    //Aj probamo sa 1601 tackom
    dut.setMeasureParameters(10,3000,1601);
    getMeasureParameters();

    //Da dohvatimo i kalibracione podatke
    calDut = new OurDut();
    calDut->setMeasureParameters(10,3000,1601);

    //nakon toga mozemo pitati dut za podatke na odredjenoj frekvenciji

    //    sweepFreq = 1000.0;
    sweepFreq = fStartMeasure;
    int sweepPoint = 0;

    //Dohvatamo redom podatke
    //    while (sweepFreq <= fStopMeasure) {
    //        getDataFromOneFreq(sweepFreq);
    //        sweepFreq += deltaFMeasure;
    //    }

    //Hmmm timer ne mozemo ovako da zovemo
    //    timer = new QTimer();
    //    connect(timer,SIGNAL(timeout()),this,SLOT(testTimer()));

    //Naravno da ne zaboravimo da ga startujemo
    //    timer->start(1000);

    //Aj da testiramo... povezivanje
    //    connect(this,SIGNAL(pointIsCalulated(Freq)) ,this,SLOT(processPoint()));
    //    connect(this,&NAmodel::ourSimpleSignal,this,&NAmodel::ourSimpleSlot);
    //    connect(this,&NAmodel::drugiSignal,this,&NAmodel::drugiSlot);

    //Ova radi... za sada cemo da izbegnemo ove poruke
    //    connect(this,&NAmodel::pointIsCalulated,this,&NAmodel::processPoint);

    int debugVar = 0;
    debugVar++;

    //da bi smo izbegli probleme sa selektovanim parametrima
    // i odmah da prikaze log mag
    setSelektovaniParametar(1);
    setLogMagGuard(true);

    //Za testiranje usrednjavanja
    //    setAverageGuard(true);
    //    setAverageCycles(5);
    //    setAverageCyclesNumber(5);
}

//metoda za debug i testiranje
void NAmodel::initSparams()
{

    complexValue.real(3);
    complexValue.imag(5.0);

    sParams << complexValue;

    complexValue.real(1.2);
    complexValue.imag(5.3);

    sParams << complexValue;

    complexValue.real(5.1);
    complexValue.imag(7.2);
    sParams << complexValue;

    complexValue.real(2.3);
    complexValue.imag(9.6);
    sParams << complexValue;

}

void NAmodel::calculateOnOneFreq(qreal freq)
{
    //prvo da podesi frekvenciju na kojoj zeli da dohvati podatke
    // hmmm nekompletan nam je model i za ostale parametre
    sweepFreq = freq;
    //    genP = 1;
    //Dohvata podatke iz Duta sa izabrane frekvencije
    FreqSparPlain dataFromDut;
    //    FreqSparPlain dataWithNoise;
    dataFromDut = dut.getDataFromFreq(sweepFreq);

    //da izgenerisemo i sum
    // pored njega menjamo ifbwfreq
    noiseModel.setIfbwfreq(30);
    SparamComplexPlain noise = noiseModel.getComplexRandomWithNoisePower();

    // pod pretpostavkom da smo izgenerisali sum
    //    dataWithNoise.setFrequency(dataFromDut.getFrequency());
    SparamComplexPlain incident = genP;
    SparamComplexPlain reflected = dataFromDut.getS11() * incident;

    //sada da izracunamo kompleksni koeficijent sa sumom
    SparamComplexPlain measuredS11 = (reflected + noise)/(incident + noise);

    qDebug() << "(NAmodel) iskalkulisani parametar je " << measuredS11.real()
             << ",i" << measuredS11.imag();
    //    measuredData <<
    int debugVar = 0;
    debugVar++;

}

void NAmodel::calculateOnAllFreq()
{
    int k = 1;
    sweepFreq = fStart + k*deltaF;
    while (sweepFreq != fStop) {
        sweepFreq = fStart + k*deltaF;
        calculateOnOneFreq(sweepFreq);
        k++;
    }
}

void NAmodel::getMeasureParameters()
{
    //Pomocna metoda koja nam sluzi da
    // dohvatimo podatke sa interpoliranih podataka
    fStartMeasure = dut.getFreqStartMeasure();
    fStopMeasure = dut.getFreqStopMeasure();
    deltaFMeasure = dut.getDeltaFMeasure();
    numOfPointsMeasure = dut.getNumberOfPointsDutMeasure();
}

//nacrtati
//zapravo tempFreq odakle ovaj hoce da izvadi podatke
// je obican FreqSparPlain bez podataka iz duta

void NAmodel::getDataFromOneFreq(qreal freqOfData)
{
    //Ovo smo cisto da vidimo koliko trosimo vremena za ovo
    //TODO ukloniti treba nam 0 milisekundi
    //    QElapsedTimer getDataElapsedTimer;
    //    getDataElapsedTimer.start();

    //TODO sledeca linija nemam pojma zasto je ovako
    // ali za sada radi bez problema tu
    //    sweepFreq = freqOfData;
    FreqSparPlain tempFreqObj;

    //da setujemo temp Objekat
    if (calGuard == true) {
        tempFreqObj = dut.getMeasuredDataFromFreq(freqOfData);
    }else {
        tempFreqObj = calDut->getMeasuredDataFromFreq(freqOfData);
    }
    //    tempFreqObj = dut.getMeasuredDataFromFreq(freqOfData);


    //Kada generisemo sum uracunavamo i koliko je ifbwfreq
    noiseModel.setIfbwfreq(getIfbwFreqModel());
    SparamComplexPlain noise =  noiseModel.getComplexRandomWithNoisePower();

    SparamComplexPlain incident = genP;
    QVector<SparamComplexPlain> reflected = tempFreqObj.getSparameters();

    //    QVector<SparamComplexPlain> dataToCalculate;
    FreqSparPlain calcutatedData;


    foreach (SparamComplexPlain oneParameter, reflected) {
        //ne zaboravi da jedan parametar pomnozis sa incidentnim talasom
        SparamComplexPlain oneParameterWithNoise;
        oneParameter *= incident;
        oneParameterWithNoise = (oneParameter + noise)/(incident + noise);
        //        dataToCalculate << oneParameterWithNoise;
        calcutatedData.setOneParameter(oneParameterWithNoise);
    }
    //Treba smisliti gde se ovo moze staviti

    //naravno nama kao rezultat treba proracunati objekat
    // ali na pravoj frekvenciji(frekvenciji koju smo trazili)
    // da nema sledece linije vracao bi na frekvenciji 0.1
    calcutatedData.setFrequency(freqOfData);

    //Aj da testiramo emitovanje
    //    pointIsCalulated(calcutatedData);
    //    emit ourSimpleSignal();
    //    emit drugiSignal(testSignalPoint);

    //Ovo radi sa obicnom drawPoint slotom u view-u
    // vise nije potrebno
    //    emit pointIsCalulated(calcutatedData);
    //    testSignalPoint++;

    //kada se zavrsi kalkulacija smestamo podatke u measuredData obj
    measuredData << calcutatedData;

    //da bi smo na kraju procesa znali koliko ih ima

    //Ovde radimo average
    //Proces je podeljen u tri faze
    // prva faza gde puni average bafer
    // druga faza gde radi cikluse dok ne dodje do poslednjeg
    // treca faza gde odradjuje poslednji prolaz
    if (getAverageGuard() == true) {

        //Hmm da li je sa ovim problem
        // kada stigne na kraju i stavi poslednju ali samo ako je averagePointPos == 0
        // frekvenciju na prvu poziciju
        //        if ((freqOfData > fStopMeasure) && (averagePointPos == 0)) {
        //            return;
        //        }

        if (averageCycles != 0) {
            //Ako je bafer prazan punimo ga
            //hmmm ovo cemo da ogranicimo samo za prvi prolaz
            //Dakle ovo je samo prvi prolaz
            if (averageCycles == averageCyclesNumber){
                //kako count daje ukupan broj tacaka puni ga do numOfPointsMeasure
                if (averageVector.count() <= (numOfPointsMeasure -1)) {
                    averageVector << calcutatedData;
                    averagePointPos++;
                    if(averagePointPos >= 1600){
                        int debugVar = 0;
                    }
                }

                if(averageVector.count() == (numOfPointsMeasure )) {
                    //da dodamo i ovaj sa kraja
                    //                    averageVector << calcutatedData;

                    //Ovaj deo se pokrece samo kada se ovo napuni
                    // i to samo jedanput i vezan je za prvobitno punjenje
                    // average bafera
                    averagePointPos = 0;
                    averageCycles--;

                    //kada ga napuni trazi jos
                    emit sigRestartCalculation();

                    //Aj da probamo sa ovim(kada zavrsi da zaustavi kalkulaciju)
                    //                    setCalculationGuard(false);

                    //Dalje ne mora da ide izlazimo
                    return;
                }

            }

            //Ako nije prazan i ima
            // Ovo je prolaz izmedju prvog i poslednjeg
            if ((averageCycles != 0) && (averageCycles < averageCyclesNumber)) {
                //zbog ovoga smo morali da prosirimo FreqSparPlain
                // Hmm da li je ovako dobro pristupati
                if (averagePointPos < (numOfPointsMeasure -1)) {

                    //Kako ovde pravi problem a treba da okida
                    //                    if (freqOfData <= fStopMeasure) {
                    //                        averageVector[averagePointPos] += calcutatedData;
                    //                        averagePointPos++;
                    //                    }
                    //Ovde pravi problem sa dodatnim clanom
                    //                    if ((freqOfData > fStopMeasure) && (averagePointPos ==0)) {
                    //                        return;
                    //                    }

                    averageVector[averagePointPos] += calcutatedData;

                    averagePointPos++;

                    //Da ne bi propao odmah dole vec na sledecu tacku
                    return;
                }if(averagePointPos == (numOfPointsMeasure - 1)) {
                    //I ovaj poslednji da dodamo (vise ne treba)
                    averageVector[averagePointPos] += calcutatedData;
                    //dakle kada ponovo napuni bafer resetuje brojac tacaka
                    averagePointPos = 0;
                    // istovremeno ako je povovo napunio bafer znaci da je
                    // skinuo jedan ciklus;
                    averageCycles--;

                    //trazi jos jedan ciklus
                    emit sigRestartCalculation();

                    //I kada zavrsi sa ovim da zaustavi
                    //                    setCalculationGuard(false);

                    //Da ne bi isao dole jer ovde nije potrebno
                    // da se dalje ide
                    return;
                }

            } //end of prolaz izmedju prvog i poslednjeg
        } // end of (averageCycles!=0)
    }

    //Za vreme dok se puni bafer * averageCycles nema emitovanja tacaka
    //Ovo je poslednji prolaz za average
    if ((averageCycles == 0) && (getAverageGuard() == true) ) {

        //Dakle kada istrosimo sve cikluse tada mozemo da usrednjimo
        // izracunatu tacku, uvek imamo 4 sparametra u tacki(mada ostavicemo
        // ovako zbog opstosti

        //Prva varijanta zadnjeg ciklusa
        // Koja slabo radi
        //            for (int var = 0; var < calcutatedData.getSparameters().count()-1; ++var) {
        //                //TODO zavrsiti ovo
        //                if (averagePointPos < numOfPointsMeasure) {
        //                    calcutatedData += averageVector.at(averagePointPos);
        //                    averagePointPos++;
        //                    calcutatedData.divideSparams(averageCyclesNumber);

        //                }else {
        //                    //Kako dobijamo tacke u jednom trenutku moramo
        //                    // resetovati brojac tacaka u baferu
        //                    averagePointPos = 0;
        //                    //Hmmm nikako da vidimo gde treba da resetujemo brojac ciklusa
        // //                    averageCycles = averageCyclesNumber;
        //                }
        //            }

        //Druga varijanta zadnjeg ciklusa

        //Kako radimo kako stigne tacka po tacka odjedanput
        //radimo operacije nad calculated data
        if(averagePointPos == (numOfPointsMeasure -1)) {
            calcutatedData += averageVector.at(averagePointPos);
            calcutatedData.divideSparams(averageCyclesNumber);
            //kada radi zadnju tacku skroz dole
            // stavili smo kada obradi zadnju da na ne bi
            // dolazilo da propadne odmah i anulira vektor
            // vec mora da averagePointPos == numOfPointsMeasure
            // ali samo na njoj
            averagePointPos++;
            //            averagePointPos = 0;
        }

        if (averagePointPos < (numOfPointsMeasure - 1)) {
            calcutatedData += averageVector.at(averagePointPos);
            calcutatedData.divideSparams(averageCyclesNumber);


            averagePointPos++;
            if (averagePointPos == (numOfPointsMeasure - 1)) {
                int debugVar = 0;
                debugVar++;
            }
        }



        //        averagePointPos++;
        //Ovo bi trebalo da uradi kada dodje do kraja ciklusa
        // i tacaka i ciklusa za average da bi kod sledece tacke ponovo
        // poceo da radi
        //Ovo treba da radi na kraju
        // Kada zavrsi sa svim tackama
        //        if ((averagePointPos == (numOfPointsMeasure-1)) &&(averageCycles == 0)) {
        //            averagePointPos = 0;
        //            //Da ne bi smo radili prvu fazu ponovo
        //            // TODO
        //            averageCycles = averageCyclesNumber;
        //            //cistimo vektor da bi ponovo poceo od pocetka
        //            averageVector.clear();

        //            emit sigRestartCalculation();
        //        }

        //Podugnut na liniju 347
        //        if(averagePointPos == (numOfPointsMeasure)) {
        //            calcutatedData += averageVector.at(averagePointPos);
        //            calcutatedData.divideSparams(averageCyclesNumber);
        ////            averagePointPos = 0;
        //        }

        //        averagePointPos++;



    }
    //Ovo bi trebalo da zavrsi usrednjavanje
    //nakon toga treba da se pobrinemo da tek u poslednjem prolazu
    // emitujemo tacke ako je ukljucen average, ako nije ukljucen
    // averageCycles je sigurno nula

    //I da obradimo na kraju


    if (averageCycles == 0) {


        //TODO ukloniti treba nam 0 milisekundi
        //    qDebug() << "(NAmodel::getDataFromOneFreq(); treba nam " << getDataElapsedTimer.elapsed() << " milisekundi";

        //Aj da dodamo selektovanje parametara
        //TODO posle cemo da ga izmenimo
        // da bi smo ga napravili sto jednostavnijim
        //    setSelektovaniParametar(1);
        QPointF ourMagPoint(0,0);
        qreal magPoint = 0;
        qreal realPart = 0;
        qreal imagPart = 0;


        //Hmmm smatracemo da ako nismo selektovali parametar uvek selektovati s11
        if ((getSelektovaniParametar() < 1) || (getSelektovaniParametar() > 4)) {
            //Ovo nam omogucava da ne dolazi do nelogicnih zakucavanja
            // tj potrebe da odmah na pocetku biramo parametar
            setSelektovaniParametar(1);
        }


        //Sada cemo zavisno od setovane kalkulacije da emitujemo izracunatu tacku
        // prvo nam treba izbor koje parametre radimo
        SparamComplexPlain ourPoint;

        //TODO ovo za debug
        //    setSelektovaniParametar(2);

        switch (getSelektovaniParametar()) {
        case 1:
            //biran S11
            ourPoint = calcutatedData.getSparameters().at(0);
            break;
        case 2:
            //biran S12
            ourPoint = calcutatedData.getSparameters().at(1);
            break;
        case 3:
            //biran S21
            ourPoint = calcutatedData.getSparameters().at(2);
            break;
        case 4:
            //biran S22
            ourPoint = calcutatedData.getSparameters().at(3);
            break;
        default:
            //Za razlicite vrednosti bira S11
            ourPoint = calcutatedData.getSparameters().at(0);
            break;
        }

        realPart = ourPoint.real();
        imagPart = ourPoint.imag();

        //Ako je jedan od ovih odmah stavljen na true
        // pravice problem kod autoscalea
        //    setLinMagGuard(true);
        //    setLogMagGuard(true);
        //    setPhaseGuard(true);

        //Ovaj naravno pravi problem zbog index-a van opsega
        //    setLogPhaseGuard(true);

        //Ovi rade kako treba
        //    setRealGuard(true);
        //    setImagGuard(true);

        //Mozda nece biti lose da za svaku mogucu tacku kreiramo poseban proracun,
        // da ne bi smo se zapetljavali previse u kalkulaciju

        //LinMag
        //Prvo radimo parove (freq,linear Point)
        //nakon ovoga mozemo da emitujemo tacku (ovo vazi za linearne vrednosti)
        // dakle za LinMag, kasnije i za ostale
        if(getLinMagGuard() == true) {
            setLogMagGuard(false);      //Videcemo kako cemo sa ovim
            qreal pointToSend = sqrt( pow(realPart,2) + pow(imagPart,2));
            emit simplePointIsCalculated(QPointF(freqOfData,pointToSend));

            //nakon svakog emitornja tacke zovemo proveru za min i max
            // da proverimo da li radi...
            maxMinCalc(pointToSend);
        }

        //Ovo za logMag (freq,log Point)
        if (getLogMagGuard() == true) {
            qreal pointToSend = 10 * log10(sqrt( pow(realPart,2) + pow(imagPart,2)));
            emit simplePointIsCalculated(QPointF(freqOfData,pointToSend));

            //i ovde stavimo za minimume i maksimume za autoscale u view
            maxMinCalc(pointToSend);

        }

        //Ovo za phase (freq, PhasePoint)
        if (getPhaseGuard() == true) {
            //daje bolju dazu uz atan2() nego uz obican atan()
            qreal pointToSend = atan2(imagPart,realPart);
            emit simplePointIsCalculated(QPointF(freqOfData,pointToSend));



            //i ovde stavimo za minimume i maksimume za autoscale u view
            maxMinCalc(pointToSend);
        }

        //Za linPhase (phase point, linear magnitude point)
        // Ako je smithChartGuard == true onda daje parove (real,imag) na kartesian chartu
        if (getLinPhaseGuard() == true) {
            qreal phasePoint = atan2(imagPart,realPart);                //zamenili smo pa da vidimo atan sa atan2
            qreal pointToSend = sqrt( pow(realPart,2) + pow(imagPart,2));


            //Provera ukoliko zelimo vrednosti za smith chart
            if (getSmithChartGuard() == true) {
                //Ovo su vrednosti za specijalizovanu verziju smita iz cartesiana
                //Prosledjujemo realnu tacku na x osi, imag ce biti na y osi
                emit simplePointIsCalculated(QPointF(realPart,imagPart));
            }else {
                //Ovo se emituje za polarne
                emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
            }

            //        qDebug() << QPointF(qRadiansToDegrees(phasePoint),pointToSend);
        }

        //Za logPhase (phase point, log magnitude point)
        if (getLogPhaseGuard() == true) {
            qreal phasePoint = atan2(imagPart,realPart);
            qreal pointToSend = log10(sqrt( pow(realPart,2) + pow(imagPart,2)));


            if (getSmithChartGuard() == true) {
                //mora qAbs da ne bi pucao na mogucoj negativnoj vrednosti
                // ma koliko to neverovatno bilo
                qreal logRealPoint = log10(qAbs(realPart));
                qreal logImagPoint = log10(qAbs(imagPart));
                //Prosledjujemo realnu tacku koja ce biti x osa, imag na y osi
                emit simplePointIsCalculated(QPointF(logRealPoint,logImagPoint));
            }else {
                //Ovo se emituje ukoliko zelimo da emitujemo na polarni dijagram
                emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
            }

        }

        //Ovde posle dodati Real/Imag,(phase,Z),(phase,Y),(FreqSWR)

        //Za SWR racunamo (freq, SWR Point)
        if (getSwrGuard() == true) {
            // umesto ourPoint tacke moramo direktno da ocitamo s11
            SparamComplexPlain s11GPoint = calcutatedData.getSparameters().at(0);
            SparamComplexPlain swrPoint = (1.0 - s11GPoint)/(1.0 + s11GPoint);

            qreal pointToSend = sqrt(pow(swrPoint.real(),2) + pow(swrPoint.imag(),2));

            //konacno mozemo da saljemo tacku...
            emit simplePointIsCalculated(QPointF(freqOfData,pointToSend));

            //i ovde stavimo za minimume i maksimume za autoscale u view
            maxMinCalc(pointToSend);
        }

        //Aj za Z parametre, koristimo transformaciju za dvoportne mreze
        // zavisi koji je parametar selektovan pa cemo koristiti to za racunanje
        //Ovo koristimo za racunanje R + jX
        if (getZParamGuard() == true) {
            //sada zavisno od tacke koja je selektovana...
            //najlakse preko switcha
            SparamComplexPlain s11 = calcutatedData.getSparameters().at(0);
            SparamComplexPlain s12 = calcutatedData.getSparameters().at(1);
            SparamComplexPlain s21 = calcutatedData.getSparameters().at(2);
            SparamComplexPlain s22 = calcutatedData.getSparameters().at(3);
            //moramo ovde da je definisemo zbog switch statementa
            qreal pointToSend = 0;
            qreal phasePoint = 0;

            SparamComplexPlain z11;
            SparamComplexPlain z12;
            SparamComplexPlain z21;
            SparamComplexPlain z22;
            switch (getSelektovaniParametar()) {
            case 1:
                // Ovo racunamo z11 parametar
                z11 = 50.0*(((1.0+s11)*(1.0- s22) + s12*s21)/((1.0-s11)*(1.0-s22) - s12*s21));
                pointToSend = sqrt(pow(z11.real(),2) + pow(z11.imag(),2));

                phasePoint = atan2(z11.imag(),z11.real());
                //na kraju emitujemo tacku

                if (getSmithChartGuard() == true) {
                    // za smitov dijagram
                    emit simplePointIsCalculated(QPointF(z11.real(),z11.imag()));
                }else {
                    //za polarni dijagram
                    emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                }


                maxMinCalc(pointToSend);
                break;

            case 2:
                z12 = 50.0*(2.0*s12/((1.0-s11)*(1.0-s22)-s12*s21));
                pointToSend = sqrt(pow(z12.real(),2) + pow(z12.imag(),2));
                phasePoint = atan2(z12.imag(),z12.real());
                //            emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                if (getSmithChartGuard() == true) {
                    // za smitov dijagram
                    emit simplePointIsCalculated(QPointF(z12.real(),z12.imag()));
                }else {
                    //za polarni dijagram
                    emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                }
                maxMinCalc(pointToSend);
                break;

            case 3:
                z21 = 50.0*(2.0*s12/((1.0-s11)*(1.0-s22) - s12*s21));
                pointToSend = sqrt(pow(z21.real(),2) + pow(z21.imag(),2));
                phasePoint = atan2(z21.imag(),z21.real());

                //            emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                if (getSmithChartGuard() == true) {
                    // za smitov dijagram
                    emit simplePointIsCalculated(QPointF(z21.real(),z21.imag()));
                }else {
                    //za polarni dijagram
                    emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                }
                maxMinCalc(pointToSend);
                break;

            case 4:
                z22 = 50.0 * (((1.0 - s11)*(1.0+s22)+ s12*s21)/(((1.0-s11)*(1.0-s22)-s12*s21)));
                pointToSend = sqrt(pow(z22.real(),2) + pow(z22.imag(),2));
                phasePoint = atan2(z22.imag(),z22.real());

                //            emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                if (getSmithChartGuard() == true) {
                    // za smitov dijagram
                    emit simplePointIsCalculated(QPointF(z22.real(),z22.imag()));
                }else {
                    //za polarni dijagram
                    emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                }
                maxMinCalc(pointToSend);
                break;

            default:
                //Za sada nemoguce da ispadnemo van koloseka
                break;
            }

        }

        //Na isti nacim i Y parametre (phase, Y params)
        //Tacnije za polarni dijagram G + jB
        if (getYParamGuard() == true) {
            //sada zavisno od tacke koja je selektovana...
            //najlakse preko switcha
            SparamComplexPlain s11 = calcutatedData.getSparameters().at(0);
            SparamComplexPlain s12 = calcutatedData.getSparameters().at(1);
            SparamComplexPlain s21 = calcutatedData.getSparameters().at(2);
            SparamComplexPlain s22 = calcutatedData.getSparameters().at(3);

            //moramo ponovo kao kod z parametara zbog
            // switch statementa ovde da inizijalizujemo
            SparamComplexPlain y11;
            SparamComplexPlain y12;
            SparamComplexPlain y21;
            SparamComplexPlain y22;
            qreal pointToSend = 0;
            qreal phasePoint = 0;

            switch (getSelektovaniParametar()) {
            case 1:
                // Ovo racunamo z11 parametar
                y11 = (1.0/50.0)*(((1.0-s11)*(1.0+s22) + s12*s21)/((1.0-s11)*(1.0+s22) - s12*s21));
                pointToSend = sqrt(pow(y11.real(),2) + pow(y11.imag(),2));

                phasePoint = atan2(y11.imag(),y11.real());
                //na kraju emitujemo tacku
                //            emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                //Provera da li hocemo na smitov dijagram
                if (getSmithChartGuard() == true) {
                    emit simplePointIsCalculated(QPointF(y11.real(),y11.imag()));
                }else {
                    //Emituje ukoliko zelimo da saljemo za polarni dijagram
                    emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                }
                maxMinCalc(pointToSend);
                break;

            case 2:
                y12 = (1.0/50.0)*(-2.0*s12/((1.0+s11)*(1.0+s22)-s12*s21));
                pointToSend = sqrt(pow(y12.real(),2) + pow(y12.imag(),2));
                phasePoint = atan2(y12.imag(),y12.real());
                //            emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                //Provera da li hocemo na smitov dijagram
                if (getSmithChartGuard() == true) {
                    emit simplePointIsCalculated(QPointF(y12.real(),y12.imag()));
                }else {
                    //Emituje ukoliko zelimo da saljemo za polarni dijagram
                    emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                }
                maxMinCalc(pointToSend);
                break;

            case 3:
                y21 = (1.0/50.0)*(-2.0*s12/((1.0+s11)*(1.0+s22) - s12*s21));
                pointToSend = sqrt(pow(y21.real(),2) + pow(y21.imag(),2));
                phasePoint = atan2(y21.imag(),y21.real());

                //            emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                //Provera da li hocemo na smitov dijagram
                if (getSmithChartGuard() == true) {
                    emit simplePointIsCalculated(QPointF(y21.real(),y21.imag()));
                }else {
                    //Emituje ukoliko zelimo da saljemo za polarni dijagram
                    emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                }
                maxMinCalc(pointToSend);
                break;

            case 4:
                y22 = (1.0/50.0)*(((1.0 + s11)*(1.0+s22)+ s12*s21)/(((1.0+s11)*(1.0+s22)-s12*s21)));
                pointToSend = sqrt(pow(y22.real(),2) + pow(y22.imag(),2));
                phasePoint = atan2(y22.imag(),y22.real());

                //            emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                //Provera da li hocemo na smitov dijagram
                if (getSmithChartGuard() == true) {
                    emit simplePointIsCalculated(QPointF(y22.real(),y22.imag()));
                }else {
                    //Emituje ukoliko zelimo da saljemo za polarni dijagram
                    emit simplePointIsCalculated(QPointF(qRadiansToDegrees(phasePoint),pointToSend));
                }
                maxMinCalc(pointToSend);
                break;

            default:
                //Za sada nemoguce da ispadnemo van koloseka
                break;
            }

        }


        //Za (freq,Real(s))
        if (getRealGuard() == true) {
            qreal pointToSend = realPart;
            emit simplePointIsCalculated(QPointF(freqOfData,pointToSend));

            //i ovde stavimo za minimume i maksimume za autoscale u view
            maxMinCalc(pointToSend);
        }

        //Za (freq,Imag(s))
        if (getImagGuard() == true) {
            qreal pointToSend = imagPart;
            emit simplePointIsCalculated(QPointF(freqOfData,pointToSend));

            //i ovde stavimo za minimume i maksimume za autoscale u view
            maxMinCalc(pointToSend);
        }

        // Ovo je sada nepotrebno...

        //    if(magCalc == 1 || phaseCalc == 1 ){
        //        realPart = ourPoint.real();
        //        imagPart = ourPoint.imag();
        //       qreal magCalculatedPoint = 10 * log10(sqrt( pow(realPart,2) + pow(imagPart,2)));
        //       qreal phaseCalculatedPoint = atan(imagPart/realPart);

        //       //sada
        //    }



        //Necemo ovako
        // TODO izbrisati kada zavrsis sa ovim...

        //    switch (selektovaniParametar) {
        //    case 0:
        //        realPart = calcutatedData.getSparameters().at(selektovaniParametar).real();
        //        imagPart = calcutatedData.getSparameters().at(selektovaniParametar).imag();
        //        magPoint = 10 * log10(sqrt( pow(realPart,2) + pow(imagPart,2)));
        //        ourMagPoint.setX(calcutatedData.getFrequency());
        //        ourMagPoint.setY(magPoint);

        //        //Sada mozemo da emitujemo tacku
        //        //Ovo povezujemo u glavnom prozoru
        //        emit simplePointIsCalculated(ourMagPoint);
        //        break;
        //    case 1:
        //        realPart = calcutatedData.getSparameters().at(selektovaniParametar).real();
        //        imagPart = calcutatedData.getSparameters().at(selektovaniParametar).imag();
        //        magPoint = 10 * log10(sqrt( pow(realPart,2) + pow(imagPart,2)));
        //        ourMagPoint.setX(calcutatedData.getFrequency());
        //        ourMagPoint.setY(magPoint);

        //        //Sada mozemo da emitujemo tacku
        //        //Ovo povezujemo u glavnom prozoru
        //        emit simplePointIsCalculated(ourMagPoint);
        //        break;
        //    case 2:
        //        realPart = calcutatedData.getSparameters().at(selektovaniParametar).real();
        //        imagPart = calcutatedData.getSparameters().at(selektovaniParametar).imag();
        //        magPoint = 10 * log10(sqrt( pow(realPart,2) + pow(imagPart,2)));
        //        ourMagPoint.setX(calcutatedData.getFrequency());
        //        ourMagPoint.setY(magPoint);

        //        //Sada mozemo da emitujemo tacku
        //        //Ovo povezujemo u glavnom prozoru
        //        emit simplePointIsCalculated(ourMagPoint);
        //        break;
        //    case 3:
        //        realPart = calcutatedData.getSparameters().at(selektovaniParametar).real();
        //        imagPart = calcutatedData.getSparameters().at(selektovaniParametar).imag();
        //        magPoint = 10 * log10(sqrt( pow(realPart,2) + pow(imagPart,2)));
        //        ourMagPoint.setX(calcutatedData.getFrequency());
        //        ourMagPoint.setY(magPoint);

        //        //Sada mozemo da emitujemo tacku
        //        //Ovo povezujemo u glavnom prozoru
        //        emit simplePointIsCalculated(ourMagPoint);
        //        break;
        //    default:
        //        //TODO dodacemo i za ostale slucejemo
        //        break;
        //    }

        //Kada zavrsi sa ciklusima usrednjavanja da se ocisti bafer itd
        // hmmm kod pretposlednje tacke on propada na mesto gde ne treba
        // zato umesto numOfPointsMeasure - 1 stavljamo numOfPointsMeasure
        if ((averagePointPos == (numOfPointsMeasure)) &&(averageCycles == 0)) {

            //
            qDebug() << "###############Zavrsio postupak Usrednjavanja!!!###############";
            averagePointPos = 0;
            //Da ne bi smo radili prvu fazu ponovo
            // TODO
            averageCycles = averageCyclesNumber;
            //cistimo vektor da bi ponovo poceo od pocetka
            averageVector.clear();

            //view ce da emituje sledecu kalkulaciju
            //            emit sigRestartCalculation();
        }


    }  //Ovde zavrsava proveru averageCycles == 0
}

void NAmodel::performCalculation()
{
    //Ovde radimo rekalkulaciju parametara kada timer okine

    //Ovaj nam treba da vidimo koliko nam vremena treba
    // TODO treba nam za debug message dole
    //    QElapsedTimer performCalculationElapsedTimer;
    //    performCalculationElapsedTimer.start();

    //Aj da probamo da ogranicimo vreme izracunavanja
    //uzeto iz https://doc.qt.io/archives/qq/qq27-responsive-guis.html

    //TODO
    //    QTime maxTimeForCalculation;
    //    maxTimeForCalculation.start();

    //Kada tajmer pokrene novi ciklus
    // otkljucavamo racunanje setovanje cancelCalculationGuard
    // na false
    //    setCancelCalculationGuard(false);
    //    int testVar = 0;

    //Hmmm a da sweep bude lokalan
    //    qreal localSweepFreq = fStartMeasure;

    //Ceo ovaj deo ne valja
    //    sweepFreq = fStartMeasure;
    //    // fStopMeasure + 1 zbog slanja poslednje frekvencije
    //    while (sweepFreq <= (fStopMeasure+ 1)) {

    //Odmah dodajemo i uslov da je cancelCalculationGuard == true
    // da bi mogli da prekinemo racunanje

    if ((sweepFreq <= (fStopMeasure + deltaF)) && (getCalculationGuard() == true)) {
        getDataFromOneFreq(sweepFreq);
        sweepFreq += deltaFMeasure;
        QTimer::singleShot(IFBWtime,this,&NAmodel::performCalculation);
    }

    //        //Hmmm ovo cemo dodato istraziti
    //        //        int timeElapsed = maxTimeForCalculation.elapsed();
    //        //        if (timeElapsed > 150)
    //        //            break;
    //        //        testVar++;
    //        //        if (testVar == 401) {
    //        //            int bla;
    //        //            bla++;
    //        //        }
    //        getDataFromOneFreq(sweepFreq);
    //        sweepFreq += deltaFMeasure;

    //        //Aj da probamo sa sleepom(ovako nece ici)
    //        //mada ne mozemo ovako moracemo iz glavnom prozora
    //        //        emit modelRequesToSleep();

    //        //        Aj da pokusamo ovako
    //        //Ne ide ni ovako
    //        //        QElapsedTimer timer;
    //        //        timer.start();
    //        //        auto te = timer.elapsed();
    //        //        while (timer.elapsed() < 100 ) {
    //        //            auto te = timer.elapsed();
    //        //            int dumbCalc = 0;
    //        //            dumbCalc++;
    //        //        }
    //        //        timer.restart();


    //        //        if (getCancelCalculationGuard() == true) {
    //        //            //Ovde menjamo ako je promenjen format
    //        //            // i prekidamo racunanje
    //        //            return;
    //        //        }
    //  }
    //Kada zavrsi sa izracunavanjem treba da kliruje podatke iz
    // measured data i eventualno ubaci na neki vektor da bi mogao
    // da radi averaging ako je ukljucen
    measuredData.clear();

    //Ovi signali su pripadali resenju horizontalne
    // linije ali izvor ne potice direktno od modela
    // vec eventualno od view objekta i njegovog bafera ili
    // interpolacije...
    //Kada zavrsi kalkulaciju salje signal
    //    emit calcIsFinished();

    //Takodje je ista problematika sta kada dodje do kraja
    // treba da zapocne ponovo, kada dodje do kraja
    // i kada pozove ponovo on sweepFreq postavlja ponovo na
    // frekvenciju merenja


    //trebace verovatno jos ako nam trebaju blokovi za razne proracune
    // podataka(u mag/angle u dbm itd)
    //    qDebug() << "(NAModel) Jedan ciklus je gotov!";
    //    qDebug() << "(NAModel) hmmm imamo:" << deltaFMeasure;

    //TODO debug message koji racuna koliko treba da prebrise sve frekvencije
    //    qDebug() << "(NAmodel::performCalculation();) nam treba da prebrisemo sve frekvencije " << performCalculationElapsedTimer.elapsed() <<
    //                " milisekundi";
}

void NAmodel::testTimer()
{
    qDebug() << "Okinuo je timer!";
}


void NAmodel::ourSimpleSlot()
{
    qDebug() << "Ok vidi radi nas jednostavan signal slot";
}

void NAmodel::drugiSlot(int sig)
{
    //Ok ovo je drugi test
    qDebug() << "Ovo je satro izracunata tacka:" << sig;
}

void NAmodel::doWork()
{
    // Prvo resenje je bilo da samo ulazi u slot performCalculation
    //
    //    performCalculation();
    //ali bolje je resenje ide unedogled
    sweepFreq = fStartMeasure;
    forever{
        while (sweepFreq <= fStopMeasure) {
            getDataFromOneFreq(sweepFreq);
            sweepFreq += deltaFMeasure;
        }
        //Kada zavrsi sa jednim brisanjem pocinje novo
        sweepFreq = fStartMeasure;
    }
}

qreal NAmodel::getIfbwFreqModel() const
{
    return ifbwFreqModel;
}

void NAmodel::setIfbwFreqModel(const qreal &value)
{
    ifbwFreqModel = value;
}

int NAmodel::getIFBWtime() const
{
    return IFBWtime;
}

void NAmodel::setIFBWtime(int value)
{
    IFBWtime = value;
}

bool NAmodel::getCalculationGuard() const
{
    return calculationGuard;
}

void NAmodel::setCalculationGuard(bool value)
{
    calculationGuard = value;
}

void NAmodel::clearAverageBuffer()
{
    //Za brisanje average buffera
    averageVector.clear();
}

bool NAmodel::getCalGuard() const
{
    return calGuard;
}

void NAmodel::setCalGuard(bool value)
{
    calGuard = value;
}

qreal NAmodel::getSweepFreq() const
{
    return sweepFreq;
}

void NAmodel::setSweepFreq(const qreal &value)
{
    sweepFreq = value;
}


bool NAmodel::getCancelCalculationGuard() const
{
    return cancelCalculationGuard;
}

void NAmodel::setCancelCalculationGuard(bool value)
{
    cancelCalculationGuard = value;
}

void NAmodel::loadCalDut()
{
    //metoda koja je potrebna za ucitavanje
    // calDut-a
    calDut = new OurDut();
    calDut->setMeasureParameters(10,3000,401);
}

QVector<QPointF> NAmodel::getCalData()
{
    //Dohvata ucitani sparam fajl iz calDuta
    //Posle cemo da ga stavimo da radi sa referencema
    // i pointerima
    // Samo treba proveriti da li radi
    if (calDut != 0) {
        auto tempVector = calDut->getVectorSparamaPointer();
        QVector<QPointF> returnVector;

        if (getSelektovaniParametar() == 0) {
            foreach (FreqSparPlain elem, tempVector) {
                returnVector.append(QPointF(elem.getFrequency(), elem.getSparameters().at(getSelektovaniParametar()).real()));
            }

            return returnVector;
        }
        if (getSelektovaniParametar() == 1) {
            foreach (FreqSparPlain elem, tempVector) {
                returnVector.append(QPointF(elem.getFrequency(), elem.getSparameters().at(getSelektovaniParametar()).real()));
            }

            return returnVector;
        }
        if (getSelektovaniParametar() == 2) {
            foreach (FreqSparPlain elem, tempVector) {
                returnVector.append(QPointF(elem.getFrequency(), elem.getSparameters().at(getSelektovaniParametar()).real()));
            }

            return returnVector;
        }
        if (getSelektovaniParametar() == 3) {
            foreach (FreqSparPlain elem, tempVector) {
                returnVector.append(QPointF(elem.getFrequency(), elem.getSparameters().at(getSelektovaniParametar()).real()));
            }

            return returnVector;
        }
    }
}

int NAmodel::getAverageCyclesNumber() const
{
    return averageCyclesNumber;
}

void NAmodel::setAverageCyclesNumber(int value)
{
    averageCyclesNumber = value;
}

void NAmodel::resetSweepFreq()
{
    //resetuje sweepFreq u performCalculation();
    //takodje bi trebalo da posalje signal da isprazni bafere
    // u view-u


    //salje signal da se brisu baferi
    emit eraseViewBuffer();

    //i da restartujemo timer
    //    emit inspectTimer();

    //Aj da resetujemo i min i max
    setMinPoint(0.0);
    setMaxPoint(0.0);

    //Da li ovo mora na kraju pre emitovanja
    // brisanja ima vec restart
    //     sweepFreq = fStartMeasure;
}

int NAmodel::getAveragePointPos() const
{
    return averagePointPos;
}

void NAmodel::setAveragePointPos(int value)
{
    averagePointPos = value;
}

int NAmodel::getAverageCycles() const
{
    return averageCycles;
}

void NAmodel::setAverageCycles(int value)
{
    averageCycles = value;
}

bool NAmodel::getAverageGuard() const
{
    return averageGuard;
}

void NAmodel::setAverageGuard(bool value)
{
    averageGuard = value;
}

void NAmodel::setDeltaFMeasure(const qreal &value)
{
    deltaFMeasure = value;
}

int NAmodel::getNumOfPointsMeasure() const
{
    return numOfPointsMeasure;
}

void NAmodel::setNumOfPointsMeasure(int value)
{
    numOfPointsMeasure = value;
}

qreal NAmodel::getGenP() const
{
    return genP;
}

void NAmodel::setGenP(const qreal &value)
{
    genP = value;
}

void NAmodel::setChangedNumberOfPoints(int changedNumberOfPoins)
{
    //Za izmenu broja tacaka koju inicira MainWindow i akcija on_actionPoints
    dut.setMeasureParameters(fStartMeasure,fStopMeasure,changedNumberOfPoins);

    //Hmmm zaboravili smo i ovde da promenimo deltaF
    // mislim da je ovde pravio problem
    qreal changedDeltaFMeasure = (getFStopMeasure() - getFStartMeasure())/(changedNumberOfPoins - 1);
    setDeltaFMeasure(changedDeltaFMeasure);

    //ne treba zaboraviti da se promeni broj tacaka i u njemu samom(do sada menjali samo u dutu)
    setNumOfPointsMeasure(changedNumberOfPoins);

}

bool NAmodel::getMaxMinPointGuard() const
{
    return maxMinPointGuard;
}

void NAmodel::setMaxMinPointGuard(bool value)
{
    maxMinPointGuard = value;
}

qreal NAmodel::getMinPoint() const
{
    return minPoint;
}

void NAmodel::setMinPoint(const qreal &value)
{
    minPoint = value;
}

void NAmodel::maxMinCalc(qreal &pointToCompare)
{
    //Ova metoda se odnosi na setovanje maksimuma i minimuma
    //Stalno racuna ove tacke, ali pogled ce se samo
    // menjati ako ga pritisnemo gde ce procitati vrednosti ovih
    // tacaka i setovace Y osu
    // Ovo cemo zvati svaki put kada racunamo nesto

    //Za prvo trazenje polaznog minimuma i maksimuma nam treba
    // da znamo polaznu tacku(ne moze 0,0 uvek)
    //Aj da probamo ovde da debug
    if (phaseGuard == true) {
        int debugVar;
        debugVar++;
    }


    if (getMaxMinPointGuard()) {
        setMaxPoint(pointToCompare);
        setMinPoint(pointToCompare);

        //na kraju ga vracamo na false ako nije prva tacka
        setMaxMinPointGuard(false);
    }



    //nadalje nastavljamo sa klasicnom proverom minimuma i
    // maksimuma

    //Prvo proveravamo da li je u pitanju maksimum
    if(pointToCompare > getMaxPoint()){
        setMaxPoint(pointToCompare);
    }

    //pa proveravamo da li je u pitanju minimum
    if(pointToCompare < getMinPoint()) {
        setMinPoint(pointToCompare);
    }
}

qreal NAmodel::getMaxPoint() const
{
    return maxPoint;
}

void NAmodel::setMaxPoint(const qreal &value)
{
    maxPoint = value;
}

void NAmodel::setFStartMeasure(const qreal &value)
{
    fStartMeasure = value;
}

void NAmodel::setFStopMeasure(const qreal &value)
{
    fStopMeasure = value;
}

//int NAmodel::getNumOfPointsMeasure() const
//{
//    return numOfPointsMeasure;
//}

//void NAmodel::setNumOfPointsMeasure(int value)
//{
//    //TODO ovde pravi probleme
//    //Ovde kada se menja moracemo da radimo ponovo
//    // interpolaciju
//    // TODO verovatno nece raditi iz prve
// //    dut.interpolateMeasure(value);
// //    numOfPointsMeasure = value;
//}

void NAmodel::resetAllFormatGuards()
{
    setLinMagGuard(false);
    setLogMagGuard(false);
    setPhaseGuard(false);
    setLinPhaseGuard(false);
    setLogPhaseGuard(false);
    setRealGuard(false);
    setImagGuard(false);
    setSwrGuard(false);
    setZParamGuard(false);
    setYParamGuard(false);
    //To bi trebali da budu svi guardovi
    // zove se prilikom menjanja formata prikaza podataka

    //setujemo specijala maxMinPointGuard point guard na true
    // da bi mogao da podesi prvu tacku
    setMaxMinPointGuard(true);

    //Setujemo smithchartguard
    setSmithChartGuard(false);

    //Posto ovu zovemo za skoro svaku akciju ovde cemo
    // resetovati sweepFreq

    //Preko ovoga resetujemo sweepFreq i saljemo signal
    // da se isprazne baferi u view-u
    resetSweepFreq();
    //ipak bolje preko cancelCalculationGuard
    //    setCancelCalculationGuard(true);

    //Umesto komplikovane procedure preko signal slota
    //Ovo je ustanovljeno da se ne resava iz na modela
    //    emit cancelNaModelViewSig();
    //Hmm vise ne koristimo resetSweepFreq vec
    //    performCalculation();
    //Valjda ovde nece praviti problem
    //    emit sigRestartCalculation();
}

bool NAmodel::getYParamGuard() const
{
    return yParamGuard;
}

void NAmodel::setYParamGuard(bool value)
{
    yParamGuard = value;
}

bool NAmodel::getSmithChartGuard() const
{
    return smithChartGuard;
}

void NAmodel::setSmithChartGuard(bool value)
{
    smithChartGuard = value;
}

bool NAmodel::getZParamGuard() const
{
    return zParamGuard;
}

void NAmodel::setZParamGuard(bool value)
{
    zParamGuard = value;
}

bool NAmodel::getSwrGuard() const
{
    return swrGuard;
}

void NAmodel::setSwrGuard(bool value)
{
    swrGuard = value;
}

bool NAmodel::getImagGuard() const
{
    return imagGuard;
}

void NAmodel::setImagGuard(bool value)
{
    imagGuard = value;
}

bool NAmodel::getRealGuard() const
{
    return realGuard;
}

void NAmodel::setRealGuard(bool value)
{
    realGuard = value;
}

bool NAmodel::getLogPhaseGuard() const
{
    return logPhaseGuard;
}

void NAmodel::setLogPhaseGuard(bool value)
{
    logPhaseGuard = value;
}

bool NAmodel::getLinPhaseGuard() const
{
    return linPhaseGuard;
}

void NAmodel::setLinPhaseGuard(bool value)
{
    linPhaseGuard = value;
}

bool NAmodel::getPhaseGuard() const
{
    return phaseGuard;
}

void NAmodel::setPhaseGuard(bool value)
{
    phaseGuard = value;
}

bool NAmodel::getLogMagGuard() const
{
    return logMagGuard;
}

void NAmodel::setLogMagGuard(bool value)
{
    logMagGuard = value;
}

bool NAmodel::getLinMagGuard() const
{
    return linMagGuard;
}

void NAmodel::setLinMagGuard(bool value)
{
    linMagGuard = value;
}

int NAmodel::getPhaseCalc() const
{
    return phaseCalc;
}

void NAmodel::setPhaseCalc(int value)
{
    phaseCalc = value;
}

int NAmodel::getMagCalc() const
{
    return magCalc;
}

void NAmodel::setMagCalc(int value)
{
    magCalc = value;
}

int NAmodel::getSelektovaniParametar() const
{
    return selektovaniParametar;
}

void NAmodel::setSelektovaniParametar(int value)
{
    selektovaniParametar = value;
}

qreal NAmodel::getDeltaFMeasure() const
{
    return deltaFMeasure;
}

qreal NAmodel::getFStopMeasure() const
{
    return fStopMeasure;
}

qreal NAmodel::getFStartMeasure() const
{
    return fStartMeasure;
}

//Cisto testiramo metodu
void NAmodel::processPoint(const FreqSparPlain &calcPoint)
{
    qDebug() << "Imamo tacku na frekvenciji (MHz): " << calcPoint.getFrequency() <<
                ", a parametri su " << calcPoint.getSparameters().at(0).real() << ", " <<
                calcPoint.getSparameters().at(0).imag() << endl << "Naravno ima jos ovoga...";
}


