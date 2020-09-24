#include <QDebug>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QAbstractSocket>
#include "Server.h"

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

	Sockets.push_back(clientSocket);
	for (QTcpSocket* socket : Sockets) {
		socket->write(QByteArray::fromStdString(clientSocket->peerAddress().toString().toStdString() + " connected to server !\n"));
	}
}

void Server::OnSocketStateChanged(QAbstractSocket::SocketState socketState)
{
	if (socketState == QAbstractSocket::UnconnectedState)
	{
		QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
		Sockets.removeOne(sender);
	}
}

void Server::OnReadyRead()
{
	QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
	QByteArray datas = sender->readAll();
	qDebug() << datas;
	for (QTcpSocket* socket : Sockets) {
		if (socket != sender)
			socket->write(datas);
	}
}

Server::~Server()
{
}
