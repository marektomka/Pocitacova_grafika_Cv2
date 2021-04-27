#pragma once
#include <QtWidgets>
class ViewerWidget :public QWidget {
	Q_OBJECT
private:
	QString name = "";
	QSize areaSize = QSize(0, 0);
	QImage* img = nullptr;
	QRgb* data = nullptr;
	QPainter* painter = nullptr;
	//QColor clr;
	int meth = 0;
	bool button = FALSE;

public:
	ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent = Q_NULLPTR);
	~ViewerWidget();
	void resizeWidget(QSize size);

	//Image functions
	bool setImage(const QImage& inputImg);
	QImage* getImage() { return img; };
	bool isEmpty();

	//Data functions
	QRgb* getData() { return data; }
	void setPixel(int x, int y, const QColor& color);
	void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
	bool isInside(int x, int y) { return (x >= 0 && y >= 0 && x < img->width() && y < img->height()) ? true : false; }

	//Get/Set functions
	QString getName() { return name; }
	void setName(QString newName) { name = newName; }

	void setPainter() { painter = new QPainter(img); }
	void setDataPtr() { data = reinterpret_cast<QRgb*>(img->bits()); }

	int getImgWidth() { return img->width(); };
	int getImgHeight() { return img->height(); };

	void clear(QColor color = Qt::white);

	void drawDDA(int x1, int y1, int x2, int y2,QColor col);
	void drawBresen(int x1, int y1, int x2, int y2, QColor col);
	void drawBresenCircle(int x1, int y1, int x2, int y2, QColor col);
	bool CyrusBeck();
	void Sutherland();
	void setEbody();
	void clearPush();
	void drawPolyg(QColor col, int method);
	void setPoints(QPoint bod);
	void setChPoints(QPoint bod, int index);
	void rightbutton();
	QVector<QPoint> body;
	QVector<QPoint> ebody;
	QVector<QPoint> newbody;

public slots:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
};