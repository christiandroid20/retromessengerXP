#include "skinmanager.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextStream>

SkinManager::SkinManager(QObject *parent)
    : QObject(parent)
{
    reload();
}

void SkinManager::reload()
{
    m_skins = {
        {QStringLiteral("msn-75"), QStringLiteral("MSN Messenger 7.5 · Windows XP Luna"),
         QStringLiteral(":/skins/msn-75.qss"), {}},
        {QStringLiteral("icq-2000b"), QStringLiteral("ICQ 2000b · Windows 98"),
         QStringLiteral(":/skins/icq-2000b.qss"), {}},
        {QStringLiteral("yahoo-messenger-9"), QStringLiteral("Yahoo! Messenger 9"),
         QStringLiteral(":/skins/yahoo-messenger-9.qss"), {}},
        {QStringLiteral("wlm-2009"), QStringLiteral("Windows Live Messenger 2009"),
         QStringLiteral(":/skins/wlm-2009.qss"), {}},
        {QStringLiteral("aim-classic"), QStringLiteral("AIM Classic · Buddy List"),
         QStringLiteral(":/skins/aim-classic.qss"), {}},
        {QStringLiteral("aim-7"), QStringLiteral("AIM 7"),
         QStringLiteral(":/skins/aim-7.qss"), {}}
    };

    const QString customRoot = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
        + QStringLiteral("/skins");
    QDir().mkpath(customRoot);

    const QDir directory(customRoot);
    const QFileInfoList files = directory.entryInfoList(
        {QStringLiteral("*.qss")}, QDir::Files | QDir::Readable, QDir::Name);

    for (const QFileInfo &file : files) {
        const QString id = QStringLiteral("custom:") + file.completeBaseName();
        m_skins.push_back({id, file.completeBaseName(), {}, file.absoluteFilePath()});
    }
}

QVector<SkinDescriptor> SkinManager::availableSkins() const
{
    return m_skins;
}

bool SkinManager::apply(const QString &skinId, QApplication *application) const
{
    for (const SkinDescriptor &skin : m_skins) {
        if (skin.id != skinId)
            continue;

        QFile file(skin.externalPath.isEmpty() ? skin.resourcePath : skin.externalPath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;

        QTextStream stream(&file);
        application->setStyleSheet(stream.readAll());
        return true;
    }
    return false;
}
