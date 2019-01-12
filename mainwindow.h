#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include <QLineEdit>


class NAmodel;
class View;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QThread workerThread;                   //Aj da probamo odavde da se ne zajebavamo


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


signals:
    void startComputation();                //Ovo nam treba da pocnemo sa racunanjem

public slots:
    void winTimer();
    void showLineEdit();
    void processZparam();                   //Ovaj slot procesira kada je pokrenuta on_actionZ_parametri_triggered();
    void processSparam();                   //Ovaj slot procesira kada je pokrenuta on actionS_parametri_triggered();

    //Ovde definisemo slotove vezane za akcije
    void processStartSet();                 //Za podesavanje start frekv pri on_actionStart_2_triggered();
    void processStopSet();                  //Za podesavanje stop frekv pri on_actionStop_triggered();

    //Ovaj slot nam treba za upis nove snage generatora
    void processChangeGenP();

    void processChangeNumOfPoints();

    //Ovaj slot se aktivara kada Na menja pogled
    void cancelNaModelViewConn();
    // treba ga naravno kada zapocne novi ciklus ponovo
    // aktivirati
    void enableNaModelViewConn();

    void timerRemainTime();

    //Treba nam slot koji ce nam sluziti za resetovanje
    // sweep frekvencije i startovanje perform calculation
    void restartCalculation();

    //Ovaj slot stavlja model na spavanje
    void putModelOnSleep();

    //Slot za promenu average faktora
    void processChangeAverageFactor();
private slots:
    void on_actionZ_parametri_triggered();

    void on_actionS_parametri_triggered();

    void on_actionS11_triggered();

    void on_actionS21_triggered();

    void on_actionS12_triggered();

    void on_actionS22_triggered();

    void on_actionStart_2_triggered();

    void on_actionStop_triggered();

    void on_actionLog_Mag_triggered();

    void on_actionPhase_triggered();

    void on_actionLin_Mag_triggered();

    void on_actionSWR_triggered();

    void on_actionReal_triggered();

    void on_actionImaginary_triggered();

    void on_actionAutoScale_triggered();

    void on_actionPower_2_triggered();

    void on_action_5_to_10_triggered();

    void on_action_15_to_0_triggered();

    void on_action_25_to_10_triggered();

    void on_action_35_to_20_triggered();

    void on_action_45_to_30_triggered();

    void on_actionPoints_triggered();

    void on_actionLin_Phase_triggered();

    void on_actionLog_Phase_triggered();

    void on_actionReal_Imag_triggered();

    void on_actionR_jX_triggered();

    void on_actionG_jB_triggered();

    void on_actionLin_Phase_2_triggered();

    void on_actionLog_Phase_2_triggered();

    void on_actionReal_Imag_2_triggered();

    void on_actionR_jX_2_triggered();

    void on_actionG_jB_2_triggered();

    void on_actionAveraging_Restart_triggered();

    void on_actionAvg_Factor_triggered();

    void on_actionIF_Bandwidth_triggered();

    void on_actionCal_triggered();

    void on_pushButton_clicked();

    void on_actionAveraging_triggered();

    void on_actionIFBW_30khz_triggered();

    void on_actionIFBW_10kHz_triggered();

    void on_actionIFBW_1kHz_triggered();

    void on_actionIFBW_100Hz_triggered();

    void on_actionIFBW_10Hz_triggered();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    NAmodel *model;
    View *view;

    //Ovaj nam treba za lineEdit
    QLineEdit* tempLineEdit = new QLineEdit();

    //Za dugmad checked nam trebaju funkcije za resetovanje
    //Prvo za menu Measurement
    void resetMenuMeasurementsChecked();
    //Pa za menuFormat
    void resetMenuFormatChecked();
    void resetMenuPowerRanges();
    void resetMenuIfBand();


};

#endif // MAINWINDOW_H
