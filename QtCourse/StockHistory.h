#pragma once

#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

class StockHistory : public QChartView
{
    Q_OBJECT
public:
    StockHistory(QWidget* parent = nullptr);
    ~StockHistory();

signals:
    void viewFinish();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void createChart();
};

