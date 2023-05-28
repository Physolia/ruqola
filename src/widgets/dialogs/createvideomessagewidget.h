/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QWidget>
class QLabel;
class QCamera;
class QComboBox;
class QVideoWidget;
class QCameraDevice;
class QToolButton;
class QMediaCaptureSession;
class KMessageWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateVideoMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateVideoMessageWidget(QWidget *parent = nullptr);
    ~CreateVideoMessageWidget() override;

private:
    void stopCamera();
    void startCamera();
    void updateCameras();
    void setCamera(const QCameraDevice &cameraDevice);
    void updateRecorderState(QMediaRecorder::RecorderState state);
    void record();
    void pause();
    void stop();
    void updateRecordTime();
    void displayRecorderError();
    void updateCameraActive(bool active);
    void displayCameraError();

    QVideoWidget *const mVideoWidget;
    QScopedPointer<QMediaRecorder> mMediaRecorder;
    QMediaCaptureSession mCaptureSession;
    QLabel *const mErrorLabel;
    QScopedPointer<QCamera> mCamera;
    QComboBox *const mListCamera;
    QToolButton *const mRecordButton;
    QToolButton *const mPauseButton;
    QLabel *const mDurationLabel;
    KMessageWidget *const mMessageWidget;
};
