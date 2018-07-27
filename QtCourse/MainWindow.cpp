#include "MainWIndow.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/QDialogButtonBox>

MainWindow::MainWindow() {
    createUI();
}

void MainWindow::createUI() {

	// 主窗体大小
    resize(800, 600);

	// 网络连接 小部件Widget
    serverAddress_ = new QLineEdit;
	serverAddress_->setText("192.168.43.6");
    serverAddress_->setFixedWidth(120);
    serverPort_ = new QLineEdit;
	serverPort_->setText("11231");
    serverPort_->setFixedWidth(40);
	fetchData_ = new QPushButton(tr(u8"获取数据"));

	// 顶部网络小部件 水平布局Layout
    auto* hbl = new QHBoxLayout();
    hbl->addWidget(new QLabel(u8"网络地址: "));
    hbl->addWidget(serverAddress_);
    hbl->addSpacing(20);
    hbl->addWidget(new QLabel(u8"端口: "));
	hbl->addWidget(serverPort_);
	hbl->addSpacing(20);
	hbl->addWidget(fetchData_);
    hbl->addStretch();

	// 主要内容部件
    stockBrowser_ = new StockBrowser(); // 股票浏览 小部件Widget
    stockHistory_ = new StockHistory(); // 股票历史行情 小部件Widget

	// 主要内容 栈布局Layout
    contextLayout_ = new QStackedLayout;
    contextLayout_->addWidget(stockBrowser_); // index: 0
    contextLayout_->addWidget(stockHistory_); // index: 1

	// 把主要内容包装成一个小部件 Widget
    context_ = new QWidget;
    context_->setLayout(contextLayout_);

	// 主窗体整体 垂直布局Layout
	// 这个整体布局由一个[网络部件布局Layout]和一个[主要内容部件Widget]组成
    auto* layout = new QVBoxLayout();
    layout->addItem(hbl);
    layout->addWidget(context_);

	// 将主窗体的整体布局包装到一个Widget里面
    auto* centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    
	// 使这个Widget居中
	this->setCentralWidget(centralWidget);
	
    connect(stockBrowser_, &StockBrowser::viewHistory, this, &MainWindow::slotViewHistory);
    connect(stockHistory_, &StockHistory::viewFinish, this, &MainWindow::slotViewHistoryFinish);
	//connect(fetchData_, SIGNAL(clicked()), this, SLOT(handleFetchData()));
	connect(fetchData_, &QPushButton::clicked, this, &MainWindow::handleClick);
	connect(serverAddress_, &QLineEdit::returnPressed, this, &MainWindow::animateClick);
	connect(serverPort_, &QLineEdit::returnPressed, this, &MainWindow::animateClick);
}

void MainWindow::slotViewHistory(const QString& code) {
    contextLayout_->setCurrentIndex(1);
}

void MainWindow::slotViewHistoryFinish() {
    contextLayout_->setCurrentIndex(0);
}

void MainWindow::handleClick()
{
	std::string add = serverAddress_->text().toStdString();
	std::string port = serverPort_->text().toStdString();
	if (add == "" || port == "") {
		QMessageBox::warning(this, tr(u8"Waring"), tr(u8"请输入完整的IP地址和端口"), QMessageBox::Yes);
		return;
	}
	switch (contextLayout_->currentIndex())
	{
	case 0:
		stockBrowser_->fetchData(add, port);
		break;
	case 1:
		stockHistory_->fetchData(add, port);
		break;
	default:
		break;
	}
}

void MainWindow::animateClick()
{
	fetchData_->animateClick();
}