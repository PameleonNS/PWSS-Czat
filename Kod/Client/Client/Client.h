#include <QDebug>
#include <QtNetwork/QTcpSocket>

const int NAME_SEND = 150;
const int MESSAGE_SEND = 151;
const int ID_SEND = 152;
const int CONNECTED_HOSTS = 153;
const int PENDING_MSG = 154;
const int CONFIRM = 180;
const int REMOVE_HOST = 254;

template <class type>
struct Message {
	short code;
	type data;
};

class Client : public QObject
{
	Q_OBJECT
public:
	Client();
	~Client();

public slots:
	void onReadyRead();
	void GetMessage(QString message, QString id);
	void SetUserName(QString name);
	void GetIdToSend(QString id);

signals:
	void PassDataToConversation(QString data, QString id);
	void PassIdToHostList(QString host);
	void SendIdToRemove(QString id);

private:
	void SendPacket(int code, QString data);
	void SendPacket(int code, QVector<QString> data);

	QTcpSocket		Socket;
	QString			Name;
	QVector<int>	connectedIds;
};

