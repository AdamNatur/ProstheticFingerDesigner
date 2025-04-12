#pragma once

#include <QDialog>
#include "ui_PlateForm.h"

class PlateForm : public QDialog
{
	Q_OBJECT

public:
	int index = 0;
	int middle = 0;
	int ring = 0;
	int pinky = 0;

	int finger_amount = 0;

	PlateForm(QWidget *parent = nullptr);
	~PlateForm();
	Ui::PlateFormClass ui;

	void func();
	void checkBoxIndexFinger();
	void checkBoxMiddleFinger();
	void checkBoxRingFinger();
	void checkBoxPinky();


};
