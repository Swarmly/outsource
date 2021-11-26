#pragma once
#include <QDateTime>
#include <QDebug>
#include <QtCore/qglobal.h>

#if defined(GTU_CORE_LIBRARY)
#define GTUCORESHARED_EXPORT Q_DECL_EXPORT
#else
#define GTUCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#define define2string_p(x) #x
#define define2string(x) define2string_p(x)

#ifndef QT_NO_DEBUG
#define LOCAL_ASSERT(x) Q_ASSERT(x)
#else
#define LOCAL_ASSERT(x) x
#endif

#define LOG_DEBUG \
    qDebug().noquote() << QDateTime::currentDateTime().toString(Qt::ISODate) \
                       << "[DEBUG:" << GetSensorName() << ":" << __FUNCTION__ << "]"
#define LOG_INFO \
    qInfo().noquote() << QDateTime::currentDateTime().toString(Qt::ISODate) \
                      << "[INFO:" << GetSensorName() << ":" << __FUNCTION__ << "]"
#define LOG_WARNING \
    qWarning().noquote() << QDateTime::currentDateTime().toString(Qt::ISODate) \
                         << "[WARNING:" << GetSensorName() << ":" << __FUNCTION__ << "]"
#define LOG_ERROR \
    qCritical().noquote() << QDateTime::currentDateTime().toString(Qt::ISODate) \
                          << "[ERROR:" << GetSensorName() << ":" << __FUNCTION__ << "]"
#define LOG_FATAL \
    qFatal().noquote() << QDateTime::currentDateTime().toString(Qt::ISODate) \
                       << "[FATAL:" << GetSensorName() << ":" << __FUNCTION__ << "]"

#define LOG_DEBUG_EXT \
    qDebug().noquote() << QDateTime::currentDateTime().toString(Qt::ISODate) \
                       << "[DEBUG:" << __FUNCTION__ << "]"
#define LOG_INFO_EXT \
    qInfo().noquote() << QDateTime::currentDateTime().toString(Qt::ISODate) \
                      << "[INFO:" << __FUNCTION__ << "]"
#define LOG_WARNING_EXT \
    qWarning().noquote() << QDateTime::currentDateTime().toString(Qt::ISODate) \
                         << "[WARNING:" << __FUNCTION__ << "]"
#define LOG_ERROR_EXT \
    qCritical().noquote() << QDateTime::currentDateTime().toString(Qt::ISODate) \
                          << "[ERROR:" << __FUNCTION__ << "]"
#define LOG_FATAL_EXT \
    qFatal().noquote() << QDateTime::currentDateTime().toString(Qt::ISODate) \
                       << "[FATAL:" << __FUNCTION__ << "]"
