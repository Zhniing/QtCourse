#include "StockHistory.h"

#include <QtCharts/QLineSeries>


StockHistory::StockHistory(QWidget* parent) : QChartView(parent)
{
    createChart();
}

void StockHistory::createChart(){
    QLineSeries *series = new QLineSeries();

    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle(u8"历史行情");

    setChart(chart);
    setRenderHint(QPainter::Antialiasing);
}

StockHistory::~StockHistory()
{
}

void StockHistory::keyPressEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)) {
        emit viewFinish();
    }
}
