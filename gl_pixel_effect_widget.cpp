#include "gl_pixel_effect_widget.h"

#include <QTimer>

GLPixelEffectWidget::GLPixelEffectWidget(QWidget* parent) :
    GLQuadWidget(parent)
{
    QTimer* updater = new QTimer(this);
    connect(updater, &QTimer::timeout, [&](){
        if (progress_ > 0.1 && progress_ < 0.9)
        {
            progress_ += 0.05;
        }
        else
        {
            progress_ += 0.01;
        }

        if (progress_ >= 1.0) {
            progress_ = 0.01;
            std::swap(textures[0], textures[1]);
        }
        update();
    });
    updater->start(100);
}

void GLPixelEffectWidget::createGeometry()
{
    GLQuadWidget::createGeometry();

    textures[0] = texture;
    textures[1] = new QOpenGLTexture(QImage(QString(":/images/landscape2.jpg")));
}

QString GLPixelEffectWidget::pixelShader() const
{
    return "uniform sampler2D texture;\n"
        "uniform sampler2D texture2;\n"
        "uniform float progress;"
        "varying mediump vec4 texc;\n"
        "void main(void)\n"
        "{\n"
        "    if (progress < 0.5)\n"
        "    {\n"
        "       vec2 pos = progress/5.0*floor(texc.st*5.0/progress);\n"
        "       gl_FragColor = texture2D(texture, pos);\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "       vec2 pos = (1.0 - progress)/5.0*floor(texc.st*5.0/(1.0 - progress));\n"
        "       gl_FragColor = texture2D(texture2, pos);\n"
        "    }\n"
        "}\n";
}

void GLPixelEffectWidget::bindResource()
{
    program->setUniformValue("texture", 0);
    textures[0]->bind(0);
    program->setUniformValue("texture2", 1);
    textures[1]->bind(1);
    program->setUniformValue("progress", progress_);
}
