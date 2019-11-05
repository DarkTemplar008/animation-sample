#include "main_window.h"
#include "ui_main_window.h"
#include "gl_quad_widget.h"
#include "gl_sweep_effect_widget.h"
#include "gl_pixel_effect_widget.h"

#include <QRadioButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto group_box_layout = new QVBoxLayout();

    auto add_effect = [=](const QString& name, QWidget* widget, bool checked = false)
    {
        QRadioButton* effect_button = new QRadioButton(name);
        group_box_layout->addWidget(effect_button);
        ui->stackedWidget->addWidget(widget);
        connect(effect_button, &QRadioButton::clicked, [=](bool checked){
           ui->stackedWidget->setCurrentWidget(widget);
        });

        if (checked)
        {
            effect_button->click();
        }
    };

    add_effect("原始", new GLQuadWidget(nullptr), true);
    add_effect("切换", new GLSweepEffectWidget(nullptr));
    add_effect("像素过渡", new GLPixelEffectWidget(nullptr));

    auto group_box_spacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    group_box_layout->addItem(group_box_spacer);
    ui->effectGroupBox->setLayout(group_box_layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
