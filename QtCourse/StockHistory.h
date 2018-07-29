#pragma once

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QWidget>

QT_CHARTS_USE_NAMESPACE

class StockHistory : public QWidget
{
    Q_OBJECT
public:
    StockHistory(QWidget* parent = nullptr);
    ~StockHistory();
	void fetchData(std::string, std::string);

signals:
    void viewFinish();
	void viewCandle();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
	void createChart();
	QLineSeries *priceSeries;
	QLineSeries *avgPrice;
	QChart *chart;
};

