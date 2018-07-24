#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedLayout>
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

private:
    QLineEdit* serverAddress_;
    QLineEdit* serverPort_;
    QWidget* context_;
    QStackedLayout* contextLayout_;
    StockBrowser* stockBrowser_;
    StockHistory* stockHistory_;
};