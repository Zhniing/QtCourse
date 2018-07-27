#pragma once

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

class StockHistory : public QChartView
{
    Q_OBJECT
public:
    StockHistory(QWidget* parent = nullptr);
    ~StockHistory();
	void fetchData(std::string, std::string);

signals:
    void viewFinish();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
	void createChart();
	QLineSeries *priceSeries;
	QLineSeries *avgPrice;
	QChart *chart;
};

