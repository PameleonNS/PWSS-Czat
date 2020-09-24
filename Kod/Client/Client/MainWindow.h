#include <QtWidgets/QMainWindow>
#include "Client.h"
#include "NameAccepter.h"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	void ShowNameAccepter();

public slots:
	void GetData(QString data);
	void CloseApplication();
	void GetName(QString name);

private slots:
	void OnSendButtonClick();

private:
	Ui::MainWindowClass ui;
	Client client;
	NameAccepter nameAccepterDialog;
};
