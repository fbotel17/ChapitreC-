#include "QtWidgetsApplication1.h"
#include <QtGui>
#include <QDebug>
#include <QtSql/QtSql>
#include <iostream>


QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);
	ui.inscriptionButtom2->hide();
	ui.back->hide();
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
		std::cout << "Connexion réussie à " << db.hostName().toStdString() << std::endl;
	}
	else
	{
		std::cout << "La connexion a échouée !" << std::endl;
	}

	// Exemple 1 :
	QSqlQuery query("SELECT contenu FROM Message ORDER BY id DESC LIMIT 0, 2");
	QString test;
	while (query.next())
	{
		QString mess = query.value(0).toString();
		test += "\n temperature : " + mess;


	}
	ui.contenuBDD->setText(test);

	// Exemple 2 :
	bool retour;
	QString requete = "SELECT * FROM mesures ORDER BY id DESC LIMIT 0, 5";
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
	// id en auto-incrément
	r.addBindValue("2009-09-10");
	r.addBindValue("09:01:00");
	r.addBindValue(35.12);
	if (r.exec())
	{
		std::cout << "Insertion réussie" << std::endl;
	}
	else
	{
		std::cout << "Echec insertion !" << std::endl;
		qDebug() << r.lastError().text();
	}

	// Utilisation des marqueurs nominatifs
	r.prepare("INSERT INTO mesures (id, date, heure, temperature) VALUES (:id, :date, :heure, :temperature)");
	r.bindValue(":id", ""); // auto-incrément
	r.bindValue(":date", "2009-09-10");
	r.bindValue(":heure", "09:01:00");
	r.bindValue(":temperature", 34.92);
	if (r.exec())
	{
		std::cout << "Insertion réussie" << std::endl;
	}
	else
	{
		std::cout << "Echec insertion !" << std::endl;
		qDebug() << r.lastError().text();
	}



}


void QtWidgetsApplication1::onDisplayMessageButtonClicket()
{
    QString ip = ui.LoginLineEdit->text();
    QString port = ui.PassLineEdit->text();

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

void QtWidgetsApplication1::onSocketReadyRead()
{
    QByteArray data = socket->read(socket->bytesAvailable());
    QString str(data);
    ui.connectionStatusLabel->setText("Status connexion : Message recu : "+str);
}

void QtWidgetsApplication1::onServerNewConnection()
{
    ui.connectionStatusLabel->setText("Un client s'est connecté");
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
    ui.connectionStatusLabel->setText("Un client s'est déconnecté");

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
	ui.contenuBDD->setEnabled(false);

	ui.pushButton->hide();
	ui.pushButton->setEnabled(false);

	ui.pushButton_2->hide();
	ui.pushButton_2->setEnabled(false);

	ui.inscriptionButtom->hide();
	ui.inscriptionButtom->setEnabled(false);

	ui.conIns->setText("Page inscription");

	ui.inscriptionButtom2->show();
	ui.inscriptionButtom2->setEnabled(true);

	ui.back->show();
	ui.back->setEnabled(true);

	




}

void QtWidgetsApplication1::backConnectButtom()
{
	ui.contenuBDD->show();
	ui.contenuBDD->setEnabled(true);

	ui.pushButton->show();
	ui.pushButton->setEnabled(true);

	ui.pushButton_2->show();
	ui.pushButton_2->setEnabled(true);

	ui.inscriptionButtom->show();
	ui.inscriptionButtom->setEnabled(true);

	ui.conIns->setText("Page connexion");

	ui.inscriptionButtom2->hide();
	ui.inscriptionButtom2->setEnabled(false);


	ui.back->hide();
	ui.back->setEnabled(false);


}

void QtWidgetsApplication1::inscriptionButtom2()
{

	QString login = ui.LoginLineEdit->text();
	QString pass = ui.PassLineEdit->text();
	ui.contenuBDD->show();

	
	QSqlQuery r;
	// Utilisation des marqueurs '?'
	// INSERT INTO `mesures` (`id`, `date`, `heure`, `temperature`) VALUES (...)
	r.prepare("INSERT INTO User (nom, mdp) VALUES (?, ?)");
	// id en auto-incrément
	r.addBindValue(login);
	r.addBindValue(pass);
	if (r.exec())
	{
		std::cout << "Insertion réussie" << std::endl;
		backConnectButtom();
			

	}
	else
	{
		std::cout << "Echec insertion !" << std::endl;
		qDebug() << r.lastError().text();
	}

}

void QtWidgetsApplication1::envoyerButtom()
{
	QString message = ui.contenuMessage->toPlainText();



	QSqlQuery re;
	// Utilisation des marqueurs '?'
	// INSERT INTO `mesures` (`id`, `date`, `heure`, `temperature`) VALUES (...)
	re.prepare("INSERT INTO Message (contenu, idUser) VALUES (?, ?)");
	// id en auto-incrément
	re.addBindValue(message);
	re.addBindValue(this->id);
	if (re.exec())
	{
		std::cout << "Insertion réussie" << std::endl;
		backConnectButtom();


	}
	else
	{
		std::cout << "Echec insertion !" << std::endl;
		qDebug() << re.lastError().text();
	}
}

void QtWidgetsApplication1::pushButton()
{
	QString log = ui.LoginLineEdit->text();
	QString pass = ui.PassLineEdit->text();


	qDebug() << "login" << log << endl;

	qDebug() << "password"<< pass;

	QSqlQuery query("SELECT id, nom, mdp FROM User WHERE nom = '"+ log + "' and mdp = '" + pass+ "'");

	while (query.next())
	{
		ui.connectBDD->setText("connecté");
		qDebug() << "connecté youhou";
		QString idd = query.value(0).toString();

		this->id = idd;

	}

}