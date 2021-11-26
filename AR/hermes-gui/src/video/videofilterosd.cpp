#include "video/videofilterosd.h"
//#include "commonmavlinkutils.h"
#include <cmath>
#include <QDateTime>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QRectF>
#include <QResizeEvent>
#include <QTimer>
#include <QtCore/qmath.h>
static const float LINEWIDTH = 0.0036f;
static const float SMALL_TEXT_SIZE = 0.028f;
static const float MEDIUM_TEXT_SIZE = SMALL_TEXT_SIZE * 1.2f;
static const float LARGE_TEXT_SIZE = MEDIUM_TEXT_SIZE * 1.2f;

static const bool SHOW_ZERO_ON_SCALES = true;

// all in units of display height
static const float ROLL_SCALE_RADIUS = 0.42f;
static const float ROLL_SCALE_TICKMARKLENGTH = 0.04f;
static const float ROLL_SCALE_MARKERWIDTH = 0.06f;
static const float ROLL_SCALE_MARKERHEIGHT = 0.04f;
// scale max. degrees
static const int ROLL_SCALE_RANGE = 60;

// fraction of height to translate for each degree of pitch.
static const float PITCHTRANSLATION = 65;
// 5 degrees for each line
static const int PITCH_SCALE_RESOLUTION = 5;
static const float PITCH_SCALE_MAJORWIDTH = 0.1f;
static const float PITCH_SCALE_MINORWIDTH = 0.066;

// Beginning from PITCH_SCALE_WIDTHREDUCTION_FROM degrees of +/- pitch, the
// width of the lines is reduced, down to PITCH_SCALE_WIDTHREDUCTION times
// the normal width. This helps keep orientation in extreme attitudes.
static const int PITCH_SCALE_WIDTHREDUCTION_FROM = 30;
static const float PITCH_SCALE_WIDTHREDUCTION = 0.3f;

static const int PITCH_SCALE_HALFRANGE = 15;

// The number of degrees to either side of the heading to draw the compass disk.
// 180 is valid, this will draw a complete disk. If the disk is partly clipped
// away, less will do.

static const int COMPASS_DISK_MAJORTICK = 10;
static const int COMPASS_DISK_ARROWTICK = 45;
// static const float COMPASS_DISK_MAJORLINEWIDTH = 0.006;
// static const float COMPASS_DISK_MINORLINEWIDTH = 0.004;
static const int COMPASS_DISK_RESOLUTION = 10;
// static const float COMPASS_SEPARATE_DISK_RESOLUTION = 5;
static const float COMPASS_DISK_MARKERWIDTH = 0.2;
static const float COMPASS_DISK_MARKERHEIGHT = 0.133;

static const int CROSSTRACK_MAX = 1000;
static const float CROSSTRACK_RADIUS = 0.6;

static const float TAPE_GAUGES_TICKWIDTH_MAJOR = 0.25;
static const float TAPE_GAUGES_TICKWIDTH_MINOR = 0.15;

// The altitude difference between top and bottom of scale
static const int ALTIMETER_LINEAR_SPAN = 50;
// every 5 meters there is a tick mark
static const int ALTIMETER_LINEAR_RESOLUTION = 5;
// every 10 meters there is a number
static const int ALTIMETER_LINEAR_MAJOR_RESOLUTION = 10;

// Projected: An experiment. Make tape appear projected from a cylinder, like a French "drum" style gauge.
// The altitude difference between top and bottom of scale
// static const int ALTIMETER_PROJECTED_SPAN = 50;
// every 5 meters there is a tick mark
// static const int ALTIMETER_PROJECTED_RESOLUTION = 5;
// every 10 meters there is a number
// static const int ALTIMETER_PROJECTED_MAJOR_RESOLUTION = 10;
// min. and max. vertical velocity

// Now the same thing for airspeed!
static const int AIRSPEED_LINEAR_SPAN = 15;
static const int AIRSPEED_LINEAR_RESOLUTION = 1;
static const int AIRSPEED_LINEAR_MAJOR_RESOLUTION = 5;

static const int UNKNOWN_ATTITUDE = -1000;
static const int UNKNOWN_ALTITUDE = -1000;
static const int UNKNOWN_SPEED = -1;
/*
 *@TODO:
 * global fixed pens (and painters too?)
 * repaint on demand multiple canvases
 * multi implementation with shared model class
 */
double PrimaryFlightDisplay_round(double value, int digits = 0)
{
    return floor(value * pow(10.0, digits) + 0.5) / pow(10.0, digits);
}

qreal PrimaryFlightDisplay_constrain(qreal value, qreal min, qreal max)
{
    if (value < min)
        value = min;
    else if (value > max)
        value = max;
    return value;
}

struct stHUD
{
    int x = 0;
    int y = 0;
    double lon = std::numeric_limits<double>::infinity();
    double lat = std::numeric_limits<double>::infinity();
    double cameraYaw = 0;
    double courseYaw = 0;
    qreal speed = 0;
    int alt = 0;
    int rel_alt = 0;
    int pitch = 0;
    int roll = 0;
    int zoom = 0;
};

const int VideoFilterOSD::tickValues[] = {10, 20, 30, 45, 60};
const QString VideoFilterOSD::compassWindNames[] = {QString("N"),
                                                    QString("NE"),
                                                    QString("E"),
                                                    QString("SE"),
                                                    QString("S"),
                                                    QString("SW"),
                                                    QString("W"),
                                                    QString("NW")};

bool VideoFilterOSD::isDrawOSD()
{
    return m_drawOSD;
}

void VideoFilterOSD::setDrawOSD(bool value)
{
    m_drawOSD = value;
}

void VideoFilterOSD::setChEngaged(float m_procent)
{
    m_engaged = m_procent;
}

void VideoFilterOSD::paintHUD(QPainter *painter, Qt::GlobalColor color)
{
    if (painter->window().width() <= 356) {
        return;
    }

    if (m_drawOSD) {
        // stHUD hud;
        m_painter = painter;
        m_mainColor = color;

        // hud = AppCore::Impl().GetDataManager().getHUD();

        m_painter->setPen(m_mainColor);
        QPen pen = m_painter->pen();
        pen.setWidth(2);
        m_painter->setPen(pen);

        QFont font = m_painter->font();
        font.setPointSize(18);
        m_painter->setFont(font);

        m_painter->setBrush(Qt::NoBrush);

        roll = m_attitude.roll * 57.2958;
        pitch = m_attitude.pitch * 57.2958;
        heading = m_attitude.yaw * 57.2958;
        if (heading < 0) {
            heading += 360;
        }

        m_altitudeRelative = global_position_int_msg.relative_alt / 1000;

        m_airspeed = vfr_hud_msg.airspeed;
        m_groundspeed = vfr_hud_msg.groundspeed;

        qreal size = 640; // width();

        lineWidth = PrimaryFlightDisplay_constrain(size * LINEWIDTH, 1, 6);
        fineLineWidth = PrimaryFlightDisplay_constrain(size * LINEWIDTH * 2 / 3, 1, 2);

        instrumentEdgePen.setWidthF(fineLineWidth);

        smallTextSize = size * SMALL_TEXT_SIZE;
        mediumTextSize = size * MEDIUM_TEXT_SIZE;
        largeTextSize = size * LARGE_TEXT_SIZE;
        // if(hud.zoom <= 10)
        {
            int deltaCross = 10;
            int deltaX = 10;

            int wWidth = m_painter->window().width();
            int wHeight = m_painter->window().height();

            m_painter->drawPoint(wWidth / 2, wHeight / 2);
            m_painter->drawLine(wWidth / 2 - deltaX * 10,
                                wHeight / 2,
                                wWidth / 2 - deltaCross,
                                wHeight / 2);
            m_painter->drawLine(wWidth / 2 + deltaX * 10,
                                wHeight / 2,
                                wWidth / 2 + deltaCross,
                                wHeight / 2);

            m_painter->drawLine(wWidth / 2,
                                wHeight / 2 - deltaX * 10,
                                wWidth / 2,
                                wHeight / 2 - deltaCross);
            m_painter->drawLine(wWidth / 2,
                                wHeight / 2 + deltaX * 10,
                                wWidth / 2,
                                wHeight / 2 + deltaCross);
        }
        doPaint();
    }
}

QSize VideoFilterOSD::sizeHint() const
{
    return QSize(width(), (width() * 3.0f) / 4);
}

// TODO: Implement. Should return true when navigating.
// That would be (APM) in AUTO and RTL modes.
// This could forward to a virtual on UAS bool isNavigatingAutonomusly() or whatever.
bool VideoFilterOSD::shouldDisplayNavigationData()
{
    return true;
}

void VideoFilterOSD::drawTextCenter(
    QPainter &painter, QString text, float pixelSize, float x, float y)
{
    font.setPixelSize(pixelSize);
    painter.setFont(font);

    QPen currentPen = painter.pen();
    painter.setPen(Qt::black);

    QFontMetrics metrics = QFontMetrics(font);
    QRect bounds = metrics.boundingRect(text);
    int flags = Qt::AlignCenter | Qt::TextDontClip; // For some reason the bounds rect is too small!
    painter.drawText(x /*+bounds.x()*/ - bounds.width() / 2,
                     y /*+bounds.y()*/ - bounds.height() / 2,
                     bounds.width(),
                     bounds.height(),
                     flags,
                     text);

    painter.setPen(currentPen);

    painter.drawText(x /*+bounds.x()*/ - bounds.width() / 2 + 1,
                     y /*+bounds.y()*/ - bounds.height() / 2 + 1,
                     bounds.width(),
                     bounds.height(),
                     flags,
                     text);
}

void VideoFilterOSD::drawTextLeftCenter(
    QPainter &painter, QString text, float pixelSize, float x, float y)
{
    font.setPixelSize(pixelSize);
    painter.setFont(font);

    QFontMetrics metrics = QFontMetrics(font);
    QRect bounds = metrics.boundingRect(text);
    int flags = Qt::AlignLeft | Qt::TextDontClip; // For some reason the bounds rect is too small!
    QPen penOrig = painter.pen();
    QPen penDark = painter.pen();
    penDark.setColor(Qt::black);
    painter.setPen(penDark);
    painter.drawText(x - 2 /*+bounds.x()*/,
                     y - 2 /*+bounds.y()*/ - bounds.height() / 2,
                     bounds.width(),
                     bounds.height(),
                     flags,
                     text);
    painter.setPen(penOrig);
    painter.drawText(x /*+bounds.x()*/,
                     y /*+bounds.y()*/ - bounds.height() / 2,
                     bounds.width(),
                     bounds.height(),
                     flags,
                     text);
}

void VideoFilterOSD::drawTextRightCenter(
    QPainter &painter, QString text, float pixelSize, float x, float y)
{
    font.setPixelSize(pixelSize);
    painter.setFont(font);

    QFontMetrics metrics = QFontMetrics(font);
    QRect bounds = metrics.boundingRect(text);
    int flags = Qt::AlignRight | Qt::TextDontClip; // For some reason the bounds rect is too small!

    QPen currentPen = painter.pen();
    painter.setPen(Qt::black);

    painter.drawText(x /*+bounds.x()*/ - bounds.width(),
                     y /*+bounds.y()*/ - bounds.height() / 2,
                     bounds.width(),
                     bounds.height(),
                     flags,
                     text);

    painter.setPen(currentPen);

    painter.drawText(x /*+bounds.x()*/ - bounds.width() + 1,
                     y /*+bounds.y()*/ - bounds.height() / 2 + 1,
                     bounds.width(),
                     bounds.height(),
                     flags,
                     text);
}

void VideoFilterOSD::drawTextCenterTop(
    QPainter &painter, QString text, float pixelSize, float x, float y)
{
    font.setPixelSize(pixelSize);
    painter.setFont(font);

    QFontMetrics metrics = QFontMetrics(font);
    QRect bounds = metrics.boundingRect(text);
    int flags = Qt::AlignCenter | Qt::TextDontClip; // For some reason the bounds rect is too small!
    painter.drawText(x /*+bounds.x()*/ - bounds.width() / 2,
                     y + bounds.height() /*+bounds.y()*/,
                     bounds.width(),
                     bounds.height(),
                     flags,
                     text);
}

void VideoFilterOSD::drawTextCenterBottom(
    QPainter &painter, QString text, float pixelSize, float x, float y)
{
    font.setPixelSize(pixelSize);
    painter.setFont(font);

    QFontMetrics metrics = QFontMetrics(font);
    QRect bounds = metrics.boundingRect(text);
    int flags = Qt::AlignCenter;
    painter.drawText(x /*+bounds.x()*/ - bounds.width() / 2,
                     y /*+bounds.y()*/,
                     bounds.width(),
                     bounds.height(),
                     flags,
                     text);
}

void VideoFilterOSD::drawInstrumentBackground(QPainter &painter, QRectF edge)
{
    painter.setPen(instrumentEdgePen);
    painter.drawRect(edge);
}

void VideoFilterOSD::fillInstrumentBackground(QPainter &painter, QRectF edge)
{
    painter.setPen(instrumentEdgePen);
    painter.setBrush(instrumentBackground);
    painter.drawRect(edge);
    painter.setBrush(Qt::NoBrush);
}

void VideoFilterOSD::fillInstrumentOpagueBackground(QPainter &painter, QRectF edge)
{
    painter.setPen(instrumentEdgePen);
    painter.setBrush(instrumentOpagueBackground);
    painter.drawRect(edge);
    painter.setBrush(Qt::NoBrush);
}

qreal pitchAngleToTranslation(qreal viewHeight, float pitch)
{
    if (pitch == UNKNOWN_ATTITUDE)
        return 0;
    return pitch * viewHeight / PITCHTRANSLATION;
}

void VideoFilterOSD::drawAIAirframeFixedFeatures(QPainter &painter, QRectF area)
{
    // red line from -7/10 to -5/10 half-width
    // red line from 7/10 to 5/10 half-width
    // red slanted line from -2/10 half-width to 0
    // red slanted line from 2/10 half-width to 0
    // red arrow thing under roll scale
    // prepareTransform(painter, width, height);
    painter.resetTransform();
    painter.translate(area.center());

    qreal w = area.width();
    qreal h = area.height();

    //    QPen pen;
    //    pen.setWidthF(lineWidth * 1.5f);
    //    pen.setColor(redColor);
    //    painter.setPen(pen);

    //    float length = 0.15f;
    //    float side = 0.5f;
    //    // The 2 lines at sides.
    //    painter.drawLine(QPointF(-side*w, 0), QPointF(-(side-length)*w, 0));
    //    painter.drawLine(QPointF(side*w, 0), QPointF((side-length)*w, 0));

    //    float rel = length/qSqrt(2.0f);
    //    // The gull
    //    painter.drawLine(QPointF(rel*w, rel*w/2), QPoint(0, 0));
    //    painter.drawLine(QPointF(-rel*w, rel*w/2), QPoint(0, 0));

    // The roll scale marker.
    QPainterPath markerPath(QPointF(0, -w * ROLL_SCALE_RADIUS + 1));
    markerPath.lineTo(-h * ROLL_SCALE_MARKERWIDTH / 2,
                      -w * (ROLL_SCALE_RADIUS - ROLL_SCALE_MARKERHEIGHT) + 1);
    markerPath.lineTo(h * ROLL_SCALE_MARKERWIDTH / 2,
                      -w * (ROLL_SCALE_RADIUS - ROLL_SCALE_MARKERHEIGHT) + 1);
    markerPath.closeSubpath();
    painter.drawPath(markerPath);
}

inline qreal min4(qreal a, qreal b, qreal c, qreal d)
{
    if (b < a)
        a = b;
    if (c < a)
        a = c;
    if (d < a)
        a = d;
    return a;
}

inline qreal max4(qreal a, qreal b, qreal c, qreal d)
{
    if (b > a)
        a = b;
    if (c > a)
        a = c;
    if (d > a)
        a = d;
    return a;
}

void VideoFilterOSD::drawAIGlobalFeatures(QPainter &painter, QRectF mainArea, QRectF paintArea)
{
    float displayRoll = this->roll;
    if (displayRoll == UNKNOWN_ATTITUDE)
        displayRoll = 0;

    painter.resetTransform();
    painter.translate(mainArea.center());

    qreal pitchPixels = pitchAngleToTranslation(mainArea.height(), pitch);
    qreal gradientEnd = pitchAngleToTranslation(mainArea.height(), 60);

    // painter.rotate(-roll);
    // painter.translate(0, pitchPixels);

    //    QTransform forwardTransform;
    // forwardTransform.translate(mainArea.center().x(), mainArea.center().y());
    painter.rotate(-displayRoll);
    painter.translate(0, pitchPixels);

    // Calculate the radius of area we need to paint to cover all.
    QTransform rtx = painter.transform().inverted();

    QPointF topLeft = rtx.map(paintArea.topLeft());
    QPointF topRight = rtx.map(paintArea.topRight());
    QPointF bottomLeft = rtx.map(paintArea.bottomLeft());
    QPointF bottomRight = rtx.map(paintArea.bottomRight());
    // Just KISS... make a rectangluar basis.
    qreal minx = min4(topLeft.x(), topRight.x(), bottomLeft.x(), bottomRight.x());
    qreal maxx = max4(topLeft.x(), topRight.x(), bottomLeft.x(), bottomRight.x());
    qreal miny = min4(topLeft.y(), topRight.y(), bottomLeft.y(), bottomRight.y());
    qreal maxy = max4(topLeft.y(), topRight.y(), bottomLeft.y(), bottomRight.y());

    QPointF hzonLeft = QPoint(minx, 0);
    QPointF hzonRight = QPoint(maxx, 0);

    QPainterPath skyPath(hzonLeft);
    skyPath.lineTo(QPointF(minx, miny));
    skyPath.lineTo(QPointF(maxx, miny));
    skyPath.lineTo(hzonRight);
    skyPath.closeSubpath();

    QLinearGradient skyGradient(0, -gradientEnd, 0, 0);
    skyGradient.setColorAt(0, QColor::fromHsvF(0.6, 1.0, 0.7));
    skyGradient.setColorAt(1, QColor::fromHsvF(0.6, 0.25, 0.9));
    QBrush skyBrush(skyGradient);
    painter.fillPath(skyPath, skyBrush);

    QPainterPath groundPath(hzonRight);
    groundPath.lineTo(maxx, maxy);
    groundPath.lineTo(minx, maxy);
    groundPath.lineTo(hzonLeft);
    groundPath.closeSubpath();

    QLinearGradient groundGradient(0, gradientEnd, 0, 0);
    groundGradient.setColorAt(0, QColor::fromHsvF(0.25, 1, 0.5));
    groundGradient.setColorAt(1, QColor::fromHsvF(0.25, 0.25, 0.5));
    QBrush groundBrush(groundGradient);
    painter.fillPath(groundPath, groundBrush);

    QPen pen;
    pen.setWidthF(lineWidth);
    pen.setColor(greenColor);
    painter.setPen(pen);

    QPointF start(-mainArea.width(), 0);
    QPoint end(mainArea.width(), 0);
    painter.drawLine(start, end);
}

void VideoFilterOSD::drawPitchScale(
    QPainter &painter, QRectF area, float intrusion, bool drawNumbersLeft, bool drawNumbersRight)
{
    Q_UNUSED(intrusion)

    float displayPitch = this->pitch;
    if (displayPitch == UNKNOWN_ATTITUDE)
        displayPitch = 0;

    // The area should be quadratic but if not width is the major size.
    qreal w = area.width();
    if (w < area.height())
        w = area.height();

    QPen pen;
    pen.setWidthF(lineWidth);
    pen.setColor(m_mainColor);
    painter.setPen(pen);

    QTransform savedTransform = painter.transform();

    // find the mark nearest center
    int snap = qRound((double) (displayPitch / PITCH_SCALE_RESOLUTION)) * PITCH_SCALE_RESOLUTION;
    int _min = snap - PITCH_SCALE_HALFRANGE;
    int _max = snap + PITCH_SCALE_HALFRANGE;
    for (int degrees = _min; degrees <= _max; degrees += PITCH_SCALE_RESOLUTION) {
        bool isMajor = degrees % (PITCH_SCALE_RESOLUTION * 2) == 0;
        float linewidth = isMajor ? PITCH_SCALE_MAJORWIDTH : PITCH_SCALE_MINORWIDTH;
        if (abs(degrees) > PITCH_SCALE_WIDTHREDUCTION_FROM) {
            // we want: 1 at PITCH_SCALE_WIDTHREDUCTION_FROM and PITCH_SCALE_WIDTHREDUCTION at 90.
            // That is PITCH_SCALE_WIDTHREDUCTION + (1-PITCH_SCALE_WIDTHREDUCTION) * f(pitch)
            // where f(90)=0 and f(PITCH_SCALE_WIDTHREDUCTION_FROM)=1
            // f(p) = (90-p) * 1/(90-PITCH_SCALE_WIDTHREDUCTION_FROM)
            // or PITCH_SCALE_WIDTHREDUCTION + f(pitch) - f(pitch) * PITCH_SCALE_WIDTHREDUCTION
            // or PITCH_SCALE_WIDTHREDUCTION (1-f(pitch)) + f(pitch)
            int fromVertical = std::abs(pitch >= 0 ? 90 - pitch : -90 - pitch);
            float temp = fromVertical * 1 / (90.0f - PITCH_SCALE_WIDTHREDUCTION_FROM);
            linewidth *= (PITCH_SCALE_WIDTHREDUCTION * (1 - temp) + temp);
        }

        float shift = pitchAngleToTranslation(w, displayPitch - degrees);

        // TODO: Intrusion detection and evasion. That is, don't draw
        // where the compass has intruded.

        painter.translate(0, shift);
        QPointF start(-linewidth * w, 0);
        QPointF end(linewidth * w, 0);
        painter.drawLine(start, end);

        if (isMajor && (drawNumbersLeft || drawNumbersRight)) {
            int displayDegrees = degrees;
            if (displayDegrees > 90)
                displayDegrees = 180 - displayDegrees;
            else if (displayDegrees < -90)
                displayDegrees = -180 - displayDegrees;
            if (SHOW_ZERO_ON_SCALES || degrees) {
                QString s_number;
                if (this->pitch == UNKNOWN_ATTITUDE)
                    s_number.sprintf("-");
                else
                    s_number.sprintf("%d", displayDegrees);
                if (drawNumbersLeft)
                    drawTextRightCenter(painter,
                                        s_number,
                                        mediumTextSize,
                                        -PITCH_SCALE_MAJORWIDTH * w - 10,
                                        0);
                if (drawNumbersRight)
                    drawTextLeftCenter(painter,
                                       s_number,
                                       mediumTextSize,
                                       PITCH_SCALE_MAJORWIDTH * w + 10,
                                       0);
            }
        }

        painter.setTransform(savedTransform);
    }
}

void VideoFilterOSD::drawRollScale(QPainter &painter, QRectF area, bool drawTicks, bool drawNumbers)
{
    qreal w = area.width();
    if (w < area.height())
        w = area.height();

    QPen pen;
    pen.setWidthF(lineWidth);
    pen.setColor(m_mainColor);
    painter.setPen(pen);

    // We should really do these transforms but they are assumed done by caller.
    // painter.resetTransform();
    // painter.translate(area.center());
    // painter.rotate(roll);

    qreal _size = w * ROLL_SCALE_RADIUS * 2;
    QRectF arcArea(-_size / 2, -_size / 2, _size, _size);
    painter.drawArc(arcArea, (90 - ROLL_SCALE_RANGE) * 16, ROLL_SCALE_RANGE * 2 * 16);
    // painter.drawEllipse(QPoint(0,0),200,200);
    if (drawTicks) {
        int length = sizeof(tickValues) / sizeof(int);
        qreal previousRotation = 0;
        for (int i = 0; i < length * 2 + 1; i++) {
            int degrees = (i == length)
                              ? 0
                              : (i > length) ? -tickValues[i - length - 1] : tickValues[i];
            // degrees = 180 - degrees;
            painter.rotate(degrees - previousRotation);
            previousRotation = degrees;

            QPointF start(0, -_size / 2);
            QPointF end(0, -(1.0 + ROLL_SCALE_TICKMARKLENGTH) * _size / 2);

            painter.drawLine(start, end);

            QString s_number; //= QString("%d").arg(degrees);
            if (SHOW_ZERO_ON_SCALES || degrees)
                s_number.sprintf("%d", abs(degrees));

            if (drawNumbers) {
                drawTextCenterBottom(painter,
                                     s_number,
                                     mediumTextSize,
                                     0,
                                     -(ROLL_SCALE_RADIUS + ROLL_SCALE_TICKMARKLENGTH * 1.7) * w);
            }
        }
    }
}

void VideoFilterOSD::drawAIAttitudeScales(QPainter &painter, QRectF area, float intrusion)
{
    float displayRoll = this->roll;
    if (displayRoll == UNKNOWN_ATTITUDE)
        displayRoll = 0;
    // To save computations, we do these transformations once for both scales:
    painter.resetTransform();
    painter.translate(area.center());
    painter.rotate(-displayRoll);
    QTransform saved = painter.transform();

    drawRollScale(painter, area, true, true);
    painter.setTransform(saved);
    // drawPitchScale(painter, area, intrusion, true, true);
}

void VideoFilterOSD::drawAICompassDisk(QPainter &painter, QRectF area, float halfspan)
{
    float displayHeading = this->heading;
    if (displayHeading == UNKNOWN_ATTITUDE)
        displayHeading = 0;

    float start = displayHeading - halfspan;
    float end = displayHeading + halfspan;

    int firstTick = ceil(start / COMPASS_DISK_RESOLUTION) * COMPASS_DISK_RESOLUTION;
    int lastTick = floor(end / COMPASS_DISK_RESOLUTION) * COMPASS_DISK_RESOLUTION;

    float radius = area.width() / 2;
    float innerRadius = radius * 0.96;
    painter.resetTransform();
    painter.setBrush(instrumentBackground);
    painter.setPen(instrumentEdgePen);
    painter.drawEllipse(area);
    painter.setBrush(Qt::NoBrush);

    QPen scalePen(Qt::darkGray);
    scalePen.setWidthF(fineLineWidth);

    for (int tickYaw = firstTick; tickYaw <= lastTick; tickYaw += COMPASS_DISK_RESOLUTION) {
        int displayTick = tickYaw;
        if (displayTick < 0)
            displayTick += 360;
        else if (displayTick >= 360)
            displayTick -= 360;

        // yaw is in center.
        float off = tickYaw - displayHeading;
        // wrap that to ]-180..180]
        if (off <= -180)
            off += 360;
        else if (off > 180)
            off -= 360;

        painter.translate(area.center());
        painter.rotate(off);
        bool drewArrow = false;
        bool isMajor = displayTick % COMPASS_DISK_MAJORTICK == 0;

        // If heading unknown, still draw marks but no numbers.
        if (this->heading != UNKNOWN_ATTITUDE
            && (displayTick == 30 || displayTick == 60 || displayTick == 120 || displayTick == 150
                || displayTick == 210 || displayTick == 240 || displayTick == 300
                || displayTick == 330)) {
            // draw a number
            QString s_number;
            s_number.sprintf("%d", displayTick / 10);
            painter.setPen(scalePen);
            drawTextCenter(painter, s_number, smallTextSize, 0, -innerRadius * 0.75);
        } else {
            if (displayTick % COMPASS_DISK_ARROWTICK == 0) {
                if (displayTick != 0) {
                    QPainterPath markerPath(
                        QPointF(0, -innerRadius * (1 - COMPASS_DISK_MARKERHEIGHT / 2)));
                    markerPath.lineTo(innerRadius * COMPASS_DISK_MARKERWIDTH / 4, -innerRadius);
                    markerPath.lineTo(-innerRadius * COMPASS_DISK_MARKERWIDTH / 4, -innerRadius);
                    markerPath.closeSubpath();
                    painter.setPen(scalePen);
                    painter.setBrush(Qt::SolidPattern);
                    painter.drawPath(markerPath);
                    painter.setBrush(Qt::NoBrush);
                    drewArrow = true;
                }
                // If heading unknown, still draw marks but no N S E W.
                if (this->heading != UNKNOWN_ATTITUDE && displayTick % 90 == 0) {
                    // Also draw a label
                    QString name = compassWindNames[displayTick / 45];
                    painter.setPen(scalePen);
                    drawTextCenter(painter, name, mediumTextSize, 0, -innerRadius * 0.75);
                }
            }
        }
        // draw the scale lines. If an arrow was drawn, stay off from it.

        QPointF p_start = drewArrow ? QPoint(0, -innerRadius * 0.94) : QPoint(0, -innerRadius);
        QPoint p_end = isMajor ? QPoint(0, -innerRadius * 0.86) : QPoint(0, -innerRadius * 0.90);

        painter.setPen(scalePen);
        painter.drawLine(p_start, p_end);
        painter.resetTransform();
    }

    painter.setPen(scalePen);
    // painter.setBrush(Qt::SolidPattern);
    painter.translate(area.center());
    QPainterPath markerPath(QPointF(0, -radius - 2));
    markerPath.lineTo(radius * COMPASS_DISK_MARKERWIDTH / 2,
                      -radius - radius * COMPASS_DISK_MARKERHEIGHT - 2);
    markerPath.lineTo(-radius * COMPASS_DISK_MARKERWIDTH / 2,
                      -radius - radius * COMPASS_DISK_MARKERHEIGHT - 2);
    markerPath.closeSubpath();
    painter.drawPath(markerPath);

    qreal digitalCompassYCenter = -radius * 0.52;
    qreal digitalCompassHeight = radius * 0.28;

    QPointF digitalCompassBottom(0, digitalCompassYCenter + digitalCompassHeight);
    QPointF digitalCompassAbsoluteBottom = painter.transform().map(digitalCompassBottom);

    qreal digitalCompassUpshift = digitalCompassAbsoluteBottom.y() > height()
                                      ? digitalCompassAbsoluteBottom.y() - height()
                                      : 0;

    QRectF digitalCompassRect(-radius / 3,
                              -radius * 0.52 - digitalCompassUpshift,
                              radius * 2 / 3,
                              radius * 0.28);
    painter.setPen(instrumentEdgePen);
    painter.drawRoundedRect(digitalCompassRect,
                            instrumentEdgePen.widthF() * 2 / 3,
                            instrumentEdgePen.widthF() * 2 / 3);

    QString s_digitalCompass;

    if (this->heading == UNKNOWN_ATTITUDE)
        s_digitalCompass.sprintf("---");
    else {
        /* final safeguard for really stupid systems */
        int digitalCompassValue = static_cast<int>(qRound((double) heading)) % 360;
        s_digitalCompass.sprintf("%03d", digitalCompassValue);
    }

    QPen pen;
    pen.setWidthF(lineWidth);
    pen.setColor(m_mainColor);
    painter.setPen(pen);

    drawTextCenter(painter,
                   s_digitalCompass,
                   largeTextSize,
                   0,
                   -radius * 0.38 - digitalCompassUpshift);

    //  dummy
    //  navigationTargetBearing = 10;
    //  navigationCrosstrackError = 500;

    // The CDI
    if (shouldDisplayNavigationData() && navigationTargetBearing != UNKNOWN_ATTITUDE
        && !qIsInf(navigationCrosstrackError)) {
        painter.resetTransform();
        painter.translate(area.center());
        // TODO : Sign might be wrong?
        // TODO : The case where error exceeds max. Truncate to max. and make that visible somehow.
        bool errorBeyondRadius = false;
        if (std::abs(navigationCrosstrackError) > CROSSTRACK_MAX) {
            errorBeyondRadius = true;
            navigationCrosstrackError = navigationCrosstrackError > 0 ? CROSSTRACK_MAX
                                                                      : -CROSSTRACK_MAX;
        }

        float r = radius * CROSSTRACK_RADIUS;
        float x = navigationCrosstrackError / CROSSTRACK_MAX * r;
        float y = qSqrt(r * r - x * x); // the positive y, there is also a negative.

        float sillyHeading = 0;
        float angle = sillyHeading - navigationTargetBearing; // TODO: sign.
        painter.rotate(-angle);

        QPen pen;
        pen.setWidthF(lineWidth);
        pen.setColor(Qt::black);
        if (errorBeyondRadius) {
            pen.setStyle(Qt::DotLine);
        }
        painter.setPen(pen);

        painter.drawLine(QPointF(x, y), QPointF(x, -y));
    }
}

void VideoFilterOSD::drawAltimeter(QPainter &painter,
                                   QRectF area, // the area where to draw the tape.
                                   float altitudeRelative,
                                   float altitudeAMSL,
                                   float vv)
{
    painter.resetTransform();
    fillInstrumentBackground(painter, area);

    QPen pen;
    pen.setWidthF(lineWidth);

    pen.setColor(m_mainColor);
    painter.setPen(pen);

    float h = area.height();
    float w = area.width();
    float secondaryAltitudeBoxHeight = mediumTextSize * 2;
    // The height where we being with new tickmarks.
    float effectiveHalfHeight = h * 0.45;

    // not yet implemented: Display of secondary altitude.
    if (altitudeAMSL != UNKNOWN_ALTITUDE) {
        effectiveHalfHeight -= secondaryAltitudeBoxHeight;
    }

    float markerHalfHeight = mediumTextSize * 0.8;
    float leftEdge = instrumentEdgePen.widthF() * 2;
    float rightEdge = w - leftEdge;
    float tickmarkLeft = leftEdge;
    float tickmarkRightMajor = tickmarkLeft + TAPE_GAUGES_TICKWIDTH_MAJOR * w;
    float tickmarkRightMinor = tickmarkLeft + TAPE_GAUGES_TICKWIDTH_MINOR * w;
    float numbersLeft = 0.42 * w;
    float markerTip = (tickmarkLeft * 2 + tickmarkRightMajor) / 3;
    float scaleCenterAltitude = altitudeRelative == UNKNOWN_ALTITUDE ? 0 : altitudeRelative;

    // altitude scale
    float start = scaleCenterAltitude - ALTIMETER_LINEAR_SPAN / 2;
    float end = scaleCenterAltitude + ALTIMETER_LINEAR_SPAN / 2;
    int firstTick = ceil(start / ALTIMETER_LINEAR_RESOLUTION) * ALTIMETER_LINEAR_RESOLUTION;
    int lastTick = floor(end / ALTIMETER_LINEAR_RESOLUTION) * ALTIMETER_LINEAR_RESOLUTION;
    for (int tickAlt = firstTick; tickAlt <= lastTick; tickAlt += ALTIMETER_LINEAR_RESOLUTION) {
        float y = (tickAlt - scaleCenterAltitude) * effectiveHalfHeight
                  / (ALTIMETER_LINEAR_SPAN / 2);
        bool isMajor = tickAlt % ALTIMETER_LINEAR_MAJOR_RESOLUTION == 0;

        painter.resetTransform();
        painter.translate(area.left(), area.center().y() - y);
        pen.setColor(tickAlt < 0 ? redColor : m_mainColor);
        painter.setPen(pen);
        if (isMajor) {
            painter.drawLine(tickmarkLeft, 0, tickmarkRightMajor, 0);
            QString s_alt;
            s_alt.sprintf("%d", abs(tickAlt));
            drawTextLeftCenter(painter, s_alt, mediumTextSize, numbersLeft, 0);
        } else {
            painter.drawLine(tickmarkLeft, 0, tickmarkRightMinor, 0);
        }
    }

    QPainterPath markerPath(QPoint(markerTip, 0));
    markerPath.lineTo(markerTip + markerHalfHeight, markerHalfHeight);
    markerPath.lineTo(rightEdge, markerHalfHeight);
    markerPath.lineTo(rightEdge, -markerHalfHeight);
    markerPath.lineTo(markerTip + markerHalfHeight, -markerHalfHeight);
    markerPath.closeSubpath();

    painter.resetTransform();
    painter.translate(area.left(), area.center().y());

    pen.setWidthF(lineWidth);
    pen.setColor(m_mainColor);
    painter.setPen(pen);

    painter.setBrush(Qt::SolidPattern);
    painter.drawPath(markerPath);
    painter.setBrush(Qt::NoBrush);

    pen.setColor(m_mainColor);
    painter.setPen(pen);

    QString s_alt;
    if (altitudeRelative == UNKNOWN_ALTITUDE)
        s_alt.sprintf("---");
    else
        s_alt.sprintf("Alt: %3.0f m", altitudeRelative);

    float xCenter = (markerTip + rightEdge) / 2;
    drawTextCenter(painter, s_alt, mediumTextSize, xCenter, 0);

    // print secondary altitude
    if (altitudeAMSL != UNKNOWN_ALTITUDE) {
        QRectF saBox(area.x(), area.y() - secondaryAltitudeBoxHeight, w, secondaryAltitudeBoxHeight);
        painter.resetTransform();
        painter.translate(saBox.center());
        QString s_salt;
        s_salt.sprintf("%3.0f", altitudeAMSL);
        drawTextCenter(painter, s_salt, mediumTextSize, 0, 0);
    }

    // print target altitude (if applicable)
}

void VideoFilterOSD::drawVelocityMeter(QPainter &painter, QRectF area, float airSpeed)
{
    painter.resetTransform();
    fillInstrumentBackground(painter, area);

    QPen pen;
    pen.setWidthF(lineWidth);

    float h = area.height();
    float w = area.width();
    float effectiveHalfHeight = h * 0.45;
    float markerHalfHeight = mediumTextSize;
    float leftEdge = instrumentEdgePen.widthF() * 2;
    float tickmarkRight = w - leftEdge;
    float tickmarkLeftMajor = tickmarkRight - w * TAPE_GAUGES_TICKWIDTH_MAJOR;
    float tickmarkLeftMinor = tickmarkRight - w * TAPE_GAUGES_TICKWIDTH_MINOR;
    float numbersRight = 0.42 * w;
    float markerTip = (tickmarkLeftMajor + tickmarkRight * 2) / 3;

    // Select between air and ground speed:

    // Airspeed is always populated on APM
    // if it Rover or Copter is always set to groundspeed
    // if it is Plane it is either real airspeed or estimated airspeed.
    float centerScaleSpeed = airSpeed;
    // QString speedType = "GND";// : "AIR"; // [TODO] Fix to show air or gnd based on vehicle type

    float start = centerScaleSpeed - AIRSPEED_LINEAR_SPAN / 2;
    float end = centerScaleSpeed + AIRSPEED_LINEAR_SPAN / 2;

    int firstTick = ceil(start / AIRSPEED_LINEAR_RESOLUTION) * AIRSPEED_LINEAR_RESOLUTION;
    int lastTick = floor(end / AIRSPEED_LINEAR_RESOLUTION) * AIRSPEED_LINEAR_RESOLUTION;
    for (int tickSpeed = firstTick; tickSpeed <= lastTick; tickSpeed += AIRSPEED_LINEAR_RESOLUTION) {
        pen.setColor(tickSpeed < 0 ? redColor : m_mainColor);
        painter.setPen(pen);

        float y = (tickSpeed - centerScaleSpeed) * effectiveHalfHeight / (AIRSPEED_LINEAR_SPAN / 2);
        bool hasText = tickSpeed % AIRSPEED_LINEAR_MAJOR_RESOLUTION == 0;
        painter.resetTransform();

        painter.translate(area.left(), area.center().y() - y);

        if (hasText) {
            painter.drawLine(tickmarkLeftMajor, 0, tickmarkRight, 0);
            QString s_speed;
            s_speed.sprintf("%d", abs(tickSpeed));
            drawTextRightCenter(painter, s_speed, mediumTextSize, numbersRight, 0);
        } else {
            painter.drawLine(tickmarkLeftMinor, 0, tickmarkRight, 0);
        }
    }

    QPainterPath markerPath(QPoint(markerTip, 0));
    markerPath.lineTo(markerTip - markerHalfHeight, markerHalfHeight);
    markerPath.lineTo(leftEdge, markerHalfHeight);
    markerPath.lineTo(leftEdge, -markerHalfHeight);
    markerPath.lineTo(markerTip - markerHalfHeight, -markerHalfHeight);
    markerPath.closeSubpath();

    painter.resetTransform();
    painter.translate(area.left(), area.center().y());

    pen.setWidthF(lineWidth);
    pen.setColor(m_mainColor);
    painter.setPen(pen);

    painter.setBrush(Qt::SolidPattern);
    painter.drawPath(markerPath);
    painter.setBrush(Qt::NoBrush);

    pen.setColor(m_mainColor);
    painter.setPen(pen);
    QString s_alt;
    if (airSpeed == UNKNOWN_SPEED)
        s_alt.sprintf("---");
    else
        s_alt.sprintf("AS: %3.1f m/s", airSpeed);
    float xCenter = (markerTip + leftEdge) / 2;
    drawTextCenter(painter, s_alt, /* TAPES_TEXT_SIZE*width()*/ mediumTextSize, xCenter, 0);

    QString s_gspeed;
    if (m_groundspeed == UNKNOWN_SPEED) {
        s_gspeed.sprintf("---");
    } else {
        s_gspeed.sprintf("GS: %3.1f m/s", m_groundspeed);
    }

    painter.setPen(Qt::yellow);
    drawTextCenter(painter, s_gspeed, smallTextSize, 60, 35);
}

static const int TOP = (1 << 0);
static const int BOTTOM = (1 << 1);
static const int LEFT = (1 << 2);
static const int RIGHT = (1 << 3);

static const int TOP_HALF = (1 << 4);
static const int BOTTOM_HALF = (1 << 5);
static const int LEFT_HALF = (1 << 6);
static const int RIGHT_HALF = (1 << 7);

void applyMargin(QRectF &area, float margin, int where)
{
    if (margin < 0.01)
        return;

    QRectF save(area);
    qreal consumed;

    if (where & LEFT) {
        area.setX(save.x() + (consumed = margin));
    } else if (where & LEFT_HALF) {
        area.setX(save.x() + (consumed = margin / 2));
    } else {
        consumed = 0;
    }

    if (where & RIGHT) {
        area.setWidth(save.width() - consumed - margin);
    } else if (where & RIGHT_HALF) {
        area.setWidth(save.width() - consumed - margin / 2);
    } else {
        area.setWidth(save.width() - consumed);
    }

    if (where & TOP) {
        area.setY(save.y() + (consumed = margin));
    } else if (where & TOP_HALF) {
        area.setY(save.y() + (consumed = margin / 2));
    } else {
        consumed = 0;
    }

    if (where & BOTTOM) {
        area.setHeight(save.height() - consumed - margin);
    } else if (where & BOTTOM_HALF) {
        area.setHeight(save.height() - consumed - margin / 2);
    } else {
        area.setHeight(save.height() - consumed);
    }
}

void setMarginsForInlineLayout(
    qreal margin, QRectF &panel1, QRectF &panel2, QRectF &panel3, QRectF &panel4)
{
    applyMargin(panel1, margin, BOTTOM | LEFT | RIGHT_HALF);
    applyMargin(panel2, margin, BOTTOM | LEFT_HALF | RIGHT_HALF);
    applyMargin(panel3, margin, BOTTOM | LEFT_HALF | RIGHT_HALF);
    applyMargin(panel4, margin, BOTTOM | LEFT_HALF | RIGHT);
}

void setMarginsForCornerLayout(
    qreal margin, QRectF &panel1, QRectF &panel2, QRectF &panel3, QRectF &panel4)
{
    applyMargin(panel1, margin, BOTTOM | LEFT | RIGHT_HALF);
    applyMargin(panel2, margin, BOTTOM | LEFT_HALF | RIGHT_HALF);
    applyMargin(panel3, margin, BOTTOM | LEFT_HALF | RIGHT_HALF);
    applyMargin(panel4, margin, BOTTOM | LEFT_HALF | RIGHT);
}

inline qreal tapesGaugeWidthFor(qreal containerWidth, qreal preferredAIWidth)
{
    qreal result = (containerWidth - preferredAIWidth) / 2.0f;
    qreal minimum = containerWidth / 5.5f;
    if (result < minimum)
        result = minimum;
    return result;
}

void VideoFilterOSD::doPaint()
{
    QPainter &painter = *m_painter;
    // painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

    // The AI centers on this area.
    QRectF AIMainArea;
    // The AI paints on this area. It should contain the AIMainArea.
    QRectF AIPaintArea;

    QRectF compassArea;
    QRectF altimeterArea;
    QRectF velocityMeterArea;

    // painter.fillRect(rect(), Qt::black);
    qreal tapeGaugeWidth;

    qreal compassHalfSpan = 180;
    float compassAIIntrusion = 0;

    // COMPASS
    // A layout for containers higher than their width.
    tapeGaugeWidth = tapesGaugeWidthFor(width(), width());

    qreal aiheight = width() - tapeGaugeWidth * 2;
    qreal panelsHeight = 0;
    qreal wdth = height() / 2 - aiheight / 2;

    AIMainArea = QRectF(tapeGaugeWidth, 0, width() - tapeGaugeWidth * 2, aiheight);

    AIPaintArea = style == OVERLAY_HSI ? QRectF(0, 0, width(), height() - panelsHeight)
                                       : AIMainArea;

    velocityMeterArea = QRectF(0, wdth, tapeGaugeWidth, aiheight);
    altimeterArea = QRectF(AIMainArea.right(), wdth, tapeGaugeWidth, aiheight);

    QPoint compassCenter = QPoint(width() / 2, height());
    qreal compassDiam = height() * 0.8;
    compassArea = QRectF(compassCenter.x() - compassDiam / 2,
                         compassCenter.y() - 128,
                         compassDiam,
                         compassDiam);

    bool hadClip = painter.hasClipping();

    painter.setClipping(true);
    painter.setClipRect(AIPaintArea);
    drawAIAttitudeScales(painter, AIMainArea, compassAIIntrusion);
    drawAIAirframeFixedFeatures(painter, AIMainArea);
    drawAICompassDisk(painter, compassArea, compassHalfSpan);

    painter.setClipping(hadClip);

    drawAltimeter(painter, altimeterArea, m_altitudeRelative, m_altitudeAMSL, m_climbRate);

    drawVelocityMeter(painter, velocityMeterArea, m_airspeed);
}

int VideoFilterOSD::width() const
{
    return m_painter->window().width();
}

QString VideoFilterOSD::getMode(uint32_t mode)
{
    enum Mode {
        MANUAL = 0,
        CIRCLE = 1,
        STABILIZE = 2,
        TRAINING = 3,
        ACRO = 4,
        FLY_BY_WIRE_A = 5,
        FLY_BY_WIRE_B = 6,
        CRUISE = 7,
        AUTOTUNE = 8,
        RESERVED_9 = 9, // RESERVED FOR FUTURE USE
        AUTO = 10,
        RTL = 11,
        LOITER = 12,
        RESERVED_13 = 13, // RESERVED FOR FUTURE USE
        RESERVED_14 = 14, // RESERVED FOR FUTURE USE
        GUIDED = 15,
        INITIALIZING = 16,
        QSTABILIZE = 17,
        QHOVER = 18,
        QLOITER = 19,
        QLAND = 20,
        QRTL = 21,
        modeCount
    };

    QMap<uint32_t, QString> enumToString;
    if (enumToString.empty()) {
        enumToString.insert(MANUAL, "Manual");
        enumToString.insert(CIRCLE, "Circle");
        enumToString.insert(STABILIZE, "Stabilize");
        enumToString.insert(TRAINING, "Training");
        enumToString.insert(ACRO, "Acro");
        enumToString.insert(FLY_BY_WIRE_A, "FBW A");
        enumToString.insert(FLY_BY_WIRE_B, "FBW B");
        enumToString.insert(CRUISE, "Cruise");
        enumToString.insert(AUTOTUNE, "Autotune");
        enumToString.insert(AUTO, "Auto");
        enumToString.insert(RTL, "RTL");
        enumToString.insert(LOITER, "Loiter");
        enumToString.insert(GUIDED, "Guided");
        enumToString.insert(INITIALIZING, "Initializing");
        enumToString.insert(QSTABILIZE, "QuadPlane Stabilize");
        enumToString.insert(QHOVER, "QuadPlane Hover");
        enumToString.insert(QLOITER, "QuadPlane Loiter");
        enumToString.insert(QLAND, "QuadPlane Land");
        enumToString.insert(QRTL, "QuadPlane RTL");
    }
    return enumToString[mode];
}

int VideoFilterOSD::height() const
{
    return m_painter->window().height();
}

QRect VideoFilterOSD::rect() const
{
    return m_painter->window();
}

VideoFilterOSD::VideoFilterOSD(QObject *parent)
    : BaseOSDVideoFilter(parent)
    , roll(UNKNOWN_ATTITUDE)
    , pitch(UNKNOWN_ATTITUDE)
    , heading(UNKNOWN_ATTITUDE)
    , m_altitudeRelative(UNKNOWN_ALTITUDE)
    , m_altitudeAMSL(UNKNOWN_ALTITUDE)
    , m_groundspeed(UNKNOWN_SPEED)
    , m_airspeed(UNKNOWN_SPEED)
    , m_climbRate(UNKNOWN_ALTITUDE)
    , navigationCrosstrackError(0)
    , navigationTargetBearing(UNKNOWN_ATTITUDE)
    , layout(COMPASS_INTEGRATED)
    , style(OVERLAY_HSI)
    , redColor(QColor::fromHsvF(0, 0.75, 0.9))
    , amberColor(QColor::fromHsvF(0.12, 0.6, 1.0))
    , greenColor(QColor::fromHsvF(0.25, 0.8, 0.8))
    , lineWidth(2)
    , fineLineWidth(1)
    , instrumentEdgePen(QColor::fromHsvF(0, 0, 0.65, 0.5))
    , instrumentBackground(QColor::fromHsvF(0, 0, 0.3, 0.3))
    , instrumentOpagueBackground(QColor::fromHsvF(0, 0, 0.3, 1.0))
    , font("Bitstream Vera Sans") //,
    , streamSpeed(0)
    , dataSpeed(0)
{
    memset(&m_attitude, 0, sizeof(m_attitude));
    memset(&global_position_int_msg, 0, sizeof(global_position_int_msg));
    memset(&vfr_hud_msg, 0, sizeof(vfr_hud_msg));
}

void VideoFilterOSD::paintOSD(QPainter *painter)
{
    paintHUD(painter, Qt::green);
}

void VideoFilterOSD::onMavlinkMessage(const QMap<int, mavlink_message_t> &messages)
{
    if (messages.contains(MAVLINK_MSG_ID_HEARTBEAT)) {
        auto &msg = messages[MAVLINK_MSG_ID_HEARTBEAT];
//        if (isItDrone(&msg)) {
//            m_targetSysID = msg.sysid;
//            m_targetCompID = msg.compid;
//        }
    }
    if (messages.contains(MAVLINK_MSG_ID_ATTITUDE)) {
        auto &msg = messages[MAVLINK_MSG_ID_ATTITUDE];
        if (msg.compid == m_targetCompID && msg.sysid == m_targetSysID) {
            mavlink_msg_attitude_decode(&msg, &m_attitude);
        }
    }
    if (messages.contains(MAVLINK_MSG_ID_GLOBAL_POSITION_INT)) {
        auto &msg = messages[MAVLINK_MSG_ID_GLOBAL_POSITION_INT];
        if (msg.compid == m_targetCompID && msg.sysid == m_targetSysID) {
            mavlink_msg_global_position_int_decode(&msg, &global_position_int_msg);
        }
    }
    if (messages.contains(MAVLINK_MSG_ID_VFR_HUD)) {
        auto &msg = messages[MAVLINK_MSG_ID_VFR_HUD];
        if (msg.compid == m_targetCompID && msg.sysid == m_targetSysID) {
            mavlink_msg_vfr_hud_decode(&msg, &vfr_hud_msg);
        }
    }
}
