#pragma warning(disable: 4996)
#include "Candlestick.h"

#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QVBoxLayout>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

using namespace boost::asio;
using namespace std;

Candlestick::Candlestick(QWidget* parent) : QWidget(parent)
{
	//fetchData("127.0.0.1", "11231");
}

Candlestick::~Candlestick()
{
}

void Candlestick::fetchData(string add, string port) {
	QCandlestickSeries *candleSeries = new QCandlestickSeries();
	QStringList categories;

	io_service ios;
	ip::tcp::socket sock(ios);
	ip::tcp::endpoint ep(ip::address::from_string(add), 11230);
	sock.connect(ep);
	sock.write_some(buffer("k|"));

	for (int i = 0; i < 50; i++) {
		ip::tcp::iostream tcp_stream(add, port);
		string buf;
		getline(tcp_stream, buf);

		vector<string> tuple;
		boost::algorithm::split(tuple, buf, boost::algorithm::is_any_of(","));

		string datetime = tuple[10];
		vector<string> datetimeVec;
		boost::algorithm::split(datetimeVec, datetime, boost::algorithm::is_any_of(" "));

		vector<string> date;
		vector<string> time;
		boost::algorithm::split(date, datetimeVec[1], boost::algorithm::is_any_of("-"));
		boost::algorithm::split(time, datetimeVec[2], boost::algorithm::is_any_of(":"));

		QDateTime momentInTime(QDate(stoi(date[0]), stoi(date[1]), stoi(date[2])),
							QTime(stoi(time[0]), stoi(time[1]), stoi(time[2])));

		qreal timestamp = momentInTime.toMSecsSinceEpoch();
		qreal open = stod(tuple[0]);
		qreal high = stod(tuple[1]);
		qreal low = stod(tuple[2]);
		qreal close = stod(tuple[3]);

		QCandlestickSet *candlestickSet = new QCandlestickSet(open, high, low, close, timestamp);

		candleSeries->append(candlestickSet);
		categories << QDateTime::fromMSecsSinceEpoch(candlestickSet->timestamp()).toString("h:m:s");
	}

	candleSeries->setIncreasingColor(QColor(Qt::red));
	candleSeries->setDecreasingColor(QColor(Qt::green));

	QChart *chart = new QChart();
	chart->addSeries(candleSeries);
	chart->setTitle(u8"ÈÕK");
	chart->setAnimationOptions(QChart::SeriesAnimations);
	chart->legend()->hide();
	chart->setTheme(QChart::ChartThemeDark);

	chart->createDefaultAxes();
	chart->axisX()->hide();

	QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
	axisX->setCategories(categories);
	//axisX->setTickCount(5);

	//QDateTimeAxis *axisX = new QDateTimeAxis;
	//axisX->setTickCount(10);
	//axisX->setFormat("h:m:s");
	//chart->addAxis(axisX, Qt::AlignBottom);

	QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
	axisY->setMax(axisY->max() * 1.0001);
	axisY->setMin(axisY->min() * 0.9999);
	//axisY->setTickCount(5);

	QChartView *chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(chartView);

	setLayout(layout);
}

void Candlestick::keyPressEvent(QKeyEvent *event) {
	if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)) {
		emit viewHistory();
	}
	else if (event->key() == Qt::Key_Escape) {
		emit viewFinish();
	}
}

//void CandlestickDataReader::readFile(QIODevice *device)
//{
//	QTextStream::setDevice(device);
//}

//QCandlestickSet *CandlestickDataReader::readCandlestickSet()
//{
//	//! [1]
//	QString line = readLine();
//	if (line.startsWith("#") || line.isEmpty())
//		return 0;
//	//! [1]
//
//	//! [2]
//	QStringList strList = line.split(" ", QString::SkipEmptyParts);
//	if (strList.count() != 5)
//		return 0;
//	//! [2]
//
//	//! [3]
//	const qreal timestamp = strList.at(0).toDouble();
//	const qreal open = strList.at(1).toDouble();
//	const qreal high = strList.at(2).toDouble();
//	const qreal low = strList.at(3).toDouble();
//	const qreal close = strList.at(4).toDouble();
//	//! [3]
//
//	//! [4]
//	QCandlestickSet *candlestickSet = new QCandlestickSet(timestamp);
//	candlestickSet->setOpen(open);
//	candlestickSet->setHigh(high);
//	candlestickSet->setLow(low);
//	candlestickSet->setClose(close);
//	//! [4]
//
//	return candlestickSet;
//}
