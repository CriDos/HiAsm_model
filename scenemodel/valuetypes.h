﻿#pragma once
//Project
#include "cgt/CGTShare.h"
#include "types.h"

//STL

//Qt
#include <QObject>
#include <QVariant>
#include <QSharedPointer>
#include <QList>

struct LinkedElementInfo {
    qintptr id{};
    QString interface;
};
typedef QSharedPointer<LinkedElementInfo> SharedLinkedElementInfo;
Q_DECLARE_METATYPE(SharedLinkedElementInfo)

struct ValueFont { //-V802
    QString name;
    uint size{};
    uchar style{};
    uint color{};
    uchar charset{};
};
typedef QSharedPointer<ValueFont> SharedValueFont;
Q_DECLARE_METATYPE(SharedValueFont)
