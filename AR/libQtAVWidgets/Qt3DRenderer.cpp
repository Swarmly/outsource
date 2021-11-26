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

#include "QtAVWidgets/Qt3DRenderer.h"
#include "QtAV/private/OpenGLRendererBase_p.h"

namespace QtAV {

class Qt3DRendererPrivate : public OpenGLRendererBasePrivate
{
public:
    typedef QScopedPointer<QOpenGLShaderProgram> ShaderProgramPtr;

    Qt3DRendererPrivate(QPaintDevice *pd)
        : OpenGLRendererBasePrivate(pd)
        , m_aspectEngine(new Qt3DCore::QAspectEngine)
        , m_renderAspect(new Qt3DRender::QRenderAspect(Qt3DRender::QRenderAspect::Threaded))
        , m_inputAspect(new Qt3DInput::QInputAspect)
        , m_logicAspect(new Qt3DLogic::QLogicAspect)
        , m_renderSettings(new Qt3DRender::QRenderSettings)
        , m_forwardRenderer(new Qt3DExtras::QForwardRenderer)
        , m_defaultCamera(new Qt3DRender::QCamera)
        , m_inputSettings(new Qt3DInput::QInputSettings)
        , m_frameAction(new Qt3DLogic::QFrameAction)
        , m_root(new Qt3DCore::QEntity)
        , m_userRoot(nullptr)
        , m_offscreenSurface(new QOffscreenSurface)
        , m_renderStateSet(new Qt3DRender::QRenderStateSet)
        , m_depthTest(new Qt3DRender::QDepthTest)
        , m_multisampleAntialiasing(new Qt3DRender::QMultiSampleAntiAliasing)
        , m_renderTargetSelector(new Qt3DRender::QRenderTargetSelector)
        , m_renderSurfaceSelector(new Qt3DRender::QRenderSurfaceSelector)
        , m_renderTarget(new Qt3DRender::QRenderTarget)
        , m_colorOutput(new Qt3DRender::QRenderTargetOutput)
        , m_colorTexture(new Qt3DRender::QTexture2DMultisample)
        , m_depthOutput(new Qt3DRender::QRenderTargetOutput)
        , m_depthTexture(new Qt3DRender::QTexture2DMultisample)
        , m_initialized(false)
    {
   
    }
    void init()
    {
        static const int coords[4][3] = {{+1, 0, 0}, {0, 0, 0}, {0, +1, 0}, {+1, +1, 0}};

        for (int i = 0; i < 4; ++i) {
            // vertex position
            m_vertexData.append(coords[i][0]);
            m_vertexData.append(coords[i][1]);
            m_vertexData.append(coords[i][2]);
            // texture coordinate
            m_vertexData.append(i == 0 || i == 3);
            m_vertexData.append(i == 0 || i == 1);
        }

        // Setup our vertex array object. We later only need to bind this
        // to be able to draw.
        m_vao.create();
        // The binder automatically binds the vao and unbinds it at the end
        // of the function.
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

        // Setup our vertex buffer object.
        m_vbo.create();
        m_vbo.bind();
        m_vbo.allocate(m_vertexData.constData(), m_vertexData.count() * sizeof(GLfloat));

        m_vbo.bind();
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        f->glEnableVertexAttribArray(m_vertexAttributeLoc);
        f->glEnableVertexAttribArray(m_texCoordAttributeLoc);
        f->glVertexAttribPointer(m_vertexAttributeLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
        f->glVertexAttribPointer(m_texCoordAttributeLoc,
                                 2,
                                 GL_FLOAT,
                                 GL_FALSE,
                                 5 * sizeof(GLfloat),
                                 reinterpret_cast<void *>(3 * sizeof(GLfloat)));
        m_vbo.release();

        m_shaderProgram.reset(new QOpenGLShaderProgram);
        m_shaderProgram
            ->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                      "#version 150\n"
                                      "in highp vec3 vertex;\n"
                                      "in mediump vec2 texCoord;\n"
                                      "out mediump vec2 texc;\n"
                                      "uniform mediump mat4 matrix;\n"
                                      "void main(void)\n"
                                      "{\n"
                                      "        gl_Position = matrix * vec4(vertex, 1.0);\n"
                                      "        texc = texCoord;\n"
                                      "}\n");
        m_shaderProgram
            ->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                      "#version 150\n"
                                      "uniform sampler2DMS texture;\n"
                                      "in mediump vec2 texc;\n"
                                      "uniform int samples;\n"
                                      "void main(void)\n"
                                      "{\n"
                                      "   ivec2 tc = ivec2(floor(textureSize(texture) * texc));\n"
                                      "   vec4 color = vec4(0.0);\n"
                                      "   for(int i = 0; i < samples; i++) {\n"
                                      "       color += texelFetch(texture, tc, i);\n"
                                      "   }\n"
                                      "   gl_FragColor = color / float(samples);\n"
                                      "}\n");
        m_shaderProgram->bindAttributeLocation("vertex", m_vertexAttributeLoc);
        m_shaderProgram->bindAttributeLocation("texCoord", m_vertexAttributeLoc);
        m_shaderProgram->link();

        m_shaderProgram->bind();
        m_shaderProgram->setUniformValue("texture", 0);
        m_shaderProgram->setUniformValue("samples", QSurfaceFormat::defaultFormat().samples());
        m_shaderProgram->release();
    }
    Qt3DCore::QAspectEngine *m_aspectEngine;

    // Aspects
    Qt3DRender::QRenderAspect *m_renderAspect;
    Qt3DInput::QInputAspect *m_inputAspect;
    Qt3DLogic::QLogicAspect *m_logicAspect;

    // Renderer configuration
    Qt3DRender::QRenderSettings *m_renderSettings;
    Qt3DExtras::QForwardRenderer *m_forwardRenderer;
    Qt3DRender::QCamera *m_defaultCamera;
    Qt3DRender::QFrameGraphNode *m_activeFrameGraph;

    // Input configuration
    Qt3DInput::QInputSettings *m_inputSettings;

    // To get notified when a frame is ready
    Qt3DLogic::QFrameAction *m_frameAction;

    // Scene
    Qt3DCore::QEntity *m_root;
    Qt3DCore::QEntity *m_userRoot;

    // Offscreen framegraph
    QOffscreenSurface *m_offscreenSurface;
    Qt3DRender::QRenderStateSet *m_renderStateSet;
    Qt3DRender::QDepthTest *m_depthTest;
    Qt3DRender::QMultiSampleAntiAliasing *m_multisampleAntialiasing;
    Qt3DRender::QRenderTargetSelector *m_renderTargetSelector;
    Qt3DRender::QRenderSurfaceSelector *m_renderSurfaceSelector;
    Qt3DRender::QRenderTarget *m_renderTarget;
    Qt3DRender::QRenderTargetOutput *m_colorOutput;
    Qt3DRender::QTexture2DMultisample *m_colorTexture;
    Qt3DRender::QRenderTargetOutput *m_depthOutput;
    Qt3DRender::QTexture2DMultisample *m_depthTexture;

    // OpenGL setup
    ShaderProgramPtr m_shaderProgram;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QVector<GLfloat> m_vertexData;
    QOpenGLTexture *m_texture = Q_NULLPTR;
    int m_vertexAttributeLoc = 0;
    int m_texCoordAttributeLoc = 0;

    bool m_initialized;
};

VideoRendererId Qt3DRenderer::id() const
{
    return VideoRendererId_OpenGLWidget;
}

Qt3DRenderer::Qt3DRenderer(QWidget *parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f)
    , OpenGLRendererBase(*new Qt3DRendererPrivate(this))
{
    DPTR_D(Qt3DRenderer);

    setMouseTracking(true);
    //    setAcceptDrops(true);
    //    setFocusPolicy(Qt::StrongFocus);

    int samples = QSurfaceFormat::defaultFormat().samples();

    d.m_offscreenSurface->setFormat(QSurfaceFormat::defaultFormat());
    d.m_offscreenSurface->create();

    //d.m_aspectEngine->registerAspect(new Qt3DCore::QCoreAspect);
    d.m_aspectEngine->registerAspect(d.m_renderAspect);
    d.m_aspectEngine->registerAspect(d.m_inputAspect);
    d.m_aspectEngine->registerAspect(d.m_logicAspect);

    // Setup color
    d.m_colorOutput->setAttachmentPoint(Qt3DRender::QRenderTargetOutput::Color0);

    // Create a color texture to render into.
    d.m_colorTexture->setSize(width(), height());
    d.m_colorTexture->setFormat(Qt3DRender::QAbstractTexture::RGB8_UNorm);
    d.m_colorTexture->setMinificationFilter(Qt3DRender::QAbstractTexture::Linear);
    d.m_colorTexture->setMagnificationFilter(Qt3DRender::QAbstractTexture::Linear);

    // Hook the texture up to our output, and the output up to this object.
    d.m_colorOutput->setTexture(d.m_colorTexture);
    d.m_colorTexture->setSamples(samples);
    d.m_renderTarget->addOutput(d.m_colorOutput);

    // Setup depth
    d.m_depthOutput->setAttachmentPoint(Qt3DRender::QRenderTargetOutput::Depth);

    // Create depth texture
    d.m_depthTexture->setSize(width(), height());
    d.m_depthTexture->setFormat(Qt3DRender::QAbstractTexture::DepthFormat);
    d.m_depthTexture->setMinificationFilter(Qt3DRender::QAbstractTexture::Linear);
    d.m_depthTexture->setMagnificationFilter(Qt3DRender::QAbstractTexture::Linear);
    d.m_depthTexture->setComparisonFunction(Qt3DRender::QAbstractTexture::CompareLessEqual);
    d.m_depthTexture->setComparisonMode(Qt3DRender::QAbstractTexture::CompareRefToTexture);

    // Hook up the depth texture
    d.m_depthOutput->setTexture(d.m_depthTexture);
    d.m_depthTexture->setSamples(samples);
    d.m_renderTarget->addOutput(d.m_depthOutput);

    d.m_renderStateSet->addRenderState(d.m_multisampleAntialiasing);
    d.m_renderStateSet->addRenderState(d.m_depthTest);
    d.m_depthTest->setDepthFunction(Qt3DRender::QDepthTest::LessOrEqual);
    d.m_renderTargetSelector->setParent(d.m_renderStateSet);
    d.m_renderTargetSelector->setTarget(d.m_renderTarget);

    d.m_renderSurfaceSelector->setSurface(d.m_offscreenSurface);
    d.m_renderSurfaceSelector->setParent(d.m_renderTargetSelector);
    d.m_defaultCamera->setParent(d.m_renderSurfaceSelector);
    d.m_forwardRenderer->setCamera(d.m_defaultCamera);
    d.m_forwardRenderer->setSurface(d.m_offscreenSurface);
    d.m_forwardRenderer->setParent(d.m_renderSurfaceSelector);
    d.m_renderSettings->setActiveFrameGraph(d.m_renderStateSet);
    d.m_inputSettings->setEventSource(this);

    d.m_activeFrameGraph = d.m_forwardRenderer;
    d.m_forwardRenderer->setClearColor("white");

    ///////////////////////
    mesh = new Qt3DExtras::QTorusMesh();
    mesh->setRings(100);
    mesh->setSlices(100);
    mesh->setMinorRadius(0.5);
    mesh->setRadius(1);
    material = new Qt3DExtras::QPhongMaterial();
    material->setAmbient("red");
    transform = new Qt3DCore::QTransform();
    entity = new Qt3DCore::QEntity();
    entity->addComponent(mesh);
    entity->addComponent(material);
    entity->addComponent(transform);
    cameraController = new Qt3DExtras::QOrbitCameraController(entity);
    cameraController->setCamera(camera());

    forwardRenderer = new Qt3DExtras::QForwardRenderer;
    forwardRenderer->setCamera(camera());
    forwardRenderer->setClearColor(Qt::gray);
    setActiveFrameGraph(forwardRenderer);

    setRootEntity(entity);
    camera()->setPosition(QVector3D(0, 0, 10));
}

void Qt3DRenderer::initializeGL()
{
    DPTR_D(Qt3DRenderer);
    d.init();
    onInitializeGL();
}

void Qt3DRenderer::paintGL()
{
    //OpenGLRendererBasePrivate &ptrOpenGlBase = dptr.pri<OpenGLRendererBasePrivate>();

    transform->setRotationX(transform->rotationX() + 1);
    onPaintGL();
    //if (1)
    // {
    DPTR_D(Qt3DRenderer);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glDisable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    d.m_shaderProgram->bind();
    {
        QMatrix4x4 m;
        m.ortho(0, 1, 1, 0, 1.0f, 3.0f);
        m.translate(0.0f, 0.0f, -2.0f);

        QOpenGLVertexArrayObject::Binder vaoBinder(&d.m_vao);

        d.m_shaderProgram->setUniformValue("matrix", m);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, d.m_colorTexture->handle().toUInt());
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    d.m_shaderProgram->release();
    // onPaintGL();
    //    } else {
    //    }
    //Qt3DWidget::paintGL();
}

void Qt3DRenderer::resizeGL(int w, int h)
{
    // Qt3DWidget::resizeGL(w, h);

    DPTR_D(Qt3DRenderer);
    d.m_defaultCamera->setAspectRatio(w / (float) h);
    d.m_colorTexture->setSize(w, h);
    d.m_depthTexture->setSize(w, h);
    d.m_renderSurfaceSelector->setExternalRenderTargetSize(QSize(w, h));

    // QGLWidget uses window()->windowHandle()->devicePixelRatio() for resizeGL(), while QOpenGLWidget does not, so scale here
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    if (!context())
        return;
    const qreal dpr = context()->screen()->devicePixelRatio();
#elif QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
    // qApp->devicePixelRatio() is global, window()->windowHandle()->devicePixelRatio() depends on screen, check window() and windowHandle() is required.
    // QWidget.devicePixelRatio() is int, but float value is not implemented in old qt, so just use int is fine.
    const qreal dpr = devicePixelRatio();
#else
    const qreal dpr = qApp->devicePixelRatio();
#endif
    onResizeGL(w*dpr, h*dpr);
}

void Qt3DRenderer::resizeEvent(QResizeEvent *e)
{
    //Qt3DWidget::resizeEvent(e);
    onResizeEvent(e->size().width(), e->size().height());
    QOpenGLWidget::resizeEvent(e); //will call resizeGL(). TODO:will call paintEvent()?
}

void Qt3DRenderer::showEvent(QShowEvent *e)
{
    DPTR_D(Qt3DRenderer);
    if (!d.m_initialized) {
        d.m_root->addComponent(d.m_renderSettings);
        d.m_root->addComponent(d.m_inputSettings);
        d.m_root->addComponent(d.m_frameAction);
        connect(d.m_frameAction, &Qt3DLogic::QFrameAction::triggered, [this]() { this->update(); });
        d.m_aspectEngine->setRootEntity(Qt3DCore::QEntityPtr(d.m_root));
        d.m_initialized = true;
    }
    //Qt3DWidget::showEvent(e);

    onShowEvent(); // TODO: onShowEvent(w, h)?
    resizeGL(width(), height());
    //QWidget::showEvent(e);
}

void Qt3DRenderer::registerAspect(Qt3DCore::QAbstractAspect *aspect)
{
    DPTR_D(Qt3DRenderer);
    d.m_aspectEngine->registerAspect(aspect);
}

void Qt3DRenderer::registerAspect(const QString &name)
{
    DPTR_D(Qt3DRenderer);
    d.m_aspectEngine->registerAspect(name);
}

void Qt3DRenderer::setRootEntity(Qt3DCore::QEntity *root)
{
    DPTR_D(Qt3DRenderer);
    if (d.m_userRoot != root) {
        if (d.m_userRoot != nullptr)
            d.m_userRoot->setParent(static_cast<Qt3DCore::QNode *>(nullptr));
        if (root != nullptr)
            root->setParent(d.m_root);
        d.m_userRoot = root;
    }
}

void Qt3DRenderer::setActiveFrameGraph(Qt3DRender::QFrameGraphNode *activeFrameGraph)
{
    DPTR_D(Qt3DRenderer);
    d.m_activeFrameGraph->setParent(static_cast<Qt3DCore::QNode *>(nullptr));
    d.m_activeFrameGraph = activeFrameGraph;
    activeFrameGraph->setParent(d.m_renderSurfaceSelector);
}

Qt3DRender::QFrameGraphNode *Qt3DRenderer::activeFrameGraph() const
{
    DPTR_D(const Qt3DRenderer);
    return d.m_activeFrameGraph;
}

Qt3DExtras::QForwardRenderer *Qt3DRenderer::defaultFrameGraph() const
{
    DPTR_D(const Qt3DRenderer);
    return d.m_forwardRenderer;
}

Qt3DRender::QCamera *Qt3DRenderer::camera() const
{
    DPTR_D(const Qt3DRenderer);
    return d.m_defaultCamera;
}

Qt3DRender::QRenderSettings *Qt3DRenderer::renderSettings() const
{
    DPTR_D(const Qt3DRenderer);
    return d.m_renderSettings;
}
} //namespace QtAV
