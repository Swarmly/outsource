#include "video/videofiltervertical.h"

QOSDVerticalLayout::QOSDVerticalLayout(QObject *parent)
    : QObject(parent)
    , textFont_{"Monospace"}
    , shadowColor_{Qt::black}
    , defaultTextColor_{Qt::color0}
{
    textFont_.setPixelSize(16);
}

int QOSDVerticalLayout::yOffset(const QFontMetrics& fontMetrics) const {
    return fontMetrics.height() + fontMetrics.height() * 0.10 + 0.5;
}

QRect QOSDVerticalLayout::boundingRect() const {
    int maxWidth = 0;
    QFontMetrics fontMetrics{textFont_};
    for (auto&& item : items_) {
        auto boundingRect = fontMetrics.boundingRect(item.text);
        if (boundingRect.width() > maxWidth) {
            maxWidth = boundingRect.width();
        }
    }
    return QRect{
        0, 0, maxWidth + 1, items_.size() * yOffset(fontMetrics) + 1
    };
}

void QOSDVerticalLayout::paint(int x, int y, QPainter* painter) {
    painter->setFont(textFont_);
    QFontMetrics fontMetrics{textFont_};
    int yOffset = this->yOffset(fontMetrics);

    int currentY = y;
    for (auto&& item : items_) {
        // draw shadow
        painter->setPen(shadowColor_);
        painter->drawText(x, currentY, item.text);

        // draw text
        painter->setPen(item.color ? *item.color : defaultTextColor_);
        painter->drawText(x + 1, currentY + 1, item.text);
        currentY += yOffset;
    }
}
