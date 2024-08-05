#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QBuffer>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
    , ffmpegProcess(new QProcess(this))
    , isRecording(false)
{
    ui->setupUi(this);
    uiController->setButtonFeatures(ui->recordButton);

    // Connect buttons
    connect(ui->recordButton, &QPushButton::clicked, this, &MainWindow::startStopRecording);
    connect(ui->selectDirButton, &QPushButton::clicked, this, &MainWindow::selectOutputDirectory);
    // Timer setup
    connect(timer, &QTimer::timeout, this, &MainWindow::captureFrame);
    timer->setInterval(1000 / 30);      // 30 FPS

    outputFilePath = "output.mp4";
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::startStopRecording()
{
    if (isRecording) {
        isRecording = false;
        timer->stop();
        ffmpegProcess->closeWriteChannel();
        if (ffmpegProcess->waitForFinished()) {
            qDebug() << "Recording saved to:" << outputFilePath;
        } else {
            qDebug() << "FFmpeg process did not finish correctly:" << ffmpegProcess->errorString();
        }

        ui->recordButton->setText("Start Record");
        ui->frameLabel->clear();
        qDebug() << "FFmpeg output:" << ffmpegProcess->readAllStandardError();
    } else {
        isRecording = true;
        QStringList arguments;
        arguments << "-y" << "-f" << "image2pipe" << "-vcodec" << "mjpeg"
                  << "-r" << "30" << "-i" << "-" << "-c:v" << "libx264"
                  << "-preset" << "ultrafast" << "-crf" << "18" << outputFilePath;

        ffmpegProcess->start("D:\\Setup\\ffmpeg\\ffmpeg-2024-08-01-git-bcf08c1171-full_build\\bin\\ffmpeg", arguments);
        if (!ffmpegProcess->waitForStarted()) {
            qDebug() << "Failed to start FFmpeg:" << ffmpegProcess->errorString();
            return;
        }
        ui->recordButton->setText("Stop Record");
        timer->start();
    }
}

void MainWindow::selectOutputDirectory()
{
    QString saveDirectory = QFileDialog::getExistingDirectory(
        this,
        tr("Select Directory"),
        QString(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    if (!saveDirectory.isEmpty()) {
        outputFilePath = saveDirectory + "/output.mp4";
        ui->dirLineEdit->setText(outputFilePath);
    } else {
        QMessageBox::warning(this, tr("Warning"), tr("No directory selected."));
    }
}

void MainWindow::captureFrame() {
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen)
        return;

    QPixmap pixmap = screen->grabWindow(0);
    ui->frameLabel->setPixmap(pixmap.scaledToHeight(360));
    ui->frameLabel->setAlignment(Qt::AlignCenter);
    QImage img = pixmap.toImage();
    img = img.convertToFormat(QImage::Format_RGB888);

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "JPG");

    ffmpegProcess->write(ba.data(), ba.size());
}
