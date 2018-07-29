#pragma warning(disable: 4996)
#include "StockBrowser.h"
//#include "ItemDelegate.h"

#include <QtGUI/QKeyEvent>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/qtableview.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qscrollbar.h>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

using namespace boost::asio;
using namespace std;

StockBrowser::StockBrowser(QWidget* parent) : QTableWidget(parent)
{
    createUI();
}

void StockBrowser::fetchData(string serverAdd, string serverPort) {

	io_service ios;
	ip::tcp::socket sock(ios);
	ip::tcp::endpoint ep(ip::address::from_string(serverAdd), 11230);
	sock.connect(ep);
	int curCount = rowCount();
	string cmd = "b|" + to_string(curCount);
	sock.write_some(buffer(cmd));

	for (int i = 0; i < 100; i++) {
		ip::tcp::iostream tcp_stream(serverAdd, serverPort);
		string buf;
		getline(tcp_stream, buf);

		vector<string> tuple;
		boost::algorithm::split(tuple, buf, boost::algorithm::is_any_of(","));
		QString rzrk = QString::fromStdString(tuple[11]);
		while (rzrk.length() < 6) {
			rzrk = "0" + rzrk;
		}

		int row = rowCount();
		insertRow(row); // ���һ�У��к�Ϊrow

		auto* item = new QTableWidgetItem("RZRK-" + rzrk);
		item->setForeground(QBrush(QColor(Qt::yellow)));
		item->setTextAlignment(Qt::AlignCenter);
		setItem(row, 0, item);

		item = new QTableWidgetItem(QString::fromStdString(tuple[2]));
		item->setForeground(QBrush(QColor(Qt::yellow)));
		item->setTextAlignment(Qt::AlignCenter);
		setItem(row, 1, item);

		item = new QTableWidgetItem(QString::fromStdString(tuple[7]));
		item->setForeground(QBrush(QColor(Qt::white)));
		item->setTextAlignment(Qt::AlignCenter);
		setItem(row, 2, item);

		item = new QTableWidgetItem(QString::fromStdString(tuple[17]));
		item->setForeground(QBrush(QColor(Qt::red)));
		item->setTextAlignment(Qt::AlignCenter);
		setItem(row, 3, item);

		item = new QTableWidgetItem(QString::fromStdString(tuple[20]));
		item->setForeground(QBrush(QColor(Qt::green)));
		item->setTextAlignment(Qt::AlignCenter);
		setItem(row, 4, item);

		item = new QTableWidgetItem(QString::fromStdString(tuple[21]));
		item->setForeground(QBrush(QColor(Qt::cyan)));
		item->setTextAlignment(Qt::AlignCenter);
		setItem(row, 5, item);

		item = new QTableWidgetItem(QString::fromStdString(tuple[22]));
		item->setForeground(QBrush(QColor(Qt::white)));
		item->setTextAlignment(Qt::AlignCenter);
		setItem(row, 6, item);

		//model = new QStandardItemModel;
		//setModel(model);
		
	}
}

StockBrowser::~StockBrowser()
{
}

void StockBrowser::createUI() {
	
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setColumnCount(7);

    auto& heads = QStringList();
    heads.push_back(u8"��Լ����");
    heads.push_back(u8"��Լ����");
    heads.push_back(u8"����");
    heads.push_back(u8"��ͣ��");
    heads.push_back(u8"��ͣ��");
    heads.push_back(u8"�ɱ�");
    heads.push_back(u8"��ͨ��");

    setHorizontalHeaderLabels(heads);
	//horizontalHeader()->setStretchLastSection(true); // �����һ����չ
	for (int i = 0; i < 7; i++) {
		// ��ÿһ�ж���չ
		horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
	}

	horizontalHeader()->setSectionsClickable(false); // ���ñ�ͷ���ɵ��
	horizontalHeader()->setStyleSheet("QHeaderView::section{background: #121218; color: white; border: none;}"); // ���ñ�ͷ����ɫ
	horizontalHeader()->setHighlightSections(false); // �����ʱ���Ա�ͷ�й�������ȡ���㣩
	verticalHeader()->hide(); // ���ش�ֱ��ͷ

	setSelectionBehavior(QAbstractItemView::SelectRows); // ͬʱѡ��һ��
	setSelectionMode(QAbstractItemView::SingleSelection); // ���ܶ�ѡ
	setShowGrid(false); // ����ʾ����
	setFrameShape(QFrame::NoFrame); // �����ޱ߿�
	//setFocusPolicy(Qt::NoFocus); //ȥ��ѡ�����߿򣬲���ȡ����
	setItemDelegate(new ItemDelegate()); //ȥ��ѡ�����߿� & ѡ�����ı�����ɫ
	
	setStyleSheet("QTableView{background-color: #121218; selection-background-color: purple;}"); // ���ñ����ʽ
	
	//setEditTriggers(QAbstractItemView::NoEditTriggers); // ���ñ༭
	//setVerticalScrollMode(QAbstractItemView::ScrollPerPixel); // �����ع���

	//verticalScrollBar()->setStyleSheet("QScrollBar::add - page:vertical, QScrollBar::sub - page : vertical{background:rgba(18, 18, 24, 100% );}");
	
	// �Զ��������
	//verticalScrollBar()->setStyleSheet("QScrollBar{background: transparent; width:12px;}"
	//	"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:0px;}"
	//	"QScrollBar::handle:hover{background:gray;}"
	//	"QScrollBar::sub-line{background:transparent;}"
	//	"QScrollBar::add-line{background:transparent;}");

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

void ItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{

	QStyleOptionViewItem itemOption(option);

	// ȥ��ѡ�е����߿򣬲�������ȡ����
	if (itemOption.state & QStyle::State_HasFocus) {
		itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
	}

	// ����ѡ������ı�����ɫ
	itemOption.palette.setColor(QPalette::HighlightedText, index.data(Qt::ForegroundRole).value<QColor>());

	QItemDelegate::paint(painter, itemOption, index);

}