#ifndef PROGRESSEFFECTWIDGET_H
#define PROGRESSEFFECTWIDGET_H

#include <QWidget>

class ProgressEffectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressEffectWidget(QWidget *parent = nullptr);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // PROGRESSEFFECTWIDGET_H
