#pragma once

#include "stdafx.h"
#include <QWidget>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QComboBox>

class RingForm : public QDialog 
{
	Q_OBJECT

public:
	RingForm(QWidget *parent = nullptr);
	~RingForm();
	double inner_diam = 0;

public:
    QVBoxLayout* verticalLayout;
    QLabel* label_Picture;
    QHBoxLayout* horizontalLayout;
    QLabel* label_InnerDiam;
    QComboBox* comboBox_InnerDiam;
    QSpacerItem* horizontalSpacer;
    QPushButton* Create_pushBtn;

public:
    void configureComboBox(QComboBox* box);



signals:

public slots:
};
