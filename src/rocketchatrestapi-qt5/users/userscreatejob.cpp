/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "userscreatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UsersCreateJob::UsersCreateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersCreateJob::~UsersCreateJob()
{
}

bool UsersCreateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersCreateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &UsersCreateJob::slotUsersCreate);
    return true;
}

void UsersCreateJob::slotUsersCreate()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("UsersCreateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT usersCreateDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("UsersCreateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

CreateUpdateUserInfo UsersCreateJob::createInfo() const
{
    return mCreateInfo;
}

void UsersCreateJob::setCreateInfo(const CreateUpdateUserInfo &createInfo)
{
    mCreateInfo = createInfo;
}

bool UsersCreateJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersCreateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mCreateInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersCreateJob: mCreateInfo is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest UsersCreateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UsersCreateJob::json() const
{
    return mCreateInfo.json();
}

QString UsersCreateJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    // qDebug() << " str " << str << " details " << detail;
    if (str == QStringLiteral("error-field-unavailable")) {
        const QString field = detail.value(QLatin1String("field")).toString();
        return i18n("%1 is already in use.", field);
    }
    return RestApiAbstractJob::errorMessage(str, detail);
}
