#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.talkButton, SIGNAL(clicked()), this, SLOT(OnTalkButtonClick()));

	connect(&client, SIGNAL(PassDataToConversation(QString, QString)), this, SLOT(GetDataAndId(QString, QString)));

	connect(&nameAccepterDialog, SIGNAL(SendExit()), this, SLOT(CloseApplication()));
	connect(&nameAccepterDialog, SIGNAL(SendName(QString)), this, SLOT(GetName(QString)));

	connect(&client, SIGNAL(PassIdToHostList(QString)), this, SLOT(AppendNewHostToList(QString)));

	connect(this, SIGNAL(PassIdToSend(QString)), &client, SLOT(GetIdToSend(QString)));

	connect(&client, SIGNAL(SendIdToRemove(QString)), this, SLOT(RemoveId(QString)));
}

MainWindow::~MainWindow()
{
	qDeleteAll(dialogVector.begin(), dialogVector.end());
}

void MainWindow::ShowNameAccepter() //okno wprowadzania nazwy użytkownika
{
	this->setEnabled(false);
	nameAccepterDialog.show();
}

void MainWindow::OnTalkButtonClick() //odbieranie danych
{
	if (ui.connectedListWidget->currentRow() != -1)
	{
		QString text = ui.connectedListWidget->currentItem()->text();
		QStringList id = text.split(QRegExp("[(,)]"));
		emit PassIdToSend(id[1]);
		bool bIfFound = false;
		for (QVector<ConversationDialog*>::iterator it = dialogVector.begin(); it < dialogVector.end(); ++it)
		{
			if ((*it)->GetId() == id[1])
			{			
				bIfFound = true;
				(*it)->show();
				break;
			}
		}

		if (!bIfFound)
		{
			ConversationDialog *dialog = new ConversationDialog();
			connect(dialog, SIGNAL(PassDataToSend(QString, QString)), &client, SLOT(GetMessage(QString, QString)));
			dialog->SetConversationId(id[1]);
			dialog->show();
			dialogVector.push_back(dialog);
		}
	}
}

void MainWindow::CloseApplication() //zamknięcie aplikacji
{
	nameAccepterDialog.close();
	this->setEnabled(true);
	QMainWindow::close();
	QApplication::quit();
}

void MainWindow::GetName(QString name) //ustawienie nazwy
{
	ui.nameLabel->setText("Name: "+name);
	client.SetUserName(name);
	nameAccepterDialog.close();
	this->setEnabled(true);
}

void MainWindow::AppendNewHostToList(QString host) //wyświetlanie nowego użytkownika
{
	ui.connectedListWidget->addItem(host);
}

void MainWindow::GetDataAndId(QString data, QString id) //
{
	bool bIfFound = false;
	for (QVector<ConversationDialog*>::iterator it = dialogVector.begin(); it < dialogVector.end(); ++it)
	{
		if ((*it)->GetId() == id)
		{
			bIfFound = true;
			(*it)->SetData(data);
		}
	}

	if (!bIfFound)
	{
		ConversationDialog *dialog = new ConversationDialog();
		connect(dialog, SIGNAL(PassDataToSend(QString, QString)), &client, SLOT(GetMessage(QString, QString)));
		dialog->SetConversationId(id);
		dialog->SetData(data);
		dialogVector.push_back(dialog);
	}
}

void MainWindow::RemoveId(QString id) //usuwanie uzytkownika z listy po rozlaczeniu
{
	for (int i = 0; i < ui.connectedListWidget->count(); ++i)
	{
		QStringList list = ui.connectedListWidget->item(i)->text().split(QRegExp("[(,)]"));

		if (list[1].trimmed() == id)
		{
			QListWidgetItem* item = ui.connectedListWidget->item(i);
			delete item;
			break;
		}
	}

	for (QVector<ConversationDialog*>::iterator it = dialogVector.begin(); it < dialogVector.end(); ++it)
	{
		if ((*it)->GetId() == id)
		{
			delete (*it);
			dialogVector.removeOne((*it));
		}
	}
}