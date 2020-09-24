#include <QDebug>
#include <QtNetwork/QHostAddress>
#include <QtCore/QTextCodec>
#include "Client.h"

Client::Client()
{
	Socket.connectToHost(QHostAddress("127.0.0.1"), 4242);
	connect(&Socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void Client::onReadyRead()	
{
	QByteArray data = Socket.readAll();
	QString dataStr = QString(data);
	emit PassDataToMainWindow(dataStr);
}

void Client::SendMessage(QString message)
{
	if (Socket.isWritable())
	{
		QString messageToSend = Name + ": " + message;
		Socket.write(messageToSend.toStdString().c_str());
	}
}

void Client::SetUserName(QString name)
{
	Name = name;
}

Client::~Client()
{
}
