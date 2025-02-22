#include "stdafx.h"
#include "PlateForm.h"

PlateForm::PlateForm(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

PlateForm::~PlateForm()
{}

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

