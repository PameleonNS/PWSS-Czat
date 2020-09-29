#include "Channel.h"

Channel::Channel(int id): ConversationID(id)
{
}


Channel::~Channel()
{
}

void Channel::AddHosts(Host *host)
{
	HostsVector.push_back(host);
	qDebug() << __FUNCTION__ + QString(" - Host added to Channel %1!").arg(ConversationID);
}

void Channel::AddMessageToConversation(QString name, QString message)
{
	conversation.push_back(name +": "+ message);
	qDebug() << __FUNCTION__ + QString(" - Message added to Channel! %1").arg(ConversationID);
}

QVector<QString>& Channel::GetMessages()
{
	qDebug() << __FUNCTION__ + QString(" - conversation of the Channel %1 returned!").arg(ConversationID);
	return conversation;
}

bool Channel::CheckHost(QTcpSocket *socket, QTcpSocket *socket_2)
{
	bool firstSocketFound = false;
	bool secondSocketFound = false;
	for (QVector<Host*>::iterator it = HostsVector.begin(); it < HostsVector.end(); ++it)
	{
		if (socket == (*it)->GetSocketHandler())
		{
			firstSocketFound = true;
		}
		else if (socket_2 == (*it)->GetSocketHandler())
		{
			secondSocketFound = true;
		}
	}
	if (firstSocketFound && secondSocketFound)
	{
		qDebug() << __FUNCTION__ + QString(" - host exists in that Channel %1").arg(ConversationID);
	}
	else
	{
		qDebug() << __FUNCTION__ + QString(" - host doesn't exists in that Channel %1").arg(ConversationID);
	}
	return firstSocketFound && secondSocketFound;
}

bool Channel::CheckHost(QTcpSocket *socket, int id)
{
	bool firstSocketFound = false;
	bool secondSocketFound = false;
	for (QVector<Host*>::iterator it = HostsVector.begin(); it < HostsVector.end(); ++it)
	{
		if (socket == (*it)->GetSocketHandler())
		{
			firstSocketFound = true;
		}
		else if ((*it)->GetId() == id)
		{
			secondSocketFound = true;
		}
	}
	if (firstSocketFound && secondSocketFound)
	{
		qDebug() << __FUNCTION__ + QString(" - host exists in that Channel %1").arg(ConversationID);
	}
	else
	{
		qDebug() << __FUNCTION__ + QString(" - host doesn't exists in that Channel %1").arg(ConversationID);
	}
	return firstSocketFound && secondSocketFound;
}

QTcpSocket* Channel::GetReceiver(QTcpSocket *sender)
{
	QTcpSocket *receiver = nullptr;
	for (QVector<Host*>::iterator it = HostsVector.begin(); it < HostsVector.end(); ++it)
	{
		if (sender != (*it)->GetSocketHandler())
		{
			qDebug() << __FUNCTION__ + QString(" - Found pointer to receiver, Channel %1").arg(ConversationID);
			receiver = (*it)->GetSocketHandler();
			break;
		}
	}
	if (receiver == nullptr)
	{
		qDebug() << __FUNCTION__ + QString(" - receiver is nullptr, Channel %1").arg(ConversationID);
	}
	return receiver;
}