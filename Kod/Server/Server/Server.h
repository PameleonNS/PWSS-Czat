#include <qobject.h>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

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
	QTcpServer  TcpServer;
	QList<QTcpSocket*>  Sockets;
};
