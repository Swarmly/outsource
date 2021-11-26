#pragma once
#include "gtu-core_global.h"
#include <boost/any.hpp>
#include <memory>
#include <QMap>
#include <QSharedPointer>
#include <QString>

class GTUCORESHARED_EXPORT SampleMetaInfo
{
public:
    SampleMetaInfo();
    using metaStroage = QMap<QString, boost::any>;

    bool hasProp(const QString &name) const;
    boost::any rawProp(const QString &name) const;
    void setPropRaw(const QString &name, const boost::any &value);
    void delProp(const QString &name);
    template<typename T1> bool checkPropType(const QString &name, bool reportError = false) const;
    template<typename T1> bool readProp(const QString &name, T1 &value) const;
    template<typename T1> T1 getProp(const QString &name, const T1 &defVal) const;
    template<typename T1> void setProp(const QString &name, const T1 &value);
    void setMetaStorage(const metaStroage &storage);
    const metaStroage &getMetaStorage() const;

private:
    metaStroage _storage;
};

using SpMetaInfoCSPtr = std::shared_ptr<const SampleMetaInfo>;
using SpMetaInfoSPtr = std::shared_ptr<SampleMetaInfo>;

template<typename T1>
bool SampleMetaInfo::checkPropType(const QString &name, bool reportError) const
{
    if (rawProp(name).type() == typeid(T1)) {
        return true;
    }
    if (reportError) {
        qWarning() << "Types not matched:" << rawProp(name).type().name() << " AND "
                   << typeid(T1).name();
    }
    return false;
}

template<typename T1> bool SampleMetaInfo::readProp(const QString &name, T1 &valueOut) const
{
    try {
        if (checkPropType<T1>(name, true)) {
            valueOut = boost::any_cast<T1>(rawProp(name));
            return true;
        }
    } catch (const boost::bad_any_cast &e) {
        //qWarning() << e.what();
        // LOG_ERROR << e.what();
    }
    return false;
}

template<typename T1> T1 SampleMetaInfo::getProp(const QString &name, const T1 &valueDefault) const
{
    T1 retVal(valueDefault);
    if (hasProp(name) && readProp(name, retVal)) {
        return retVal;
    }
    // LOG_ERROR << e.what();
    return valueDefault;
}

template<typename T1> void SampleMetaInfo::setProp(const QString &name, const T1 &value)
{
    boost::any anyVal(value);
    setPropRaw(name, anyVal);
}
