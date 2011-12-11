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
    void on_select_Mandelbox_toggled(bool checked);
    void on_select_Julia_toggled(bool checked);
    void on_reflect_b_valueChanged(int value);
    void on_reflect_g_valueChanged(int value);
    void on_reflect_r_valueChanged(int value);
    void on_spec_b_valueChanged(int value);
    void on_spec_g_valueChanged(int value);
    void on_spec_r_valueChanged(int value);
    void on_enable_normal_reflection_stateChanged(int );
    void on_enable_specular_reflection_stateChanged(int );
    void on_F_C_w_valueChanged(int value);
    void on_F_C_z_valueChanged(int value);
    void on_F_C_y_valueChanged(int value);
    void on_F_C_x_valueChanged(int value);
    void on_F_Z3_valueChanged(int value);
};

#endif // MAINWINDOW_H
