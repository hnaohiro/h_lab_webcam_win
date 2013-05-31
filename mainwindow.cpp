#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QKeyEvent>
#include <QDesktopWidget>

#include "dialogconfig.h"
#include "popup.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new QSettings("setting.ini", QSettings::IniFormat, this);
    graphicsScene = new QGraphicsScene;

    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(connectSocket()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(disconnectSocket()));

    wsSocket = new QWsSocket(this);
    connect(wsSocket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(wsSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(wsSocket, SIGNAL(frameReceived(QString)), this, SLOT(messageRecieved(QString)));
    connect(wsSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));

    connectSocket();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConfig_triggered()
{
    DialogConfig dialog(this);
    dialog.exec();
}


void MainWindow::on_actionCapture_triggered()
{
    if (!prevImage.isNull()) {
        QImage image = prevImage.copy();
        QString fileName = QFileDialog::getSaveFileName(this, "Save image", "", "png file (*.png)");
        image.save(fileName);
    }
}

void MainWindow::on_actionFull_triggered()
{
    ui->mainToolBar->setVisible(false);
    ui->statusBar->setVisible(false);
    ui->centralWidget->layout()->setMargin(0);

    int current = QApplication::desktop()->screenNumber(this);
    QRect screenres = QApplication::desktop()->screenGeometry(current);
    this->move(QPoint(screenres.x(), screenres.y()));
    this->showFullScreen();

    Popup *popup = new Popup();
    popup->show();
}

void MainWindow::connectSocket()
{
    QString address = settings->value("address").toString();
    if (address.isEmpty()) {
        ui->statusBar->showMessage("Please set address!");
        return;
    }

    QString port = settings->value("port").toString();
    if (port.isEmpty()) {
        ui->statusBar->showMessage("Please set the port number!");
        return;
    }

    wsSocket->connectToHost(address, port.toInt());
}

void MainWindow::disconnectSocket()
{
    wsSocket->disconnectFromHost();
}

void MainWindow::socketConnected()
{
    ui->actionConnect->setEnabled(false);
    ui->actionDisconnect->setEnabled(true);
}

void MainWindow::socketDisconnected()
{
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
}

void MainWindow::messageRecieved(const QString &message)
{
    QImage image = QImage::fromData(QByteArray::fromBase64(message.toStdString().c_str()), "jpg");
    if (image.isNull()) {
         return;
    }
    prevImage = image.copy();

    QSize size = ui->graphicsView->frameSize();
    QPixmap pixmap = QPixmap::fromImage(image.scaled(size.width()-2, size.height()-2));

    delete graphicsScene;
    graphicsScene = new QGraphicsScene;
    graphicsScene->addPixmap(pixmap);

    ui->graphicsView->setScene(graphicsScene);
}

void MainWindow::socketStateChanged(const QAbstractSocket::SocketState &socketState)
{
    switch (socketState)
    {
        case QAbstractSocket::UnconnectedState:
            ui->statusBar->showMessage("Unconnected", 2000);
            break;
        case QAbstractSocket::HostLookupState:
            ui->statusBar->showMessage("HostLookup", 2000);
            break;
        case QAbstractSocket::ConnectingState:
            ui->statusBar->showMessage("Connecting", 2000);
            break;
        case QAbstractSocket::ConnectedState:
            ui->statusBar->showMessage("Connected", 2000);
            break;
        case QAbstractSocket::BoundState:
            ui->statusBar->showMessage("Bound", 2000);
            break;
        case QAbstractSocket::ClosingState:
            ui->statusBar->showMessage("Closing", 2000);
            break;
        case QAbstractSocket::ListeningState:
            ui->statusBar->showMessage("Listening", 2000);
            break;
        default:
            // unknown
            break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (this->isFullScreen() && event->key() == Qt::Key_Escape) {
        ui->mainToolBar->setVisible(true);
        ui->statusBar->setVisible(true);
        ui->centralWidget->layout()->setMargin(12);
        this->showNormal();
    }
}
