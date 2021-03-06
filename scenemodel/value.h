#pragma once
//Project
#include "valuetypes.h"
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>
#include <QVariant>

class Value {

private:
    //Self
    DataType m_type = data_null;
    QVariant m_value;
    QString m_name;
    DataType m_subType = data_null;

public:
    explicit Value(DataType type = data_null, const QVariant &value = QVariant(),
        const QString &name = QString(), DataType subType = data_null);

public:
    //Serialize
    QVariantMap serialize() const;

    //Self
    void setType(DataType type);
    DataType getType() const;
    void setName(const QString &name);
    QString getName() const;

    //Value
    void setValue(const QVariant &value);
    QVariant getValue() const;

    //Byte
    uchar toByte() const;

    //Int
    qint32 toInt() const;

    //Real
    qreal toReal() const;

    //String
    QString toString() const;

    //SubType
    void setSubType(DataType type);
    DataType getSubType() const;

    //Data
    DataType getDataType() const;

    //Array
    qint32 getArraySize() const;
    Value *getArrayItemByIndex(uint index) const;
    QString getArrayItemName(uint index) const;

    //Font
    SharedValueFont toFont() const;

    //LinkedElementInfo
    SharedLinkedElementInfo toLinkedElementInfo() const;
};

typedef QVector<Value *> Values;
Q_DECLARE_METATYPE(Values)
