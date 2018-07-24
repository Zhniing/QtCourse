#include "StockBrowser.h"

#include <QtGUI/QKeyEvent>

StockBrowser::StockBrowser(QWidget* parent) : QTableWidget(parent)
{
    createUI();
}


StockBrowser::~StockBrowser()
{
}

void StockBrowser::createUI() {

    showGrid();
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

    setRowCount(1);

    setItem(0, 0, new QTableWidgetItem("RZRK-001"));
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
