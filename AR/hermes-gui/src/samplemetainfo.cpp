#include "samplemetainfo.h"

SampleMetaInfo::SampleMetaInfo() = default;

bool SampleMetaInfo::hasProp(const QString &name) const
{
    return _storage.contains(name);
}

boost::any SampleMetaInfo::rawProp(const QString &name) const
{
    if (_storage.contains(name)) {
        return _storage.value(name);
    }
    return boost::any();
}

void SampleMetaInfo::setPropRaw(const QString &name, const boost::any &value)
{
    _storage[name] = value;
}

void SampleMetaInfo::delProp(const QString &name)
{
    if (_storage.contains(name)) {
        _storage.find(name);
    }
}

void SampleMetaInfo::setMetaStorage(const metaStroage &storage)
{
    _storage = storage;
}

const SampleMetaInfo::metaStroage &SampleMetaInfo::getMetaStorage() const
{
    return _storage;
}
