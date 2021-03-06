﻿//Project
#include "scenemodel.h"
#include "cgt/cgt.h"
#include "container.h"
#include "element.h"
#include "point.h"
#include "property.h"

//STL

//Native
#include <windows.h>

//Qt
#include <QDebug>

SceneModel::SceneModel(PackageManager *package, QObject *parent)
    : QObject(parent)
    , m_packageManager(package)
{
}

SceneModel::~SceneModel()
{
}

void SceneModel::collectingData(qintptr id_sdk)
{
    m_isDebug = m_cgt->isDebug(id_sdk);
    qintptr id_element = m_cgt->sdkGetElement(id_sdk, 0);

    //ru Тут мощная магия, однако;D
    qint32 iBuf{};
    QByteArray buf("", 512);

    buf.fill('\0');
    reinterpret_cast<qintptr *>(buf.data())[0] = id_element; //-V206
    m_cgt->GetParam(PARAM_CODE_PATH, buf.data());
    m_codePath = QString::fromLocal8Bit(buf);

    m_cgt->GetParam(PARAM_DEBUG_MODE, &iBuf);
    m_debugMode = iBuf;

    m_cgt->GetParam(PARAM_DEBUG_SERVER_PORT, &iBuf);
    m_debugServerPort = iBuf;

    m_cgt->GetParam(PARAM_DEBUG_CLIENT_PORT, &iBuf);
    m_debugClientPort = iBuf;

    buf.fill('\0');
    reinterpret_cast<qintptr *>(buf.data())[0] = id_element; //-V206
    m_cgt->GetParam(PARAM_PROJECT_PATH, buf.data());
    m_projectPath = QString::fromLocal8Bit(buf);

    const char f[] = "%mj.%mn.%bl";
    char *tmpBuf = new char[strlen(f) + 1];
    strcpy(tmpBuf, f);
    m_cgt->GetParam(PARAM_HIASM_VERSION, tmpBuf);
    m_hiasmVersion = QString::fromLatin1(tmpBuf);
    delete[] tmpBuf;

    buf.fill('\0');
    m_cgt->GetParam(PARAM_USER_NAME, buf.data());
    m_userName = QString::fromLocal8Bit(buf);

    buf.fill('\0');
    m_cgt->GetParam(PARAM_USER_MAIL, buf.data());
    m_userMail = QString::fromLocal8Bit(buf);

    buf.fill('\0');
    reinterpret_cast<qintptr *>(buf.data())[0] = id_element; //-V206
    m_cgt->GetParam(PARAM_PROJECT_NAME, buf.data());
    m_projectName = QString::fromLocal8Bit(buf);

    qint32 tmpW[1] = { reinterpret_cast<qint32>(id_element) };
    m_cgt->GetParam(PARAM_SDE_WIDTH, tmpW);
    m_sdeWidth = tmpW[0];

    qint32 tmpH[1] = { reinterpret_cast<qint32>(id_element) };
    m_cgt->GetParam(PARAM_SDE_HEIGHT, tmpH);
    m_sdeHeight = tmpH[0];

    buf.fill('\0');
    reinterpret_cast<qintptr *>(buf.data())[0] = id_element; //-V206
    m_cgt->GetParam(PARAM_COMPILER, buf.data());
    m_compiler = QString::fromLocal8Bit(buf);
}

QJsonDocument SceneModel::serialize()
{
    QVariantMap cgtParams;
    cgtParams.insert("CODE_PATH", m_codePath);
    cgtParams.insert("DEBUG_MODE", m_debugMode);
    cgtParams.insert("DEBUG_SERVER_PORT", m_debugServerPort);
    cgtParams.insert("DEBUG_CLIENT_PORT", m_debugClientPort);
    cgtParams.insert("PROJECT_PATH", m_projectPath);
    cgtParams.insert("HIASM_VERSION", m_hiasmVersion);
    cgtParams.insert("USER_NAME", m_userName);
    cgtParams.insert("USER_MAIL", m_userMail);
    cgtParams.insert("PROJECT_NAME", m_projectName);
    cgtParams.insert("SDE_WIDTH", m_sdeWidth);
    cgtParams.insert("SDE_HEIGHT", m_sdeHeight);
    cgtParams.insert("COMPILER", m_compiler);

    QVariantMap model;
    model.insert("CGTParams", cgtParams);
    model.insert("Container", m_container->serialize());

    return QJsonDocument::fromVariant(model);
}

TCodeGenTools *SceneModel::getCgt()
{
    return m_cgt;
}

SceneModel *SceneModel::getModel()
{
    return this;
}

void SceneModel::initFromCgt(TCodeGenTools *cgt, qintptr idMainSDK)
{
    m_cgt = cgt;

    //ru Собираем данные о среде
    collectingData(idMainSDK);

    //ru Запуск процесса сборка данных о схеме
    m_container = new Container(idMainSDK, this);
}

bool SceneModel::saveModel(const QString &filePath)
{
    QJsonDocument doc = serialize();
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    file.write(doc.toJson());
    file.close();
    return true;
}

void SceneModel::setPackage(Package *package)
{
    m_package = package;
}

Package *SceneModel::getPackage()
{
    return m_package;
}

bool SceneModel::loadPackage(const QString &name)
{
    m_package = m_packageManager->getPackage(name);
    if (!m_package) {
        qWarning("Failed to load package \"%s\".", qUtf8Printable(name));
        return false;
    }

    return true;
}

void SceneModel::addContainerToMap(Container *id_sdk)
{
    if (id_sdk)
        m_mapContainers.insert(id_sdk->getId(), id_sdk);
}

void SceneModel::addElementToMap(Element *id_element)
{
    if (id_element)
        m_mapElements.insert(id_element->getId(), id_element);
}

Container *SceneModel::getContainerById(qintptr id_sdk) const
{
    return m_mapContainers[id_sdk];
}

qint32 SceneModel::getCountElementsInContainer(qintptr id_sdk) const
{
    const Container *c = getContainerById(id_sdk);
    if (!c)
        return 0;

    return c->getCountElements();
}

qintptr SceneModel::getIdRootContainer() const
{
    if (!m_container)
        return 0;

    return m_container->getId();
}

Element *SceneModel::getElementById(qintptr id_element) const
{
    return m_mapElements[id_element];
}

Element *SceneModel::getElementFromSDKByIndex(qintptr id_sdk, qint32 index) const
{
    const Container *c = getContainerById(id_sdk);
    if (!c)
        return nullptr;
    return c->getElementByIndex(index);
}

qintptr SceneModel::getIdElementFromSDKByIndex(qintptr id_sdk, qint32 index) const
{
    const Container *c = getContainerById(id_sdk);
    if (!c)
        return 0;
    return c->getIdElementByIndex(index);
}

const char *SceneModel::addStreamRes(Property *id_prop)
{
    QString ret;
    if (!id_prop)
        return nullptr;

    QString nameTypeRes;
    QString fileName;
    QString ext;
    const Value *v = id_prop->getValue();
    switch (id_prop->getType()) {
    case data_icon: {
        if (v->getValue().isNull())
            return fcgt::strToCString("ASMA");

        nameTypeRes = "ICON";
        fileName = "ICON";
        ext = ".ico";
        break;
    }
    case data_stream: {
        nameTypeRes = "100";
        fileName = "STREAM";
        ext = ".dat";
        break;
    }
    case data_bitmap: {
        if (v->getValue().isNull())
            return nullptr;

        nameTypeRes = "BITMAP";
        fileName = "BITMAP";
        ext = ".bmp";
        break;
    }
    case data_jpeg:
    case data_wave: {
    }
    default:
        return nullptr;
    }

    QString SEP = QDir::separator();
    QString CURRENT_PATH = QDir::currentPath();

    QByteArray resData = v->getValue().toByteArray();
    QString suffix = QString::number(m_resourcesForCompile.size());
    QString fileNameRes = fileName + suffix;
    QString fullFileNameRes = fileName + suffix + ext;
    QString filePathRes = CURRENT_PATH + SEP + m_resourcesDir + SEP + fullFileNameRes;

    QFile file(filePathRes);
    if (!file.open(QIODevice::WriteOnly))
        return nullptr;

    file.write(resData);
    file.close();
    m_resourcesToDelete.insert(filePathRes);
    m_resourcesForCompile.insert(fullFileNameRes, nameTypeRes);

    return fcgt::strToCString(fileNameRes);
}

const char *SceneModel::addStringRes(const QString &str)
{
    if (str.isEmpty())
        return nullptr;
    /*
        static const QString nameCompilerDir = "resources";
        static const QString nameFileRes = "STREAM";
        QString suffix = QString::number(m_resourcesToDelete.size());
        QString fileName = nameFileRes + suffix;
        QString resFilePath = QDir::toNativeSeparators(
                                  QDir::currentPath() + QDir::separator() +
                                  nameCompilerDir + QDir::separator() + fileName
                              );
        QFile file(resFilePath);
        if (!file.open(QIODevice::WriteOnly))
            return nullptr;

        file.write(str.toLocal8Bit());
        file.close();
        m_resourcesToDelete.insert(resFilePath);

    */
    return nullptr;
}
void SceneModel::deleteResources()
{
    for (const auto &filePath : m_resourcesToDelete) {
        QFile::remove(filePath);
    }
    m_resourcesToDelete.clear();
}

void SceneModel::compileResources()
{
    static const QString NAME_FILE_RC = "allres.rc";
    static const QString NAME_FILE_RES = "allres.res";
    static const QString NAME_FILE_GORC = "GoRC.exe";

    const QString currentPath = QDir::currentPath();
    const QString resourcesPath = currentPath + QDir::separator() + m_resourcesDir;
    const QString rcFilePath = resourcesPath + QDir::separator() + NAME_FILE_RC;
    const QString resFilePath = m_codePath + QDir::separator() + NAME_FILE_RES;

    QDir::setCurrent(resourcesPath);
    QFile file(NAME_FILE_RC);
    file.open(QIODevice::WriteOnly);
    QTextStream write(&file);

    for (const auto &filePath : m_resourcesForCompile.keys()) {
        QFileInfo file(filePath);

        write << QString("%1 %2 %3\r\n").arg(file.baseName()).arg(m_resourcesForCompile[filePath]).arg(filePath);
    }

    write << "ASMA ICON main.ico";
    file.close();

    QProcess::execute(QString("%1 /nw /fo \"%2\" %3").arg(NAME_FILE_GORC).arg(resFilePath).arg(NAME_FILE_RC));
    QDir::setCurrent(currentPath);

    addResList(rcFilePath);
    addResList(resFilePath);
}

qint32 SceneModel::addResList(const QString &filePath)
{
    m_resourcesToDelete.insert(filePath);
    return 0;
}

bool SceneModel::resIsEmpty() const
{
    return m_resourcesToDelete.isEmpty();
}

void SceneModel::getCgtParam(CgtParams index, void *buf) const
{
    auto writeString = [buf](const QString &str) {
        strcpy(reinterpret_cast<char *>(buf), str.toStdString().c_str());
    };
    auto writeInt = [buf](qint32 value) {
        *reinterpret_cast<qint32 *>(buf) = value; //-V206
    };

    switch (index) {
    case PARAM_CODE_PATH:
        writeString(m_codePath);
        break;
    case PARAM_DEBUG_MODE:
        writeInt(m_debugMode);
        break;
    case PARAM_DEBUG_SERVER_PORT:
        writeInt(m_debugServerPort);
        break;
    case PARAM_DEBUG_CLIENT_PORT:
        writeInt(m_debugClientPort);
        break;
    case PARAM_PROJECT_PATH:
        writeString(m_projectPath);
        break;
    case PARAM_HIASM_VERSION:
        writeString(m_hiasmVersion);
        break;
    case PARAM_USER_NAME:
        writeString(m_userName);
        break;
    case PARAM_USER_MAIL:
        writeString(m_userMail);
        break;
    case PARAM_PROJECT_NAME:
        writeString(m_projectName);
        break;
    case PARAM_SDE_WIDTH:
        writeInt(m_sdeWidth);
        break;
    case PARAM_SDE_HEIGHT:
        writeInt(m_sdeHeight);
        break;
    case PARAM_COMPILER:
        writeString(m_compiler);
        break;
    }
}

bool SceneModel::getIsDebug() const
{
    return m_isDebug;
}

void SceneModel::setIsDebug(bool isDebug)
{
    m_isDebug = isDebug;
}

qint32 SceneModel::getDebugMode() const
{
    return m_debugMode;
}

void SceneModel::setDebugMode(qint32 debugMode)
{
    m_debugMode = debugMode;
}

qint32 SceneModel::getDebugServerPort() const
{
    return m_debugServerPort;
}

void SceneModel::setDebugServerPort(qint32 debugServerPort)
{
    m_debugServerPort = debugServerPort;
}

qint32 SceneModel::getDebugClientPort() const
{
    return m_debugClientPort;
}

void SceneModel::setDebugClientPort(qint32 debugClientPort)
{
    m_debugClientPort = debugClientPort;
}

qint32 SceneModel::getSdeWidth() const
{
    return m_sdeWidth;
}

void SceneModel::setSdeWidth(qint32 sdeWidth)
{
    m_sdeWidth = sdeWidth;
}

qint32 SceneModel::getSdeHeight() const
{
    return m_sdeHeight;
}

void SceneModel::setSdeHeight(qint32 sdeHeight)
{
    m_sdeHeight = sdeHeight;
}

QString SceneModel::getCodePath() const
{
    return m_codePath;
}

void SceneModel::setCodePath(const QString &codePath)
{
    m_codePath = codePath;
}

QString SceneModel::getProjectPath() const
{
    return m_projectPath;
}

void SceneModel::setProjectPath(const QString &projectPath)
{
    m_projectPath = projectPath;
}

QString SceneModel::getHiasmVersion() const
{
    return m_hiasmVersion;
}

void SceneModel::setHiasmVersion(const QString &hiasmVersion)
{
    m_hiasmVersion = hiasmVersion;
}

QString SceneModel::getUserName() const
{
    return m_userName;
}

void SceneModel::setUserName(const QString &userName)
{
    m_userName = userName;
}

QString SceneModel::getUserMail() const
{
    return m_userMail;
}

void SceneModel::setUserMail(const QString &userMail)
{
    m_userMail = userMail;
}

QString SceneModel::getProjectName() const
{
    return m_projectName;
}

void SceneModel::setProjectName(const QString &projectName)
{
    m_projectName = projectName;
}

QString SceneModel::getCompiler() const
{
    return m_compiler;
}

void SceneModel::setCompiler(const QString &compiler)
{
    m_compiler = compiler;
}
