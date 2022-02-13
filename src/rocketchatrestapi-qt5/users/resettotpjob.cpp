/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resettotpjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ResetTOTPJob::ResetTOTPJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ResetTOTPJob::~ResetTOTPJob() = default;

bool ResetTOTPJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ResetTOTPJob::start");
    submitPostRequest(json());
    return true;
}

void ResetTOTPJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ResetTOTPJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT resetAvatarDone();
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("ResetTOTPJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ResetTOTPJob::requireHttpAuthentication() const
{
    return true;
}

bool ResetTOTPJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    //    if (!hasUserIdentifier()) {
    //        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ResetTOTPJob: identifier is empty";
    //        return false;
    //    }
    return true;
}

QNetworkRequest ResetTOTPJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersResetTOTP);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ResetTOTPJob::json() const
{
    QJsonObject jsonObj;
    // generateJson(jsonObj);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool ResetTOTPJob::requireTwoFactorAuthentication() const
{
    return true;
}
