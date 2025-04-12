#include "stdafx.h"
#include "PlateForm.h"

PlateForm::PlateForm(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.lineEdit_IndexFinger->setDisabled(true);
	ui.lineEdit_RingFinger->setDisabled(true);
	ui.lineEdit_MiddleFinger->setDisabled(true);
	ui.lineEdit_Pinky->setDisabled(true);

	QObject::connect(this->ui.checkBox_Index, &QCheckBox::stateChanged, this, &PlateForm::checkBoxIndexFinger);
	QObject::connect(this->ui.checkBox_middle, &QCheckBox::stateChanged, this, &PlateForm::checkBoxMiddleFinger);
	QObject::connect(this->ui.checkBox_Ring, &QCheckBox::stateChanged, this, &PlateForm::checkBoxRingFinger);
	QObject::connect(this->ui.checkBox_Pinky, &QCheckBox::stateChanged, this, &PlateForm::checkBoxPinky);
}

PlateForm::~PlateForm()
{}


void PlateForm::checkBoxIndexFinger() {
	if (ui.checkBox_Index->isChecked()) {
		ui.lineEdit_IndexFinger->setDisabled(false);
		func();

	}
	else {
		ui.lineEdit_IndexFinger->setDisabled(true);
		ui.lineEdit_IndexFinger->setText("");
		func();
	}
}

void PlateForm::checkBoxMiddleFinger() {
	if (ui.checkBox_middle->isChecked()) {
		ui.lineEdit_MiddleFinger->setDisabled(false);
		func();
	}
	else {
		ui.lineEdit_MiddleFinger->setText("");
		ui.lineEdit_MiddleFinger->setDisabled(true);
		func();
	}
}

void PlateForm::checkBoxRingFinger() {
	if (ui.checkBox_Ring->isChecked()) {
		ui.lineEdit_RingFinger->setDisabled(false);
		func();
	}
	else {
		ui.lineEdit_MiddleFinger->setText("");
		ui.lineEdit_RingFinger->setDisabled(true);
		func();
	}
}

void PlateForm::checkBoxPinky() {
	if (ui.checkBox_Pinky->isChecked()) {
		ui.lineEdit_Pinky->setDisabled(false);
		func();
	}
	else {
		ui.lineEdit_Pinky->setText("");
		ui.lineEdit_Pinky->setDisabled(true);
		func();
	}
}

void PlateForm:: func() {


	finger_amount = ui.checkBox_Pinky->isChecked() + ui.checkBox_Index->isChecked() + ui.checkBox_middle->isChecked() + ui.checkBox_Ring->isChecked();

	switch (finger_amount) {
		case 1: {
			ui.label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Plates/1-Finger.jpg")));
			break;
		}
		case 2: {
			ui.label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Plates/2-Finger.jpg")));
			break;
		}
		case 3: {
			ui.label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Plates/3-Finger.jpg")));
			break;
		}
		case 4: {
			ui.label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Plates/4-Finger.jpg")));
			break;
		}
		default: {
			ui.label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Plates/1-Finger.jpg")));
			break;
		}
	}
	
	update();
}

