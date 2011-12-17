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
    Ui::MainWindow *ui;

protected:
    //Overridden QMainWindow methods
    void keyPressEvent(QKeyEvent *event);

private:


private slots:
    void on_DEP_slider_valueChanged(int value);
    void on_coloring_distance_toggled(bool checked);
    void on_coloring_escape_toggled(bool checked);
    void on_coloring_lights_toggled(bool checked);
    void on_depth_fog_button_toggled(bool checked);
    void on_BRK_slider_valueChanged(int value);
    void on_EPS_slider_valueChanged(int value);
    void on_ITR_slider_valueChanged(int value);
    void on_reset_camera_button_released();
    void on_select_game_cam_toggled(bool checked);
    void on_select_orbit_cam_toggled(bool checked);
    void on_skybox_button_toggled(bool checked);
    void on_reflection_button_toggled(bool checked);
    void on_specular_button_toggled(bool checked);
    void on_select_Mandelbox_toggled(bool checked);
    void on_select_Julia_toggled(bool checked);
    void on_reflect_b_valueChanged(int value);
    void on_reflect_g_valueChanged(int value);
    void on_reflect_r_valueChanged(int value);
    void on_spec_b_valueChanged(int value);
    void on_spec_g_valueChanged(int value);
    void on_spec_r_valueChanged(int value);
    void on_F_C_w_valueChanged(int value);
    void on_F_C_z_valueChanged(int value);
    void on_F_C_y_valueChanged(int value);
    void on_F_C_x_valueChanged(int value);
    void on_F_Z3_valueChanged(int value);
    void on_ss_button_toggled(bool checked);
};

#endif // MAINWINDOW_H
