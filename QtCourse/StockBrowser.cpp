#pragma warning(disable: 4996)
#include "StockBrowser.h"

#include <QtGUI/QKeyEvent>
#include <QtWidgets/QMessageBox>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

using namespace boost::asio;
using namespace std;

StockBrowser::StockBrowser(QWidget* parent) : QTableWidget(parent)
{
    createUI();
}

void StockBrowser::fetchData(string serverAdd, string serverPort) {

	io_service ios;
	ip::tcp::socket sock(ios);
	ip::tcp::endpoint ep(ip::address::from_string(serverAdd), 11230);
	sock.connect(ep);
	int curCount = rowCount();
	string cmd = "b|" + to_string(curCount);
	sock.write_some(buffer(cmd));

	for (int i = 0; i < 100; i++) {
		ip::tcp::iostream tcp_stream(serverAdd, serverPort);
		string buf;
		getline(tcp_stream, buf);

		vector<string> tuple;
		boost::algorithm::split(tuple, buf, boost::algorithm::is_any_of(","));
		QString rzrk = QString::fromStdString(tuple[11]);
		while (rzrk.length() < 6) {
			rzrk = "0" + rzrk;
		}

		int row = rowCount();
		insertRow(row);
		setItem(row, 0, new QTableWidgetItem("RZRK-" + rzrk));
		setItem(row, 1, new QTableWidgetItem(QString::fromStdString(tuple[2])));
		setItem(row, 2, new QTableWidgetItem(QString::fromStdString(tuple[7])));
		setItem(row, 3, new QTableWidgetItem(QString::fromStdString(tuple[17])));
		setItem(row, 4, new QTableWidgetItem(QString::fromStdString(tuple[20])));
		setItem(row, 5, new QTableWidgetItem(QString::fromStdString(tuple[21])));
		setItem(row, 6, new QTableWidgetItem(QString::fromStdString(tuple[22])));
	}
}

StockBrowser::~StockBrowser()
{
}

void StockBrowser::createUI() {
	
    //showGrid(); // 默认开启
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setColumnCount(7);

    auto& heads = QStringList();
    heads.push_back(u8"合约代码");
    heads.push_back(u8"合约名称");
    heads.push_back(u8"前收");
    heads.push_back(u8"涨停价");
    heads.push_back(u8"跌停价");
    heads.push_back(u8"股本");
    heads.push_back(u8"流通盘");

    setHorizontalHeaderLabels(heads);
	//this->horizontalHeader()->setStretchLastSection(true);
	setShowGrid(true); // 显示格子
	setSelectionBehavior(QAbstractItemView::SelectRows); // 同时选择一行

	//setItem(0, 0, new QTableWidgetItem("001"));

	//insertRow(1);
}

void StockBrowser::keyPressEvent(QKeyEvent *event) {
    QTableWidget::keyPressEvent(event);

    if (rowCount() <= 0)
        return;

    auto row = currentRow();
    auto* codeItem = item(row, 0);
    if (codeItem && (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)) {
        emit viewHistory(codeItem->text());
    }
}
