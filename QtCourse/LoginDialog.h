#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

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

private:
	QLineEdit* nameText_;
    QLineEdit* passwordText_;
};

