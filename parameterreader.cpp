#include "parameterreader.h"
#include <QFileDialog>
#include <QTextStream>
#include <QStringList>

/*
 * Ovde nam treba qfileDialog da bi smo lagano nasli podatke
*/

ParameterReader::ParameterReader(QObject *parent) : QObject(parent)
{
    //while petlju smo cisto stavili da nam ne dolazi do pucanja kada ne selektujemo
    // fajl, mozda bi smo mogli to nekako elegantnije da resimo, ali ovo je dovoljno za sada
    while (selectedFile.length() == 0) {


        //Preko ovoga citamo fajl
        selectedFile = QFileDialog::getOpenFileName(0,
                                                    tr("Otvori s parametar fajlove(prvo se ucitava standardan,"
                                                       "drugi se ucitava kalibracioni)"),
                                                    "D:\\Downloads\\sPAram\\diplKomplParametri",
                                                    tr("Touchstone Csv (*.s2p *.csv)"));

        //Nakon ovoga ga citamo

        QFile file(selectedFile);
        file.open(QIODevice::ReadOnly);

        //ne moramo da proveravamo da li je open
        if (file.exists()) {
            QTextStream in(&file);

            while (!in.atEnd()) {
                QString line = in.readLine();
                rawData.append(line);
            }
        }
        file.close();

        //Sada treba da ga isparsiramo
        //NAPOMENA Frekvencije smo normalizovali na megaherce
        foreach (QString stringToParse, rawData) {
            if (stringToParse.startsWith('!') || (stringToParse.startsWith('#')) ||
                    stringToParse.startsWith('B') || stringToParse.startsWith('F') ||
                    (stringToParse.length() == 0) || stringToParse.startsWith('E')) {
                continue;
            }else {
                QStringList strData;
                //Ako ne pocinje sa ovim onda treba da izvucemo podatke iz linije
                //moramo da dodamo da ako je s2p fajl
                if (selectedFile.endsWith('p')) {
                    strData = stringToParse.split('\t',QString::SkipEmptyParts);
                }else {
                    //Ovo je za slucaj kada imamo csv fajl
                    strData = stringToParse.split(',',QString::SkipEmptyParts);
                }

                //Ok ovo nam je trebalo samo da vidimo da li korektno radi
                int innerDebugVar = 0;
                innerDebugVar++;

                //prvi nam je frekvencija, slede posle parametri
                FreqSparPlain tempFreqObj;
                //Aj sada da krenemo da konvertujemo podatke
                // za ekstraktovanje iz scientific notacije pogledati toDouble od QString-a
                //istovremeno normalizujemo na MHz-ove
                QString extractedFreqString = strData.at(0);
                double extractedFreq = extractedFreqString.toDouble()/1e+06;

                //Da bi smo to uradili moramo da vidimo kolika nam je zaista red
                // frekvencija, pa onda treba normalizovati sa obzirom na to...
                // delicemo frekvenciju sa 10 i povecavati sve dok ne nadjemo blisku
                // vrednost, posle cemo da vidimo gde ovo mozemo da stavimo
                // mada ovo deluje glupavo posto vec radimo sa redom megaherca
                // pa je dovoljno da ga podelimo sa 10^6

                //            int redFrekvencije = 10;
                //            while (redFrekvencije < extractedFreq ) {
                //                redFrekvencije *= redFrekvencije;
                //            }

                //            //Tek ovde normalizujemo frekvenciju
                //            extractedFreq = extractedFreq/redFrekvencije;




                //sada mozemo da krenemo da konstruisemo FreqSparPlain objekat
                tempFreqObj.setFrequency(extractedFreq);

                //sada na isti nacin i ostale parametre
                // naravno ovo cemo uraditi u petlji da se ne bi smo ponavljali

                for (int var = 1; var < strData.count(); var = var + 2) {
                    QString realPartString = strData.at(var);
                    double realPart = realPartString.toDouble();
                    QString imagPartString = strData.at(var+1);
                    double imagPart = imagPartString.toDouble();

                    //Treba nam ova temp varijabla da bi smo jednostavno dodali
                    // parametar
                    SparamComplexPlain tempComplexSpar{realPart,imagPart};

                    //sada mozemo da nastavimo da konstruisemo FreqSparPlain objekat
                    // kada koristimo setOneParamer metodu ovaj jednostavno dodaje
                    // na listu vrednosti
                    tempFreqObj.setOneParameter(tempComplexSpar);


                }

                //moramo da obradimo i slucaj ako imamo samo s11 parametre
                if (tempFreqObj.getSparameters().count() <= 4) {
                    SparamComplexPlain tempComplexZeroSparam {0,0};

                    //Naravno stavicemo ukoliko jos neki fali
                    //Ovo bi trebalo da radi posao
                    for (int var = tempFreqObj.getSparameters().count(); var < 4; ++var) {
                        tempFreqObj.setOneParameter(tempComplexZeroSparam);
                    }
                }

                //sada jos ostaje da unesemo rezultate
                extractedData << tempFreqObj;
            }
        }
    }

    int debugVar = 0;
    debugVar++;

}

QVector<FreqSparPlain> ParameterReader::getExtractedData() const
{
    return extractedData;
}
