#include <qobject.h>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <vector>

class Host : public QObject
{
	Q_OBJECT
public:
	Host::Host() {

	}
	Host::Host(const Host &obj)
	{
		Id = obj.Id;
		Name = obj.Name;
		SocketHandler = obj.SocketHandler;
		Channel = obj.Channel;
	}
	Host::Host(QTcpSocket* socketHandler, QString name, int id):
		SocketHandler(socketHandler),
		Name(name),
		Id(id),
		Channel(0) {}
	Host::~Host()
	{
	}
	QTcpSocket *GetSocketHandler()
	{
		return SocketHandler;
	}
	int GetId()
	{
		return Id;
	}
	QString GetName()
	{
		return Name;
	}
	void SetName(QString name)
	{
		Name = name;
	}
	void SetSocket(QTcpSocket *socket)
	{
		SocketHandler = socket;
	}
	void SetId(int id)
	{
		Id = id;
	}
private:
	int Id;
	QString Name;
	QTcpSocket *SocketHandler;
	int Channel;
};

