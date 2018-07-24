#include "LoginDialog.h"

#include <QtWidgets/QLayout>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QPushButton>


LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent)
{
    createUI();

    //setModal(true);
}


void LoginDialog::createUI()
{
    setWindowTitle(u8"��ӭ��¼");

	auto * nameLabel = new QLabel;
    nameLabel->setText(tr(u8"�û���: "));

	auto* passwordLabel = new QLabel;
	passwordLabel->setText(tr(u8"��  ��: "));

	nameText_ = new QLineEdit;
    //userNameLabel_->setBuddy(userNameText_);

	passwordText_ = new QLineEdit;
    //passwordLabel_->setBuddy(passwordText_);

	QHBoxLayout* name = new QHBoxLayout();
	name->addWidget(nameLabel);
	name->addWidget(nameText_);

	QHBoxLayout* password = new QHBoxLayout();
	password->addWidget(passwordLabel);
	password->addWidget(passwordText_);

    // initialize buttons
    auto buttons = new QDialogButtonBox(this);
    buttons->addButton(QDialogButtonBox::Ok);
    buttons->addButton(QDialogButtonBox::Cancel);
    buttons->button(QDialogButtonBox::Ok)->setText(tr(u8"��¼"));
    buttons->button(QDialogButtonBox::Cancel)->setText(tr(u8"����"));

    // connects slots
    connect(buttons->button(QDialogButtonBox::Cancel),
        SIGNAL(clicked()),
        this,
        SLOT(close())
    );

    connect(buttons->button(QDialogButtonBox::Ok),
        SIGNAL(clicked()),
        this,
        SLOT(slotAcceptLogin()));

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addItem(name);
	layout->addItem(password);
    layout->addWidget(buttons);
    layout->setSizeConstraint(QLayout::SetFixedSize);

	setLayout(layout);
}

void LoginDialog::slotAcceptLogin() {
    close();
}