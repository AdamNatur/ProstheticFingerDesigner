#ifndef RINGFORM_H
#define RINGFORM_H

#include <QWidget>


namespace Ui {
class RingForm;
}

class RingForm : public QWidget
{
    Q_OBJECT

public:
    explicit RingForm(QWidget *parent = nullptr);
    ~RingForm();

private:
    Ui::RingForm *ui;
};

#endif // RINGFORM_H
