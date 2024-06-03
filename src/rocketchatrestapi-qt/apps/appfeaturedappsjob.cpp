/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appfeaturedappsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
AppFeaturedAppsJob::AppFeaturedAppsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

AppFeaturedAppsJob::~AppFeaturedAppsJob() = default;

bool AppFeaturedAppsJob::requireHttpAuthentication() const
{
    return true;
}

bool AppFeaturedAppsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get app count job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("AppFeaturedAppsJob: get report info starting"));
    return true;
}

void AppFeaturedAppsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("AppFeaturedAppsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT appFeaturedAppsDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("AppFeaturedAppsJob: Problem when we tried to get app featured info : ")
                         + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest AppFeaturedAppsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CategoriesApps, QString(), RestApiUtil::RestApiUrlExtensionType::Apps);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_appfeaturedappsjob.cpp"
