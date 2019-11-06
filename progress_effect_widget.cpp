#include "progress_effect_widget.h"

#include <QPainter>
#include <QTimer>

ProgressEffectWidget::ProgressEffectWidget(QWidget *parent) : QWidget(parent)
{
    QTimer* updater = new QTimer(this);
    connect(updater, &QTimer::timeout, [&](){
        update();
    });
    updater->start(40);
}

void ProgressEffectWidget::paintEvent(QPaintEvent *event)
{
    static std::vector<std::pair<int, QColor>> gdp_vec;
    if (gdp_vec.empty())
    {
        int size = rand() % 5 + 15;
        gdp_vec.resize(size);
        for (int i = 0; i < size; ++i)
        {
            gdp_vec[i].first = rand()%5000 + 50;
            gdp_vec[i].second.setRgb(rand()%256, rand()%256, rand()%256);
        }
    }

    struct GDPItemView
    {
        int     index;
        int     score;
        QColor  color;
        float     current;
        float     target;
        float     delta;
#define MAX_STEP 10
        int     step;
    };
    static std::vector<GDPItemView> gdp_view_vec;
    if (gdp_view_vec.empty())
    {
        gdp_view_vec.resize(gdp_vec.size());
        for (int i = 0; i < gdp_vec.size(); ++i)
        {
            gdp_view_vec[i].index = i;
            gdp_view_vec[i].score = gdp_vec[i].first;
            gdp_view_vec[i].color = gdp_vec[i].second;
        }
        std::sort(gdp_view_vec.begin(), gdp_view_vec.end(), [](const GDPItemView& a, const GDPItemView& b)
        {
            return a.score > b.score;
        });
        int item_height = height()*1.0f / gdp_view_vec.size();
        for (size_t i = 0; i < gdp_view_vec.size(); ++i)
        {
            gdp_view_vec[i].current = gdp_view_vec[i].target = i *item_height;
            gdp_view_vec[i].delta = 0;
            gdp_view_vec[i].step = 0;
        }
    }

    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    painter.setPen(Qt::white);
    int factor = 10000;
    while (gdp_view_vec[0].score*2 < factor) factor /= 2;
    int item_height = height()*1.0f / gdp_view_vec.size();
    QRect item_rect(0, 0, width(), item_height);
    for (int i = 0; i < gdp_view_vec.size(); ++i)
    {
        QRect rect(0, gdp_view_vec[i].current, width()*gdp_view_vec[i].score/factor, item_height);
        rect.adjust(5, 5, -5, -5);
        painter.fillRect(rect, gdp_view_vec[i].color);
        painter.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, QString("%1").arg(gdp_view_vec[i].score));
    }

    static bool achieve = false;
    if (!achieve)
    {
        for (int i = 0; i < gdp_view_vec.size(); ++i)
        {
            gdp_view_vec[i].current += gdp_view_vec[i].delta;
            if (--gdp_view_vec[i].step < 0)
            {
                gdp_view_vec[i].current = gdp_view_vec[i].target;
                gdp_view_vec[i].delta = 0;
                gdp_view_vec[i].step = 0;
            }
        }
        for (int i = 0; i < gdp_vec.size(); ++i)
        {
            gdp_vec[i].first += rand()%5 +  (i << 1 == gdp_vec.size())*10 + (i%2)*2;
            if (gdp_vec[i].first > 10000)
            {
                gdp_vec[i].first = 10000;
                achieve = true;
            }
        }
        std::vector<std::pair<int,int>> index_vec;
        index_vec.resize(gdp_view_vec.size());
        for (int i = 0; i < gdp_view_vec.size(); ++i)
        {
            gdp_view_vec[i].score = gdp_vec[gdp_view_vec[i].index].first;
            index_vec[i] = std::make_pair(gdp_view_vec[i].score, gdp_view_vec[i].index);
        }
        std::sort(index_vec.begin(), index_vec.end(), [](const std::pair<int,int>& a, const std::pair<int,int>& b) {return a.first > b.first;});
        for (int i = 0; i < gdp_view_vec.size(); ++i)
        {
            if (gdp_view_vec[i].index != index_vec[i].second)
            {
                for (int j = 0; j < index_vec.size(); ++j)
                {
                    if (gdp_view_vec[i].index == index_vec[j].second)
                    {
                        int item_height = height()*1.0f / gdp_view_vec.size();
                        gdp_view_vec[i].target = item_height*j;
                        gdp_view_vec[i].step = MAX_STEP;
                        gdp_view_vec[i].delta = (gdp_view_vec[i].target - gdp_view_vec[i].current)*1.0f/MAX_STEP;
                    }
                }
            }
        }
    }
}
