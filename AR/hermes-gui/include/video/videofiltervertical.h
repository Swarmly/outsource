#pragma once
#include <QObject>
#include <QString>
#include <QPainter>
#include <optional>

class QOSDVerticalLayout : public QObject {
    struct OSDItem {
        QString text;
        std::optional<QColor> color;
    };
public:
    QOSDVerticalLayout(QObject *parent = nullptr);

    void addText(const QString& text) { items_.append({text, {}}); }
    void addText(const QString& text, const QColor& color) { items_.append({text, color}); }
    void setTextColor(const QColor& color) { defaultTextColor_ = color; }
    void setTextFont(const QFont& font) { textFont_ = font; }
    void setShadowTextColor(const QColor& color) { shadowColor_ = color; }

    QRect boundingRect() const;

    void paint(int x, int y, QPainter* painter);

private:
    int yOffset(const QFontMetrics& fontMetrics) const;;

private:
    QList<OSDItem> items_;
    QFont textFont_;
    QColor shadowColor_;
    QColor defaultTextColor_;
};
