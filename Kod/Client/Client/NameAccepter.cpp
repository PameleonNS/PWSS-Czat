#include "NameAccepter.h"

NameAccepter::NameAccepter(QDialog *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.okButton->setEnabled(false);
	connect(ui.nameEdit, SIGNAL(textChanged(QString)), this, SLOT(OnNameEditChange(QString)));
	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(OnOkButtonClick()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(OnCancelButtonClick()));
}

NameAccepter::~NameAccepter()
{
}

void NameAccepter::reject()
{
	emit SendExit();
	QDialog::reject();
}

void NameAccepter::OnNameEditChange(QString text)
{
	if (text.isEmpty())
	{
		ui.okButton->setEnabled(false);
	}
	else
	{
		ui.okButton->setEnabled(true);
	}
}

void NameAccepter::OnOkButtonClick()
{
	QString name = ui.nameEdit->text();
	emit SendName(name);
}

void NameAccepter::OnCancelButtonClick()
{
	emit SendExit();
}