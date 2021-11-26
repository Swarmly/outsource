/******************************************************************************
    QtAV:  Multimedia framework based on Qt and FFmpeg
    Copyright (C) 2012-2016 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV (from 2014)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/

#ifndef QTAV_QT3DRENDERER_H
#define QTAV_QT3DRENDERER_H

#include <QtAVWidgets/global.h>
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
#include <QtWidgets/QOpenGLWidget>
#else
#include <QtAVWidgets/QOpenGLWidget.h>
#endif //QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
#include <QtAV/OpenGLRendererBase.h>

#include <QOffscreenSurface>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QResizeEvent>
#include <QScopedPointer>
#include <QSurfaceFormat>
#include <Qt3DCore/QAspectEngine>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DInput/QInputAspect>
#include <Qt3DInput/QInputSettings>
#include <Qt3DLogic/QFrameAction>
#include <Qt3DLogic/QLogicAspect>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraSelector>
#include <Qt3DRender/QClearBuffers>
#include <Qt3DRender/QDepthTest>
#include <Qt3DRender/QMultiSampleAntiAliasing>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QRenderCapture>
#include <Qt3DRender/QRenderCaptureReply>
#include <Qt3DRender/QRenderSettings>
#include <Qt3DRender/QRenderStateSet>
#include <Qt3DRender/QRenderSurfaceSelector>
#include <Qt3DRender/QRenderTarget>
#include <Qt3DRender/QRenderTargetOutput>
#include <Qt3DRender/QRenderTargetSelector>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QViewport>
#include <QtGui/QGuiApplication>
#include <QtGui/QResizeEvent>
#include <QtGui/QScreen>

extern bool renderType;

namespace QtAV {
// do not define QOpenGLWidget here with ifdef to avoid moc error
class Qt3DRendererPrivate;
class Q_AVWIDGETS_EXPORT Qt3DRenderer : public QOpenGLWidget, public OpenGLRendererBase
{
    Q_OBJECT
    DPTR_DECLARE_PRIVATE(Qt3DRenderer)
    Q_PROPERTY(qreal brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(qreal contrast READ contrast WRITE setContrast NOTIFY contrastChanged)
    Q_PROPERTY(qreal hue READ hue WRITE setHue NOTIFY hueChanged)
    Q_PROPERTY(qreal saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QRectF regionOfInterest READ regionOfInterest WRITE setRegionOfInterest NOTIFY regionOfInterestChanged)
    Q_PROPERTY(qreal sourceAspectRatio READ sourceAspectRatio NOTIFY sourceAspectRatioChanged)
    Q_PROPERTY(qreal outAspectRatio READ outAspectRatio WRITE setOutAspectRatio NOTIFY outAspectRatioChanged)
    //fillMode
    // TODO: how to use enums in base class as property or Q_ENUM
    Q_PROPERTY(OutAspectRatioMode outAspectRatioMode READ outAspectRatioMode WRITE setOutAspectRatioMode NOTIFY outAspectRatioModeChanged)
    Q_ENUMS(OutAspectRatioMode)
    Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(QRect videoRect READ videoRect NOTIFY videoRectChanged)
    Q_PROPERTY(QSize videoFrameSize READ videoFrameSize NOTIFY videoFrameSizeChanged)
    Q_ENUMS(Quality)
public:
    explicit Qt3DRenderer(QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual VideoRendererId id() const Q_DECL_OVERRIDE;
    virtual QWidget* widget() Q_DECL_OVERRIDE { return this; }
Q_SIGNALS:
    void sourceAspectRatioChanged(qreal value) Q_DECL_OVERRIDE Q_DECL_FINAL;
    void regionOfInterestChanged() Q_DECL_OVERRIDE;
    void outAspectRatioChanged() Q_DECL_OVERRIDE;
    void outAspectRatioModeChanged() Q_DECL_OVERRIDE;
    void brightnessChanged(qreal value) Q_DECL_OVERRIDE;
    void contrastChanged(qreal) Q_DECL_OVERRIDE;
    void hueChanged(qreal) Q_DECL_OVERRIDE;
    void saturationChanged(qreal) Q_DECL_OVERRIDE;
    void orientationChanged() Q_DECL_OVERRIDE;
    void videoRectChanged() Q_DECL_OVERRIDE;
    void videoFrameSizeChanged() Q_DECL_OVERRIDE;
    void backgroundColorChanged() Q_DECL_OVERRIDE;
protected:
    void registerAspect(Qt3DCore::QAbstractAspect *aspect);
    void registerAspect(const QString &name);

    void setRootEntity(Qt3DCore::QEntity *root);

    void setActiveFrameGraph(Qt3DRender::QFrameGraphNode *activeFrameGraph);
    Qt3DRender::QFrameGraphNode *activeFrameGraph() const;
    Qt3DExtras::QForwardRenderer *defaultFrameGraph() const;

    Qt3DRender::QCamera *camera() const;
    Qt3DRender::QRenderSettings *renderSettings() const;

    virtual void initializeGL() Q_DECL_OVERRIDE;
    virtual void paintGL() Q_DECL_OVERRIDE;
    virtual void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    virtual void showEvent(QShowEvent *) Q_DECL_OVERRIDE;

    Qt3DCore::QTransform *transform = nullptr;
    Qt3DExtras::QTorusMesh *mesh = nullptr;
    Qt3DExtras::QPhongMaterial *material = nullptr;
    Qt3DCore::QEntity *entity = nullptr;
    Qt3DExtras::QOrbitCameraController *cameraController = nullptr;
    Qt3DExtras::QForwardRenderer *forwardRenderer = nullptr;
};
typedef Qt3DRenderer VideoRendererQt3D;

} //namespace QtAV

#endif // QTAV_QT3DRENDERER_H
