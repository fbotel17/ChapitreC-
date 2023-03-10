#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"
#include <qtcpsocket.h>

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget* parent = Q_NULLPTR);

private:
    Ui::QtWidgetsApplication1Class ui;
    QTcpSocket* socket;

public slots:
    void onDisplayMessageButtonClicket();
    void onSocketConnected();
    void onSocketDisconnected();
    void onSendMessageButonClicket();
};
