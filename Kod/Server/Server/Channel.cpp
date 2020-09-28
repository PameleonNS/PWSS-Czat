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
}

void Channel::AddMessageToConversation(QString name, QString message)
{
	conversation.push_back(name +": "+ message);
}

QVector<QString>& Channel::GetMessages()
{
	return conversation;
}

bool Channel::CheckHost(QTcpSocket *socket)
{
	bool ifFound = false;
	for (QVector<Host*>::iterator it = HostsVector.begin(); it < HostsVector.end(); ++it)
	{
		if (socket == (*it)->GetSocketHandler())
		{
			ifFound = true;
			break;
		}
	}
	return ifFound;
}

QTcpSocket* Channel::GetReceiver(QTcpSocket *sender)
{
	QTcpSocket *receiver = nullptr;
	for (QVector<Host*>::iterator it = HostsVector.begin(); it < HostsVector.end(); ++it)
	{
		if (sender != (*it)->GetSocketHandler())
		{
			receiver = (*it)->GetSocketHandler();
			break;
		}
	}
	return receiver;
}