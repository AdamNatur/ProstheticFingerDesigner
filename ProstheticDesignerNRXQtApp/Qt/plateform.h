#ifndef PLATEFORM_H
#define PLATEFORM_H

#include <QWidget>

namespace Ui {
class PlateForm;
}

class PlateForm : public QWidget
{
    Q_OBJECT

public:
    explicit PlateForm(QWidget *parent = nullptr);
    ~PlateForm();

private:
    Ui::PlateForm *ui;
};

#endif // PLATEFORM_H
