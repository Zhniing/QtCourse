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
    heads.push_back(u8"��Լ����");
    heads.push_back(u8"��Լ����");
    heads.push_back(u8"ǰ��");
    heads.push_back(u8"��ͣ��");
    heads.push_back(u8"��ͣ��");
    heads.push_back(u8"�ɱ�");
    heads.push_back(u8"��ͨ��");

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
