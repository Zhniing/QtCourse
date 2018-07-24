#include "MainWIndow.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>

MainWindow::MainWindow() {
    createUI();
}

void MainWindow::createUI() {

    resize(800, 800);

    serverAddress_ = new QLineEdit;
    serverAddress_->setFixedWidth(120);
    serverPort_ = new QLineEdit;
    serverPort_->setFixedWidth(40);

    auto* hbl = new QHBoxLayout();
    hbl->addWidget(new QLabel(u8"网络地址: "));
    hbl->addWidget(serverAddress_);
    hbl->addSpacing(20);
    hbl->addWidget(new QLabel(u8"端口: "));
    hbl->addWidget(serverPort_);
    hbl->addStretch();
    stockBrowser_ = new StockBrowser();
    stockHistory_ = new StockHistory();

    contextLayout_ = new QStackedLayout;
    contextLayout_->addWidget(stockBrowser_);
    contextLayout_->addWidget(stockHistory_);

    context_ = new QWidget;
    context_->setLayout(contextLayout_);

    auto* layout = new QVBoxLayout();
    layout->addItem(hbl);
    layout->addWidget(context_);

    auto* centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    
    this->setCentralWidget(centralWidget);

    connect(stockBrowser_, &StockBrowser::viewHistory, this, &MainWindow::slotViewHistory);
    connect(stockHistory_, &StockHistory::viewFinish, this, &MainWindow::slotViewHistoryFinish);
}

void MainWindow::slotViewHistory(const QString& code) {
    /*QMessageBox msgBox;
    msgBox.setText(u8"你想查看行情历史？还没有实现.");
    msgBox.exec();*/

    contextLayout_->setCurrentIndex(1);
}

void MainWindow::slotViewHistoryFinish() {
    contextLayout_->setCurrentIndex(0);
}