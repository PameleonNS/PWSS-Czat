#include <QDialog>
#include "ui_ConversationDialog.h"

class ConversationDialog : public QDialog
{
	Q_OBJECT
public:
	ConversationDialog(QDialog *parent = Q_NULLPTR);
	~ConversationDialog();

public slots:
	void GetData(QString data);

private slots:
	void OnSendButtonClick();
	void reject();

signals:
	void PassDataToSend(QString messsage);

private:
	Ui::ConversationDialog ui;
};

