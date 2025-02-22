#pragma once


#include "stdafx.h"
#include <QWidget>
#include "ui_RingForm.h"

class RingForm : public QDialog 
{
	Q_OBJECT

public:
	explicit RingForm(QWidget *parent = nullptr);
	~RingForm();
	double inner_diam = 0;
	Ui::RingFormClass ui;
	nds_real inner_diameter;
signals:

public slots:
	void on_CreateBtn_clicked();
};
