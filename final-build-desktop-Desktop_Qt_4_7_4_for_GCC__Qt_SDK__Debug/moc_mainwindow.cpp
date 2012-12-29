/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Mon Jan 2 23:18:14 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/support/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   12,   11,   11, 0x08,
      51,   12,   11,   11, 0x08,
      85,   12,   11,   11, 0x08,
     116,   12,   11,   11, 0x08,
     147,   12,   11,   11, 0x08,
     184,  178,   11,   11, 0x08,
     216,   12,   11,   11, 0x08,
     251,   12,   11,   11, 0x08,
     284,   12,   11,   11, 0x08,
     317,   12,   11,   11, 0x08,
     351,  178,   11,   11, 0x08,
     383,  178,   11,   11, 0x08,
     415,  178,   11,   11, 0x08,
     447,   11,   11,   11, 0x08,
     481,   12,   11,   11, 0x08,
     514,   12,   11,   11, 0x08,
     548,   12,   11,   11, 0x08,
     579,   12,   11,   11, 0x08,
     614,   12,   11,   11, 0x08,
     647,   12,   11,   11, 0x08,
     681,   12,   11,   11, 0x08,
     711,  178,   11,   11, 0x08,
     738,  178,   11,   11, 0x08,
     765,  178,   11,   11, 0x08,
     792,  178,   11,   11, 0x08,
     819,  178,   11,   11, 0x08,
     845,   12,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0checked\0on_Record_Button_toggled(bool)\0"
    "on_coloring_normals_toggled(bool)\0"
    "on_select_colorC_toggled(bool)\0"
    "on_select_colorB_toggled(bool)\0"
    "on_select_colorA_toggled(bool)\0value\0"
    "on_DEP_slider_valueChanged(int)\0"
    "on_coloring_distance_toggled(bool)\0"
    "on_coloring_escape_toggled(bool)\0"
    "on_coloring_lights_toggled(bool)\0"
    "on_depth_fog_button_toggled(bool)\0"
    "on_BRK_slider_valueChanged(int)\0"
    "on_EPS_slider_valueChanged(int)\0"
    "on_ITR_slider_valueChanged(int)\0"
    "on_reset_camera_button_released()\0"
    "on_select_game_cam_toggled(bool)\0"
    "on_select_orbit_cam_toggled(bool)\0"
    "on_skybox_button_toggled(bool)\0"
    "on_reflection_button_toggled(bool)\0"
    "on_specular_button_toggled(bool)\0"
    "on_select_Mandelbox_toggled(bool)\0"
    "on_select_Julia_toggled(bool)\0"
    "on_F_C_w_valueChanged(int)\0"
    "on_F_C_z_valueChanged(int)\0"
    "on_F_C_y_valueChanged(int)\0"
    "on_F_C_x_valueChanged(int)\0"
    "on_F_Z3_valueChanged(int)\0"
    "on_ss_button_toggled(bool)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_Record_Button_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: on_coloring_normals_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: on_select_colorC_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: on_select_colorB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: on_select_colorA_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: on_DEP_slider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: on_coloring_distance_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: on_coloring_escape_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: on_coloring_lights_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: on_depth_fog_button_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: on_BRK_slider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: on_EPS_slider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: on_ITR_slider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: on_reset_camera_button_released(); break;
        case 14: on_select_game_cam_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: on_select_orbit_cam_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: on_skybox_button_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: on_reflection_button_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: on_specular_button_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: on_select_Mandelbox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: on_select_Julia_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: on_F_C_w_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: on_F_C_z_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: on_F_C_y_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: on_F_C_x_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: on_F_Z3_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: on_ss_button_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 27;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
