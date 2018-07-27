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

	// �������С
    resize(800, 600);

	// �������� С����Widget
    serverAddress_ = new QLineEdit;
	serverAddress_->setText("192.168.43.6");
    serverAddress_->setFixedWidth(120);
    serverPort_ = new QLineEdit;
	serverPort_->setText("11231");
    serverPort_->setFixedWidth(40);
	fetchData_ = new QPushButton(tr(u8"��ȡ����"));

	// ��������С���� ˮƽ����Layout
    auto* hbl = new QHBoxLayout();
    hbl->addWidget(new QLabel(u8"�����ַ: "));
    hbl->addWidget(serverAddress_);
    hbl->addSpacing(20);
    hbl->addWidget(new QLabel(u8"�˿�: "));
	hbl->addWidget(serverPort_);
	hbl->addSpacing(20);
	hbl->addWidget(fetchData_);
    hbl->addStretch();

	// ��Ҫ���ݲ���
    stockBrowser_ = new StockBrowser(); // ��Ʊ��� С����Widget
    stockHistory_ = new StockHistory(); // ��Ʊ��ʷ���� С����Widget

	// ��Ҫ���� ջ����Layout
    contextLayout_ = new QStackedLayout;
    contextLayout_->addWidget(stockBrowser_); // index: 0
    contextLayout_->addWidget(stockHistory_); // index: 1

	// ����Ҫ���ݰ�װ��һ��С���� Widget
    context_ = new QWidget;
    context_->setLayout(contextLayout_);

	// ���������� ��ֱ����Layout
	// ������岼����һ��[���粿������Layout]��һ��[��Ҫ���ݲ���Widget]���
    auto* layout = new QVBoxLayout();
    layout->addItem(hbl);
    layout->addWidget(context_);

	// ������������岼�ְ�װ��һ��Widget����
    auto* centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    
	// ʹ���Widget����
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
		QMessageBox::warning(this, tr(u8"Waring"), tr(u8"������������IP��ַ�Ͷ˿�"), QMessageBox::Yes);
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