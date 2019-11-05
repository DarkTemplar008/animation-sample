#include "gl_sweep_effect_widget.h"

#include <QTimer>

GLSweepEffectWidget::GLSweepEffectWidget(QWidget* parent) :
    GLQuadWidget (parent)
{
    QTimer* updater = new QTimer(this);
    connect(updater, &QTimer::timeout, [&](){
        progress_ += 0.05;
        if (progress_ >= 1.0) {
            progress_ = 0;
            std::swap(textures[0], textures[1]);
        }
        update();
    });
    updater->start(40);
}

void GLSweepEffectWidget::createGeometry()
{
    GLQuadWidget::createGeometry();

    textures[0] = texture;
    textures[1] = new QOpenGLTexture(QImage(QString(":/images/landscape2.jpg")));
}

QString GLSweepEffectWidget::pixelShader() const
{
    return "uniform sampler2D texture;\n"
        "uniform sampler2D texture2;\n"
        "uniform float progress;"
        "varying mediump vec4 texc;\n"
        "void main(void)\n"
        "{\n"
        "    if (texc.s < progress)\n"
        "       gl_FragColor = texture2D(texture, texc.st);\n"
        "    else\n"
        "       gl_FragColor = texture2D(texture2, texc.st);\n"
        "}\n";
}

void GLSweepEffectWidget::bindResource()
{
    program->setUniformValue("texture", 0);
    textures[0]->bind(0);
    program->setUniformValue("texture2", 1);
    textures[1]->bind(1);
    program->setUniformValue("progress", progress_);
}
