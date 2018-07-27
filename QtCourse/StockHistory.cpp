#pragma warning(disable: 4996)
#include "StockHistory.h"

#include <QtCharts/QLineSeries>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

using namespace boost::asio;
using namespace std;

StockHistory::StockHistory(QWidget* parent) : QChartView(parent)
{
    createChart();
}

void StockHistory::fetchData(string add, string port) {
	priceSeries = new QLineSeries();
	avgPrice = new QLineSeries();
	auto* dealSet = new QBarSet("成交量");

	io_service ios;
	ip::tcp::socket sock(ios);
	ip::tcp::endpoint ep(ip::address::from_string(add), 11230);
	sock.connect(ep);
	sock.write_some(buffer("h|"));

	int preDeal = 0; // 记录前一次的总成交量
	for (int i = 0; i < 100; i++) {
		ip::tcp::iostream tcp_stream(add, port);
		string buf;
		getline(tcp_stream, buf);

		vector<string> tuple;
		boost::algorithm::split(tuple, buf, boost::algorithm::is_any_of(","));
		
		double temp = stod(tuple[0]);
		qreal y = temp;
		QPointF p(i, y);
		priceSeries->append(p);

		temp = stod(tuple[2]) / stod(tuple[1]) / 100;
		qreal y2 = temp;
		QPointF p2(i, y2);
		avgPrice->append(p2);

		if (i > 0) {
			int t = stoi(tuple[1]) - preDeal;
			qreal b = t;
			dealSet->append(b);
		}
		preDeal = stoi(tuple[1]); // 记录上一次的总成交量
	}

	chart = new QChart();
	chart->legend()->hide();
	chart->addSeries(priceSeries);
	chart->addSeries(avgPrice);
	chart->createDefaultAxes();
	chart->setTitle(u8"分时图");
	chart->axisY()->setRange(8.7, 8.76);

	//setChart(chart);
	//setRenderHint(QPainter::Antialiasing); // 抗锯齿

	auto* dealSeries = new QBarSeries();
	dealSeries->append(dealSet);
	auto* dealChart = new QChart();
	dealChart->addSeries(dealSeries);
	dealChart->setTitle(tr(u8"成交量"));

	auto* chartLayout = new QGridLayout();
	auto* c = new QChartView(chart);
	c->setRenderHint(QPainter::Antialiasing);
	chartLayout->addWidget(c, 0, 0);
	//chartLayout->addWidget(new QChartView(dealChart), 0, 0);

	setLayout(chartLayout);
}

void StockHistory::createChart(){
	priceSeries = new QLineSeries();

	priceSeries->append(0, 6);
	priceSeries->append(2, 4);
	priceSeries->append(3, 8);
	priceSeries->append(7, 4);
	priceSeries->append(10, 5);
    *priceSeries << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(priceSeries);
    chart->createDefaultAxes();
    chart->setTitle(u8"历史行情");

    //setChart(chart);
    setRenderHint(QPainter::Antialiasing); // 抗锯齿
}

StockHistory::~StockHistory()
{
}

void StockHistory::keyPressEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)) {
        emit viewFinish();
    }
}
