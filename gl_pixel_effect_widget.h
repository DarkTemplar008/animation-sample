#ifndef GLPIXELEFFECTWIDGET_H
#define GLPIXELEFFECTWIDGET_H

#include "gl_quad_widget.h"

class GLPixelEffectWidget : public GLQuadWidget
{
public:
    GLPixelEffectWidget(QWidget* parent = nullptr);

protected:
    virtual QString pixelShader() const override;
    virtual void    createGeometry() override;
    virtual void    bindResource() override;

protected:
    QOpenGLTexture* textures[2];
    GLfloat         progress_ = 0;
};

#endif // GLPIXELEFFECTWIDGET_H
