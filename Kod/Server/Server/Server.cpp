#include <QDebug>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QAbstractSocket>
#include <QtCore/QRandomGenerator>
#include <qdatastream.h>
#include "Server.h"

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

QDataStream & operator << (QDataStream &stream, const Message<QVector<QString>>& obj)
{
	return stream << static_cast<short>(obj.code) << static_cast<QVector<QString>>(obj.data);
}

QDataStream & operator >> (QDataStream &stream, Message<QVector<QString>>& obj)
{
	short code;
	stream >> code;
	obj.code = code;
	QVector<QString> str;
	stream >> str;
	obj.data = str;
	return stream;
}

Server::Server()
{
	TcpServer.listen(QHostAddress::Any, 4242);
	connect(&TcpServer, SIGNAL(newConnection()), this, SLOT(OnNewConnection()));
}


void Server::OnNewConnection()
{
	QTcpSocket *clientSocket = TcpServer.nextPendingConnection();

	connect(clientSocket, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
	connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(OnSocketStateChanged(QAbstractSocket::SocketState)));
	int number = 0;
	do
	{
		number = QRandomGenerator::global()->bounded(100000, 200000);
	} while (!CheckIfIdIsAvailable(number));

	Host host(clientSocket, "", number);

	SendPacket(clientSocket, CONNECTED_HOSTS); //Sending to new host, all connected hosts


	Hosts.push_back(host);
}

bool Server::CheckIfIdIsAvailable(int id)
{
	bool retState = true;
	for (QList<Host>::Iterator it = Hosts.begin(); it < Hosts.end(); ++it)
	{
		if (it->GetId() == id)
		{
			retState = false;
		}
	}
	return retState;
}

bool Server::CheckIfChannelIdIsAvailable(int id)
{
	bool retState = true;
	for (QList<Channel*>::Iterator it = Channels.begin(); it < Channels.end(); ++it)
	{
		if ((*it)->GetId() == id)
		{
			retState = false;
		}
	}
	return retState;
}

void Server::SendPacket(QTcpSocket *socket, int code, QString data)
{
	if (socket != nullptr)
	{
		Message<QString> msg;
		msg.code = code;
		msg.data = data;
		QByteArray bytes;
		QDataStream stream(&bytes, QIODevice::WriteOnly);
		stream << msg;
		socket->write(bytes);
	}
}

void Server::SendPacket(QTcpSocket *socket, int code, QVector<QString> &data)
{
	if (socket != nullptr)
	{
		Message<QVector<QString>> msg;
		msg.code = code;
		msg.data = data;
		QByteArray bytes;
		QDataStream stream(&bytes, QIODevice::WriteOnly);
		stream << msg;
		socket->write(bytes);
	}
}

void Server::SendPacket(QTcpSocket *socket, int code)
{
	if (socket != nullptr)
	{
		Message<QVector<QString>> msg;
		msg.code = code;
		for (QList<Host>::iterator it = Hosts.begin(); it < Hosts.end(); ++it)
		{
			QString data = it->GetName() + ":" + QString::number(it->GetId());
			msg.data.push_back(data);
		}
		QByteArray bytes;
		QDataStream stream(&bytes, QIODevice::WriteOnly);
		stream << msg;
		socket->write(bytes);
	}
}

void Server::OnSocketStateChanged(QAbstractSocket::SocketState socketState)
{
	if (socketState == QAbstractSocket::UnconnectedState)
	{
		QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
		int id = 0;
		for (QList<Host>::Iterator it = Hosts.begin(); it < Hosts.end(); ++it)
		{
			if (it->GetSocketHandler() == sender)
			{
				int dist = it - Hosts.begin();
				id = it->GetId();
				Hosts.removeAt(dist);
			}
		}
		for (QList<Host>::Iterator it = Hosts.begin(); it < Hosts.end(); ++it)
		{
			QTcpSocket* receiver = it->GetSocketHandler();
			SendPacket(receiver, REMOVE_HOST, QString::number(id));
		}
	}
}

void Server::OnReadyRead()
{
	QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());

	QByteArray data = sender->readAll();
	QDataStream stream(data);
	short code;
	stream >> code;

	if (code == MESSAGE_SEND)
	{
		QVector<QString> vector;
		stream >> vector;
		int id = vector[0].toInt();

		for (QList<Channel*>::iterator it = Channels.begin(); it < Channels.end(); ++it)
		{
			if ((*it)->CheckHost(sender, id))
			{
				for (QList<Host>::Iterator it = Hosts.begin(); it < Hosts.end(); ++it)
				{
					QTcpSocket *recv = it->GetSocketHandler();
					if (recv == sender)
					{
						vector[0] = QString::number(it->GetId());
					}
				}
				QTcpSocket* receiver = (*it)->GetReceiver(sender);
				SendPacket(receiver, MESSAGE_SEND, vector);
				break;
			}
		}
	}
	else if (code == NAME_SEND)
	{
		QString message;
		stream >> message;
		Message<QString> msgBack;
		int number = 0;
		for (QList<Host>::Iterator it = Hosts.begin(); it < Hosts.end(); ++it)
		{
			QTcpSocket *receiver = it->GetSocketHandler();
			if (receiver == sender)
			{
				
				number = it->GetId();
				it->SetName(message);
			}
		}

		if (number != 0)
		{
			for (QList<Host>::Iterator it = Hosts.begin(); it < Hosts.end(); ++it)
			{
				QTcpSocket* receiver = it->GetSocketHandler();
				if (receiver != sender)
				{
					SendPacket(receiver, ID_SEND, message + ":" + QString::number(number));
				}
			}
		}
	}
	else if (code == PENDING_MSG)
	{
		bool bIfExist = false;
		QString target;
		int id;
		Host *receiver;
		Host *hostSender;
		stream >> target;
		id = target.toInt();

		for (QList<Host>::iterator it = Hosts.begin(); it < Hosts.end(); ++it)
		{
			if (id == it->GetId())
			{
				receiver = &(*it);
				break;
			}
		}

		for (QList<Channel*>::iterator it = Channels.begin(); it < Channels.end(); ++it)
		{
			if ((*it)->CheckHost(receiver->GetSocketHandler(), sender))
			{
				bIfExist = true;
				//QTcpSocket *receiver = (*it)->GetReceiver(sender);
				//if (!(*it)->GetMessages().isEmpty())
				//{
				//	SendPacket(receiver, PENDING_MSG, (*it)->GetMessages());
				//	break;
				//}
				break;
			}
		}

		if (!bIfExist)
		{
			int number = 0;
			do
			{
				number = QRandomGenerator::global()->bounded(100000, 200000);
			} while (!CheckIfChannelIdIsAvailable(number));
			Channel *channel = new Channel(number);
			channel->AddHosts(receiver);
			for (QList<Host>::iterator it = Hosts.begin(); it < Hosts.end(); ++it)
			{
				if (it->GetSocketHandler() == sender)
				{
					hostSender = &(*it);
					break;
				}
			}
			channel->AddHosts(hostSender);
			Channels.push_back(channel);
		}
	}
}

Server::~Server()
{
}
