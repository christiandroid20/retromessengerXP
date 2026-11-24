#pragma once

#include <QObject>
#include <QString>
#include <QVector>

class QApplication;

struct SkinDescriptor
{
    QString id;
    QString displayName;
    QString resourcePath;
    QString externalPath;
};

class SkinManager final : public QObject
{
    Q_OBJECT

public:
    explicit SkinManager(QObject *parent = nullptr);

    QVector<SkinDescriptor> availableSkins() const;
    void reload();
    bool apply(const QString &skinId, QApplication *application) const;

private:
    QVector<SkinDescriptor> m_skins;
};
