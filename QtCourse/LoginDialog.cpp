#include "LoginDialog.h"

#include <QtWidgets/QLayout>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp> 

#include <vector>

using namespace boost::asio;

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent)
{
    createUI();

    //setModal(true);
}

void LoginDialog::createUI()
{
    setWindowTitle(u8"欢迎登录");

	auto * nameLabel = new QLabel;
    nameLabel->setText(tr(u8"用户名: "));

	auto* passwordLabel = new QLabel;
	passwordLabel->setText(tr(u8"密  码: "));

	nameText_ = new QLineEdit;
	nameText_->setPlaceholderText(tr(u8"请输入用户名"));
	nameText_->setText("zhn");

	passwordText_ = new QLineEdit;
	passwordText_->setEchoMode(QLineEdit::Password);
	passwordText_->setPlaceholderText(tr(u8"请输入密码"));
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
    //buttons->button(QDialogButtonBox::Ok)->setText(tr(u8"登录"));
    //buttons->button(QDialogButtonBox::Cancel)->setText(tr(u8"注册"));
	auto* btnLogin = new QPushButton(tr(u8"登陆"));
	auto* btnSignup = new QPushButton(tr(u8"注册"));
	auto* btnExit = new QPushButton(tr(u8"退出"));
	QHBoxLayout* buttons = new QHBoxLayout();
	buttons->addWidget(btnLogin);
	buttons->addWidget(btnSignup);
	buttons->addWidget(btnExit);

    // connects slots
	connect(btnExit, SIGNAL(clicked()), this, SLOT(reject())); // 退出
	connect(btnSignup, SIGNAL(clicked()), this, SLOT(slotSignUp())); // 注册
	connect(btnLogin, SIGNAL(clicked()), this, SLOT(slotAcceptLogin())); // 登陆

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addItem(name);
	layout->addItem(password);
    layout->addItem(buttons);
    layout->setSizeConstraint(QLayout::SetFixedSize);

	setLayout(layout);
}

void LoginDialog::slotAcceptLogin() {
	if (!checkText()) {
		QMessageBox::warning(this, tr(u8"Waring"), tr(u8"用户名和密码不能为空！"), QMessageBox::Yes);
		return;
	}

	QString name = nameText_->text();
	QString passwd = passwordText_->text();
	// 登陆验证
	io_service ios;
	ip::tcp::socket sock(ios);
	ip::tcp::endpoint ep(ip::address::from_string("192.168.43.6"), 11230);
	sock.connect(ep);

	std::string loginInfo = "l";
	loginInfo += "|";
	loginInfo += nameText_->text().toStdString();
	loginInfo += "|";
	loginInfo += passwordText_->text().toStdString();
	sock.write_some(buffer(loginInfo)); // 发送登录验证请求
	std::vector<char> buf(100);
	sock.read_some(buffer(buf)); // 接收验证答复

	std::string ack = &buf[0]; 
	if (ack == "accept") {
		accept();
	}
	else { // 验证失败
		QMessageBox::warning(this, tr(u8"Waring"), tr(u8"用户名或密码错误"), QMessageBox::Yes);
		nameText_->clear();
		passwordText_->clear();
		nameText_->setFocus();
	}
}

void LoginDialog::slotSignUp() {
	if (!checkText()) {
		QMessageBox::warning(this, tr(u8"Waring"), tr(u8"用户名和密码不能为空！"), QMessageBox::Yes);
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
	sock.write_some(buffer(signupInfo)); // 发送注册验证请求
	std::vector<char> buf(100);
	sock.read_some(buffer(buf)); // 接收注册验证答复

	std::string ack = &buf[0];
	if (ack == "accept") {
		QMessageBox::warning(this, tr(u8"Waring"), tr(u8"注册成功"), QMessageBox::Yes);
	}
	else { // 验证失败
		QMessageBox::warning(this, tr(u8"Waring"), tr(u8"该用户名已被注册"), QMessageBox::Yes);
		sock.close();
	}

	//connFlag = false;
	//sock.async_connect(ep, boost::bind(&LoginDialog::conn, this, &sock, boost::asio::placeholders::error));

	//deadline_timer t(ios, boost::posix_time::seconds(5));
	//t.async_wait(boost::bind(&LoginDialog::timeOut, this, &sock));

	//ios.run();
}

bool LoginDialog::checkText() {
	std::string username = nameText_->text().toStdString();
	std::string password = passwordText_->text().toStdString();
	if (username != "" && password != "") {
		return true;
	}
	return false;
}

void LoginDialog::conn(ip::tcp::socket * sock, const boost::system::error_code& ec) {
	if (ec) {
		//QMessageBox::warning(this, tr(u8"Waring"), tr(u8"连接错误"), QMessageBox::Yes);
	}
	else {
		connFlag = true;
		//QMessageBox::warning(this, tr(u8"Waring"), tr(u8"连接成功"), QMessageBox::Yes);
		std::string signupInfo = "s";
		signupInfo += "|";
		signupInfo += nameText_->text().toStdString();
		signupInfo += "|";
		signupInfo += passwordText_->text().toStdString();
		sock->write_some(buffer(signupInfo)); // 发送注册验证请求
		std::vector<char> buf(100);
		sock->read_some(buffer(buf)); // 接收注册验证答复

		std::string ack = &buf[0];
		if (ack == "accept") {
			QMessageBox::warning(this, tr(u8"Waring"), tr(u8"注册成功"), QMessageBox::Yes);
		}
		else { // 验证失败
			QMessageBox::warning(this, tr(u8"Waring"), tr(u8"该用户名已被注册"), QMessageBox::Yes);
			sock->close();
		}
	}
}

void LoginDialog::timeOut(ip::tcp::socket * sock) {
	if (!connFlag) {
		QMessageBox::warning(this, tr(u8"Waring"), tr(u8"关闭socket"), QMessageBox::Yes);
		sock->close();
	}
}