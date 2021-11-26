#pragma once
#include "datasample.h"
#include "gui_tools.h"
#include <QWidget>

class HERMESGUI_EXPORT BaseGuiWidget : public QWidget
{
    Q_OBJECT
public:
    BaseGuiWidget(QWidget *parent = nullptr);
    ~BaseGuiWidget();
    virtual void connectSignals();
    void exec();

    QWidget *getParrentOfParrent(QWidget *pWidget);

signals:
    void commandSend(const QString &command, const boost::any &params = nullptr);
    void enevtUpSend(const QString &command, const boost::any &params = nullptr);
    void focusChanged();

public slots:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual bool event(QEvent *event) override;
    virtual void onCommand(const QString &command, const boost::any &params);
    virtual void onMavlinkMessage(const std::list<mavlink_message_t> &messages);
    virtual void onTxtFrame(QString str, QString name);
    virtual void onDataFrame(const DataSampleCSPtr &frame);
    virtual void onEventDown(const QString &event, const boost::any &param);
    virtual void onInitProps(const QJsonObject &props);

protected:
    QWidget *m_parrentPosition = nullptr;
    QPoint m_offsetPosition;
};
