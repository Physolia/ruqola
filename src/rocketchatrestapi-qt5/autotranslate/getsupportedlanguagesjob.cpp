/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getsupportedlanguagesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
GetSupportedLanguagesJob::GetSupportedLanguagesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetSupportedLanguagesJob::~GetSupportedLanguagesJob() = default;

bool GetSupportedLanguagesJob::requireHttpAuthentication() const
{
    return true;
}

bool GetSupportedLanguagesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetSupportedLanguagesJob";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &GetSupportedLanguagesJob::slotGetSupportedLanguagesFinished);
    addStartRestApiInfo(QByteArrayLiteral("GetSupportedLanguagesJob: get message starting"));
    return true;
}

void GetSupportedLanguagesJob::slotGetSupportedLanguagesFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetSupportedLanguagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT getSupportedLanguagesDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("GetSupportedLanguagesJob: Problem when we tried to GetSupportedLanguages : ")
                             + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest GetSupportedLanguagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::AutoTranslateGetSupportedLanguages);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);
    return request;
}

bool GetSupportedLanguagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}
