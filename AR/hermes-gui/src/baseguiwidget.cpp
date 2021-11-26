#include "baseguiwidget.h"
#include "hermes.h"
#include <QEvent>
#include <QEventLoop>

BaseGuiWidget::BaseGuiWidget(QWidget *parent)
    : QWidget(getParrentOfParrent(parent))
    , m_parrentPosition(parent)
{
    //fill point offset;
    getParrentOfParrent(parent);
    connectSignals();
}

BaseGuiWidget::~BaseGuiWidget()
{
    //qInfo() << "Point Close";
    Hermes::getImpl()->disconnect(this);
}

bool BaseGuiWidget::event(QEvent *event)
{
    // qInfo() << event->type();
    if (event->type() == QEvent::FocusOut) {
        //qInfo() << "Point out";
        emit focusChanged();
    }
    if (event->type() == QEvent::HideToParent) {
        //qInfo() << "Point hide";
        emit focusChanged();
    }
    return QWidget::event(event);
}

void BaseGuiWidget::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    focusChanged();
}

QWidget *BaseGuiWidget::getParrentOfParrent(QWidget *pWidget)
{
    while (pWidget->parentWidget()) {
        m_offsetPosition += pWidget->pos();
        pWidget = pWidget->parentWidget();
    }
    return pWidget;
}

void BaseGuiWidget::exec()
{
    move(m_offsetPosition.x() - this->width(), std::max(m_offsetPosition.y() - this->height(), 0));
    show();
    setFocus();
    QEventLoop loop(this);
    LOCAL_ASSERT(connect(this, SIGNAL(destroyed()), &loop, SLOT(quit())));
    LOCAL_ASSERT(connect(this, SIGNAL(focusChanged()), this, SLOT(close())));
    LOCAL_ASSERT(connect(this, &BaseGuiWidget::commandSend, this, &BaseGuiWidget::focusChanged));
    loop.exec();
}

void BaseGuiWidget::connectSignals()
{
    LOCAL_ASSERT(connect(Hermes::getImpl(), &Hermes::commandCame, this, &BaseGuiWidget::onCommand));
    LOCAL_ASSERT(
        connect(Hermes::getImpl(), &Hermes::mavlinkMessage, this, &BaseGuiWidget::onMavlinkMessage));
    LOCAL_ASSERT(connect(Hermes::getImpl(), &Hermes::txtFrame, this, &BaseGuiWidget::onTxtFrame));
    LOCAL_ASSERT(connect(Hermes::getImpl(), &Hermes::dataFrame, this, &BaseGuiWidget::onDataFrame));
    LOCAL_ASSERT(connect(Hermes::getImpl(), &Hermes::eventDown, this, &BaseGuiWidget::onEventDown));
    LOCAL_ASSERT(connect(Hermes::getImpl(), &Hermes::initProps, this, &BaseGuiWidget::onInitProps));
    LOCAL_ASSERT(
        connect(this, &BaseGuiWidget::enevtUpSend, Hermes::getImpl(), &Hermes::eventUpReSend));
    LOCAL_ASSERT(
        connect(this, &BaseGuiWidget::commandSend, Hermes::getImpl(), &Hermes::commandReSend));
}

void BaseGuiWidget::onCommand(const QString &command, const boost::any &params) {}
void BaseGuiWidget::onMavlinkMessage(const std::list<mavlink_message_t> &messages) {}
void BaseGuiWidget::onTxtFrame(QString str, QString name) {}
void BaseGuiWidget::onDataFrame(const DataSampleCSPtr &frame) {}
void BaseGuiWidget::onEventDown(const QString &event, const boost::any &param) {}
void BaseGuiWidget::onInitProps(const QJsonObject & /*props*/) {}
