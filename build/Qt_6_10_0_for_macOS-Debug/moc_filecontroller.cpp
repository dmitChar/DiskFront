/****************************************************************************
** Meta object code from reading C++ file 'filecontroller.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Controllers/filecontroller.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filecontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.0. It"
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
struct qt_meta_tag_ZN14FileControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto FileController::qt_create_metaobjectdata<qt_meta_tag_ZN14FileControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "FileController",
        "currentPathChanged",
        "",
        "busyChanged",
        "errorChanged",
        "uploadFinished",
        "pathChanged",
        "itemDeleted",
        "path",
        "navigateTo",
        "navigateUp",
        "refresh",
        "mkdir",
        "name",
        "mkCopy",
        "filename",
        "uploadFiles",
        "localPaths",
        "downloadFile",
        "remotePath",
        "localPath",
        "moveFile",
        "from",
        "to",
        "deleteItem",
        "clearError",
        "currentPath",
        "busy",
        "error"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'currentPathChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'busyChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'errorChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'uploadFinished'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'pathChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'itemDeleted'
        QtMocHelpers::SignalData<void(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'navigateTo'
        QtMocHelpers::MethodData<void(const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'navigateUp'
        QtMocHelpers::MethodData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'refresh'
        QtMocHelpers::MethodData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'mkdir'
        QtMocHelpers::MethodData<void(const QString &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Method 'mkCopy'
        QtMocHelpers::MethodData<void(const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 },
        }}),
        // Method 'uploadFiles'
        QtMocHelpers::MethodData<void(const QStringList &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 17 },
        }}),
        // Method 'downloadFile'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 19 }, { QMetaType::QString, 20 },
        }}),
        // Method 'moveFile'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 }, { QMetaType::QString, 23 },
        }}),
        // Method 'deleteItem'
        QtMocHelpers::MethodData<void(const QString &)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'clearError'
        QtMocHelpers::MethodData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'currentPath'
        QtMocHelpers::PropertyData<QString>(26, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'busy'
        QtMocHelpers::PropertyData<bool>(27, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'error'
        QtMocHelpers::PropertyData<QString>(28, QMetaType::QString, QMC::DefaultPropertyFlags, 2),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<FileController, qt_meta_tag_ZN14FileControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject FileController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14FileControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14FileControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14FileControllerE_t>.metaTypes,
    nullptr
} };

void FileController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FileController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->currentPathChanged(); break;
        case 1: _t->busyChanged(); break;
        case 2: _t->errorChanged(); break;
        case 3: _t->uploadFinished(); break;
        case 4: _t->pathChanged(); break;
        case 5: _t->itemDeleted((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->navigateTo((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->navigateUp(); break;
        case 8: _t->refresh(); break;
        case 9: _t->mkdir((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->mkCopy((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->uploadFiles((*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 12: _t->downloadFile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 13: _t->moveFile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 14: _t->deleteItem((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->clearError(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (FileController::*)()>(_a, &FileController::currentPathChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (FileController::*)()>(_a, &FileController::busyChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (FileController::*)()>(_a, &FileController::errorChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (FileController::*)()>(_a, &FileController::uploadFinished, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (FileController::*)()>(_a, &FileController::pathChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (FileController::*)(const QString & )>(_a, &FileController::itemDeleted, 5))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->currentPath(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->busy(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->error(); break;
        default: break;
        }
    }
}

const QMetaObject *FileController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FileController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14FileControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FileController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FileController::currentPathChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FileController::busyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void FileController::errorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void FileController::uploadFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void FileController::pathChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void FileController::itemDeleted(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}
QT_WARNING_POP
