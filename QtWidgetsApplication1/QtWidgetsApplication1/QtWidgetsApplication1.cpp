#include "QtWidgetsApplication1.h"

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    socket = new QTcpSocket(this);
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
}


void QtWidgetsApplication1::onDisplayMessageButtonClicket()
{
    QString ip = ui.IPLineEdit->text();
    QString port = ui.portLineEdit->text();

    bool ok;
    int portAsInt = port.toInt(&ok);
    if (ok)
    {
        socket->connectToHost(ip, portAsInt);
    }
}

void QtWidgetsApplication1::onSocketConnected()
{
    ui.connectionStatusLabel->setText("Status connexion : Connecté");
}

void QtWidgetsApplication1::onSocketDisconnected()
{
    ui.connectionStatusLabel->setText("Status connexion : Déconnecté");
}

void QtWidgetsApplication1::onSendMessageButonClicket()
{
    if (socket->state() == QTcpSocket::ConnectedState)
    {
        socket->write("Hello server !\n");
    }
}
