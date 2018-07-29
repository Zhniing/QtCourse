#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedLayout>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/QDialogButtonBox>

#include "StockBrowser.h"
#include "StockHistory.h"
#include "Candlestick.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

private:
    void createUI();

private slots:
	void slotViewHistory(const QString& code);
	void slotToHistory();
	void slotViewFinish();
	void slotViewCandle();
	void handleClick();
	void animateClick();

private:
    QLineEdit* serverAddress_;
    QLineEdit* serverPort_;
	QPushButton* fetchData_;
    QWidget* context_;
    QStackedLayout* contextLayout_; // 栈布局
    StockBrowser* stockBrowser_; // 股票浏览
    StockHistory* stockHistory_; // 历史行情
	Candlestick* stockCandle_; // K线
};