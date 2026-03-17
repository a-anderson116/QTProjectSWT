/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SnakeGame/mainwindow.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "onStartGame",
    "",
    "GameMode",
    "mode",
    "Difficulty",
    "difficulty",
    "onGameOver",
    "score1",
    "score2",
    "winner",
    "onRestartRequested",
    "onMenuRequested",
    "onPauseStateChanged",
    "paused",
    "onShowHighScores"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[32];
    char stringdata0[11];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[5];
    char stringdata5[11];
    char stringdata6[11];
    char stringdata7[11];
    char stringdata8[7];
    char stringdata9[7];
    char stringdata10[7];
    char stringdata11[19];
    char stringdata12[16];
    char stringdata13[20];
    char stringdata14[7];
    char stringdata15[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 11),  // "onStartGame"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 8),  // "GameMode"
        QT_MOC_LITERAL(33, 4),  // "mode"
        QT_MOC_LITERAL(38, 10),  // "Difficulty"
        QT_MOC_LITERAL(49, 10),  // "difficulty"
        QT_MOC_LITERAL(60, 10),  // "onGameOver"
        QT_MOC_LITERAL(71, 6),  // "score1"
        QT_MOC_LITERAL(78, 6),  // "score2"
        QT_MOC_LITERAL(85, 6),  // "winner"
        QT_MOC_LITERAL(92, 18),  // "onRestartRequested"
        QT_MOC_LITERAL(111, 15),  // "onMenuRequested"
        QT_MOC_LITERAL(127, 19),  // "onPauseStateChanged"
        QT_MOC_LITERAL(147, 6),  // "paused"
        QT_MOC_LITERAL(154, 16)   // "onShowHighScores"
    },
    "MainWindow",
    "onStartGame",
    "",
    "GameMode",
    "mode",
    "Difficulty",
    "difficulty",
    "onGameOver",
    "score1",
    "score2",
    "winner",
    "onRestartRequested",
    "onMenuRequested",
    "onPauseStateChanged",
    "paused",
    "onShowHighScores"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   50,    2, 0x08,    1 /* Private */,
       7,    4,   55,    2, 0x08,    4 /* Private */,
      11,    0,   64,    2, 0x08,    9 /* Private */,
      12,    0,   65,    2, 0x08,   10 /* Private */,
      13,    1,   66,    2, 0x08,   11 /* Private */,
      15,    0,   69,    2, 0x08,   13 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString, 0x80000000 | 3,    8,    9,   10,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'onStartGame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<GameMode, std::false_type>,
        QtPrivate::TypeAndForceComplete<Difficulty, std::false_type>,
        // method 'onGameOver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<GameMode, std::false_type>,
        // method 'onRestartRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onMenuRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPauseStateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onShowHighScores'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onStartGame((*reinterpret_cast< std::add_pointer_t<GameMode>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Difficulty>>(_a[2]))); break;
        case 1: _t->onGameOver((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<GameMode>>(_a[4]))); break;
        case 2: _t->onRestartRequested(); break;
        case 3: _t->onMenuRequested(); break;
        case 4: _t->onPauseStateChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->onShowHighScores(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
