#pragma once

//Project
#include "types.h"
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>

class Container: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Container)

private:
    //Self
    qintptr m_id{};
    QString m_name;

    //CGT
    PCodeGenTools m_cgt{};

    //Model
    PSceneModel m_model{};

    //Element
    Elements m_elements;

private:
    Q_PROPERTY(PSceneModel model READ getModel)
    Q_PROPERTY(PCodeGenTools cgt READ getCgt)

public:
    explicit Container(qintptr id_sdk, QObject *parent);

private:
    void collectingData();

public:
    //Serialize
    QVariantMap serialize();

    //Self
    qintptr getId() const;
    PElement getParent() const;
    void setName(const QString &name);
    QString getName() const;

    //CGT
    PCodeGenTools getCgt();

    //Model
    PSceneModel getModel() const;

    //Element
    int getCountElements() const;
    PElement getElementByIndex(uint index) const;
    qintptr getIdElementByIndex(uint index) const;
    PElement getElementByName(const QString &name) const;
    qintptr getIdElementByName(const QString &name) const;
    PElement addElement(PElement element);
    void removeElement(uint index);
};
