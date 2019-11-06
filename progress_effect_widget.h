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
    void showEvent(QShowEvent* evt) override;

private:
    void initProgressItems();
    void updateProgressItems();

private:
    struct ProgressItemView
    {
        int     index;
        int     score;
        QColor  color;
        float   current;
        float   target;
        float   delta;
#define MAX_STEP 10
        int     step;
    };

    std::vector<std::pair<int, QColor>> item_vec_;
    std::vector<ProgressItemView>       item_view_vec_;
    bool                                completed_ = false;
};

#endif // PROGRESSEFFECTWIDGET_H
