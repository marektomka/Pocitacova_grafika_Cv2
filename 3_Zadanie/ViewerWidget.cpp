#include   "ViewerWidget.h"
#include <math.h> 

ViewerWidget::ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setMouseTracking(true);
	name = viewerName;
	if (imgSize != QSize(0, 0)) {
		img = new QImage(imgSize, QImage::Format_ARGB32);
		img->fill(Qt::white);
		resizeWidget(img->size());
		setPainter();
		setDataPtr();
	}
	body.clear();
	ebody.clear();
}
ViewerWidget::~ViewerWidget()
{
	delete painter;
	delete img;
}
void ViewerWidget::resizeWidget(QSize size)
{
	this->resize(size);
	this->setMinimumSize(size);
	this->setMaximumSize(size);
}

//Image functions
bool ViewerWidget::setImage(const QImage& inputImg)
{
	if (img != nullptr) {
		delete img;
	}
	img = new QImage(inputImg);
	if (!img) {
		return false;
	}
	resizeWidget(img->size());
	setPainter();
	update();

	return true;
}
bool ViewerWidget::isEmpty()
{
	if (img->size() == QSize(0, 0)) {
		return true;
	}
	return false;
}

//Data function
void ViewerWidget::setPixel(int x, int y, const QColor& color)
{
	if (isInside(x, y)) {
		data[x + y * img->width()] = color.rgb();
	}
}
void ViewerWidget::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	if (isInside(x, y)) {
		QColor color(r, g, b);
		setPixel(x, y, color);
	}
}

void ViewerWidget::clear(QColor color)
{
	for (size_t x = 0; x < img->width(); x++)
	{
		for (size_t y = 0; y < img->height(); y++)
		{
			setPixel(x, y, color);
		}
	}
	update();
}


void ViewerWidget::drawDDA(int x1, int y1, int x2, int y2, QColor col)
{	

	double m = DBL_MAX;      // smernica
	int X, Y;               // pomocne pre vymenu suradnic
	double xo, yo;			// pomocne pre posunutie v danom smere
	
	m = (y2 - y1) / (double)(x2 - x1);
	m = abs(m);

	if (m <= 1)
	{
		if (x1 > x2)
		{
			X = x2; x2 = x1; x1 = X;
			Y = y2; y2 = y1; y1 = Y;
		}
		yo = y1;
		setPixel(x1, y1, col);

		do
		{
			x1 = x1 + 1;
			if (y1 < y2) {
				yo = yo + m;
			}
			else {
				yo = yo - m;
			}
			setPixel(x1, static_cast<int>(yo + 0.5), col);
		} while (x1 < x2);

	}
	else if (m >= 1)
	{
		if (y1 > y2)
		{
			Y = y2; y2 = y1; y1 = Y;
			X = x2; x2 = x1; x1 = X;

		}
		xo = x1;
		setPixel(x1, y1, col);

		do
		{
			y1 = y1 + 1;
			if (x1 < x2) {
				xo = xo + (1 / m);
			}
			else {
				xo = xo - (1 / m);
			}
			setPixel(static_cast<int>(xo + 0.5), y1, col);
		} while (y1 < y2);
	}
	update();
}

void ViewerWidget::drawBresen(int x1, int y1, int x2, int y2, QColor col)
{
	int k1, k2, p1;
	int xo, yo;
	double m = DBL_MAX;
	int X, Y;

	m = (y2 - y1) / (double)(x2 - x1);

	if (m > 0 && m < 1) 
	{
		if (x1 > x2) 
		{
			X = x2; x2 = x1; x1 = X;
			Y = y2; y2 = y1; y1 = Y;
		}
		k1 = 2 * (y2 - y1);
		k2 = k1 - 2 * (x2 - x1);
		p1 = k1 - (x2 - x1);
		setPixel(x1, y1, col);

		do
		{
			x1 += 1;
			if (p1 > 0) {
				p1 = p1 + k2;
				if (y1 < y2) {
					y1 += 1;
				}
				else {
				y1 -= 1;
				}
			}
			else {
				p1 = p1 + k1;
			}
			setPixel(x1, y1, col);

		} while (x1 < x2);
	}
	else if (m > -1 && m <= 0) 
	{
		if (x1 > x2)
		{
			X = x2; x2 = x1; x1 = X;
			Y = y2; y2 = y1; y1 = Y;
		}
		k1 = 2 * (y2 - y1);
		k2 = k1 + 2 * (x2 - x1);
		p1 = k1 + (x2 - x1);
		setPixel(x1, y1, col);

		do
		{
			x1 += 1;
			if (p1 < 0) {
				y1 -= 1;
				p1 = p1 + k2;
			}
			else {
				p1 = p1 + k1;
			}
			setPixel(x1, y1, col);

		} while (x1 < x2);

	}
	else if (m >= 1) 
	{
		if (y1 > y2)
		{
			X = x2; x2 = x1; x1 = X;
			Y = y2; y2 = y1; y1 = Y;
		}
		k1 = 2 * (x2 - x1);
		k2 = k1 - 2 * (y2 - y1);
		p1 = k1 - (y2 - y1);
		setPixel(x1, y1, col);

		do
		{
			y1 += 1;
			if (p1 > 0) {
				p1 = p1 + k2;
				if (y1 < y2) {
					x1 += 1;
				}
				else {
					x1 -= 1;
				}
			}
			else {
				p1 = p1 + k1;
			}
			setPixel(x1, y1, col);
		} while (y1 < y2);

	}
	else if (m <= -1)
	{
		if (y1 > y2)
		{
			X = x2; x2 = x1; x1 = X;
			Y = y2; y2 = y1; y1 = Y;
		}

		k1 = 2 * (x2 - x1);
		k2 = k1 + 2 * (y2 - y1);
		p1 = k1 + (y2 - y1);
		setPixel(x1, y1, col);

		do
		{
			y1 += 1;
			if (p1 < 0) {
				x1 -= 1;
				p1 = p1 + k2;
			}
			else {
				p1 = p1 + k1;
			}
			setPixel(x1, y1, col);

		} while (y1 < y2);

	}
	update();
}

void ViewerWidget::drawBresenCircle(int x1, int y1, int x2, int y2, QColor col)
{
	int r = 0;
	int p1;
	int X, Y,dvaX,dvaY;
	r = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	p1 = 1 - r;
	X = 0;
	Y = r ;
	dvaX = 3;
	dvaY = 2 * r + 2;
	do
	{
		setPixel(X+x1, Y+y1, col);
		setPixel(-X + x1, Y + y1, col);
		setPixel(X + x1, -Y + y1, col);
		setPixel(-X + x1, -Y + y1, col);
		setPixel(Y + x1,X + y1, col);
		setPixel(-Y + x1, X + y1, col);
		setPixel(Y + x1, -X + y1, col);
		setPixel(-Y + x1, -X + y1, col);
		if (p1 > 0) {
			p1 = p1 - dvaY;
			Y -= 1;
			dvaY -= 2;
		}
		p1 = p1 + dvaX;
		dvaX += 2;
		X += 1;
		

	} while (X<=Y);

	update();
}

bool ViewerWidget::CyrusBeck()
{	
	newbody.clear();
	setEbody();
	bool out = true;
	for (size_t i = 0; i < 4; i++)
	{
		QPoint n = QPoint(-(ebody[i].y() - ebody[i + 1].y()), ebody[i].x() - ebody[i + 1].x());
		QPoint pMe1 = body[0] - ebody[i];
		QPoint pMe2 = body[1] - ebody[i];

		int nXp1 = (n.x() * pMe1.x() + n.y() * pMe1.y());
		int nXp2 = (n.x() * pMe2.x() + n.y() * pMe2.y());
		if (nXp1 > 0 || nXp2 > 0){ out = true;}
		else { out = false; break; }
	}

	
	if (out) return false;
	else {
		QPoint d;
		double tl = 0, tu = 1;
		d = body[1] - body[0];
		for (size_t i = 0; i < 4; i++)
		{

			QPoint n = QPoint(-(ebody[i + 1].y() - ebody[i].y()), ebody[i + 1].x() - ebody[i].x());
			QPoint w = body[0] - ebody[i];


			int dn = d.x() * n.x() + d.y() * n.y();
			int wn = w.x() * n.x() + w.y() * n.y();
			if (dn != 0) {
				double t = -wn / static_cast<double>(dn);

				if (dn > 0 && t <= 1) {
					if (t > tl) {	// max
						tl = t;
					}
				}
				if (dn < 0 && t >= 0) {
					if (t < tu) {   // min
						tu = t;
					}
				}
			}
		}
		if (tl == 0 && tu == 1) {
			newbody.push_back(QPoint(body[0]));
			newbody.push_back(QPoint(body[1]));
			
		}
		else if (tl < tu) {
			newbody.push_back(body[0] + QPoint(d * tl));
			newbody.push_back(body[0] + QPoint(d * tu));
		
		}
		return true;

	}


}

void ViewerWidget::Sutherland()
{
	newbody.clear();
	QVector<QPoint> V = body;
	QVector<QPoint> W;
	int minX[4]{ 0,0,(-1)*(img->width() - 1),(-1)*(img->height() - 1)};

	if (V.size() > 2) {

		for (int i = 0; i < 4; i++) {
			QPoint S = V[V.size() - 1];
			int Xmin = minX[i];

			for  (int j = 0; j < V.size(); j++){
				if (V[j].x() >= Xmin) {
					if (S.x() >= Xmin){
						W.append(V[j]);
					}
					else {
						QPoint P(Xmin, static_cast<int>(S.y() + (Xmin - S.x()) * ((V[j].y() - S.y()) / static_cast<double>(V[j].x() - S.x()))));
						W.append(P);
						W.append(V[j]);
					}
				}
				else 
				{
					if (S.x() >= Xmin) {
						QPoint P(Xmin, static_cast<int>(S.y() + (Xmin - S.x()) * ((V[j].y() - S.y()) / static_cast<double>(V[j].x() - S.x()))));
						W.append(P);
					}
				}
				S = V[j];
			}
			V = W;
			W.clear();

			for (int j = 0; j < V.size(); j++) {
				V[j] = QPoint(V[j].y(), V[j].x() * (-1));
			}
		}
		newbody = V;	
	}
}

void ViewerWidget::setEbody()
{
	ebody.clear();
	ebody.push_back(QPoint(0, 0));	
	ebody.push_back(QPoint(getImgWidth()-1, 0));
	ebody.push_back(QPoint(getImgWidth()-1, getImgHeight()-1));
	ebody.push_back(QPoint(0, getImgHeight()-1));
	ebody.push_back(QPoint(0, 0));

	
}

void ViewerWidget::clearPush()
{
	body.clear();
	newbody.clear();
	button = FALSE;
}

void ViewerWidget::drawPolyg(QColor col, int method)
{
	clear();
	meth = method;
	
	if (body.size() == 2)
	{
		CyrusBeck();
		if (method == 0) {
			drawDDA(newbody[0].x(), newbody[0].y(), newbody[1].x(), newbody[1].y(), col);
		}
		else if (method == 1) {
			drawBresen(newbody[0].x(), newbody[0].y(), newbody[1].x(), newbody[1].y(), col);
		}
	}
	else if (body.size() > 2) {
		Sutherland();
		for (size_t i = 0; i < newbody.size()-1; i++)   
		{
			if (method == 0) {
				drawDDA(newbody[i].x(),newbody[i].y(), newbody[i + 1].x(), newbody[i + 1].y(), col);
			}else if (method == 1) {
				drawBresen(newbody[i].x(), newbody[i].y(), newbody[i + 1].x(), newbody[i + 1].y(), col);
			}	
		}
		if (button == TRUE) {
			if (method == 0) {
				drawDDA(newbody[newbody.size() - 1].x(), newbody[newbody.size() - 1].y(), newbody[0].x(), newbody[0].y(), col);
			}else {
				drawBresen(newbody[newbody.size() - 1].x(), newbody[newbody.size() - 1].y(), newbody[0].x(), newbody[0].y(), col);
			}
		}
	}
}

void ViewerWidget::setPoints(QPoint bod)
{
	body.push_back(bod);
}

void ViewerWidget::setChPoints(QPoint bod, int index)
{
	body[index] = bod;
}

void ViewerWidget::rightbutton()
{
	//body.push_back(QPoint(body.first()));
	button =  TRUE;
}

//Slots
void ViewerWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}