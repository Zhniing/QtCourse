#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <boost/asio.hpp>

class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LoginDialog(QWidget* parent = nullptr);

private slots:
    void slotAcceptLogin();
	void slotSignUp();

private:
	void createUI();
	bool checkText();
	void conn(boost::asio::ip::tcp::socket * sock, const boost::system::error_code & ec);
	void timeOut(boost::asio::ip::tcp::socket * sock);

private:
	QLineEdit* nameText_;
    QLineEdit* passwordText_;
	bool connFlag;
};

