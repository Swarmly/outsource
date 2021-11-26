#pragma once
//#include "gtucore.h"
#include <QMutex>
#include <QPainter>
#include <boost/any.hpp>
struct AVFrame;

namespace mavlink {};
using namespace mavlink;
#include "mavlink/v2.0/ardupilotmega/mavlink.h"
#include "mavlink/v2.0/mavlink_get_info.h"
#include "mavlink/v2.0/mavlink_types.h"

#include "mavlink/nv-ext/mavlink_nv_ext.h"

#if defined(HERMES_GUI_LIBRARY)
#define HERMESGUI_EXPORT Q_DECL_EXPORT
#else
#define HERMESGUI_EXPORT Q_DECL_IMPORT
#endif
bool HERMESGUI_EXPORT FrameToImg(QImage &img, QSharedPointer<AVFrame> &frame);
HERMESGUI_EXPORT extern QMutex g_dataLogGuard;
HERMESGUI_EXPORT extern QStringList g_errors;
