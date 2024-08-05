// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QResizeEvent>
#include <QScreen>
#include <QScrollArea>
#include <QSplitter>
#include <QTimer>
#include <QVBoxLayout>
#include <camerawidget.h>
#include <QHBoxLayout>
#include "videoplayer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupUI();  // 调用setupUI函数来设置UI
    ui->centralwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // 延迟调用调整布局比例，确保设置生效
    QTimer::singleShot(0, this, [=]() {
        adjustSplitterSizes();
    });

}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    setupTopBox();
    setupThumbnailArea();
    stateLabel = new QLabel("no action");
    ui->statusbar->addWidget(stateLabel);
    setWindowTitle("示教系统");
}

void MainWindow::setupTopBox()
{
    // 创建并初始化 QSplitter
    topSplitter = new QSplitter(Qt::Horizontal, ui->top_groupBox);

    // 水平布局分割
    QHBoxLayout* topBoxLayout = new QHBoxLayout(ui->top_groupBox);
    topBoxLayout->addWidget(topSplitter);  // 添加 QSplitter 到布局

    // 主监控画面设置
    mainCameraWidget = new CameraWidget(ui->top_groupBox);
    mainCameraWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    topSplitter->addWidget(mainCameraWidget);  // 将主摄像头小部件添加到 QSplitter

    // 控制面板设置
    controlPanel = ui->contrl_groupBox;
    setupControlPanel();
    controlPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    topSplitter->addWidget(controlPanel);  // 将控制面板添加到 QSplitter

    ui->top_groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    adjustSplitterSizes();
}

void MainWindow::setupControlPanel()
{
    QVBoxLayout *controlLayout = qobject_cast<QVBoxLayout*>(ui->contrl_groupBox->layout());
    if (controlLayout == nullptr) {
        // 创建并设置布局
        controlLayout = new QVBoxLayout(ui->contrl_groupBox);
        ui->contrl_groupBox->setLayout(controlLayout);
    }

    QLabel *label = new QLabel("控制面板", ui->contrl_groupBox);
    controlLayout->addWidget(label);


    // 添加更多控制面板的控件
}

void MainWindow::setupThumbnailArea()
{
    thumbnailArea = new QScrollArea(ui->thum_groupBox);
    QHBoxLayout* thumBoxLayout = new QHBoxLayout(ui->thum_groupBox);

    thumbnailArea->setWidgetResizable(true);
    thumbnailArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    thumbnailArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    thumbnailContainer = new QWidget(thumbnailArea);
    QHBoxLayout *thumbnailLayout = new QHBoxLayout(thumbnailContainer);
    thumbnailLayout->setContentsMargins(0, 0, 0, 0);
    thumbnailLayout->setSpacing(0);

    setupThumbnails(thumbnailLayout);
    thumbnailContainer->setLayout(thumbnailLayout);
    thumbnailArea->setWidget(thumbnailContainer);

    thumBoxLayout->addWidget(thumbnailArea);
    ui->thum_groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MainWindow::setupThumbnails(QHBoxLayout *layout)
{
    for (int i = 0; i < 4; ++i) {
        CameraWidget *thumbnailWidget = new CameraWidget(this);
        thumbnailWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        thumbnailWidget->setFixedSize(thumbnailArea->height() * 16 / 9, thumbnailArea->height());
        layout->addWidget(thumbnailWidget);
        connect(thumbnailWidget, &CameraWidget::switchToMain, this, &MainWindow::onSwitchToMain);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    adjustThumbnailSizes();
    adjustMainCameraSize();
    adjustSplitterSizes();
}

void MainWindow::adjustThumbnailSizes()
{
    int newHeight = height() * 0.3;
    thumbnailArea->setFixedHeight(newHeight);

    for (int i = 0; i < thumbnailContainer->layout()->count(); ++i) {
        QWidget *widget = thumbnailContainer->layout()->itemAt(i)->widget();
        if (widget) {
            widget->setFixedHeight(newHeight);
            widget->setFixedWidth(newHeight * 16 / 9);
        }
    }
}

void MainWindow::adjustMainCameraSize()
{
    if (mainCameraWidget) {
        int mainHeight = height() * 0.7;
        int mainWidth = width() * 0.7;
        int adjustedWidth = mainHeight * 16 / 9;

        if (adjustedWidth > mainWidth) {
            adjustedWidth = mainWidth;
            mainHeight = adjustedWidth * 9 / 16;
        }

        mainCameraWidget->setFixedHeight(mainHeight);
        mainCameraWidget->setFixedWidth(adjustedWidth);
    }
}

void MainWindow::adjustSplitterSizes()
{
    QList<int> sizes;
    sizes << width() * 0.7 << width() * 0.3;
    topSplitter->setSizes(sizes);
}

void MainWindow::onSwitchToMain(CameraWidget *widget)
{
    mainCameraWidget->setVisible(false);
    widget->setVisible(false);

    CameraWidget *oldMainCameraWidget = mainCameraWidget;
    mainCameraWidget = widget;
    topSplitter->replaceWidget(0, mainCameraWidget);
    mainCameraWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    adjustMainCameraSize();

    oldMainCameraWidget->setParent(thumbnailContainer);
    oldMainCameraWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    oldMainCameraWidget->setFixedSize(thumbnailArea->height() * 16 / 9, thumbnailArea->height());

    QHBoxLayout *thumbnailLayout = qobject_cast<QHBoxLayout *>(thumbnailContainer->layout());
    if (thumbnailLayout) {
        thumbnailLayout->addWidget(oldMainCameraWidget);
    }

    mainCameraWidget->setVisible(true);
    oldMainCameraWidget->setVisible(true);
    qDebug()<<"切换主显示成功---------------------------------";
}

void MainWindow::on_btnStart_clicked()
{
    mainCameraWidget->do_slots(slotName::startRecordingSlot);
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);
    stateLabel->setText("正在开始录制视频");
}


void MainWindow::on_btnStop_clicked()
{
    mainCameraWidget->do_slots(slotName::stopRecordingSlot);
    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);
    stateLabel->setText("视频录制完毕");
}


void MainWindow::on_btnPlayHistory_clicked()
{
    videoPlayer* historyLookWidget = new videoPlayer();
    historyLookWidget->resize(this->width(), this->height());
    historyLookWidget->show();

    connect(this, &MainWindow::destroyed, historyLookWidget, &videoPlayer::close);
}

