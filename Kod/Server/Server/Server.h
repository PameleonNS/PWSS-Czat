#include <qobject.h>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <qlist.h>
#include "Channel.h"

const int NAME_SEND = 150;			//pakiet wysyłany przez klienta do serwera (nazwa)
const int MESSAGE_SEND = 151;		//przekazywanie wiadomości pomiędzy klientami
const int ID_SEND = 152;			//rozgłoszenie informacji o nowym kliencie dla już podłączonych
const int CONNECTED_HOSTS = 153;	//wysłanie nowemy klientowi informacji o innych podłączonych klientach
const int REMOVE_HOST = 254;		//rozgłoszenie informacji o rozłączeniu klienta

template<class type>
struct Message {
	short code;
	type data;
};

class Server : public QObject
{
	Q_OBJECT
public:
	Server();
	~Server();
public slots:
	void OnNewConnection();
	void OnSocketStateChanged(QAbstractSocket::SocketState socketState);
	void OnReadyRead();
private:
	bool CheckIfIdIsAvailable(int id);
	bool CheckIfChannelIdIsAvailable(int id);
	void SendPacket(QTcpSocket *socket, int code, QString data);
	void SendPacket(QTcpSocket *socket, int code, QVector<QString> &data);
	void SendPacket(QTcpSocket *socket, int code);

	QTcpServer  TcpServer;
	QList<Host>  Hosts;
	QList<Channel*> Channels;
};
