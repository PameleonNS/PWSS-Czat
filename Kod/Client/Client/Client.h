#include <QDebug>
#include <QtNetwork/QTcpSocket>

class Client : public QObject
{
	Q_OBJECT
public:
	Client();
	~Client();

public slots:
	void onReadyRead();
	void SendMessage(QString message);
	void SetUserName(QString name);

signals:
	void PassDataToMainWindow(QString data);

private:
	QTcpSocket  Socket;
	QString		Name;
};

