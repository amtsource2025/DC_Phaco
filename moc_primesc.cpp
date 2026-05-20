/****************************************************************************
** Meta object code from reading C++ file 'primesc.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "primesc.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'primesc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_primesc_t {
    QByteArrayData data[16];
    char stringdata0[169];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_primesc_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_primesc_t qt_meta_stringdata_primesc = {
    {
QT_MOC_LITERAL(0, 0, 7), // "primesc"
QT_MOC_LITERAL(1, 8, 5), // "timer"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 7), // "current"
QT_MOC_LITERAL(4, 23, 3), // "tab"
QT_MOC_LITERAL(5, 27, 6), // "update"
QT_MOC_LITERAL(6, 34, 2), // "hp"
QT_MOC_LITERAL(7, 37, 9), // "connected"
QT_MOC_LITERAL(8, 47, 5), // "prime"
QT_MOC_LITERAL(9, 53, 4), // "Tune"
QT_MOC_LITERAL(10, 58, 5), // "Clean"
QT_MOC_LITERAL(11, 64, 10), // "Start_Tune"
QT_MOC_LITERAL(12, 75, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(13, 97, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(14, 121, 23), // "on_pushButton_3_clicked"
QT_MOC_LITERAL(15, 145, 23) // "on_pushButton_4_clicked"

    },
    "primesc\0timer\0\0current\0tab\0update\0hp\0"
    "connected\0prime\0Tune\0Clean\0Start_Tune\0"
    "on_pushButton_clicked\0on_pushButton_2_clicked\0"
    "on_pushButton_3_clicked\0on_pushButton_4_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_primesc[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a /* Public */,
       3,    1,   75,    2, 0x0a /* Public */,
       5,    0,   78,    2, 0x0a /* Public */,
       6,    1,   79,    2, 0x0a /* Public */,
       8,    0,   82,    2, 0x08 /* Private */,
       9,    0,   83,    2, 0x08 /* Private */,
      10,    0,   84,    2, 0x08 /* Private */,
      11,    0,   85,    2, 0x08 /* Private */,
      12,    0,   86,    2, 0x08 /* Private */,
      13,    0,   87,    2, 0x08 /* Private */,
      14,    0,   88,    2, 0x08 /* Private */,
      15,    0,   89,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void primesc::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<primesc *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->timer(); break;
        case 1: _t->current((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->update(); break;
        case 3: _t->hp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->prime(); break;
        case 5: _t->Tune(); break;
        case 6: _t->Clean(); break;
        case 7: _t->Start_Tune(); break;
        case 8: _t->on_pushButton_clicked(); break;
        case 9: _t->on_pushButton_2_clicked(); break;
        case 10: _t->on_pushButton_3_clicked(); break;
        case 11: _t->on_pushButton_4_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject primesc::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_primesc.data,
    qt_meta_data_primesc,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *primesc::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *primesc::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_primesc.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int primesc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
