#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include "LoginDialog.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

    LoginDialog dlg;
    dlg.exec();

    MainWindow widget;
    widget.show();
	
	return app.exec();
}