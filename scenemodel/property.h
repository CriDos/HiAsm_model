#pragma once

//Project
#include "types.h"
#include "value.h"
#include "valuetypes.h"
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>

class Property: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Property)

private:
    //Self
    quintptr m_id{};
    QString m_name;
    DataType m_type{};
    bool m_isDefProp{};

    //CGT
    PCodeGenTools m_cgt{};

    //Model
    PSceneModel m_model{};

    //Value
    Value m_value;

private:
    Q_PROPERTY(PSceneModel model READ getModel)
    Q_PROPERTY(PCodeGenTools cgt READ getCgt)

public:
    explicit Property(quintptr id_prop, QObject *parent);
    explicit Property(const QJsonObject &object, QObject *parent);
    explicit Property(quintptr id = 0,
                      DataType type = data_null,
                      const QVariant &data = QVariant(),
                      const QString &name = QString());

private:
    void collectingData();

public:
    //Serialize
    QVariantMap serialize();
    void deserialize(const QJsonObject &object);

    //Self
    quintptr getId() const;

    void setName(const QString &name);
    QString getName() const;

    void setType(DataType type);
    DataType getType() const;

    void setIsDefProp(bool value);
    bool getIsDefProp() const;

    //Value
    void setValue(quintptr id = 0,
                  DataType type = data_null,
                  const QVariant &data = QVariant(),
                  const QString &name = QString(),
                  DataType arrayType = data_null);

    PValue getValue();
    uchar toByte() const;
    int toInt() const;
    qreal toReal() const;
    QString toString() const;
    const PLinkedElementInfo toLinkedElementInfo() const;

    //CGT
    PCodeGenTools getCgt();

    //Model
    PSceneModel getModel();
};
