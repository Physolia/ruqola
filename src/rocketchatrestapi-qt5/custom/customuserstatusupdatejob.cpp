/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "customuserstatusupdatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
CustomUserStatusUpdateJob::CustomUserStatusUpdateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomUserStatusUpdateJob::~CustomUserStatusUpdateJob() = default;

bool CustomUserStatusUpdateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CustomUserStatusUpdateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &CustomUserStatusUpdateJob::slotCustomUserUpdate);
    return true;
}

void CustomUserStatusUpdateJob::slotCustomUserUpdate()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("CustomUserStatusUpdateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT customUserUpdateDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("CustomUserStatusUpdateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

CustomUserStatusUpdateJob::StatusUpdateInfo CustomUserStatusUpdateJob::statusUpdateInfo() const
{
    return mStatusUpdateInfo;
}

void CustomUserStatusUpdateJob::setStatusUpdateInfo(const StatusUpdateInfo &statusUpdateInfo)
{
    mStatusUpdateInfo = statusUpdateInfo;
}

bool CustomUserStatusUpdateJob::requireHttpAuthentication() const
{
    return true;
}

bool CustomUserStatusUpdateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mStatusUpdateInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CustomUserStatusUpdateJob: CustomUserStatusUpdateJob is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest CustomUserStatusUpdateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusUpdate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument CustomUserStatusUpdateJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QStringLiteral("_id")] = mStatusUpdateInfo.identifier;
    jsonObj[QStringLiteral("name")] = mStatusUpdateInfo.name;
    jsonObj[QStringLiteral("statusType")] = mStatusUpdateInfo.statusType;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
