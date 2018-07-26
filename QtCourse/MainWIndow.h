#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedLayout>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/QDialogButtonBox>
#include "StockBrowser.h"
#include "StockHistory.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

private:
    void createUI();

private slots:
	void slotViewHistory(const QString& code);
	void slotViewHistoryFinish();
	void handleClick();
	void animateClick();

private:
    QLineEdit* serverAddress_;
    QLineEdit* serverPort_;
	QPushButton* fetchData_;
    QWidget* context_;
    QStackedLayout* contextLayout_; // Õ»²¼¾Ö
    StockBrowser* stockBrowser_;
    StockHistory* stockHistory_;
};