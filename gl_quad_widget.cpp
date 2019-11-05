#include "gl_quad_widget.h"

GLQuadWidget::GLQuadWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

GLQuadWidget::~GLQuadWidget()
{
    makeCurrent();
    vbo.destroy();
    delete texture;
    delete program;
    doneCurrent();
}

QSize GLQuadWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize GLQuadWidget::sizeHint() const
{
    return QSize(200, 200);
}

void GLQuadWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    update();
}

void GLQuadWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    update();
}

void GLQuadWidget::initializeGL()
{
    initializeOpenGLFunctions();

    createGeometry();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vshader->compileSourceCode(vertexShader());

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fshader->compileSourceCode(pixelShader());

    program = new QOpenGLShaderProgram;
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();

    program->bind();
}

QString GLQuadWidget::vertexShader() const
{
    return "attribute highp vec4 vertex;\n"
        "attribute mediump vec4 texCoord;\n"
        "varying mediump vec4 texc;\n"
        "uniform mediump mat4 matrix;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = matrix * vertex;\n"
        "    texc = texCoord;\n"
        "}\n";
}

QString GLQuadWidget::pixelShader() const
{
    return "uniform sampler2D texture;\n"
        "varying mediump vec4 texc;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = texture2D(texture, texc.st);\n"
           "}\n";
}

void GLQuadWidget::bindResource()
{
    if (nullptr != texture)
    {
        program->setUniformValue("texture", 0);
        texture->bind();
    }
}

void GLQuadWidget::paintGL()
{
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m;
    m.ortho(-0.5f, +0.5f, +0.5f, -0.5f, 4.0f, 15.0f);
    m.translate(0.0f, 0.0f, -10.0f);
    m.rotate(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    m.rotate(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    m.rotate(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

    program->setUniformValue("matrix", m);
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

    bindResource();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void GLQuadWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void GLQuadWidget::createGeometry()
{
    static const int coords[4][3] = {
        { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 }
    };

    texture = new QOpenGLTexture(QImage(QString(":/images/landscape1.jpg")));

    QVector<GLfloat> vertData;
    for (int i = 0; i < 4; ++i)
    {
        // vertex position
        vertData.append(0.5 * coords[i][0]);
        vertData.append(0.5 * coords[i][1]);
        vertData.append(0.5 * coords[i][2]);
        // texture coordinate
        vertData.append(i == 0 || i == 3);
        vertData.append(i == 2 || i == 3);
    }

    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
}
