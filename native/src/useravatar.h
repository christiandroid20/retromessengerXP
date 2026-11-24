#pragma once

#include <QLabel>
#include <QPixmap>

class UserAvatar final : public QLabel
{
    Q_OBJECT

public:
    explicit UserAvatar(int extent = 48, QWidget *parent = nullptr);
    void setPhoto(const QString &filePath);
    void setInitials(const QString &initials);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_photo;
    QString m_initials;
};
