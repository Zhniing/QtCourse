#pragma warning(disable: 4996)
#include "StockHistory.h"

#include <QtCharts/QLineSeries>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

using namespace boost::asio;
using namespace std;

StockHistory::StockHistory(QWidget* parent) : QWidget(parent)
{
    //createChart();
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
	for (int i = 0; i < 200; i++) {
		ip::tcp::iostream tcp_stream(add, port);
		string buf;
		getline(tcp_stream, buf);

		vector<string> tuple;
		boost::algorithm::split(tuple, buf, boost::algorithm::is_any_of(","));

		string datetime = tuple[9];
		vector<string> datetimeVec;
		boost::algorithm::split(datetimeVec, datetime, boost::algorithm::is_any_of(" "));

		vector<string> date;
		vector<string> time;
		boost::algorithm::split(date, datetimeVec[1], boost::algorithm::is_any_of("-"));
		boost::algorithm::split(time, datetimeVec[2], boost::algorithm::is_any_of(":"));

		QDateTime momentInTime(QDate(stoi(date[0]), stoi(date[1]), stoi(date[2])), QTime(stoi(time[0]), stoi(time[1]), stoi(time[2])));

		double value = stod(tuple[0]);
		qreal y = value;
		QPointF p(momentInTime.toMSecsSinceEpoch(), y);
		priceSeries->append(p);

		value = stod(tuple[2]) / stod(tuple[1]) / 100;
		qreal y2 = value;
		QPointF p2(momentInTime.toMSecsSinceEpoch(), y2);
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
	chart->axisX()->hide();
	chart->setMinimumHeight(300);

	chart->setTheme(QChart::ChartThemeDark);
	priceSeries->setColor(Qt::white);
	avgPrice->setColor(Qt::yellow);

	QDateTimeAxis *axisX = new QDateTimeAxis;
	axisX->setTickCount(10);
	axisX->setFormat("h:m:s");
	//axisX->setTitleText("Date");
	chart->addAxis(axisX, Qt::AlignBottom);
	priceSeries->attachAxis(axisX);
	avgPrice->attachAxis(axisX);

	//setChart(chart);
	//setRenderHint(QPainter::Antialiasing); // 抗锯齿

	auto* dealSeries = new QBarSeries();
	dealSeries->append(dealSet);
	auto* dealChart = new QChart();
	dealChart->addSeries(dealSeries);
	dealChart->setTitle(tr(u8"成交量"));
	dealChart->createDefaultAxes();
	dealChart->legend()->hide();
	dealChart->axisX()->hide();
	dealChart->setMinimumHeight(300);
	dealChart->setTheme(QChart::ChartThemeDark);
	dealChart->setAnimationOptions(QChart::SeriesAnimations);
	//dealChart->addAxis(axisX, Qt::AlignBottom);
	//dealSeries->attachAxis(axisX);

	auto* chartLayout = new QGridLayout();
	auto* c = new QChartView(chart);
	c->setRenderHint(QPainter::Antialiasing);
	chartLayout->addWidget(c, 0, 0);
	chartLayout->addWidget(new QChartView(dealChart), 1, 0);

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
	//setRenderHint(QPainter::Antialiasing); // 抗锯齿
}

StockHistory::~StockHistory()
{
}

void StockHistory::keyPressEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)) {
        emit viewFinish();
    }
}
