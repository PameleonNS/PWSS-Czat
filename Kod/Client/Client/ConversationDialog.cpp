#include "ConversationDialog.h"
#include <qdatetime.h>

ConversationDialog::ConversationDialog(QDialog *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.sendButton, SIGNAL(clicked()), this, SLOT(OnSendButtonClick()));
}

ConversationDialog::~ConversationDialog()
{
}

void ConversationDialog::GetData(QString data) //Wyświetlenie daty
{
	QDateTime time;
	QString date = time.currentDateTime().toString();
	ui.incomingEdit->append("<" + date + ">" + QString(data));
}

void ConversationDialog::OnSendButtonClick() 
{
	QDateTime time;
	QString date = time.currentDateTime().toString();
	QString message = ui.outcomingEdit->toPlainText();
	ui.outcomingEdit->clear();
	ui.incomingEdit->append(QString("<"+date+">You: " + message));
	emit PassDataToSend(message);
}

void ConversationDialog::reject()
{
	ui.incomingEdit->clear();
	ui.outcomingEdit->clear();
	QDialog::reject();
}