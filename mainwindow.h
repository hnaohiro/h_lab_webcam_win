#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QGraphicsScene>

#include "QWsSocket.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionConfig_triggered();
    void on_actionCapture_triggered();
    void on_actionFull_triggered();
    void connectSocket();
    void disconnectSocket();
    void socketConnected();
    void socketDisconnected();
    void messageRecieved(const QString &message);
    void socketStateChanged(const QAbstractSocket::SocketState &socketState);

    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    QSettings *settings;
    QWsSocket *wsSocket;
    QGraphicsScene *graphicsScene;
    QImage prevImage;
};

#endif // MAINWINDOW_H
