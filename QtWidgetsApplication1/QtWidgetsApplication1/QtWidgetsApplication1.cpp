#include "QtWidgetsApplication1.h"
#include <QtGui>
#include <QDebug>
#include <QtSql/QtSql>
#include <iostream>

QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	socket = new QTcpSocket(this);
	QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
	QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));

	server = new QTcpServer(this);
	QObject::connect(server, SIGNAL(newConnection()), this, SLOT(onServerNewConnection()));
	server->listen(QHostAddress::AnyIPv4, 4321);




	QApplication app();

	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); // ou mettre QSQLITE pour SQLite

	db.setHostName("192.168.64.95");
	db.setUserName("root");
	db.setPassword("root");
	db.setDatabaseName("initiationC"); // ou mettre le nom du fichier sqlite

	if (db.open())
	{
		std::cout << "Connexion r�ussie � " << db.hostName().toStdString() << std::endl;
	}
	else
	{
		std::cout << "La connexion a �chou�e !" << std::endl;
	}

	// Exemple 1 :
	QSqlQuery query("SELECT temperature FROM mesures");
	QString test;
	while (query.next())
	{
		QString temperature = query.value(0).toString();
		qDebug() << "temperature : " << temperature;
		test += "\n temperature : " + temperature;


	}
	ui.contenuBDD->setText(test);

	// Exemple 2 :
	bool retour;
	QString requete = "SELECT * FROM mesures";
	retour = query.exec(requete);
	if (retour)
	{
		qDebug() << "nb enregistrements : " << query.size();
		qDebug() << "nb colonnes : " << query.record().count();
		int fieldNo = query.record().indexOf("temperature");
		while (query.next())
		{
			QString date = query.value(fieldNo).toString();
			qDebug() << "temperature : " << date;
		}
	}
	else  qDebug() << query.lastError().text();

	// Exemple 3 :
	requete = "SELECT * FROM mesures";
	retour = query.exec(requete);
	if (retour)
	{
		while (query.next())
		{
			qDebug() << "enregistrement -> ";
			for (int i = 0; i < query.record().count(); i++)
				qDebug() << query.value(i).toString();
		}
	}
	else  qDebug() << query.lastError().text();

	// Exemple 4 :
	requete = "SELECT count(temperature) AS nb FROM mesures";

	query.exec(requete);
	query.first();
	int nb = query.value(0).toInt();
	qDebug() << "nb mesures : " << nb;

	// Exemple 5 :
	QSqlQuery r;
	// Utilisation des marqueurs '?'
	// INSERT INTO `mesures` (`id`, `date`, `heure`, `temperature`) VALUES (...)
	r.prepare("INSERT INTO mesures (id, date, heure, temperature) VALUES ('', ?, ?, ?)");
	// id en auto-incr�ment
	r.addBindValue("2009-09-10");
	r.addBindValue("09:01:00");
	r.addBindValue(35.12);
	if (r.exec())
	{
		std::cout << "Insertion r�ussie" << std::endl;
	}
	else
	{
		std::cout << "Echec insertion !" << std::endl;
		qDebug() << r.lastError().text();
	}

	// Utilisation des marqueurs nominatifs
	r.prepare("INSERT INTO mesures (id, date, heure, temperature) VALUES (:id, :date, :heure, :temperature)");
	r.bindValue(":id", ""); // auto-incr�ment
	r.bindValue(":date", "2009-09-10");
	r.bindValue(":heure", "09:01:00");
	r.bindValue(":temperature", 34.92);
	if (r.exec())
	{
		std::cout << "Insertion r�ussie" << std::endl;
	}
	else
	{
		std::cout << "Echec insertion !" << std::endl;
		qDebug() << r.lastError().text();
	}

	db.close();


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
    ui.connectionStatusLabel->setText("Status connexion : Connect�");
}

void QtWidgetsApplication1::onSocketDisconnected()
{
    ui.connectionStatusLabel->setText("Status connexion : D�connect�");
}

void QtWidgetsApplication1::onSendMessageButonClicket()
{
    if (socket->state() == QTcpSocket::ConnectedState)
    {
        socket->write("Hello server !\n");
    }

    
}

void QtWidgetsApplication1::onSocketReadyRead()
{
    QByteArray data = socket->read(socket->bytesAvailable());
    QString str(data);
    ui.connectionStatusLabel->setText("Status connexion : Message recu : "+str);
}

void QtWidgetsApplication1::onServerNewConnection()
{
    ui.connectionStatusLabel->setText("Un client s'est connect�");
    QTcpSocket* client = server->nextPendingConnection();
    QObject::connect(client, SIGNAL(readyRead()), this, SLOT(onClientReadyRead()));
    QObject::connect(client, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
}

void QtWidgetsApplication1::onClientDisconnected()
{
    QTcpSocket* obj = qobject_cast<QTcpSocket*>(sender());
    QObject::disconnect(obj, SIGNAL(readyRead()), this, SLOT(onClientReadyRead()));
    QObject::disconnect(obj, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
    obj->deleteLater();
    ui.connectionStatusLabel->setText("Un client s'est d�connect�");

}

void QtWidgetsApplication1::onClientReadyRead()
{
    QTcpSocket* obj = qobject_cast<QTcpSocket*>(sender());
    QByteArray data = obj->read(obj->bytesAvailable());
    QString str(data);
    ui.connectionStatusLabel->setText(str);
    obj->write(data);
}

void QtWidgetsApplication1::inscriptionButtom()
{
	ui.contenuBDD->hide();


}