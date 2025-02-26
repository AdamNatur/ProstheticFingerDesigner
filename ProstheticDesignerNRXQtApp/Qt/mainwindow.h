#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class ProstheticDesigner;
}
QT_END_NAMESPACE

class ProstheticDesigner : public QMainWindow
{
    Q_OBJECT

public:

    ProstheticDesigner(QWidget *parent = nullptr);
    ~ProstheticDesigner();

private slots:
    //void on_openBtn_clicked();



    void on_pushButton_clicked();

private:
    Ui::ProstheticDesigner *ui;
};
#endif // MAINWINDOW_H
