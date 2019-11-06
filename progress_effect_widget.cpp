#include "progress_effect_widget.h"

#include <QPainter>
#include <QTimer>

ProgressEffectWidget::ProgressEffectWidget(QWidget *parent) : QWidget(parent)
{
    QTimer* updater = new QTimer(this);
    connect(updater, &QTimer::timeout, [&](){
        updateProgressItems();
        update();
    });
    updater->start(40);

    initProgressItems();
}

void ProgressEffectWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    painter.setPen(Qt::white);

    int max_score = 10000;
    while (item_view_vec_[0].score*2 < max_score) max_score /= 2;
    int item_height = height()*1.0f / item_view_vec_.size();
    QRect item_rect(0, 0, width(), item_height);
    for (int i = 0; i < item_view_vec_.size(); ++i)
    {
        QRect rect(0,
                   item_view_vec_[i].step == 0 ? item_height * i : item_view_vec_[i].current,
                   width()*item_view_vec_[i].score/max_score,
                   item_height);
        rect.adjust(5, 5, -5, -5);
        painter.fillRect(rect, item_view_vec_[i].color);
        painter.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, QString("%1").arg(item_view_vec_[i].score));
    }
}

void ProgressEffectWidget::showEvent(QShowEvent *evt)
{
    initProgressItems();
    completed_ = false;
}

void ProgressEffectWidget::initProgressItems()
{
    int size = rand() % 5 + 15;
    item_vec_.resize(size);
    for (int i = 0; i < size; ++i)
    {
        item_vec_[i].first = rand()%5000 + 50;
        item_vec_[i].second.setRgb(rand()%256, rand()%256, rand()%256);
    }

    item_view_vec_.resize(item_vec_.size());
    for (int i = 0; i < item_vec_.size(); ++i)
    {
        item_view_vec_[i].index = i;
        item_view_vec_[i].score = item_vec_[i].first;
        item_view_vec_[i].color = item_vec_[i].second;
    }
    std::sort(item_view_vec_.begin(), item_view_vec_.end(), [](const ProgressItemView& a, const ProgressItemView& b)
    {
        return a.score > b.score;
    });

    int item_height = height()*1.0f / item_view_vec_.size();
    for (size_t i = 0; i < item_view_vec_.size(); ++i)
    {
        item_view_vec_[i].current = item_view_vec_[i].target = i *item_height;
        item_view_vec_[i].delta = 0;
        item_view_vec_[i].step = 0;
    }
}

void ProgressEffectWidget::updateProgressItems()
{
    for (int i = 0; i < item_view_vec_.size(); ++i)
    {
        item_view_vec_[i].current += item_view_vec_[i].delta;
        if (--item_view_vec_[i].step < 0)
        {
            item_view_vec_[i].current = item_view_vec_[i].target;
            item_view_vec_[i].delta = 0;
            item_view_vec_[i].step = 0;
        }
    }

    for (int i = 0; i < item_vec_.size() && !completed_; ++i)
    {
        item_vec_[i].first += rand()%5 +  (i << 1 == item_vec_.size())*10 + (i%2)*2;
        if (item_vec_[i].first > 10000)
        {
            item_vec_[i].first = 10000;
            completed_ = true;
        }
    }

    std::vector<std::pair<int,int>> index_vec;
    index_vec.resize(item_view_vec_.size());
    for (int i = 0; i < item_view_vec_.size(); ++i)
    {
        item_view_vec_[i].score = item_vec_[item_view_vec_[i].index].first;
        index_vec[i] = std::make_pair(item_view_vec_[i].score, item_view_vec_[i].index);
    }

    std::sort(index_vec.begin(), index_vec.end(), [](const std::pair<int,int>& a, const std::pair<int,int>& b) {return a.first > b.first;});
    for (int i = 0; i < item_view_vec_.size(); ++i)
    {
        if (item_view_vec_[i].index != index_vec[i].second)
        {
            for (int j = 0; j < index_vec.size(); ++j)
            {
                if (item_view_vec_[i].index == index_vec[j].second)
                {
                    int item_height = height()*1.0f / item_view_vec_.size();
                    item_view_vec_[i].target = item_height*j;
                    item_view_vec_[i].step = MAX_STEP;
                    item_view_vec_[i].delta = (item_view_vec_[i].target - item_view_vec_[i].current)*1.0f/MAX_STEP;
                }
            }
        }
    }
}
