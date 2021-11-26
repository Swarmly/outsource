#pragma once
#include "coreutils.h"
#include "samplemetainfo.h"
#include <QSharedPointer>

using QByteArrayPtr = QSharedPointer<QByteArray>;
using DataSampleSPtr = QSharedPointer<class DataSample>;
//using DataSampleCSPtr = QSharedPointer<const class DataSample>;
using DataSampleCSPtr = QSharedPointer<class DataSample>;
using DataSampleWPtr = QWeakPointer<class DataSample>;

using CapsSPtr = QSharedPointer<struct Caps>;
using CapsCSPtr = QSharedPointer<const struct Caps>;

struct GTUCORESHARED_EXPORT Caps
{
    int width = 0;
    int height = 0;
    int m_sampleSize = -1; //! 1 byte for pixel or 2 or 3 ...
    int m_sourceId = -1;   //! producer of the sample
    int m_streamId = 0;    //! stream ID, should be unique for one source.
    QString m_sensorName;  //! Producer sensor name. Similar to source ID
    QString m_sampleName;  //! Sample name like DEPTH, COLOR, FLIR etc

    const QString &getFormat() const;
    void setFormat(const QString &value);
    CapsSPtr Clone() const;

private:
    //! Contains pixel format for decoded frame(RGB, BGR, GRAY16, etc), otherwise it contains
    //! codec name(H264, etc)
    QString format;
};


class GTUCORESHARED_EXPORT DataSample
{
public:
    DataSample();
    virtual ~DataSample();
    virtual quint64 GetTimeStampNS() const;
    virtual void SetTimeStampNS(const quint64 &nsec);
    static quint64 GetTimeStampOffsetNS(quint64 referenceTime = 0);
    static quint64 GetCurrentTimeStamp();
    virtual int GetSourceID() const;
    virtual int GetStreamID() const;
    virtual const QByteArray &GetByteArray() const;
    virtual const char *GetRawPointer() const;
    virtual void SetRawData(const QByteArray &rawData);
    virtual int GetSize() const;

    const SampleMetaInfo &metaInfo() const;
    SampleMetaInfo &metaInfo();
    void setMetaInfo(const SampleMetaInfo &metaInfo);

    const QString &format() const;     //!< format like h264
    const QString &getName() const;    //!< name like frontal_camera or sensor name
    const QString &sensorName() const; //!< sensor name like ffsrc
    const CapsCSPtr &caps() const;
    void setCaps(const CapsCSPtr &caps);

    DataSampleCSPtr GetChild(int idx) const;
    void AddChild(const DataSampleCSPtr &next);
    int GetChildCount() const;
    std::vector<DataSampleCSPtr> &GetChlds();
    std::vector<DataSampleCSPtr> &GetChlds() const;
    virtual DataSampleSPtr Clone() const;

protected:
    SampleMetaInfo m_metaInfo;
    quint64 m_timeStamp;
    mutable QByteArray m_rawData;
    CapsCSPtr m_caps;
    std::shared_ptr<std::vector<DataSampleCSPtr>> m_childs;
};
Q_DECLARE_METATYPE(DataSample)

/*!
 * @class DataSample
 * @brief GTU Core DataSample-interface.
 */

/**
 * @page DataSample DataSample
 * @brief DataSample interface.
 *
 * @section description Description
 * @detail
 * It is base interface for DataSamoles. Provides basic functionalyty
 *
 */
