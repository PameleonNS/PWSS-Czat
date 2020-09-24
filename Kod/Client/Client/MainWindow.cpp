#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.sendButton, SIGNAL(clicked()), this, SLOT(OnSendButtonClick()));
	connect(&client, SIGNAL(PassDataToMainWindow(QString)), this, SLOT(GetData(QString)));
	connect(&nameAccepterDialog, SIGNAL(SendExit()), this, SLOT(CloseApplication()));
	connect(&nameAccepterDialog, SIGNAL(SendName(QString)), this, SLOT(GetName(QString)));
}

void  MainWindow::ShowNameAccepter()
{
	this->setEnabled(false);
	nameAccepterDialog.show();
}

void MainWindow::GetData(QString data)
{
	ui.incomingEdit->append(QString(data));
}

void MainWindow::CloseApplication()
{
	nameAccepterDialog.close();
	this->setEnabled(true);
	QMainWindow::close();
	QApplication::quit();
}

void MainWindow::GetName(QString name)
{
	client.SetUserName(name);
	nameAccepterDialog.close();
	this->setEnabled(true);
}

void MainWindow::OnSendButtonClick()
{
	QString Message = ui.outcomingEdit->toPlainText();
	ui.outcomingEdit->clear();
	ui.incomingEdit->append(QString("You: " + Message));
	client.SendMessage(Message);
}