#include "video/videofilterrfstat.h"
#include "video/videofiltervertical.h"

VideoFilterRFStat::VideoFilterRFStat(QObject *parent)
    : BaseOSDVideoFilter(parent)
{}

void VideoFilterRFStat::setRxStat(const signalStatus &stat)
{
    if (stat.chipID == 0) {
        m_RXStatus_A = stat;
    }
    if (stat.chipID == 1) {
        m_RXStatus_B = stat;
    }
    if (stat.chipID == -1) {
        m_RXStatus_A = stat;
        m_RXStatus_B = stat;
    }
}

void VideoFilterRFStat::paintOSD(QPainter *painter)
{
    QString separator;
    if (m_RXStatus_A.reg_0xF733) {
        separator = "<| ";
    } else if (m_RXStatus_A.reg_0xF734) {
        separator = " |>";
    } else {
        separator = " | ";
    }

    QString msg;

    msg = "Signal : ";
    if (m_RXStatus_A.signalPresented) {
        msg += "YES";
    } else {
        msg += "N0 ";
    }
    if (m_RXStatus_A.signalLocked) {
        msg += " LOCK";
    } else {
        msg += " UNLOCK";
    }

    QOSDVerticalLayout layout;
    layout.addText(msg);

    msg = "Quality: " + QString::number(m_RXStatus_A.signalQuality, 'g', 4) + "%"
          + separator + QString::number(m_RXStatus_B.signalQuality, 'g', 4) + "%";
    layout.addText(msg);

    msg = "Corrupted: " + QString::number(m_RXStatus_A.abortCount, 'g', 4)
          + " Pkg:" + QString::number(m_RXStatus_A.errorPackages, 'g', 7);
    layout.addText(msg);

    msg = "Level: "
          + QString::number(m_RXStatus_A.signalStrength, 'g', 4).rightJustified(3, ' ')
          + separator
          + QString::number(m_RXStatus_B.signalStrength, 'g', 4).rightJustified(3, ' ')
          + "%";
    layout.addText(msg);

    msg = "Power: "
          + QString::number(m_RXStatus_A.strengthDbm, 'g', 4).rightJustified(3, ' ')
          + separator
          + QString::number(m_RXStatus_B.strengthDbm, 'g', 4).rightJustified(3, ' ') + "dB";
    layout.addText(msg);

    msg = "Frequency: " + QString::number(m_RXStatus_A.frequency / 1000, 'g', 4) + " MHz";
    layout.addText(msg);

    msg = "Bandwidth: " + QString::number(m_RXStatus_A.bandwith / 1000, 'g', 4) + " MHz";
    layout.addText(msg);

    layout.paint(0, 20, painter);
}
