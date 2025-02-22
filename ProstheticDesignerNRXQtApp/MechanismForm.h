#pragma once

#include <QDialog>
#include "ui_MechanismForm.h"

class MechanismForm : public QDialog
{
	Q_OBJECT

public:
	MechanismForm(QWidget *parent = nullptr);
	~MechanismForm();
	Ui::MechanismFormClass ui;

};
