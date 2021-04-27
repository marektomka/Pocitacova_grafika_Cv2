#include "ImageViewer.h"

ImageViewer::ImageViewer(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::ImageViewerClass)
{
	ui->setupUi(this);
	int width = 580;
	int height = 500;
	QString name = "default";
	openNewTabForImg(new ViewerWidget(name, QSize(width, height)));
	ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
	ui->pushButtonClear->setDisabled(TRUE);
	ui->groupBoxTransform->setDisabled(TRUE);
}

void ImageViewer::drawPoly()
{
	clearImage();

	for (size_t i = 0; i < points; i++)
	{
		QPoint b;
		b.setX(point[i].get_posX());
		b.setY(point[i].get_posY());
		getCurrentViewerWidget()->setChPoints(b, i);
		getCurrentViewerWidget()->drawPolyg(current, method);
	}
	update();
}

void ImageViewer::Symetry(int a, int b, int c)
{
	double Xs=0, Ys=0;
	int x=0, y=0;
	for (size_t i = 0; i < points; i++)
	{
		x = point[i].get_posX();
		y = point[i].get_posY();

		Xs = x - 2 * a * ((a * x + b * y + c) / (pow(a, 2) + pow(b, 2)));
		Ys = y - 2 * b * ((a * x + b * y + c) / (pow(a, 2) + pow(b, 2)));

		point[i].set_posX(Xs);
		point[i].set_posY(Ys);
	}
}

//ViewerWidget functions
ViewerWidget* ImageViewer::getViewerWidget(int tabId)
{
	QScrollArea* s = static_cast<QScrollArea*>(ui->tabWidget->widget(tabId));
	if (s) {
		ViewerWidget* vW = static_cast<ViewerWidget*>(s->widget());
		return vW;
	}
	return nullptr;
}
ViewerWidget* ImageViewer::getCurrentViewerWidget()
{
	return getViewerWidget(ui->tabWidget->currentIndex());
}

// Event filters
bool ImageViewer::eventFilter(QObject* obj, QEvent* event)
{
	if (obj->objectName() == "ViewerWidget") {
		return ViewerWidgetEventFilter(obj, event);
	}
	return false;
}

//ViewerWidget Events
bool ImageViewer::ViewerWidgetEventFilter(QObject* obj, QEvent* event)
{
	ViewerWidget* w = static_cast<ViewerWidget*>(obj);

	if (!w) {
		return false;
	}

	if (event->type() == QEvent::MouseButtonPress) {
		ViewerWidgetMouseButtonPress(w, event);
	}
	else if (event->type() == QEvent::MouseButtonRelease) {
		ViewerWidgetMouseButtonRelease(w, event);
	}
	else if (event->type() == QEvent::MouseMove) {
		ViewerWidgetMouseMove(w, event);
	}
	else if (event->type() == QEvent::Leave) {
		ViewerWidgetLeave(w, event);
	}
	else if (event->type() == QEvent::Enter) {
		ViewerWidgetEnter(w, event);
	}
	else if (event->type() == QEvent::Wheel) {
		ViewerWidgetWheel(w, event);
	}

	return QObject::eventFilter(obj, event);
}
void ImageViewer::ViewerWidgetMouseButtonPress(ViewerWidget* w, QEvent* event)
{

	QMouseEvent* e = static_cast<QMouseEvent*>(event);
	
	// kreslenie 2.Zadanie //
	/*if (e->button() == Qt::LeftButton) {	
	points++;
		if (points == 1) {
			point.push_back(MYPOINT(e->pos().x(), e->pos().y()));
			points++;
		}
		else {
			point.push_back(MYPOINT(e->pos().x(), e->pos().y()));
			
			if (ui->comboBoxType->currentIndex() == 0 && ui->comboBoxAlgorithm->currentIndex() == 0) {
				getCurrentViewerWidget()->drawDDA(point[0].get_posX(), point[0].get_posY(), point[1].get_posX(), point[1].get_posY(), current);
			}
			else if (ui->comboBoxType->currentIndex() == 0 && ui->comboBoxAlgorithm->currentIndex() == 1) {
				getCurrentViewerWidget()->drawBresen(point[0].get_posX(), point[0].get_posY(), point[1].get_posX(), point[1].get_posY(), current);
			}
			else if (ui->comboBoxType->currentIndex() == 1) {
				getCurrentViewerWidget()->drawBresenCircle(point[0].get_posX(), point[0].get_posY(), point[1].get_posX(), point[1].get_posY(), current);
			}
			points = 0;
			point.clear();
		}
	}*/

	// kreslenie 3.Zadanie // 
	if (e->button() == Qt::LeftButton && ready == TRUE) {
		if (polyg == TRUE) {
			points++;
			stPoint = e->pos();
		}
		else {
			point.clear();
		}

		
		if (points != 0) {
			point.push_back(MYPOINT(e->pos().x(), e->pos().y()));
			getCurrentViewerWidget()->setPoints(e->pos());
			if (points > 1) {
				drawPoly();
			}
		}
		else {
			point.clear();
		}

	}
	else if (e->button() == Qt::RightButton) {
		if (points >= 2) {
			getCurrentViewerWidget()->rightbutton();
			ready = FALSE;
			trans = TRUE;
			ui->groupBoxTransform->setEnabled(TRUE);
			
			drawPoly();
		}

	}
	else if (e->button() == Qt::LeftButton) {
		if (points >= 2 && ready == FALSE) {
			firstTClick = e->pos();
		}
	}
}


void ImageViewer::ViewerWidgetMouseButtonRelease(ViewerWidget* w, QEvent* event)
{
	QMouseEvent* e = static_cast<QMouseEvent*>(event);
	if (e->button() == Qt::LeftButton) {
		if (points >= 2 && ready == FALSE) {
			lastTClick = e->pos();
			trans = TRUE;
			vektor.set_posX(lastTClick.x() - firstTClick.x());
			vektor.set_posY(lastTClick.y() - firstTClick.y());
		}
	}
}
void ImageViewer::ViewerWidgetMouseMove(ViewerWidget* w, QEvent* event)
{
	
	QMouseEvent* e = static_cast<QMouseEvent*>(event);
	if (e->buttons() == Qt::LeftButton && trans == TRUE) {
		clearImage();

		for (size_t i = 0; i < points; i++)
		{
			point[i].set_posX(point[i].get_posX() + vektor.get_posX() + (e->pos().x() - firstTClick.x()));
			point[i].set_posY(point[i].get_posY() + vektor.get_posY() + (e->pos().y() - firstTClick.y()));
			
		}
		drawPoly();

		firstTClick = e->pos();
	}

}


void ImageViewer::ViewerWidgetLeave(ViewerWidget* w, QEvent* event)
{
}
void ImageViewer::ViewerWidgetEnter(ViewerWidget* w, QEvent* event)
{
}
void ImageViewer::ViewerWidgetWheel(ViewerWidget* w, QEvent* event)
{
	QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);

	double wheelScale = wheelEvent->angleDelta().y();
	double scaleW = 0;
	if (wheelScale > 0) {
		scaleW = 1.25;
	}
	else if (wheelScale < 0) {
		scaleW = 0.75;
	}


	clearImage();

	for (size_t i = 0; i < points; i++)
	{
		point[i].set_posX(point[0].get_posX() + ((point[i].get_posX() - point[0].get_posX()) * scaleW));
		point[i].set_posY(point[0].get_posY() + ((point[i].get_posY() - point[0].get_posY()) * scaleW));
	}

	drawPoly();

}

//ImageViewer Events
void ImageViewer::closeEvent(QCloseEvent* event)
{
	if (QMessageBox::Yes == QMessageBox::question(this, "Close Confirmation", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No))
	{
		event->accept();
	}
	else {
		event->ignore();
	}
}

//Image functions
void ImageViewer::openNewTabForImg(ViewerWidget* vW)
{
	QScrollArea* scrollArea = new QScrollArea;
	scrollArea->setWidget(vW);

	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidgetResizable(true);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	vW->setObjectName("ViewerWidget");
	vW->installEventFilter(this);

	QString name = vW->getName();

	ui->tabWidget->addTab(scrollArea, name);
}
bool ImageViewer::openImage(QString filename)
{
	QFileInfo fi(filename);

	QString name = fi.baseName();
	openNewTabForImg(new ViewerWidget(name, QSize(0, 0)));
	ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);

	ViewerWidget* w = getCurrentViewerWidget();

	QImage loadedImg(filename);
	return w->setImage(loadedImg);
}
bool ImageViewer::saveImage(QString filename)
{
	QFileInfo fi(filename);
	QString extension = fi.completeSuffix();
	ViewerWidget* w = getCurrentViewerWidget();

	QImage* img = w->getImage();
	return img->save(filename, extension.toStdString().c_str());
}
void ImageViewer::clearImage()
{
	ViewerWidget* w = getCurrentViewerWidget();
	w->clear();
}
void ImageViewer::setBackgroundColor(QColor color)
{
	ViewerWidget* w = getCurrentViewerWidget();
	w->clear(color);
}

//Slots
//Tabs slots
void ImageViewer::on_tabWidget_tabCloseRequested(int tabId)
{
	ViewerWidget* vW = getViewerWidget(tabId);
	delete vW; //vW->~ViewerWidget();
	ui->tabWidget->removeTab(tabId);
}
void ImageViewer::on_actionRename_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}
	ViewerWidget* w = getCurrentViewerWidget();
	bool ok;
	QString text = QInputDialog::getText(this, QString("Rename image"), tr("Image name:"), QLineEdit::Normal, w->getName(), &ok);
	if (ok && !text.trimmed().isEmpty())
	{
		w->setName(text);
		ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), text);
	}
}

//Image slots
void ImageViewer::on_actionNew_triggered()
{
	newImgDialog = new NewImageDialog(this);
	connect(newImgDialog, SIGNAL(accepted()), this, SLOT(newImageAccepted()));
	newImgDialog->exec();
}
void ImageViewer::newImageAccepted()
{
	NewImageDialog* newImgDialog = static_cast<NewImageDialog*>(sender());

	int width = newImgDialog->getWidth();
	int height = newImgDialog->getHeight();
	QString name = newImgDialog->getName();
	openNewTabForImg(new ViewerWidget(name, QSize(width, height)));
	ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}
void ImageViewer::on_actionOpen_triggered()
{
	QString folder = settings.value("folder_img_load_path", "").toString();

	QString fileFilter = "Image data (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm .*xbm .* xpm);;All files (*)";
	QString fileName = QFileDialog::getOpenFileName(this, "Load image", folder, fileFilter);
	if (fileName.isEmpty()) { return; }

	QFileInfo fi(fileName);
	settings.setValue("folder_img_load_path", fi.absoluteDir().absolutePath());

	if (!openImage(fileName)) {
		msgBox.setText("Unable to open image.");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
	}
}
void ImageViewer::on_actionSave_as_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image to save.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}
	QString folder = settings.value("folder_img_save_path", "").toString();

	ViewerWidget* w = getCurrentViewerWidget();

	QString fileFilter = "Image data (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm .*xbm .* xpm);;All files (*)";
	QString fileName = QFileDialog::getSaveFileName(this, "Save image", folder + "/" + w->getName(), fileFilter);
	if (fileName.isEmpty()) { return; }

	QFileInfo fi(fileName);
	settings.setValue("folder_img_save_path", fi.absoluteDir().absolutePath());

	if (!saveImage(fileName)) {
		msgBox.setText("Unable to save image.");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
	}
	else {
		msgBox.setText(QString("File %1 saved.").arg(fileName));
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
	}
}
void ImageViewer::on_actionClear_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}
	clearImage();
}
void ImageViewer::on_actionSet_background_color_triggered()
{
	QColor backgroundColor = QColorDialog::getColor(Qt::white, this, "Select color of background");
	if (backgroundColor.isValid()) {
		setBackgroundColor(backgroundColor);
	}
}

void ImageViewer::on_pushButtonColor_clicked()
{

	QColor changedColor = QColorDialog::getColor(Qt::black, this, "Vyberte farbu");
	if (changedColor.isValid()) {
		current = changedColor;
	}
	
}

void ImageViewer::on_pushButtonClear_clicked()
{
	clearImage();
	ui->pushButtonPolygon->setEnabled(TRUE);
	ui->comboBoxAlgorithm->setEnabled(TRUE);
	polyg = FALSE;
	points = 0;
	point.clear();
	ready = FALSE;
	trans = FALSE;
	ui->doubleSpinBoxAngle->setValue(0);
	ui->doubleSpinBoxX->setValue(1);
	ui->doubleSpinBoxY->setValue(1);
	ui->groupBoxTransform->setDisabled(TRUE);
	ui->doubleSpinBoxFactor->setValue(0);
	getCurrentViewerWidget()->clearPush();
}


/*
void ImageViewer::on_comboBoxType_currentIndexChanged()
{
	if (ui->comboBoxType->currentIndex() == 1) {
		ui->comboBoxAlgorithm->setDisabled(TRUE);
	}
	else {
		ui->comboBoxAlgorithm->setEnabled(TRUE);
	}
}
*/

void ImageViewer::on_pushButtonPolygon_clicked()
{	
	ready = TRUE;
	polyg = TRUE;
	ui->pushButtonClear->setEnabled(TRUE);
	ui->pushButtonPolygon->setDisabled(TRUE);
	ui->comboBoxAlgorithm->setDisabled(TRUE);
	method = ui->comboBoxAlgorithm->currentIndex();
	
}

void ImageViewer::on_pushButtonRotation_clicked()
{
	double x, y, x0, y0;
	x0 = point[0].get_posX();
	y0 = point[0].get_posY();
	angle = ui->doubleSpinBoxAngle->value();

	for (size_t i = 0; i < points; i++)
	{
		x = point[i].get_posX();
		y = point[i].get_posY();
		point[i].set_posX(((x-x0)* cos(angle*(M_PI/180))) - ((y-y0)* sin(angle * (M_PI / 180))) + x0);
		point[i].set_posY(((x - x0)* sin(angle * (M_PI / 180))) + ((y - y0)* cos(angle * (M_PI / 180)))  + y0);
	}

	drawPoly();
}

void ImageViewer::on_pushButtonScale_clicked()
{
	double scaleX = ui->doubleSpinBoxX->value();
	double scaleY = ui->doubleSpinBoxY->value();

	for (size_t i = 0; i < points; i++)
	{
		point[i].set_posX(point[0].get_posX() + ((point[i].get_posX() - point[0].get_posX()) * scaleX));
		point[i].set_posY(point[0].get_posY() + ((point[i].get_posY() - point[0].get_posY()) * scaleY));
	}

	drawPoly();
}

void ImageViewer::on_pushButtonSymetry_clicked()
{
	int a=0, b=0, c=0, u=0, v=0;
	if (points > 2) {  // symetry polygon
		u = point[points - 1].get_posX() - point[0].get_posX();
		v = point[points - 1].get_posY() - point[0].get_posY();
		a = v;
		b = -u;
		c = -a * point[0].get_posX() - b * point[0].get_posY();
		Symetry(a,b,c);
		
	}
	else if (points == 2) {  // horizontal symetry úseèky
		u = (point[0].get_posX() + 50) - (point[0].get_posX() - 50);
		v = point[0].get_posY() - point[0].get_posY();  // staèilo da 0, ale takto to vyzerá lepšie :D 
		a = v;
		b = -u;
		c = -a * point[0].get_posX() - b * point[0].get_posY();
		Symetry(a, b, c);
	}

	drawPoly();
}

void ImageViewer::on_pushButtonShear_clicked()
{
	double factor = 0;
	int newX = 0;
	factor = ui->doubleSpinBoxFactor->value();
	for (size_t i = 0; i < points; i++)
	{
		newX = point[i].get_posX() + (point[i].get_posY() - point[0].get_posY())* factor;
		point[i].set_posX(newX);
	}
	drawPoly();

}

