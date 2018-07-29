#pragma once

//#ifndef CANDLESTICKDATAREADER_H
//#define CANDLESTICKDATAREADER_H

#include <QtCharts/QCandlestickSet>
#include <QtWidgets/QWidget>
//#include <QtCore/QTextStream>

//QT_CHARTS_USE_NAMESPACE

using namespace std;

QT_CHARTS_USE_NAMESPACE

class Candlestick : public QWidget
{
	Q_OBJECT
public:
	explicit Candlestick(QWidget* parent = nullptr);
	~Candlestick();

signals:
	void viewFinish();
	void viewHistory();

public slots:
	void fetchData(string add, string port);

protected:
	void keyPressEvent(QKeyEvent *event) override;

	//void readFile(QIODevice *device);
	//QCandlestickSet *readCandlestickSet();
};

//#endif // CANDLESTICKDATAREADER_H
