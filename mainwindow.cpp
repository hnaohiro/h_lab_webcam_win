#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>

#include "dialogconfig.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new QSettings("setting.ini", QSettings::IniFormat, this);
    graphicsScene = new QGraphicsScene(this);

    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(connectSocket()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(disconnectSocket()));

    wsSocket = new QWsSocket(this);
    connect(wsSocket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(wsSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(wsSocket, SIGNAL(frameReceived(QString)), this, SLOT(messageRecieved(QString)));
    connect(wsSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
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

void MainWindow::connectSocket()
{
    QString address = settings->value("address").toString();
    if (address.isEmpty()) {
        QMessageBox::critical(this, NULL, "You must set the address!");
        return;
    }

    QString port = settings->value("port").toString();
    if (port.isEmpty()) {
        QMessageBox::critical(this, NULL, "You must set the port!");
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

    QPixmap pixmap = QPixmap::fromImage(image.scaled(ui->graphicsView->frameSize()));
    graphicsScene->clear();
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
