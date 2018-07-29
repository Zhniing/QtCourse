#pragma once

#include <QtWidgets/QTableWidget>
//#include <QtGui>
#include <QtWidgets/qstyleditemdelegate.h>
#include <QtWidgets/qitemdelegate.h>
//QStyledItemDelegate

class StockBrowser : public QTableWidget
{
    Q_OBJECT
public:
    StockBrowser(QWidget* parent= nullptr);
	~StockBrowser();

signals:
    void viewHistory(const QString& code);

public slots:
	void fetchData(std::string, std::string);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void createUI();
};


class ItemDelegate : public QItemDelegate
{
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};