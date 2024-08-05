#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include "HCNetSDK.h"
enum class slotName
{
    startRecordingSlot,
    stopRecordingSlot,
    viewHistorySlot
};

class CameraWidget : public QWidget {
    Q_OBJECT


public:
    CameraWidget(QWidget *parent = nullptr);
    ~CameraWidget();
    void do_slots(slotName sName);

signals:
    void switchToMain(CameraWidget* widget);
    void startRecording(CameraWidget* widget);
    void stopRecording(CameraWidget* widget);
    void viewHistory(CameraWidget* widget);


private slots:
    void showContextMenu(const QPoint &pos);
    void startRecordingSlot();
    void stopRecordingSlot();
    void viewHistorySlot();

private:
    QLabel *videoLabel;
    QTimer *timer;
    LONG lUserID;
    LONG lRealPlayHandle;
    bool isRecording;
    QString recordSavePath;
    QString videoFileName = "";

    // QWidget interface
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void convertVideo(const QString &inputFile, const QString &outputFile);
};


#endif // CAMERAWIDGET_H
