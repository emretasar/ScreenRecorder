#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QScreen>
#include <QPixmap>
#include <QFile>
#include <QProcess>
#include <QDateTime>
#include <QLabel>

#include "uicontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startStopRecording();
    void selectOutputDirectory();
    void captureFrame();

private:
    Ui::MainWindow *ui;
    UiController* uiController;
    QTimer *timer;
    QFile *outputFile;
    QProcess *ffmpegProcess;
    QString outputFilePath;
    QString tempFilePath;

    bool isRecording;
};

#endif // MAINWINDOW_H
