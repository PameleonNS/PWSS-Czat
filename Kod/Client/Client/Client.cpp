#include <QDebug>
#include <QtNetwork/QHostAddress>
#include <QtCore/QTextCodec>
#include <qdatastream.h>
#include "Client.h"

QDataStream & operator << (QDataStream &stream, const Message<QString>& obj)
{
	return stream << (short)obj.code << (QString)obj.data;
}

QDataStream & operator >> (QDataStream &stream, Message<QString>& obj)
{
	short code;
	stream >> code;
	obj.code = code;
	QString str;
	stream >> str;
	obj.data = str;
	return stream;
}

Client::Client() //połączenie z hostem
{
	Socket.connectToHost(QHostAddress("127.0.0.1"), 4242);
	connect(&Socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void Client::onReadyRead() //odbiór paczek z serwera
{
	QByteArray data = Socket.readAll();

	QDataStream stream(data);
	short code;
	stream >> code;

	if (code == MESSAGE_SEND)
	{
		QString data;
		stream >> data;
		emit PassDataToConversation(data);
	}
	else if (code == ID_SEND)
	{
		QString data;
		stream >> data;
		QStringList list;
		list = data.split(":");
		emit PassIdToHostList(list[0] + "(" + list[1] + ")");
	}
	else if (code == CONNECTED_HOSTS)
	{
		QVector<QString> vect;
		stream >> vect;
		for (QVector<QString>::iterator it = vect.begin(); it < vect.end(); ++it)
		{
			QStringList list;
			list = it->split(":");
			emit PassIdToHostList(list[0]+"("+list[1]+")");
		}
	}
	else if (code == PENDING_MSG)
	{
		QVector<QString> vect;
		stream >> vect;
		for (QVector<QString>::iterator it = vect.begin(); it < vect.end(); ++it)
		{
			emit PassDataToConversation(data);
		}
	}
}

void Client::GetMessage(QString message)
{
	QString messageToSend = Name + ": " + message;
	SendPacket(MESSAGE_SEND, messageToSend);
}

void Client::SendPacket(int code, QString data) //wysłanie wiadomości do serwera
{
	if (Socket.isWritable())
	{
		Message<QString> msg;
		msg.code = code;
		msg.data = data;
		QByteArray bytes;
		QDataStream stream(&bytes, QIODevice::WriteOnly);
		stream << msg;
		Socket.write(bytes);
	}
}

void Client::SetUserName(QString name) //ustawienie nazwy użytkownika
{
	Name = name;
	SendPacket(NAME_SEND, Name);
}

void Client::GetIdToSend(QString id)	
{
	SendPacket(PENDING_MSG, id);
}

Client::~Client()
{
}
