#include "LoginDialog.h"

#include <QtWidgets/QLayout>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>
#include <boost/asio.hpp>

#include <vector>

using namespace boost::asio;

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
	nameText_->setPlaceholderText(tr(u8"�������û���"));
	nameText_->setText("zhn");

	passwordText_ = new QLineEdit;
	passwordText_->setEchoMode(QLineEdit::Password);
	passwordText_->setPlaceholderText(tr(u8"����������"));
	passwordText_->setText("1123");

	QHBoxLayout* name = new QHBoxLayout();
	name->addWidget(nameLabel);
	name->addWidget(nameText_);

	QHBoxLayout* password = new QHBoxLayout();
	password->addWidget(passwordLabel);
	password->addWidget(passwordText_);

    // initialize buttons
    //auto buttons = new QDialogButtonBox(this);
    //buttons->addButton(QDialogButtonBox::Ok);
    //buttons->addButton(QDialogButtonBox::Cancel);
    //buttons->button(QDialogButtonBox::Ok)->setText(tr(u8"��¼"));
    //buttons->button(QDialogButtonBox::Cancel)->setText(tr(u8"ע��"));
	auto* btnLogin = new QPushButton(tr(u8"��½"));
	auto* btnSignup = new QPushButton(tr(u8"ע��"));
	auto* btnExit = new QPushButton(tr(u8"�˳�"));
	QHBoxLayout* buttons = new QHBoxLayout();
	buttons->addWidget(btnLogin);
	buttons->addWidget(btnSignup);
	buttons->addWidget(btnExit);

    // connects slots
	connect(btnExit, SIGNAL(clicked()), this, SLOT(reject())); // �˳�
	connect(btnSignup, SIGNAL(clicked()), this, SLOT(slotSignUp())); // ע��
	connect(btnLogin, SIGNAL(clicked()), this, SLOT(slotAcceptLogin())); // ��½

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addItem(name);
	layout->addItem(password);
    layout->addItem(buttons);
    layout->setSizeConstraint(QLayout::SetFixedSize);

	setLayout(layout);
}

void LoginDialog::slotAcceptLogin() {
	if (!checkText()) {
		QMessageBox::warning(this, tr(u8"Waring"), tr(u8"�û��������벻��Ϊ�գ�"), QMessageBox::Yes);
		return;
	}

	QString name = nameText_->text();
	QString passwd = passwordText_->text();
	// ��½��֤
	io_service ios;
	ip::tcp::socket sock(ios);
	ip::tcp::endpoint ep(ip::address::from_string("192.168.43.6"), 11230);
	sock.connect(ep);

	std::string loginInfo = "l";
	loginInfo += "|";
	loginInfo += nameText_->text().toStdString();
	loginInfo += "|";
	loginInfo += passwordText_->text().toStdString();
	sock.write_some(buffer(loginInfo)); // ���͵�¼��֤����
	std::vector<char> buf(100);
	sock.read_some(buffer(buf)); // ������֤��

	std::string ack = &buf[0]; 
	if (ack == "accept") {
		accept();
	}
	else { // ��֤ʧ��
		QMessageBox::warning(this, tr(u8"Waring"), tr(u8"�û������������"), QMessageBox::Yes);
		nameText_->clear();
		passwordText_->clear();
		nameText_->setFocus();
	}
}

void LoginDialog::slotSignUp() {
	if (!checkText()) {
		QMessageBox::warning(this, tr(u8"Waring"), tr(u8"�û��������벻��Ϊ�գ�"), QMessageBox::Yes);
		return;
	}

	io_service ios;
	ip::tcp::socket sock(ios);
	ip::tcp::endpoint ep(ip::address::from_string("192.168.43.6"), 11230);
	sock.connect(ep);

	std::string signupInfo = "s";
	signupInfo += "|";
	signupInfo += nameText_->text().toStdString();
	signupInfo += "|";
	signupInfo += passwordText_->text().toStdString();
	sock.write_some(buffer(signupInfo)); // ����ע����֤����
	std::vector<char> buf(100);
	sock.read_some(buffer(buf)); // ����ע����֤��

	std::string ack = &buf[0];
	if (ack == "accept") {
		QMessageBox::warning(this, tr(u8"Waring"), tr(u8"ע��ɹ�"), QMessageBox::Yes);
	}
	else { // ��֤ʧ��
		QMessageBox::warning(this, tr(u8"Waring"), tr(u8"���û����ѱ�ע��"), QMessageBox::Yes);
	}
}

bool LoginDialog::checkText() {
	std::string username = nameText_->text().toStdString();
	std::string password = passwordText_->text().toStdString();
	if (username != "" && password != "") {
		return true;
	}
	return false;
}