#pragma once

#include <QtWidgets/QTableWidget>

class StockBrowser : public QTableWidget
{
    Q_OBJECT
public:
    StockBrowser(QWidget* parent= nullptr);
    ~StockBrowser();

signals:
    void viewHistory(const QString& code);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void createUI();
};

