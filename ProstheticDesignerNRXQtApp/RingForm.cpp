#include "stdafx.h"
#include "RingForm.h"

#include <QFileDialog>

RingForm::RingForm(QWidget *parent)
	: QDialog(parent)
{
    this->resize(724, 651);
    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    label_Picture = new QLabel(this);
    label_Picture->setObjectName(QString::fromUtf8("label_Picture"));
    label_Picture->setMinimumSize(QSize(100, 100));
    label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Ring.jpg")));
    label_Picture->setScaledContents(true);

    verticalLayout->addWidget(label_Picture);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(6);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label_InnerDiam = new QLabel("Inner Diameter", this);
    label_InnerDiam->setObjectName(QString::fromUtf8("label_InnerDiam"));

    horizontalLayout->addWidget(label_InnerDiam);

    comboBox_InnerDiam = new QComboBox(this);

    configureComboBox(comboBox_InnerDiam);

    horizontalLayout->addWidget(comboBox_InnerDiam);

    horizontalSpacer = new QSpacerItem(350, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);


    Create_pushBtn = new QPushButton("Create", this);
    Create_pushBtn->setObjectName(QString::fromUtf8("Create_pushBtn"));
    horizontalLayout->addWidget(Create_pushBtn);

    verticalLayout->addLayout(horizontalLayout);

    this->label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Qt/Media/Ring.jpg")));

    connect(this->Create_pushBtn, &QPushButton::clicked, [&]() {});

}

RingForm::~RingForm()
{}

void RingForm::configureComboBox(QComboBox* box)
{
    unsigned int init_size = 18;
    unsigned int max_size = 25;

    for (unsigned int i = init_size; i <= max_size; i++)
    {
        box->addItem(QString::number(i));
    }
}


