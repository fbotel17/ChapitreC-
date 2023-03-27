#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"
#include <qtcpsocket.h>
#include <qtcpserver.h>

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT
        QString id;

public:
    QtWidgetsApplication1(QWidget* parent = Q_NULLPTR);

private:
    Ui::QtWidgetsApplication1Class ui;
    QTcpSocket* socket;
    QTcpServer* server;

public slots:
    void onDisplayMessageButtonClicket();
    void onSocketConnected();
    void onSocketDisconnected();
    void onSendMessageButonClicket();
    void onSocketReadyRead();

    void onServerNewConnection();
    void onClientDisconnected();
    void onClientReadyRead();
    void inscriptionButtom();
    void backConnectButtom();
    void inscriptionButtom2();
    void envoyerButtom();
    void pushButton();
};
