#pragma once

//Project
#include "types.h"
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>
#include <QString>

class Element: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Element)

private:
    //Self
    qintptr m_id{};
    qintptr m_userData{};
    ElementClass m_classIndex{};
    ElementFlgs m_flags{};
    int m_group{};
    bool m_linkIs{};
    qintptr m_linkMain{};
    int m_posX{};
    int m_posY{};
    int m_sizeW{};
    int m_sizeH{};
    QString m_className;
    QString m_codeName;
    QString m_interface;
    QString m_inherit;
    QString m_infSub;

    //CGT
    PCodeGenTools m_cgt{};

    //Model
    PSceneModel m_model{};

    //Container
    Containers m_containers;

    //Point
    Points m_points;

    //Property
    Properties m_properties;

private:
    Q_PROPERTY(PSceneModel model READ getModel)
    Q_PROPERTY(PCodeGenTools cgt READ getCgt)

public:
    explicit Element(qintptr id_element, QObject *parent);
    explicit Element(const QJsonObject &object, QObject *parent);

private:
    void collectingData();

public:
    //Serialize
    QVariantMap serialize();
    void deserialize(const QJsonObject &object);

    //Self
    qintptr getId() const;
    PContainer getParent() const;

    void setUserData(qintptr userData);
    qintptr getUserData() const;

    void setClassIndex(ElementClass classIndex);
    ElementClass getClassIndex();

    void setFlags(const ElementFlgs &flags);
    ElementFlags getFlags() const;

    void setGroup(int group);
    int getGroup() const;

    void setLinkIs(bool linkIs);
    bool getLinkIs() const;

    void setLinkMain(qintptr linkMain);
    qintptr getLinkMain() const;

    void setPosX(int posX);
    int getPosX() const;

    void setPosY(int posY);
    int getPosY() const;

    void setSizeW(int sizeW);
    int getSizeW() const;

    void setSizeH(int sizeH);
    int getSizeH() const;

    void setClassName(const QString &className);
    QString getClassName() const;

    void setCodeName(const QString &name);
    QString getCodeName() const;

    void setInterface(const QString &interface);
    QString getInterface() const;

    void setInherit(const QString &inherit);
    QString getInherit() const;

    void setInfSub(const QString &infSub);
    QString getInfSub() const;

    //CGT
    PCodeGenTools getCgt();

    //Model
    PSceneModel getModel();

    //Container
    int getCountContainers() const;
    PContainer getContainer() const;
    qintptr getIdContainer() const;
    PContainer getContainerByIndex(uint index);
    qintptr getIdContainerByIndex(uint index);
    PContainer addContainer(PContainer container);
    void removeContainer(uint index);

    //Point
    int getCountPoints() const;
    PPoint getPointByIndex(uint index) const;
    qintptr getIdPointByIndex(uint index) const;
    PPoint getPointByName(const QString &name) const;
    qintptr getIdPointByName(const QString &name) const;
    PPoint addPoint(PPoint point);
    void removePoint(uint index);

    //Property
    int getCountProps() const;
    PProperty getPropertyByIndex(uint index) const;
    qintptr getIdPropertyByIndex(uint index) const;
    PProperty getPropertyById(qintptr id_prop) const;
    PProperty getPropertyByName(const QString &name) const;
    qintptr getIdPropertyByName(const QString &name) const;
    PProperty addProperty(PProperty property);
    void removeProperty(uint index);
};
