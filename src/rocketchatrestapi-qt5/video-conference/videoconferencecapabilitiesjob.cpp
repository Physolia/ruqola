/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencecapabilitiesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
VideoConferenceCapabilitiesJob::VideoConferenceCapabilitiesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

VideoConferenceCapabilitiesJob::~VideoConferenceCapabilitiesJob() = default;

bool VideoConferenceCapabilitiesJob::requireHttpAuthentication() const
{
    return true;
}

bool VideoConferenceCapabilitiesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start VideoConferenceCapabilitiesJob";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("VideoConferenceCapabilitiesJob: Ask for capabilities"));
    return true;
}

void VideoConferenceCapabilitiesJob::onGetRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("VideoConferenceCapabilitiesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT videoConferenceCapabilitiesDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("VideoConferenceCapabilitiesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest VideoConferenceCapabilitiesJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceCapabilities);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}