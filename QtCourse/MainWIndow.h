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
    QStackedLayout* contextLayout_; // ջ����
    StockBrowser* stockBrowser_; // ��Ʊ���
    StockHistory* stockHistory_; // ��ʷ����
	Candlestick* stockCandle_; // K��
};