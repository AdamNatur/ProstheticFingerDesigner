#ifndef MECHANISMFORM_H
#define MECHANISMFORM_H

#include <QWidget>

namespace Ui {
class MechanismForm;
}

class MechanismForm : public QWidget
{
    Q_OBJECT

public:
    explicit MechanismForm(QWidget *parent = nullptr);
    ~MechanismForm();

private slots:
    void on_radBtnAutoSet_clicked();

    void on_radBtnCustomSet_clicked();

    void on_pushBtn_Export_clicked();

private:
    Ui::MechanismForm *ui;
};

#endif // MECHANISMFORM_H
