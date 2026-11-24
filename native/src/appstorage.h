#pragma once

#include <QString>
#include <QVector>

struct StoredAccount
{
    QString id;
    QString displayName;
    QString directory;
};

class AppStorage final
{
public:
    AppStorage();

    QString dataRoot() const;
    QString accountsRoot() const;
    QString storageDescription() const;
    QVector<StoredAccount> accounts() const;
    bool addAccount(const QString &displayName, StoredAccount *created, QString *error = nullptr) const;

private:
    QString m_dataRoot;
    QString m_storageDescription;

    bool saveAccountIndex(const QVector<StoredAccount> &accounts, QString *error) const;
};
