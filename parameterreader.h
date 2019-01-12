#ifndef PARAMETERREADER_H
#define PARAMETERREADER_H

#include <QObject>
#include <QString>
#include <QVector>

#include "freqsparplain.h"

/*
 * Predstavlja citac parametra iz fajla, napravicemo ga sto jednostavnijeg.
 * Ucitava i parsira sve podatke i na zahtev ih sve prenosti
 * Zbog toga nam zapravo treba da uvedemo freqsparPlain
 *
 * Implementacija je krajnje jednostavna i posao zavrsi na
 * cistoj konstrukciji objekta.
 * Kao rezultat vraca Vector<FreqSparPlain> podataka
*/

class ParameterReader : public QObject
{
    Q_OBJECT
public:
    explicit ParameterReader(QObject *parent = nullptr);
    QVector<FreqSparPlain> getExtractedData() const;

signals:

public slots:

private:
    QString selectedFile;
    QVector<QString> rawData;               //Ovo su sirovi podaci iz fajla
    QVector<FreqSparPlain> extractedData;   //Ovo su extraktovani podaci

};

#endif // PARAMETERREADER_H
