#ifndef GL_SWEEP_EFFECT_WIDGET_H
#define GL_SWEEP_EFFECT_WIDGET_H

#include "gl_quad_widget.h"

class GLSweepEffectWidget : public GLQuadWidget
{
public:
    GLSweepEffectWidget(QWidget* parent = nullptr);

protected:
    QString pixelShader() const override;
    void    createGeometry() override;
    void    bindResource() override;

    void    showEvent(QShowEvent* evt) override;

protected:
    QOpenGLTexture* textures[2];
    GLfloat         progress_ = 0;
};

#endif // GL_SWEEP_EFFECT_WIDGET_H
