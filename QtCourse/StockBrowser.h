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

public slots:
	void fetchData(std::string, std::string);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void createUI();
};

