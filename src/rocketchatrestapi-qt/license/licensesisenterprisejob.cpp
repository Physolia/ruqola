/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesisenterprisejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
LicensesIsEnterpriseJob::LicensesIsEnterpriseJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

LicensesIsEnterpriseJob::~LicensesIsEnterpriseJob() = default;

bool LicensesIsEnterpriseJob::requireHttpAuthentication() const
{
    return true;
}

bool LicensesIsEnterpriseJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start LicensesIsEnterpriseJob";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("LicensesIsEnterpriseJob: Ask for licenses"));
    return true;
}

void LicensesIsEnterpriseJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("LicensesIsEnterpriseJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT licensesIsEnterpriseDone(replyObject[QLatin1String("isEnterprise")].toBool());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("LicensesIsEnterpriseJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest LicensesIsEnterpriseJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::LicensesIsEntreprise);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_licensesisenterprisejob.cpp"
