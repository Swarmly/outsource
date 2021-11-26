#include "datasample.h"
#include "coreutils.h"
#include <chrono>

DataSample::DataSample()
    : m_timeStamp(GetCurrentTimeStamp())
{}

DataSample::~DataSample() {}

quint64 DataSample::GetTimeStampNS() const
{
    return m_timeStamp;
}

int DataSample::GetSourceID() const
{
    return m_caps->m_sourceId;
}

int DataSample::GetStreamID() const
{
    return m_caps->m_streamId;
}

const QByteArray &DataSample::GetByteArray() const
{
    return m_rawData;
}

int DataSample::GetSize() const
{
    return GetByteArray().size();
}

const char *DataSample::GetRawPointer() const
{
    return GetByteArray().data();
}

void DataSample::SetRawData(const QByteArray &rawData)
{
    m_rawData = rawData;
}

const SampleMetaInfo &DataSample::metaInfo() const
{
    return m_metaInfo;
}

SampleMetaInfo &DataSample::metaInfo()
{
    return m_metaInfo;
}

void DataSample::setMetaInfo(const SampleMetaInfo &metaInfo)
{
    m_metaInfo = metaInfo;
}

const QString &DataSample::format() const
{
    return caps()->getFormat();
}

const QString &DataSample::getName() const
{
    if (!caps()->m_sampleName.isEmpty()) {
        return caps()->m_sampleName;
    }
    return sensorName();
}

const QString &DataSample::sensorName() const
{
    return m_caps->m_sensorName;
}

const CapsCSPtr &DataSample::caps() const
{
    return m_caps;
}

void DataSample::setCaps(const CapsCSPtr &caps)
{
    m_caps = caps;
}

std::vector<DataSampleCSPtr> &DataSample::GetChlds()
{
    if (!m_childs) {
        m_childs.reset(new std::vector<DataSampleCSPtr>());
    }
    return *m_childs;
}

std::vector<DataSampleCSPtr> &DataSample::GetChlds() const
{
    return *m_childs;
}

DataSampleCSPtr DataSample::GetChild(int idx) const
{
    if (m_childs) {
        std::vector<DataSampleCSPtr> &childs = GetChlds();
        if (idx < int(childs.size())) {
            return childs[idx];
        }
    }
    return DataSampleSPtr();
}

void DataSample::AddChild(const DataSampleCSPtr &next)
{
    std::vector<DataSampleCSPtr> &childs = GetChlds();
    childs.push_back(next);
}

int DataSample::GetChildCount() const
{
    if (m_childs) {
        return m_childs->size();
    }
    return 0;
}

void DataSample::SetTimeStampNS(const quint64 &nsec)
{
    m_timeStamp = nsec;
}

quint64 DataSample::GetTimeStampOffsetNS(quint64 referenceTime)
{
    static quint64 startTime = referenceTime ? referenceTime : GetCurrentTimeStampNs();
    return startTime;
}

quint64 DataSample::GetCurrentTimeStamp()
{
    auto timestampOffset = GetTimeStampOffsetNS();
    return GetCurrentTimeStampNs() - timestampOffset;
}

const QString &Caps::getFormat() const
{
    return format;
}

void Caps::setFormat(const QString &value)
{
    format = value.toLower();
}

CapsSPtr Caps::Clone() const
{
    CapsSPtr caps(new Caps());
    *caps = *this;
    return caps;
}

DataSampleSPtr DataSample::Clone() const
{
    DataSampleSPtr sample(new DataSample());
    *sample = *this;
    return sample;
}
