#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QVector>

//Ovo nam sledece treba da vidimo koliko nam zapravo treba
// vremena
#include <QElapsedTimer>

//Za test slota kada obrisemo seriju
#include <QLineSeries>

#include "namodel.h"
#include "ourdut.h"
#include "freqspar.h"
#include "freqsparplain.h"
#include "ourdut.h"
#include "noisemodel.h"
#include "parameterreader.h"
#include "view.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(nullptr),
    model(nullptr),
    view(nullptr)
{
    ui->setupUi(this);
    qDebug() << "Blaa....";
//    NAmodel our_model;

    //Aj da vidimo koliko nam treba vremena za ovo
//    QElapsedTimer elapTimer;
//    elapTimer.start();

    model = new NAmodel();

    //Odmah da ga stavimo u worker Thread
    model->moveToThread(&workerThread);
    //I kod starta i ovde nema nesto ucinka sa prioritetima
     //   workerThread.setPriority(QThread::HighestPriority);
    //Aj da ubacimo i view u poseban thread
    connect(&workerThread,&QThread::finished,model,&NAmodel::deleteLater);
    workerThread.start();

//    qDebug() << "(MainWindow)Ok za kreiranje modela nam treba " << elapTimer.elapsed() << " milisekundi";
//    elapTimer.restart();

    //TODO ok je pogled i on radi kako treba
    // za izvuceni pogled(poseban prozor)
    // dovoljno je (bez parenta)
    // view = new View();
    view = new View();
    ui->verticalLayout->addWidget(view);


    //Sada da sinhronizujemo model i view
    // ovo smo mogli i preko specijalnog help objekta
    // ali za sada cemo ovako, TODO eventualno
    // to raditi preko help objeta
    // View::initializeData mora da ima podatak of deltaF
    // TODO iako mozemo da setujemo frekvencije na pocetku
    // ipak zbog problema sa initData u viewu moramo odjedanput da ih setujemo
    // Tek kasnije kada pomeramo frekvencije mozemo i moramo da zovemo initData
    // i na ovakav nacin ne bi trebao da upada u beskonacne petlje
//    view->setFreqDeltaFMeasure(model->getDeltaFMeasure());
//    view->setFreqStartMeasure(model->getFStartMeasure());
//    view->setFreqStopMeasure(model->getFStopMeasure());
    view->setMeasureParameters(model->getFStartMeasure(),model->getFStopMeasure(),model->getDeltaFMeasure(),
                               model->getNumOfPointsMeasure());

    //sada mozemo da inicijalizujemo i pogled
//    view->initializeSeriesData();
    // sada mozemo da prosledjujemo tacke posle kalucije


//    qDebug() << "(MainWindow) Za kreiranje pogleda nam treba " << elapTimer.elapsed() << " milisekundi";

//    OurDut dut;
//    dut.generateFakeData();
//    FreqSpar spar(nullptr);
//    qDebug() << spar.simpleReport();
//    FreqSpar spar2(1.2,3.5,1.0);
//    qDebug() << spar2.simpleReport();

//    QVector<FreqSpar> testvec(5);
//    testvec << spar << spar2;
//    qDebug() << testvec;
//    FreqSparPlain plain(0.1,4.5,4.6);
//    plain.setFrequency(3.6);
//    QVector<FreqSparPlain> plainPoints;


//    for (int var = 0; var < 10; ++var) {
//        plain.setFrequency(var*0.1);
//        SparamComplexPlain tempSparam;
//        tempSparam.real(var+var/10);
//        tempSparam.imag(var*3/10);
//        plain.setS11(tempSparam);

//        //Sada da ih dodamo
//        plainPoints << plain;


//    }

    //Ok ovo radi
//    OurDut dut;
//    dut.generateFakeData();
//    dut.setNumberOfPointsDut(3);

//    NoiseModel noisemodel;

    //I ovaj radi kako treba,
//    ParameterReader paramReader;

    //mora ovde da se zove timer
    // videcemo da li cemo posle zvati timer ili cemo
    // iz posebnog worker threada ovo da radimo.


    //Ovaj smo koristili za prvobitnu non thread verziju
    timer = new QTimer();
//    connect(timer,SIGNAL(timeout()),model,SLOT(testTimer()));
// !    connect(timer,SIGNAL(timeout()),model,SLOT(performCalculation()));

    //    connect(model,SIGNAL(point))
//    connect(model,&NAmodel::pointIsCalulated(),model,&NAmodel::processPoint());

//    connect(model,SIGNAL(oursi)
//    connect(model,&NAmodel::ourSimpleSignal,model,&NAmodel::ourSimpleSlot);    //Naravno da ne zaboravimo da ga startujemo
//    connect(model,SIGNAL())
    // za sada cemo staviti da se timer startuje svake
    // sekunde
// ! timer->start(1000);

    //Aj da probamo da povezemo pogled sa modelom
// !    connect(model,&NAmodel::pointIsCalulated,view,&View::getDataToDrawFromNAModel,Qt::DirectConnection);


    int debugTempVar = 0;
    debugTempVar++;

    //cek da testiramo view
   view->show();
    qRegisterMetaType<FreqSparPlain>("FreqSparPlain");
   //aj da pokusamo ovo da premestimo u drugi thread
   // pobudjivacemo ga sa timerom....mada cini mi se da to nece biti potrebno
//   model->moveToThread(&workerThread);
//   //I kod starta i ovde nema nesto ucinka sa prioritetima
//    //   workerThread.setPriority(QThread::HighestPriority);
//   //Aj da ubacimo i view u poseban thread
//   connect(&workerThread,&QThread::finished,model,&NAmodel::deleteLater);

//   connect(this,&MainWindow::startComputation,model,&NAmodel::doWork);
   //Aj pokusamo sa timerom
//   connect(timer,&QTimer::timeout,model,&NAmodel::doWork);
//   connect(timer,&QTimer::timeout,model,&NAmodel::testTimer);

   //Ovo ide u forever petlji(radi)
//   connect(this,&MainWindow::startComputation,model,&NAmodel::doWork);

   //Aj preko timera da ide...
//   connect(timer,&QTimer::timeout,model,&NAmodel::performCalculation);
   //Umesto timera kada zavrsi sa sa crtanjem dajemo signal za novi ciklus
   //Na liniji 208 je preneto
//   model->performCalculation();
//   Konektujemo signal kada je crtanje zavrseno
   connect(view,&View::sigDrawIsDone,model,&NAmodel::performCalculation);

   //Aj da ga zamenimo sa drawPoint
//   connect(model,&NAmodel::pointIsCalulated,view,&View::getDataToDrawFromNAModel,Qt::QueuedConnection);



   //Ova konekcija dodaje tacku po tacku...
//   connect(model,&NAmodel::simplePointIsCalculated,view,&View::drawPoint,Qt::QueuedConnection);

   //Ova konekcija radi baferovano punjenje prikaza
   //Napravili smo varijantu iste
//   connect(model,&NAmodel::simplePointIsCalculated,view,&View::drawPointBuffered,Qt::QueuedConnection);
   connect(model,&NAmodel::simplePointIsCalculated,view,&View::drawPoint2,Qt::QueuedConnection);
//   connect(this,&MainWindow::startComputation,this,&MainWindow::winTimer);
   //kada zavrsimo sa inicijalizacijom....
//   workerThread.start();

//   emit startComputation();

   // Izgleda da je minimum oko 50 milisekundi da izracuna sve
   //TODO ovde ce trebati dodatna zajebancija da vidimo gde
   // gubi toliko vremena... I eventualno pauziranje tamo gde ga racuna
   //Hmmm naravno postoji da li ga bildamo u debugu ili releaseu...interesantno..
   //za sada se odlicmnp
   timer->start(250);

   ui->lineEdit->setVisible(false);
//   ui->menuBar->actions().at(0)->

    //Jos da povezemo signale za cancel i enable connectio
   // izmedju pogleda i NaModela
//    Prekomplikovano ( a i pokazano je da ne zavisi od ovoga, problem je u view-u)
//   connect(model,&NAmodel::cancelNaModelViewSig,this,&MainWindow::cancelNaModelViewConn);
   //kada zapocne novi ciklus ponovo povezuje

//   connect(this,&MainWindow::startComputation,this,&MainWindow::enableNaModelViewConn);

   //I da izbrisemo bafere(pravi probleme)
//   connect(model,&NAmodel::cancelNaModelViewSig,view,&View::eraseBuffers);

//   connect(timer,&QTimer::timeout,this,&MainWindow::enableNaModelViewConn);
//   connect(model,&NAmodel::calcIsFinished,this,&MainWindow::enableNaModelViewConn);

   //Treba nam samo konekcija da kada dodje do resetsweepa da se obrisu i baferi u view-u
//    connect(model,&NAmodel::eraseViewBuffer,view,&View::eraseBuffers,Qt::DirectConnection);

    //Da vidimo da li treba da resetujemo timer kada resetuje sweepFreq u NaModel-u
    //Videcemo kako cemo da ga restartujemo
//    connect(model,&NAmodel::inspectTimer,this,&MainWindow::timerRemainTime);
    //Umesto timera kada zavrsi sa sa crtanjem dajemo signal za novi ciklus
//    model->performCalculation();

    //Konektujemo signal kada je crtanje zavrseno
    //mada moracemo da ga modifikujemo sa dva slota, dodacemo slot da ponovo pokrenemo
    // perform calculation i resetujemo startnu frekvenciju
//    connect(view,&View::sigDrawIsDone,model,&NAmodel::performCalculation);
    connect(view,&View::sigDrawIsDone,this,&MainWindow::restartCalculation);
    connect(this,&MainWindow::startComputation,model,&NAmodel::performCalculation);
    //kada menjamo format treba da restartujemo kalkulaciju
    connect(model,&NAmodel::sigRestartCalculation,this,&MainWindow::restartCalculation);
    //Ovo da pokusamo da napravimo da stavimo model u sleep
//    connect(model,&NAmodel::modelRequesToSleep,this,&MainWindow::putModelOnSleep,Qt::QueuedConnection);

    //Aj da probamo da inicijalizujemo racunanje kada konstruisemo view
    emit startComputation();
//    connect(view,&View::sigDrawIsDone,model,&NAmodel::performCalculation,Qt::QueuedConnection);

    //Aj da povezemo kada uklonimo sve iz serije
//    connect(view->getSeries1(),&QLineSeries::pointsRemoved,this,&MainWindow::restartCalculation);
}

MainWindow::~MainWindow()
{
    //Ovo smo morali da dodamo u destruktor
    workerThread.quit();
    workerThread.wait();
    delete ui;
}

void MainWindow::winTimer()
{
    qDebug() << "Ok ovde radi...";
}

void MainWindow::showLineEdit()
{
    //Ovo cemo kasnije da implementiramo
//    qDebug() << "Ok uneo si " << ui->verticalLayout->children().at(0)->
//    QList<QLineEdit*> = ui->verticalLayout->findChildren<QLineEdit*>();
//    QList<QLineEdit> = ui->verticalLayout->children();
    qDebug() << "ok pritisnuo si enter" << tempLineEdit->text();
    tempLineEdit->clear();

    disconnect(tempLineEdit,&QLineEdit::returnPressed,this,&MainWindow::showLineEdit);
//    ui->verticalLayout->removeWidget(tempLineEdit);
}

void MainWindow::processZparam()
{
    qDebug() << ui->lineEdit->text();

    qDebug() << "Ovo je iz processZparam text-a";
    this->setToolTip(ui->lineEdit->text());
    //Aj da pokusamo ovo...
    qreal tempVal = ui->lineEdit->text().toInt();
    view->setOurXAxisRange(tempVal);

    //na kraju diskonektujemo signal koji ga je pobudio
    // dokazano radi
    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processZparam);

    //na kraju se sakriva da ne bi unosio zabunu
    // posto mora da se inicira i da se uradi clear
    ui->lineEdit->clear();
    ui->lineEdit->hide();

}

void MainWindow::processSparam()
{
    //Trebalo bi isto da radi kao i processZparam();
    qDebug() << "Ovo je iz processSparam";
    //Ovo sa tooltipom samo smeta
//    this->setToolTip(ui->lineEdit->text());
    qreal tempVal = ui->lineEdit->text().toInt();
    view->setOurYMinAxisRange(tempVal);

    //na kraju diskonektujemo signal koji ga je pobudio
    // i tako se to resava
    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processSparam);

    //na kraju se sakriva da ne bi unosio zabunu
    // posto mora da se inicira i da se uradi clear
    ui->lineEdit->clear();
    ui->lineEdit->hide();

}

void MainWindow::processStartSet()
{
    //Ovaj slot odgovara na pokretanje akcije
    // MainWindow::on_actionStart_2_triggered()
    // Dakle upisujemo startnu frekvenciju i podesavamo
    // model i view
    qreal tempVal = ui->lineEdit->text().toInt();

    //Dodajemo proveru ako je start frekv od stop frekv
    // onda ne radi nista
//    if (model->getFStopMeasure() > tempVal) {
//        model->setFStartMeasure(tempVal);
//        view->setFreqStartMeasure(tempVal);
//    }

    //Pre nego sto krene bilo sta da menja da zaustavimo kalkulaciju
    model->setCalculationGuard(false);

    //Aj da probamo da vidimo kako radi brisanje bafera
    QTimer timerToDelay;
    timerToDelay.singleShot(250,view,&View::eraseBuffers);

    //kalkulaciju cemo nastaviti kada setujemo parametre

    // Pomerili gore zbog provere da nam ne puca bezveze
    model->setFStartMeasure(tempVal);
    //Potrebno je da ponovo uradi interpolaciju kada menja startnu frekvenciju
    //ali samo ako je manje od stop frekvencije
    if (model->getFStartMeasure() < model->getFStopMeasure()) {
        model->setChangedNumberOfPoints(model->getNumOfPointsMeasure());
    }

    //Menjamo startnu frekvenciju u view-u
    view->setFreqStartMeasure(tempVal);

    //dakle ovde mozemo da nastavimo kalkulaciju
    //naravno ne treba zaboraviti da treba da se omoguci kalkulacija
    // u perform calculation
//    model->setCalculationGuard(true);
    timerToDelay.singleShot(500,this,&MainWindow::restartCalculation);

    //za sada cemo videti kako cemo zapoceti kalkulaciju
    // ukidamo ovo ponovno pozivanje posto pravi problem u view-u
//    QTimer innTime;
//    innTime.singleShot(1000,this,&MainWindow::restartCalculation);

    //Po upisu raskidamo vezu
    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processStartSet);
    ui->lineEdit->clear();
    ui->lineEdit->hide();

    //i da pokrenemo promenu u tekstu akcije
    ui->actionStart_2->setText(QString("Start: %1 MHz").arg(model->getFStartMeasure()));

}

void MainWindow::processStopSet()
{
    //Ovaj slot odgovara ona pokretanje akcije
    // MainWindow::on_actionStop_triggered();
    qreal tempVal = ui->lineEdit->text().toInt();

    //dodajemo da ako je stop manje od start frekv ne radi nista
//    if (model->getFStartMeasure() < tempVal) {
//        model->setFStopMeasure(tempVal);
//        view->setFreqStopMeasure(tempVal);
//    }

    //Pre nego sto krene bilo sta da menja da zaustavimo kalkulaciju
    model->setCalculationGuard(false);

    //Kao sto smo radili u processStartSet();
    //Aj da vidimo umesto timera ovo direktno da zovemo
    QTimer timerToDelay;
    timerToDelay.singleShot(500,view,&View::eraseBuffers);
//    view->eraseBuffers();


    //Ovo je smo pomerili gore da bi imali proveru da nam
    // ne puca bezveze
    model->setFStopMeasure(tempVal);
    //Potrebno je da uradi interpolaciju ali samo ako je fstop
    // vece od start
    if (model->getFStartMeasure() < model->getFStopMeasure()) {
        model->setChangedNumberOfPoints(model->getNumOfPointsMeasure());
    }

    view->setFreqStopMeasure(tempVal);

    //Ovde ponovo ukljucujemo proracun
    //dakle ovde mozemo da nastavimo kalkulaciju
    //naravno ne treba zaboraviti da treba da se omoguci kalkulacija
    // u perform calculation
//    model->setCalculationGuard(true);

    //Umesto timera ovo direktno da zovemo
    timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);
//    restartCalculation();
    //aj da probamo da restartujemo kalkulaciju tek kada se uklone sve
    // tacke iz serije posle erased buffer

    //Po upisu raskidamo vezu
    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processStopSet);
    ui->lineEdit->clear();
    ui->lineEdit->hide();

    //i da pokrenemo promenu u tekstu akcije
    ui->actionStop->setText(QString("Stop: %1 MHz").arg(model->getFStopMeasure()));

}

void MainWindow::processChangeGenP()
{
    //Reaguje na unos nove snage u milivatima
    // reaguje na pokretanje akcije MainWindow::on_actionPower_2_triggered();
    qreal tempVal = ui->lineEdit->text().toDouble();                       // u int vrednosti prebacijue

    model->setGenP(tempVal);

    //ne mora da radi update view-a

    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processChangeGenP);
    ui->lineEdit->clear();
    ui->lineEdit->hide();

    //I kada se promeni resetujemo sve
    // checkove u okviru menuPowerRanges
    resetMenuPowerRanges();

    //I da zabelezimo koliko snagu imamo
    ui->actionPower_2->setText(QString("Power: %1 dBm").arg(model->getGenP()));

}

void MainWindow::processChangeNumOfPoints()
{
    //Reaguje na unos u okviru akcije MainWindow::on_actionPoints_triggered();
    qreal tempVal = ui->lineEdit->text().toInt();                   //mora da bude celobrojna vrednost

    // TODO
    //Ovo tek treba da implementiramo
    //Verovatno moramo da promenimo i broj tacaka u view-u
    //hmmm na sledecoj liniji tek dole
//    model->setChangedNumberOfPoints(tempVal);

    //Pre nego sto krene bilo sta da menja da zaustavimo kalkulaciju
    model->setCalculationGuard(false);

    //Kao sto smo radili u processStartSet();
    //Aj da vidimo umesto timera ovo direktno da zovemo
    QTimer timerToDelay;
    timerToDelay.singleShot(500,view,&View::eraseBuffers);
//    view->eraseBuffers();

    //Tek sada mozemo da menjamo broj tacaka
    if (model->getFStartMeasure() < model->getFStopMeasure()) {
        model->setChangedNumberOfPoints(tempVal);
    }

    //Hmmm svejedno treba sinhronizovati parametre
//    model->getMeasureParameters();
    //Hmm desava se da se ovde cesto napravi razlika u jednoj tacki
    // pa se mora sinhronizovati
//    view->setNumberOfPoints(tempVal);
    view->setNumberOfPoints(model->getNumOfPointsMeasure());
    //pored broja tacaka menjamo i pointpos
    view->setPointPos(0);

    //Sada da pokrenemo ponovo proces iscrtavanja
     timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);

    //mozda bi trebalo da se resetuje sweep kada radimo ovo
//    model->resetSweepFreq();
    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processChangeNumOfPoints);
    ui->lineEdit->clear();
    ui->lineEdit->hide();

    //i jos kada menjamo broj tacaka kada ih promenimo
    ui->actionPoints->setText(QString("Points: %1").arg(model->getNumOfPointsMeasure()));
}

void MainWindow::cancelNaModelViewConn()
{
    //slot koji se aktivira kada menjamo proracun u NA modelu
    disconnect(model,&NAmodel::simplePointIsCalculated,view,&View::drawPointBuffered);
    //sada da ga ponovo povezemo
    connect(model,&NAmodel::simplePointIsCalculated,view,&View::drawPointBuffered,Qt::QueuedConnection);
}

void MainWindow::enableNaModelViewConn()
{
    //Ovaj slot ce se aktivirati kada zapocne novi ciklus
    connect(model,&NAmodel::simplePointIsCalculated,view,&View::drawPointBuffered,Qt::QueuedConnection);

}

void MainWindow::timerRemainTime()
{
    //Jednostavan slot da vidimo kada resetujemo sve koliko
    // je timer otkucao, moguce je da je nepotreban kako cemo
    // cekati u View-u da krene sa startnom frekvencijom
    qDebug() << "Ok timer je otkucao " << timer->remainingTime();
//    timer->dumpObjectInfo();
    timer->stop();
    //Ponovo ga startujemo
    timer->start(250);
}

void MainWindow::restartCalculation()
{
    qDebug() << "Aktiviran Slot restartCalculation";
    //Slot za resetovanje sweepFreq i za startovanje racunanja
    model->setSweepFreq(model->getFStartMeasure());
    //Aj da pokusamo i ovo(ovo pravi probleme)
//    view->eraseBuffers();
    //i emitujemo signal startComputation();

    //Hmmm aj ovde da omogucimo kalkulaciju
    if (model->getCalculationGuard() == false) {
        model->setCalculationGuard(true);
    }

    //Ne pomaze
//    //Ako je ukljucen drawGuard iskljucicemo ga
//    if(view->getDrawGuard() == true) {
//        view->setDrawGuard(false);
//    }

    emit startComputation();
}

void MainWindow::putModelOnSleep()
{
//    workerThread.msleep(10);
    //Ne koristimo ovo
    workerThread.wait(100);
}

void MainWindow::processChangeAverageFactor()
{
    qreal tempVal = ui->lineEdit->text().toInt();
    //Pokrece promenu average faktora
    model->setAverageCyclesNumber(tempVal);
    //Ovaj nam je od presudne vaznosti za racun, pa
    // da ga ne bi blokirali tek ga setujemo kada ukljucujemo proracun
//    model->setAverageCycles(model->getAverageCyclesNumber());

    //Aj da probamo i info
    ui->actionAvg_Factor->setText(QString("Average Factor: %1").arg(model->getAverageCyclesNumber()));

    //Po upisu raskidamo vezu
    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processChangeAverageFactor);
    ui->lineEdit->clear();
    ui->lineEdit->hide();
}

void MainWindow::on_actionZ_parametri_triggered()
{

    ui->lineEdit->show();
    ui->lineEdit->setFocus();
//    ui->lineEdit->
//    ui->lineEdit->returnPressed();
//    connect(ui->lineEdit,&QLineEdit::returnPressed,ui->label,&QLabel::hide);
//    connect(ui->lineEdit,&QLineEdit::returnPressed,ui->lineEdit,&QLineEdit::hide);

    //Aj da ga isprobamo sa lambdom
    QString *bla = new QString("Ovo je na pocetku");
    ui->lineEdit->setText("Za sada upisi x range");
    ui->lineEdit->selectAll();
//    connect(ui->lineEdit,&QLineEdit::returnPressed,ui->lineEdit,[=](){
//
//        qDebug() << ui->lineEdit->text();
//        bla->append(ui->lineEdit->text());
//        qDebug() << "Ovo je iz Z akcije";
//        this->setToolTip(ui->lineEdit->text());
//        //Aj da pokusamo ovo...
//        qreal tempVal = ui->lineEdit->text().toInt();
//        view->setOurXAxisRange(tempVal);
//    });

    //Bolje je ipak ovako
//    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processSparam);

    connect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processZparam);
//    connect(ui->lineEdit,&QLineEdit::returnPressed,ui->lineEdit,&QLineEdit::clear);
    qDebug() << "Ok Z param akcija radi, a upisali smo " << *bla;
}

void MainWindow::on_actionS_parametri_triggered()
{
    ui->lineEdit->show();
//    ui->label->show();

    //Aj da dodamo fokus i da je selektovan text
    ui->lineEdit->setText("Upisi minimum y ose");
    ui->lineEdit->setFocus();
    ui->lineEdit->selectAll();

//    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processZparam);

    //Sada primenjujemo akciju u ovom kontekstu
    //dakle diskonektujemo sve i ostavljamo samo onaj koji nam treba...
    connect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processSparam);

    //Ovaj da kliruje nam uvek treba da je aktivan
//    disconnect(ui->lineEdit,&QLineEdit::returnPressed,ui->lineEdit,&QLineEdit::clear);


    //Aj da vidimo
    // izgleda da moramo ovako dinamicki da dodajemo
//    QLineEdit* tempLineEdit = new QLineEdit();
//    ui->verticalLayout->addWidget(tempLineEdit);
//    tempLineEdit->setFocus();
//    connect(tempLineEdit,&QLineEdit::returnPressed,this,&MainWindow::showLineEdit);
//    qDebug() << ui->verticalLayout->findChild<QLineEdit*>("lineEdit");
//    qDebug() << ui->verticalLayout;
//    connect(ui->verticalLayout->children().first(),&QLineEdit::returnPressed,this,&MainWindow::showLineEdit);
}

void MainWindow::on_actionS11_triggered()
{
    //Setuje S11 parametar
    model->setSelektovaniParametar(1);

    // i za setovanje da bude checked...
    resetMenuMeasurementsChecked();
    ui->actionS11->setChecked(true);
}

void MainWindow::on_actionS21_triggered()
{
    //Setuje S21 parametar
    model->setSelektovaniParametar(2);

    // i setovanje da bude checked
    resetMenuMeasurementsChecked();
    ui->actionS21->setChecked(true);
}

void MainWindow::on_actionS12_triggered()
{
    //Setuje S12 parametar
    model->setSelektovaniParametar(3);

    // i setovanje da bude checked
    resetMenuMeasurementsChecked();
    ui->actionS12->setChecked(true);
}

void MainWindow::on_actionS22_triggered()
{
    //Setuje S22 parametar
    model->setSelektovaniParametar(4);

    // i setovanje da bude checked
    resetMenuMeasurementsChecked();
    ui->actionS22->setChecked(true);
}

void MainWindow::on_actionStart_2_triggered()
{
    //Ova akcija pokrece menjanje startne ucestanosti
    ui->lineEdit->show();
//    ui->label->show();

    //Aj da dodamo fokus i da je selektovan text
    ui->lineEdit->setText("Upisi startnu frekvenciju");
    ui->lineEdit->setFocus();
    ui->lineEdit->selectAll();

//    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processZparam);

    //Sada primenjujemo akciju u ovom kontekstu
    //dakle diskonektujemo sve i ostavljamo samo onaj koji nam treba...
    connect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processStartSet);
}

void MainWindow::on_actionStop_triggered()
{
    //Ova akcija pokrece menjanje stop ucestanosti
    ui->lineEdit->show();
//    ui->label->show();

    //Aj da dodamo fokus i da je selektovan text
    ui->lineEdit->setText("Upisi stop frekvenciju");
    ui->lineEdit->setFocus();
    ui->lineEdit->selectAll();

//    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processZparam);

    //Sada primenjujemo akciju u ovom kontekstu
    //dakle diskonektujemo sve i ostavljamo samo onaj koji nam treba...
    connect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processStopSet);
}

void MainWindow::on_actionLog_Mag_triggered()
{
    //Posto je smitov dijagram u osnovi istog tipa
    // kao i ovaj nema potrebe da menjamo dijagram
    // vec samo da ga podesimo na staro stanje

    if (model->getSmithChartGuard() == true) {
        //Ovde ga vracamo suprotno akciji koja ga setuje
        //koristimo metodu u view objektu da ne bi nam pravio
        // probleme
        //TODO je implementirati do kraja(obrisati ovo kada se uradi)
        view->ourSmithToCartesianChart();
    }

    //Ovde podesavamo setujemo logmag
    model->resetAllFormatGuards();
    model->setLogMagGuard(true);

    //Cisto da istrazimo da li prazni
    // bafer
    view->clearViewSeriesBuffer();

    //Ovo samo testiramo..
//    view->switchChartType();
    //Hmm ovo bi trebalo da radi...
    //Isto treba uraditi ove provere za polarne akcije
    if (view->getChartType() == 2) {
//       //Prvo zaustavimo kalkulaciju
        model->setCalculationGuard(false);

        //Posto se razlikuju tipovi dijagrama potrebno je
        // da resetujemo merenje...
        QTimer timerToDelay;
        timerToDelay.singleShot(500,view,&View::eraseBuffers);


        //ako je razlicit od polarnog koji je 2 promeni
        view->switchChartType();

        // i restartujemo kalkulaciju
        timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);
    }

    //I naravno da setujemo checked
    resetMenuFormatChecked();
    ui->actionLog_Mag->setChecked(true);
}

void MainWindow::on_actionPhase_triggered()
{
    //Dodajemo proveru za smitov dijagram
    if (model->getSmithChartGuard() == true) {
        //Ako jeste promeni na kartezijan chart
        view->ourSmithToCartesianChart();
    }

    //setuje da prikaze fazu
    model->resetAllFormatGuards();
    model->setPhaseGuard(true);

    //i da setujemo checked
    resetMenuFormatChecked();
    ui->actionPhase->setChecked(true);
}

void MainWindow::on_actionLin_Mag_triggered()
{
    if (model->getSmithChartGuard() == true) {
        //Ako jeste promeni na kartezijan chart
        view->ourSmithToCartesianChart();
    }
    //setuje da prikaze linearnu magnitudu
    // hmmm nesto i ne lici proveriti proracun za ovo
    model->resetAllFormatGuards();
    model->setLinMagGuard(true);

    //i naravno da setujemo checked
    resetMenuFormatChecked();
    ui->actionLin_Mag->setChecked(true);
}

void MainWindow::on_actionSWR_triggered()
{
    //setuje da prikaze SWR
    // mada u pogledu na visim frekvencijama
    // pokazuje neke pikove, sada da li su to
    // nedostajuce tacke ili ne, videcemo, mada
    // vise me asocira na vecu vrednost suma
    if (model->getSmithChartGuard() == true) {
        //Ako jeste promeni na kartezijan chart
        view->ourSmithToCartesianChart();
    }
    model->resetAllFormatGuards();
    model->setSwrGuard(true);

    //i naravno da setujemo checked
    resetMenuFormatChecked();
    ui->actionSWR->setChecked(true);
}

void MainWindow::on_actionReal_triggered()
{
    if (model->getSmithChartGuard() == true) {
        //Ako jeste promeni na kartezijan chart
        view->ourSmithToCartesianChart();
    }
    //setuje prikaz realnog dela
    model->resetAllFormatGuards();
    model->setRealGuard(true);

    //i naravno da setujemo checked
    resetMenuFormatChecked();
    ui->actionReal->setChecked(true);
}

void MainWindow::on_actionImaginary_triggered()
{
    if (model->getSmithChartGuard() == true) {
        //Ako jeste promeni na kartezijan chart
        view->ourSmithToCartesianChart();
    }
    //setuje prikaz imagirarnog dela
    model->resetAllFormatGuards();
    model->setImagGuard(true);

    //i naravno da setujemo checked
    resetMenuFormatChecked();
    ui->actionImaginary->setChecked(true);
}

void MainWindow::on_actionAutoScale_triggered()
{
    //Podesava Y osu na osnovu procitanih min i max vrednosti u modelu
    qreal ourMin = model->getMinPoint();
    qreal ourMax =  model->getMaxPoint();
//    view->setOurYMinAxisRange(model->getMinPoint());
//    view->setOurYMaxRange(model->getMaxPoint());
    //kako dokumentacija kaze, moze da pravi problema
   // ako je minimum veci od maksimuma onda pravi problem
    // u redosledu, ukoliko bude pucao videti preko poredjenja
    // sta prvo setovati
    view->setOurYMinAxisRange(ourMin);
    view->setOurYMaxRange(ourMax);
}

void MainWindow::on_actionPower_2_triggered()
{
    //Pokrece akciju da se u lineEditu upise snaga izvora
    // u milivatima

    ui->lineEdit->show();
//    ui->label->show();

    //Aj da dodamo fokus i da je selektovan text
    ui->lineEdit->setText("Upisi novu snagu u milivatima");
    ui->lineEdit->setFocus();
    ui->lineEdit->selectAll();

//    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processZparam);

    //Sada primenjujemo akciju u ovom kontekstu
    //dakle konektujemo sa dogadjajem kada je enter pritisnut
    connect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processChangeGenP);
}

void MainWindow::on_action_5_to_10_triggered()
{
    //Ova akcija trenutno podesava snagu izvora na najnizu
    // da ne bi smo bezveze upisivali vrednosti ima da ih racuna
    // Snaga generatora je u ovom slucaju Pizvor = 1mW * 10exp(-5/10);
    // Vec je normalizovan za mW (miliwat)

    qreal tempVal = pow(10.0,-0.5);

    //setujemo snagu u modelu
    model->setGenP(tempVal);

    //i da setujemo checked
    resetMenuPowerRanges();
    ui->action_5_to_10->setChecked(true);
}

void MainWindow::on_action_15_to_0_triggered()
{
    //Ova akcija trenutno podesava snagu izvora na najnizu
    // da ne bi smo bezveze upisivali vrednosti ima da ih racuna
    // Snaga generatora je u ovom slucaju Pizvor = 1mW * 10exp(-15/10);
    // Vec je normalizovan za mW (miliwat)
    qreal tempVal = pow(10.0,-1.5);

    //setujemo snagu u modelu
    model->setGenP(tempVal);

    //i da setujemo checked
    resetMenuPowerRanges();
    ui->action_15_to_0->setChecked(true);

}

void MainWindow::on_action_25_to_10_triggered()
{
    //Ova akcija trenutno podesava snagu izvora na najnizu
    // da ne bi smo bezveze upisivali vrednosti ima da ih racuna
    // Snaga generatora je u ovom slucaju Pizvor = 1mW * 10exp(-25/10);
    // Vec je normalizovan za mW (miliwat)
    qreal tempVal = pow(10.0,-2.5);

    //setujemo snagu u modelu
    model->setGenP(tempVal);

    //i da setujemo checked
    resetMenuPowerRanges();
    ui->action_25_to_10->setChecked(true);

}

void MainWindow::on_action_35_to_20_triggered()
{
    //Ova akcija trenutno podesava snagu izvora na najnizu
    // da ne bi smo bezveze upisivali vrednosti ima da ih racuna
    // Snaga generatora je u ovom slucaju Pizvor = 1mW * 10exp(-35/10);
    // Vec je normalizovan za mW (miliwat)
    qreal tempVal = pow(10.0,-3.5);

    //setujemo snagu u modelu
    model->setGenP(tempVal);

    //i da setujemo checked
    resetMenuPowerRanges();
    ui->action_35_to_20->setChecked(true);

}

void MainWindow::on_action_45_to_30_triggered()
{
    //Ova akcija trenutno podesava snagu izvora na najnizu
    // da ne bi smo bezveze upisivali vrednosti ima da ih racuna
    // Snaga generatora je u ovom slucaju Pizvor = 1mW * 10exp(-45/10);
    // Vec je normalizovan za mW (miliwat)
    qreal tempVal = pow(10.0,-4.5);

    //setujemo snagu u modelu
    model->setGenP(tempVal);

    //I da setujemo checked
    resetMenuPowerRanges();
    ui->action_45_to_30->setChecked(true);
}

void MainWindow::on_actionPoints_triggered()
{
    //Ova akcija nam sluzi da pokrene
    // promenu broja tacaka koji ce se prikazivati
    ui->lineEdit->show();
//    ui->label->show();

    //Aj da dodamo fokus i da je selektovan text
    ui->lineEdit->setText("Upisi novi broj tacaka");
    ui->lineEdit->setFocus();
    ui->lineEdit->selectAll();

//    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processZparam);

    //Sada primenjujemo akciju u ovom kontekstu
    //dakle konektujemo sa dogadjajem kada je enter pritisnut
    connect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processChangeNumOfPoints);
}

void MainWindow::on_actionLin_Phase_triggered()
{
    //Setuje prikaz (phase mag) tacaka
    // Ovo cemo morati posebno da istrazimo

    model->resetAllFormatGuards();
    model->setLinPhaseGuard(true);
//    view->initializeSeriesData();
    //I trebalo bi da updatujemo view
    if (view->getChartType() != 2) {

        //Prvo zaustavimo kalkulaciju
        model->setCalculationGuard(false);

        //Posto se razlikuju tipovi dijagrama potrebno je
        // da resetujemo merenje...
        QTimer timerToDelay;
        timerToDelay.singleShot(500,view,&View::eraseBuffers);


        //ako je razlicit od polarnog koji je 2 promeni
        view->switchChartType();

        // i restartujemo kalkulaciju
        timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);
    }
    //view->switchChartType();

    //I naravno da setujemo checked
    resetMenuFormatChecked();
    ui->actionLin_Phase->setChecked(true);
}

void MainWindow::on_actionLog_Phase_triggered()
{
    //Setuje prikaz (phase Log(mag) tacaka
    model->resetAllFormatGuards();
    model->setLogPhaseGuard(true);

    if (view->getChartType() != 2) {
        //Prvo zaustavimo kalkulaciju
        model->setCalculationGuard(false);

        //Posto se razlikuju tipovi dijagrama potrebno je
        // da resetujemo merenje...
        QTimer timerToDelay;
        timerToDelay.singleShot(500,view,&View::eraseBuffers);


        //ako je razlicit od polarnog koji je 2 promeni
        view->switchChartType();

        // i restartujemo kalkulaciju
        timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);
    }

    //I naravno da setujemo checked
    resetMenuFormatChecked();
    ui->actionLog_Phase->setChecked(true);
}

void MainWindow::on_actionReal_Imag_triggered()
{
    //Setuje prikaz (real, imag) na polarnom chartu
    // medjutim na kraju se svodi na istu stvar kao i (lin phase,mag)
    model->resetAllFormatGuards();
    model->setLinPhaseGuard(true);
    if (view->getChartType() != 2) {
        //Prvo zaustavimo kalkulaciju
        model->setCalculationGuard(false);

        //Posto se razlikuju tipovi dijagrama potrebno je
        // da resetujemo merenje...
        QTimer timerToDelay;
        timerToDelay.singleShot(500,view,&View::eraseBuffers);


        //ako je razlicit od polarnog koji je 2 promeni
        view->switchChartType();

        // i restartujemo kalkulaciju
        timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);
    }

    //I naravno da setujemo checked
    resetMenuFormatChecked();
    ui->actionReal_Imag->setChecked(true);
}

void MainWindow::on_actionR_jX_triggered()
{
    //Setuje prikaz (R,jX) na polarnom chartu
    // isto se svodi na (lin phase,mag)
    //jedina razlika sto prikazuje na tooltipovima drugacije vrednosti
    model->resetAllFormatGuards();
    model->setZParamGuard(true);

    if (view->getChartType() != 2) {
        //Prvo zaustavimo kalkulaciju
        model->setCalculationGuard(false);

        //Posto se razlikuju tipovi dijagrama potrebno je
        // da resetujemo merenje...
        QTimer timerToDelay;
        timerToDelay.singleShot(500,view,&View::eraseBuffers);


        //ako je razlicit od polarnog koji je 2 promeni
        view->switchChartType();

        // i restartujemo kalkulaciju
        timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);
    }

    //i naravno da setujemo checked
    resetMenuFormatChecked();
    ui->actionR_jX->setChecked(true);
}

void MainWindow::on_actionG_jB_triggered()
{
    //Setuje prikaz
    model->resetAllFormatGuards();
    model->setYParamGuard(true);

    if (view->getChartType() != 2) {
        //Prvo zaustavimo kalkulaciju
        model->setCalculationGuard(false);

        //Posto se razlikuju tipovi dijagrama potrebno je
        // da resetujemo merenje...
        QTimer timerToDelay;
        timerToDelay.singleShot(500,view,&View::eraseBuffers);


        //ako je razlicit od polarnog koji je 2 promeni
        view->switchChartType();

        // i restartujemo kalkulaciju
        timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);
    }

    //i naravno da setujemo checked
    resetMenuFormatChecked();
    ui->actionG_jB->setChecked(true);
}

void MainWindow::on_actionLin_Phase_2_triggered()
{
    //Setuje prikaz za smitov chart
    // Posto koristi deo koji smo racunali za fazu ima dva guarda
    // setsmithchart i mora da pusti linphase guard na true.
    // ako je linphase guard true a smithchart false ima da daje parove
    // (lin phase , mag). Ako su oba true vraca (real imag) parove
    model->resetAllFormatGuards();

    //Ovde podesavamo ovu kombinaciju
    model->setLinPhaseGuard(true);
    model->setSmithChartGuard(true);

    //Ako je drugaciji chart type od cartezijan
    // promeni ga (dakle ovo je promena iz polarnog u smitov)
    // Ova promena mora da dodje pre promene
    // iz dekartovog u smitov chart da bi znao da ne mora
    // prilikom switcha da brise smith grid
    if (view->getChartType() == 2) {
        //Prvo zaustavimo kalkulaciju
        model->setCalculationGuard(false);

        //Posto se razlikuju tipovi dijagrama potrebno je
        // da resetujemo merenje...
        QTimer timerToDelay;
        timerToDelay.singleShot(500,view,&View::eraseBuffers);


        //ako je razlicit od polarnog koji je 2 promeni
        view->switchChartType();

        // i restartujemo kalkulaciju
        timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);
    }


    //TODO kasnije cemo dodati podesavanja
    // za mogucu izmenu tipa grafika

    //Ako je vec u pitanju smitov chart
    // ovo ne moramo da radimo, ako jeste tj true
    // ovo se ne radi
    if (view->getIsSmithChart() == false) {

        view->ourCartesianToSmithChart();
        //U smitu ne prikazujemo grid line
        //    view->setGridVisible(false);


        //Aj da probamo drugu funkciju
        //    view->drawSmithGrid();
        //    view->drawSmithGrid2();

        //Aj da probamo trecu verziju funkcije

        view->drawSmithGrid3();
    }
    //i da setujemo checked
    resetMenuFormatChecked();
    ui->actionLin_Phase_2->setChecked(true);
}

void MainWindow::on_actionLog_Phase_2_triggered()
{
    //Setuje prikaz za smitov dijagram
    // pogledati on_actionLin_Phase_2_triggered();
    model->resetAllFormatGuards();

    model->setLogPhaseGuard(true);
    model->setSmithChartGuard(true);

    //Ako je drugaciji chart type od cartezijan
    // promeni ga (dakle ovo je promena iz polarnog u smitov)
    // Ova promena mora da dodje pre promene
    // iz dekartovog u smitov chart da bi znao da ne mora
    // prilikom switcha da brise smith grid
    if (view->getChartType() == 2) {
        //Prvo zaustavimo kalkulaciju
        model->setCalculationGuard(false);

        //Posto se razlikuju tipovi dijagrama potrebno je
        // da resetujemo merenje...
        QTimer timerToDelay;
        timerToDelay.singleShot(500,view,&View::eraseBuffers);


        //ako je razlicit od polarnog koji je 2 promeni
        view->switchChartType();

        // i restartujemo kalkulaciju
        timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);
    }

    //Prebacujemo u smitov dijagram pogled
    //Ako nije smitov chart prebaci, ako jeste
    // onda preskoci ovaj korak
    if (view->getIsSmithChart() == false) {
        view->ourCartesianToSmithChart();

        //TODO kasnije cemo dodati podesavanja
        // za mogucu izmenu tipa grafika
        //    view->setOurXAxisMin(-5);
        //    view->setOurXAxisMax(1);
        //    view->setOurYAxisMin(-5);
        //    view->setOurYAxisMax(1);

        //U smitu ne prikazujemo grid line
        //    view->setGridVisible(false);
        //ovde crtamo smith grid
        view->drawSmithGrid3();
    }
    //i naravno da setujemo checked
    resetMenuFormatChecked();
    ui->actionLog_Phase_2->setChecked(true);
}

void MainWindow::on_actionReal_Imag_2_triggered()
{
    //TODO Ovaj tek treba da vidimo kako da uradimo
    // nedostaje u na modelu i guardovi za ovo i deo koji obradjuje ovo
}

void MainWindow::on_actionR_jX_2_triggered()
{
    //Setuje prikaz za smitov dijagram
    // pogledati on_actionLin_Phase_2_triggered();
    model->resetAllFormatGuards();
    model->setZParamGuard(true);
    model->setSmithChartGuard(true);

    //Ako je drugaciji chart type od cartezijan
    // promeni ga (dakle ovo je promena iz polarnog u smitov)
    // Ova promena mora da dodje pre promene
    // iz dekartovog u smitov chart da bi znao da ne mora
    // prilikom switcha da brise smith grid
    if (view->getChartType() == 2) {
        //Prvo zaustavimo kalkulaciju
        model->setCalculationGuard(false);

        //Posto se razlikuju tipovi dijagrama potrebno je
        // da resetujemo merenje...
        QTimer timerToDelay;
        timerToDelay.singleShot(500,view,&View::eraseBuffers);


        //ako je razlicit od polarnog koji je 2 promeni
        view->switchChartType();

        // i restartujemo kalkulaciju
        timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);
    }

    //Prebacujemo u smitov dijagram pogled
    //Ako nije smitov chart prebaci, ako jeste
    if (view->getIsSmithChart() == false) {


        view->ourCartesianToSmithChart();
        //TODO kasnije cemo dodati podesavanja
        // za mogucu izmenu tipa grafika
        //    view->setOurXAxisMin(0);
        //    view->setOurXAxisMax(200);
        //    view->setOurYAxisMin(-100);
        //    view->setOurYAxisMax(100);

        //U smitu ne prikazujemo grid line
        //    view->setGridVisible(false);

        //Ovde crtamo smith grid
        view->drawSmithGrid3();
    }
    //I naravno da setujemo checked
    resetMenuFormatChecked();
    ui->actionR_jX_2->setChecked(true);

}

void MainWindow::on_actionG_jB_2_triggered()
{
    //Setuje prikaz za smitov dijagram
    // pogledati on_actionLin_Phase_2_triggered();
    model->resetAllFormatGuards();
    model->setZParamGuard(true);
    model->setSmithChartGuard(true);

    //Ako je drugaciji chart type od cartezijan
    // promeni ga (dakle ovo je promena iz polarnog u smitov)
    // Ova promena mora da dodje pre promene
    // iz dekartovog u smitov chart da bi znao da ne mora
    // prilikom switcha da brise smith grid
    if (view->getChartType() == 2) {
        //Prvo zaustavimo kalkulaciju
        model->setCalculationGuard(false);

        //Posto se razlikuju tipovi dijagrama potrebno je
        // da resetujemo merenje...
        QTimer timerToDelay;
        timerToDelay.singleShot(500,view,&View::eraseBuffers);


        //ako je razlicit od polarnog koji je 2 promeni
        view->switchChartType();

        // i restartujemo kalkulaciju
        timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);
    }

    //Prebacujemo u smitov dijagram pogled
    //Ako nije smitov chart prebaci, ako jeste
    if (view->getIsSmithChart() == false) {

        view->ourCartesianToSmithChart();
        //TODO kasnije cemo dodati podesavanja
        // za mogucu izmenu tipa grafika
        //    view->setOurXAxisMin(0);
        //    view->setOurXAxisMax(200);
        //    view->setOurYAxisMin(-100);
        //    view->setOurYAxisMax(100);

        //U smitu ne prikazujemo grid line
        //    view->setGridVisible(false);

        //Ovde crtamo smith grid
        view->drawSmithGrid3();
    }
    //i naravno da setujemo checked
    resetMenuFormatChecked();
    ui->actionG_jB_2->setChecked(true);
}

void MainWindow::on_actionAveraging_Restart_triggered()
{
    //Restartuje Average
    // dakle ocisti average bafer i postavi vrednosti
    // na pocetne
    model->setCalculationGuard(false);
    model->setAverageCycles(model->getAverageCyclesNumber());
    model->setAveragePointPos(0);

    //I da ispraznimo average bafer
    model->clearAverageBuffer();

    //I treba bi da restartuje merenje
    //slot restartCalc ga ukljucuje ponovo



    //I ovde treba restart kalkulacije
    QTimer timerToDelay;
    timerToDelay.singleShot(500,view,&View::eraseBuffers);

    //ovde da ukljucimo ponovo
    timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);
}

void MainWindow::on_actionAvg_Factor_triggered()
{
    //Podesava broj ciklusa average
    // i startuje average(usrednjavanje)

    //prvo pokrece operaciju upisa zeljenog broja
    // average ciklusa
    //Ova akcija nam sluzi da pokrene
    // promenu broja tacaka koji ce se prikazivati
    ui->lineEdit->show();
//    ui->label->show();

    //Aj da dodamo fokus i da je selektovan text
    ui->lineEdit->setText("Upisi average Faktor");
    ui->lineEdit->setFocus();
    ui->lineEdit->selectAll();

//    disconnect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processZparam);

    //Sada primenjujemo akciju u ovom kontekstu
    //dakle konektujemo sa dogadjajem kada je enter pritisnut
    connect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::processChangeAverageFactor);
}

void MainWindow::on_actionIF_Bandwidth_triggered()
{
    // podesava IF bandwidth
    //pokrece operaciju zeljenog broja u khz
}


void MainWindow::on_actionCal_triggered()
{
    //Ovako nece modi da ide
    //Ova akcija pokrece ucitavanje Cal merenja
    // i prikaz na view-u istog prebacivanjem
    // ucitanog fajla u view
//    model->loadCalDut();
//    view->drawCallDut(model->getCalData());
}

void MainWindow::on_pushButton_clicked()
{
    //Ovde implementiramo kliktanje na cal dugme
    // kojim ukljucujemo kalibraciju
    if (ui->pushButton->isChecked()) {
        model->setCalGuard(false);
    }else {
        model->setCalGuard(true);
    }
}

void MainWindow::on_actionAveraging_triggered()
{
    //Pritiskom na ovo dugme ukljucuje i iskljucuje
    // usrednjavanje
    if (model->getAverageGuard() == true) {

        //slot restartCalc ga ukljucuje ponovo
        model->setCalculationGuard(false);

        //Dakle ukoliko je upaljen ugasi ga
        model->setAverageGuard(false);
        ui->actionAveraging->setChecked(false);

        //Da bi ponovo racunao moramo averageCyCles da bude 0
        model->setAverageCycles(0);
        //Takodje moramo da obrisemo bafer zbog sledeceg averagea
        model->clearAverageBuffer();



        //I ovde treba restart kalkulacije
        QTimer timerToDelay;
        timerToDelay.singleShot(500,view,&View::eraseBuffers);

        //ovde da ukljucimo ponovo
        timerToDelay.singleShot(1000,this,&MainWindow::restartCalculation);

    }else {

        //medjutim svejedno moramo da blokiramo proracun dok
        // ne bude sve spremno
        model->setCalculationGuard(false);
        model->setAverageCycles(model->getAverageCyclesNumber());


        model->setAverageGuard(true);
        ui->actionAveraging->setChecked(true);




        //Ovde bi trebao da ukljuci average pa cemo
        // morati da resetujemo sve kao u start i stop setu..
        QTimer timerToDelay2;
        timerToDelay2.singleShot(500,view,&View::eraseBuffers);

        //Ovde ga ukljucujemo ponovo
        timerToDelay2.singleShot(1000,this,&MainWindow::restartCalculation);
    }
}

void MainWindow::on_actionIFBW_30khz_triggered()
{
    //celokupan opseg maksimalna brzina
    model->setIFBWtime(0);
    model->setIfbwFreqModel(30);

    //i naravno checked
    resetMenuIfBand();
    ui->actionIFBW_30khz->setChecked(true);
}

void MainWindow::on_actionIFBW_10kHz_triggered()
{
    //dodaje zadrsku jedne sekunde
    model->setIFBWtime(1);
//    model->setIfbwFreqModel(10);
    model->setIfbwFreqModel(25);

    //i naravno checked
    resetMenuIfBand();
    ui->actionIFBW_10kHz->setChecked(true);
}

void MainWindow::on_actionIFBW_1kHz_triggered()
{
    //zadrska od tri milisekunde
    model->setIFBWtime(3);
//    model->setIfbwFreqModel(1);
    model->setIfbwFreqModel(20);

    //i naravno checked
    resetMenuIfBand();
    ui->actionIFBW_1kHz->setChecked(true);
}

void MainWindow::on_actionIFBW_100Hz_triggered()
{
    //zadrska od 5 milisekundi
    model->setIFBWtime(5);
//    model->setIfbwFreqModel(0.1);
    model->setIfbwFreqModel(10);

    //i naravno checked
    resetMenuIfBand();
    ui->actionIFBW_100Hz->setChecked(true);
}

void MainWindow::on_actionIFBW_10Hz_triggered()
{
    //zadrska od 7 milisekundi po odbirku
    model->setIFBWtime(7);
//    model->setIfbwFreqModel(0.01);
    model->setIfbwFreqModel(5);

    //i naravno checked
    resetMenuIfBand();
    ui->actionIFBW_10Hz->setChecked(true);
}

void MainWindow::resetMenuMeasurementsChecked()
{
    //Resetuje checked u menu-u measurements
    // Kada sve resetuje onda postavlja true na odredjeni...
    ui->actionS11->setChecked(false);
    ui->actionS21->setChecked(false);
    ui->actionS12->setChecked(false);
    ui->actionS22->setChecked(false);
}

void MainWindow::resetMenuFormatChecked()
{
    //Resetuje u format menu-u akcije za checked
    ui->actionLog_Mag->setChecked(false);
    ui->actionPhase->setChecked(false);
    ui->actionLin_Phase_2->setChecked(false);
    ui->actionLog_Phase_2->setChecked(false);
    ui->actionReal_Imag_2->setChecked(false);
    ui->actionR_jX_2->setChecked(false);
    ui->actionG_jB_2->setChecked(false);

    ui->actionLin_Phase->setChecked(false);
    ui->actionLog_Phase->setChecked(false);
    ui->actionReal_Imag->setChecked(false);
    ui->actionR_jX->setChecked(false);
    ui->actionG_jB->setChecked(false);

    ui->actionLin_Mag->setChecked(false);
    ui->actionSWR->setChecked(false);
    ui->actionReal->setChecked(false);
    ui->actionImaginary->setChecked(false);
}

void MainWindow::resetMenuPowerRanges()
{
    //Resetuje checked u powerRanges meniju
    ui->action_5_to_10->setChecked(false);
    ui->action_15_to_0->setChecked(false);
    ui->action_25_to_10->setChecked(false);
    ui->action_35_to_20->setChecked(false);
    ui->action_45_to_30->setChecked(false);
}

void MainWindow::resetMenuIfBand()
{
    //Resetuje if Band Menu
    ui->actionIFBW_30khz->setChecked(false);
    ui->actionIFBW_10kHz->setChecked(false);
    ui->actionIFBW_1kHz->setChecked(false);
    ui->actionIFBW_100Hz->setChecked(false);
    ui->actionIFBW_10Hz->setChecked(false);
}

void MainWindow::on_pushButton_2_clicked()
{
    //kada kliknemo dugme za menjanje dut objekta...
}
