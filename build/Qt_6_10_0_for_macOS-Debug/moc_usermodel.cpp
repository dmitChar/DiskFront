/****************************************************************************
** Meta object code from reading C++ file 'usermodel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../models/usermodel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usermodel.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9UserModelE_t {};
} // unnamed namespace

template <> constexpr inline auto UserModel::qt_create_metaobjectdata<qt_meta_tag_ZN9UserModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "UserModel",
        "userChanged",
        "",
        "quotaChanged",
        "userId",
        "username",
        "email",
        "quotaBytes",
        "usedBytes",
        "freeBytes",
        "usedPercent",
        "humanQuota",
        "humanUsed"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'userChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'quotaChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'userId'
        QtMocHelpers::PropertyData<qint64>(4, QMetaType::LongLong, QMC::DefaultPropertyFlags, 0),
        // property 'username'
        QtMocHelpers::PropertyData<QString>(5, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'email'
        QtMocHelpers::PropertyData<QString>(6, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'quotaBytes'
        QtMocHelpers::PropertyData<qint64>(7, QMetaType::LongLong, QMC::DefaultPropertyFlags, 1),
        // property 'usedBytes'
        QtMocHelpers::PropertyData<qint64>(8, QMetaType::LongLong, QMC::DefaultPropertyFlags, 1),
        // property 'freeBytes'
        QtMocHelpers::PropertyData<qint64>(9, QMetaType::LongLong, QMC::DefaultPropertyFlags, 1),
        // property 'usedPercent'
        QtMocHelpers::PropertyData<int>(10, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'humanQuota'
        QtMocHelpers::PropertyData<QString>(11, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
        // property 'humanUsed'
        QtMocHelpers::PropertyData<QString>(12, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<UserModel, qt_meta_tag_ZN9UserModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject UserModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UserModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UserModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9UserModelE_t>.metaTypes,
    nullptr
} };

void UserModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<UserModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->userChanged(); break;
        case 1: _t->quotaChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (UserModel::*)()>(_a, &UserModel::userChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (UserModel::*)()>(_a, &UserModel::quotaChanged, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<qint64*>(_v) = _t->userId(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->username(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->email(); break;
        case 3: *reinterpret_cast<qint64*>(_v) = _t->quotaBytes(); break;
        case 4: *reinterpret_cast<qint64*>(_v) = _t->usedBytes(); break;
        case 5: *reinterpret_cast<qint64*>(_v) = _t->freeBytes(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->usedPercent(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->humanQuota(); break;
        case 8: *reinterpret_cast<QString*>(_v) = _t->humanUsed(); break;
        default: break;
        }
    }
}

const QMetaObject *UserModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UserModelE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UserModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void UserModel::userChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void UserModel::quotaChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
