#include "gui_tools.h"
//#include "gtuplugin-av/include/ffpackagedatasample.h"
//#include "rxquality.h"
#include <QImage>

QMutex g_dataLogGuard(QMutex::Recursive);
QStringList g_errors;
static const float SMALL_TEXT_SIZE = 0.028f;
static const float MEDIUM_TEXT_SIZE = SMALL_TEXT_SIZE * 1.2f;

bool FrameToImg(QImage &img, QSharedPointer<AVFrame> &frame)
{
    if (!frame) {
        return false;
    }

//    if (frame->format == AV_PIX_FMT_RGB32) {
//        if (img.width() != frame->width || img.height() != frame->height) {
//            img = QImage(frame->width, frame->height, QImage::Format_RGB32);
//        }

//        //        for (int y = 0; y < frame->height; ++y) {
//        //            memcpy(img.scanLine(y), frame->data[0] + y * frame->linesize[0], frame->linesize[0]);
//        //        }
//        img = QImage((uchar *) frame->data[0], frame->width, frame->height, QImage::Format_RGB32)
//                  .copy();
//        return true;
//    } else if (frame->format == AV_PIX_FMT_RGB24) {
//        if (img.width() != frame->width || img.height() != frame->height) {
//            img = QImage(frame->width, frame->height, QImage::Format_RGB888);
//        }
//        for (int y = 0; y < frame->height; ++y) {
//            memcpy(img.scanLine(y), frame->data[0] + y * frame->linesize[0], frame->linesize[0]);
//        }

//        //        QImage rgbImage((unsigned char *) (dataOut->GetByteArray().data()),
//        //                        dataOut->caps()->width,
//        //                        dataOut->caps()->height,
//        //                        QImage::Format_RGB888);
//        //        if (img.width() != frame->width || img.height() != frame->height) {
//        //            img = QImage(frame->width, frame->height, QImage::Format_RGB888);
//        //        }
//        //        //        for (int y = 0; y < frame->height; ++y) {
//        //        //            memcpy(img.scanLine(y), frame->data[0] + y * frame->linesize[0], frame->linesize[0]);
//        //        //        }
//        //        img = QImage((uchar *) frame->data[0], frame->width, frame->height, QImage::Format_RGB888)
//        //                  .copy();
//        return true;
//    } else {
//        //        if (img.width() != frame->width || img.height() != frame->height) {
//        //            img = QImage(frame->width, frame->height, QImage::Format_RGB888);
//        //        }
//        //        m_swsCtx = sws_getCachedContext(m_swsCtx,
//        //                                        frame->width,
//        //                                        frame->height,
//        //                                        (AVPixelFormat) frame->format,
//        //                                        img.width(),
//        //                                        img.height(),
//        //                                        AV_PIX_FMT_RGB24,
//        //                                        SWS_FAST_BILINEAR,
//        //                                        nullptr,
//        //                                        nullptr,
//        //                                        nullptr);
//        //        if (m_swsCtx) {
//        //            uint8_t *dstSlice[] = {img.bits()};
//        //            int dstStride = img.width() * 3;
//        //            sws_scale(m_swsCtx, frame->data, frame->linesize, 0, frame->height, dstSlice, &dstStride);
//        //            return true;
//        //        }
//    }
    return false;
}
