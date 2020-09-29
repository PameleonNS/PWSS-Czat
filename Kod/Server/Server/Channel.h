#include <qobject.h>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <vector>
#include "Host.h"

class Channel : public QObject
{
public:
	Channel() {};
	Channel(const Channel &obj)
	{

	}
	Channel(int id);
	~Channel();
	void AddHosts(Host *host);
	void AddMessageToConversation(QString name, QString message);
	QVector<QString>& GetMessages();
	bool CheckHost(QTcpSocket *socket, QTcpSocket *socket_2);
	bool CheckHost(QTcpSocket *socket, int id);
	QTcpSocket* GetReceiver(QTcpSocket *sender);
	int GetId() 
	{
		return ConversationID;
	}

private:
	int ConversationID;
	QVector<Host*> HostsVector;
	QVector<QString> conversation;
};
