#pragma once
#include "hermes.h"
#include "videofilterbase.h"
#include <QPen>
#include <QTimer>
#include <QWidget>

class VideoFilterOSD: public BaseOSDVideoFilter
{
    Q_OBJECT
public:
    VideoFilterOSD(QObject *parent);

    void paintHUD(QPainter *painter, Qt::GlobalColor color);
    void setChEngaged(float m_procent);
    bool isDrawOSD();
    void setDrawOSD(bool value);

public slots:
    void onMavlinkMessage(const QMap<int, mavlink_message_t> &messages) override;

protected:
    bool m_drawOSD = true;

    enum Layout {
        COMPASS_INTEGRATED,
        COMPASS_SEPARATED // For a very high container. Feature panels are at bottom.
    };

    enum Style {
        NO_OVERLAYS, // Hzon not visible through tapes nor through feature panels. Frames with margins between.
        OVERLAY_HORIZONTAL, // Hzon visible through tapes and (frameless) feature panels.
        OVERLAY_HSI         // Hzon visible through everything except bottom feature panels.
    };
    QSize sizeHint() const;

private:
    Qt::GlobalColor m_mainColor;
    /*
     * There are at least these differences between airplane and copter PDF view:
     * - Airplane show absolute altutude in altimeter, copter shows relative to home
     */
    bool shouldDisplayNavigationData();

    void drawTextCenter(QPainter &painter, QString text, float fontSize, float x, float y);
    void drawTextLeftCenter(QPainter &painter, QString text, float fontSize, float x, float y);
    void drawTextRightCenter(QPainter &painter, QString text, float fontSize, float x, float y);
    void drawTextCenterBottom(QPainter &painter, QString text, float fontSize, float x, float y);
    void drawTextCenterTop(QPainter &painter, QString text, float fontSize, float x, float y);
    void drawAIGlobalFeatures(QPainter &painter, QRectF mainArea, QRectF paintArea);
    void drawAIAirframeFixedFeatures(QPainter &painter, QRectF area);
    void drawPitchScale(QPainter &painter,
                        QRectF area,
                        float intrusion,
                        bool drawNumbersLeft,
                        bool drawNumbersRight);
    void drawRollScale(QPainter &painter, QRectF area, bool drawTicks, bool drawNumbers);
    void drawAIAttitudeScales(QPainter &painter, QRectF area, float intrusion);
    void drawAICompassDisk(QPainter &painter, QRectF area, float halfspan);
    void drawSeparateCompassDisk(QPainter &painter, QRectF area);

    void drawAltimeter(
        QPainter &painter, QRectF area, float altitudeRelative, float altitudeAMSL, float vv);
    void drawVelocityMeter(QPainter &painter, QRectF area, float airSpeed);
    void fillInstrumentBackground(QPainter &painter, QRectF edge);
    void fillInstrumentOpagueBackground(QPainter &painter, QRectF edge);
    void drawInstrumentBackground(QPainter &painter, QRectF edge);

    void doPaint();

    bool didReceivePrimaryAltitude;
    bool didReceivePrimarySpeed;

    float roll;
    float pitch;
    float heading;

    float m_altitudeRelative;
    float m_altitudeAMSL;

    // APM: GPS and baro mix above home (GPS) altitude. This value comes from the GLOBAL_POSITION_INT message.
    // Do !!!NOT!!! ever do altitude calculations at the ground station. There are enough pitfalls already.
    // If the MP "set home altitude" button is migrated to here, it must set the UAS home altitude, not a GS-local one.
    // float aboveHomeAltitude;

    float m_groundspeed;
    float m_airspeed;
    float m_climbRate;

    float navigationAltitudeError;
    float navigationSpeedError;
    float navigationCrosstrackError;
    float navigationTargetBearing;

    Layout layout; // The display layout.
    Style style;   // The AI style (tapes translucent or opague)

    // TODO: Use stylesheet colors?
    QColor redColor;
    QColor amberColor;
    QColor greenColor;

    qreal lineWidth;
    qreal fineLineWidth;

    qreal smallTextSize;
    qreal mediumTextSize;
    qreal largeTextSize;

    // Globally used stuff only.
    QPen instrumentEdgePen;
    QBrush instrumentBackground;
    QBrush instrumentOpagueBackground;

    QFont font;

    static const int tickValues[];
    static const QString compassWindNames[];

    static const int updateInterval = 250;

    QPainter *m_painter;

    int width() const;
    int height() const;
    QRect rect() const;

    int streamSpeed;
    int dataSpeed;

    mavlink_attitude_t m_attitude;
    mavlink_global_position_int_t global_position_int_msg;
    mavlink_vfr_hud_t vfr_hud_msg;

    QString getMode(uint32_t mode);
    float m_engaged;

    int m_targetSysID = 0;
    int m_targetCompID = 0;

protected:
    void paintOSD(QPainter *painter) override;
};
