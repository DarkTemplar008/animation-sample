#include "main_window.h"
#include "ui_main_window.h"
#include "gl_quad_widget.h"
#include "gl_sweep_effect_widget.h"

#include <QRadioButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto group_box_layout = new QVBoxLayout();

    QRadioButton* original_effect_button = new QRadioButton("原始");
    group_box_layout->addWidget(original_effect_button);
    GLQuadWidget* original_effect_widget = new GLQuadWidget(nullptr);
    ui->stackedWidget->addWidget(original_effect_widget);
    connect(original_effect_button, &QRadioButton::clicked, [=](bool checked){
       ui->stackedWidget->setCurrentWidget(original_effect_widget);
    });

    QRadioButton* sweep_effect_button = new QRadioButton("切换", ui->effectGroupBox);
    group_box_layout->addWidget(sweep_effect_button);
    GLSweepEffectWidget* sweep_effect_widget = new GLSweepEffectWidget(nullptr);
    ui->stackedWidget->addWidget(sweep_effect_widget);
    connect(sweep_effect_button, &QRadioButton::clicked, [=](bool checked){
       ui->stackedWidget->setCurrentWidget(sweep_effect_widget);
    });

    auto group_box_spacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    group_box_layout->addItem(group_box_spacer);
    ui->effectGroupBox->setLayout(group_box_layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
