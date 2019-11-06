#ifndef GLPIXELEFFECTWIDGET_H
#define GLPIXELEFFECTWIDGET_H

#include "gl_quad_widget.h"

class GLPixelEffectWidget : public GLQuadWidget
{
public:
    GLPixelEffectWidget(QWidget* parent = nullptr);

protected:
    QString pixelShader() const override;
    void    createGeometry() override;
    void    bindResource() override;

    void    showEvent(QShowEvent* evt) override;

protected:
    QOpenGLTexture* textures[2];
    GLfloat         progress_ = 0;
};

#endif // GLPIXELEFFECTWIDGET_H
