/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencemanager.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include "video-conference/videoconferencejoinjob.h"
#include "videoconferencenotificationjob.h"
#include <QDesktopServices>
#include <QJsonObject>

VideoConferenceManager::VideoConferenceManager(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mRocketChatAccount(account)
{
}

VideoConferenceManager::~VideoConferenceManager() = default;

void VideoConferenceManager::parseVideoConference(const QJsonArray &contents)
{
    for (const QJsonValue &current : contents) {
        const QJsonObject videoConfObject = current.toObject();
        VideoConference videoConference;
        videoConference.parseVideoConference(videoConfObject);
        if (videoConference.isValid()) {
            if (!mVideoConferenceList.contains(videoConference)) {
                mVideoConferenceList.append(videoConference);

                auto job = new VideoConferenceNotificationJob(this);
                connect(job, &VideoConferenceNotificationJob::acceptVideoConference, this, [this, videoConference]() {
                    mRocketChatAccount->ddp()->videoConferenceAccepted(videoConference.roomId(), videoConference.callId(), mRocketChatAccount->userId());

                    auto conferenceJoinJob = new RocketChatRestApi::VideoConferenceJoinJob(this);
                    RocketChatRestApi::VideoConferenceJoinJob::VideoConferenceJoinInfo joinInfo;
                    QJsonObject obj;
                    joinInfo.callId = videoConference.callId();
                    // TODO joinInfo.useCamera = callInfo.useCamera;
                    // TODO joinInfo.useMicro = callInfo.useMic;
                    conferenceJoinJob->setInfo(joinInfo);
                    mRocketChatAccount->restApi()->initializeRestApiJob(conferenceJoinJob);
                    connect(conferenceJoinJob, &RocketChatRestApi::VideoConferenceJoinJob::videoConferenceJoinDone, this, [](const QJsonObject &obj) {
                        // qDebug() << " join info " << obj;
                        QDesktopServices::openUrl(QUrl(obj[QLatin1String("url")].toString()));
                    });
                    if (!conferenceJoinJob->start()) {
                        qCWarning(RUQOLA_LOG) << "Impossible to start VideoConferenceJoinJob job";
                    }

                    qDebug() << " Accept";
                    // TODO
                });
                connect(job, &VideoConferenceNotificationJob::rejectVideoConference, this, [this, videoConference]() {
                    qDebug() << " REject";
                    mRocketChatAccount->ddp()->videoConferenceRejected(videoConference.roomId(), videoConference.callId(), mRocketChatAccount->userId());
                });
                job->setRocketChatAccount(mRocketChatAccount);
                job->setVideoConference(videoConference);
                job->start();
            }
        }
    }
    // [{"action":"call","params":{"callId":"63983180a7f9e1466a4eedc6","rid":"YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K","uid":"YbwG4T2uB3wZSZSKB"}}]
    // TODO
}
