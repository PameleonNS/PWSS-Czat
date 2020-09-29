#include <QDialog>
#include "ui_ConversationDialog.h"

class ConversationDialog : public QDialog
{
	Q_OBJECT
public:
	ConversationDialog(QDialog *parent = Q_NULLPTR);
	ConversationDialog(const ConversationDialog &dialog)
	{
		this->ui = dialog.ui;
		this->conversationId = dialog.conversationId;
	}
	~ConversationDialog();

	void SetConversationId(QString id)
	{
		conversationId = id;
	}
	void SetData(QString data);
	QString GetId()
	{
		return conversationId;
	}

private slots:
	void OnSendButtonClick();
	void reject();

signals:
	void PassDataToSend(QString messsage, QString id);

private:
	Ui::ConversationDialog ui;
	QString conversationId;
};

