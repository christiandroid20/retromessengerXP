#include "useravatar.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>

UserAvatar::UserAvatar(int extent, QWidget *parent)
    : QLabel(parent),
      m_initials(QStringLiteral("?"))
{
    setFixedSize(extent, extent);
    setAccessibleName(QStringLiteral("Foto de perfil"));
    setToolTip(QStringLiteral("Foto de perfil"));
}

void UserAvatar::setPhoto(const QString &filePath)
{
    QPixmap photo(filePath);
    m_photo = photo;
    update();
}

void UserAvatar::setInitials(const QString &initials)
{
    m_initials = initials.trimmed().left(2).toUpper();
    if (m_initials.isEmpty())
        m_initials = QStringLiteral("?");
    update();
}

void UserAvatar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    const QRectF bounds = rect().adjusted(1, 1, -1, -1);
    QPainterPath frame;
    frame.addRoundedRect(bounds, 4, 4);

    painter.fillPath(frame, QColor(QStringLiteral("#ffffff")));
    painter.save();
    painter.setClipPath(frame);

    const QRect photoRect = bounds.adjusted(3, 3, -3, -3).toRect();
    if (!m_photo.isNull()) {
        const QPixmap scaled = m_photo.scaled(photoRect.size(), Qt::KeepAspectRatioByExpanding,
                                              Qt::SmoothTransformation);
        const int x = (scaled.width() - photoRect.width()) / 2;
        const int y = (scaled.height() - photoRect.height()) / 2;
        painter.drawPixmap(photoRect, scaled, QRect(x, y, photoRect.width(), photoRect.height()));
    } else {
        QLinearGradient gradient(photoRect.topLeft(), photoRect.bottomRight());
        gradient.setColorAt(0.0, QColor(QStringLiteral("#b8d9ff")));
        gradient.setColorAt(1.0, QColor(QStringLiteral("#4b83ce")));
        painter.fillRect(photoRect, gradient);
        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setBold(true);
        font.setPointSize(qMax(8, width() / 4));
        painter.setFont(font);
        painter.drawText(photoRect, Qt::AlignCenter, m_initials);
    }

    painter.restore();
    painter.setPen(QPen(QColor(QStringLiteral("#6f91c8")), 1));
    painter.drawPath(frame);
}
