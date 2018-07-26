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
	//fetchData();
}

void StockBrowser::fetchData(string serverAdd, string serverPort) {

	io_service ios;
	ip::tcp::socket sock(ios);
	ip::tcp::endpoint ep(ip::address::from_string(serverAdd), 11230);
	sock.connect(ep);
	sock.write_some(buffer("f|"));

	//auto stockList = new QStringList();

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
		setItem(i, 0, new QTableWidgetItem("RZRK-" + rzrk));
		setItem(i, 1, new QTableWidgetItem(QString::fromStdString(tuple[2])));
		setItem(i, 2, new QTableWidgetItem(QString::fromStdString(tuple[7])));
		setItem(i, 3, new QTableWidgetItem(QString::fromStdString(tuple[17])));
		setItem(i, 4, new QTableWidgetItem(QString::fromStdString(tuple[20])));
		setItem(i, 5, new QTableWidgetItem(QString::fromStdString(tuple[21])));
		setItem(i, 6, new QTableWidgetItem(QString::fromStdString(tuple[22])));
	}
}

StockBrowser::~StockBrowser()
{
}

void StockBrowser::createUI() {
	
    //showGrid(); // Ĭ�Ͽ���
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setColumnCount(7);

    auto& heads = QStringList();
    heads.push_back(u8"��Լ����");
    heads.push_back(u8"��Լ����");
    heads.push_back(u8"ǰ��");
    heads.push_back(u8"��ͣ��");
    heads.push_back(u8"��ͣ��");
    heads.push_back(u8"�ɱ�");
    heads.push_back(u8"��ͨ��");

    setHorizontalHeaderLabels(heads);
	//this->horizontalHeader()->setStretchLastSection(true);
	setShowGrid(true); // ��ʾ����
	setSelectionBehavior(QAbstractItemView::SelectRows); // ͬʱѡ��һ��

	setRowCount(100);
	setItem(0, 0, new QTableWidgetItem("001"));

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
