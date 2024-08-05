#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CameraWidget.h"
#include <QScrollArea>
#include <QSplitter>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onSwitchToMain(CameraWidget *widget);

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_btnPlayHistory_clicked();

private:
    CameraWidget *mainCameraWidget;
    QGroupBox *controlPanel;
    QWidget *thumbnailContainer;
    QScrollArea *thumbnailArea;
    QSplitter *topSplitter;
    QLabel* stateLabel;

    void setupControlPanel();
    void setupUI();
    void adjustThumbnailSizes();
    void adjustSplitterSizes();
    void adjustMainCameraSize();
    void setupThumbnails(QHBoxLayout *layout);
    void setupThumbnailArea();
    void setupTopBox();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
