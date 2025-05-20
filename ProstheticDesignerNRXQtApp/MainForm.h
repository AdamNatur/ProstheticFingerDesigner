#pragma once

#include <QDialog>
#include "ui_MainForm.h"
#include "CustomQGroupBox.h"
#include <QFrame>

class MainForm : public QDialog
{
	Q_OBJECT

public:
	MainForm(QWidget *parent = nullptr);
	~MainForm();

public:
	CustomQGroupBox* pCGBoxIndex;
	CustomQGroupBox* pCGBoxMiddle;
	CustomQGroupBox* pCGBoxRing;
	CustomQGroupBox* pCGBoxPinky;
	
	QHBoxLayout* pMainLayout;
	QVBoxLayout* pDesignLayout;
	QHBoxLayout* pButtonLayout;
	QLabel* plabel_picture;
	QFrame* pVerticalLine = new QFrame();

	QPushButton* pPushBtnReject = new QPushButton("Cancel", this);
	QPushButton* pPushBtnCreate = new QPushButton("Create", this);

	int finger_amount = 0;

	std::map<int, QString> infoMessages = { {0, "No finger specified"},
		{1, "Incorrect specified data"} };

	int totalCheckState = 0;

public:
	int totalCheck();

private slots:
	void func();

private:
	Ui::MainFormClass ui;
};
