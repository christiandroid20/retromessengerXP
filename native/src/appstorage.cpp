#include "appstorage.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileDevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QStandardPaths>
#include <QUuid>

#ifdef Q_OS_UNIX
#include <sys/stat.h>
#endif

namespace {
bool ensurePrivateDirectory(const QString &path)
{
    QDir directory;
    if (!directory.mkpath(path))
        return false;
#ifdef Q_OS_UNIX
    QFile::setPermissions(path, QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner);
#endif
    return true;
}

bool isWritableDirectory(const QString &path)
{
    if (!ensurePrivateDirectory(path))
        return false;

    const QString probePath = QDir(path).filePath(QStringLiteral(".write-check"));
    QFile probe(probePath);
    if (!probe.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;
    probe.write("ok", 2);
    probe.close();
    QFile::remove(probePath);
    return true;
}
}

AppStorage::AppStorage()
{
    const QString installData = QDir(QCoreApplication::applicationDirPath())
        .filePath(QStringLiteral("data"));

    if (isWritableDirectory(installData)) {
        m_dataRoot = installData;
        m_storageDescription = QStringLiteral("Datos junto a la instalación");
    } else {
        const QString fallback = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation))
            .filePath(QStringLiteral("data"));
        m_dataRoot = fallback;
        m_storageDescription = QStringLiteral("Datos privados del usuario (la carpeta de instalación no permite escritura)");
        ensurePrivateDirectory(m_dataRoot);
    }

    ensurePrivateDirectory(accountsRoot());
}

QString AppStorage::dataRoot() const
{
    return m_dataRoot;
}

QString AppStorage::accountsRoot() const
{
    return QDir(m_dataRoot).filePath(QStringLiteral("accounts"));
}

QString AppStorage::storageDescription() const
{
    return m_storageDescription;
}

QVector<StoredAccount> AppStorage::accounts() const
{
    QVector<StoredAccount> result;
    QFile file(QDir(m_dataRoot).filePath(QStringLiteral("accounts.json")));
    if (!file.open(QIODevice::ReadOnly))
        return result;

    const QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    if (!document.isArray())
        return result;

    for (const QJsonValue &value : document.array()) {
        if (!value.isObject())
            continue;
        const QJsonObject object = value.toObject();
        StoredAccount account;
        account.id = object.value(QStringLiteral("id")).toString();
        account.displayName = object.value(QStringLiteral("displayName")).toString();
        account.directory = QDir(accountsRoot()).filePath(account.id);
        if (!account.id.isEmpty() && !account.displayName.isEmpty()
            && QFileInfo::exists(account.directory))
            result.push_back(account);
    }
    return result;
}

bool AppStorage::saveAccountIndex(const QVector<StoredAccount> &accounts, QString *error) const
{
    QJsonArray array;
    for (const StoredAccount &account : accounts) {
        QJsonObject object;
        object.insert(QStringLiteral("id"), account.id);
        object.insert(QStringLiteral("displayName"), account.displayName);
        array.append(object);
    }

    QSaveFile file(QDir(m_dataRoot).filePath(QStringLiteral("accounts.json")));
    if (!file.open(QIODevice::WriteOnly)) {
        if (error)
            *error = file.errorString();
        return false;
    }
    file.write(QJsonDocument(array).toJson(QJsonDocument::Indented));
    if (!file.commit()) {
        if (error)
            *error = file.errorString();
        return false;
    }
    return true;
}

bool AppStorage::addAccount(const QString &displayName, StoredAccount *created, QString *error) const
{
    const QString cleanName = displayName.trimmed();
    if (cleanName.isEmpty()) {
        if (error)
            *error = QStringLiteral("Escribe un nombre para identificar la cuenta.");
        return false;
    }

    StoredAccount account;
    account.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    account.displayName = cleanName;
    account.directory = QDir(accountsRoot()).filePath(account.id);

    const QStringList folders = {
        account.directory,
        QDir(account.directory).filePath(QStringLiteral("profile")),
        QDir(account.directory).filePath(QStringLiteral("session")),
        QDir(account.directory).filePath(QStringLiteral("media"))
    };
    for (const QString &folder : folders) {
        if (!ensurePrivateDirectory(folder)) {
            if (error)
                *error = QStringLiteral("No se pudo crear el directorio de la cuenta: %1").arg(folder);
            return false;
        }
    }

    QJsonObject metadata;
    metadata.insert(QStringLiteral("id"), account.id);
    metadata.insert(QStringLiteral("displayName"), account.displayName);
    metadata.insert(QStringLiteral("createdUtc"), QDateTime::currentDateTimeUtc().toString(Qt::ISODate));

    QSaveFile metadataFile(QDir(account.directory).filePath(QStringLiteral("account.json")));
    if (!metadataFile.open(QIODevice::WriteOnly)
        || metadataFile.write(QJsonDocument(metadata).toJson(QJsonDocument::Indented)) < 0
        || !metadataFile.commit()) {
        if (error)
            *error = metadataFile.errorString();
        return false;
    }

    QVector<StoredAccount> all = accounts();
    all.push_back(account);
    if (!saveAccountIndex(all, error))
        return false;

    if (created)
        *created = account;
    return true;
}
