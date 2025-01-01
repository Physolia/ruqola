/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatuslistjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace RocketChatRestApi;
CustomUserStatusListJob::CustomUserStatusListJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomUserStatusListJob::~CustomUserStatusListJob() = default;

bool CustomUserStatusListJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CustomUserStatusJobList job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("CustomUserStatusJob: Ask custom user status info"));

    return true;
}

void CustomUserStatusListJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("CustomUserStatusJob done: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT customUserStatusDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("CustomUserStatusJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool CustomUserStatusListJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest CustomUserStatusListJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusList);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}

bool CustomUserStatusListJob::hasQueryParameterSupport() const
{
    return false;
}

#include "moc_customuserstatuslistjob.cpp"
