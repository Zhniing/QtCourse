#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>

#include "LoginDialog.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
    LoginDialog dlg;
	MainWindow widget;

    int res = dlg.exec();

	if (res == QDialog::Accepted) {
		widget.show();
		return app.exec();
	}

	return 0;
}