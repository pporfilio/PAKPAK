#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_F_C_w_valueChanged(int value);
    void on_F_C_z_valueChanged(int value);
    void on_F_C_y_valueChanged(int value);
    void on_F_C_x_valueChanged(int value);
    void on_F_Z3_valueChanged(int value);
};

#endif // MAINWINDOW_H
