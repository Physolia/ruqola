/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchmessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
SearchMessageJob::SearchMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SearchMessageJob::~SearchMessageJob() = default;

bool SearchMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool SearchMessageJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start search message job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &SearchMessageJob::slotSearchMessageFinished);
    addStartRestApiInfo(QByteArrayLiteral("SearchMessageJob: search message starting"));
    return true;
}

void SearchMessageJob::slotSearchMessageFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SearchMessageJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT searchMessageDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("SearchMessageJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool SearchMessageJob::useRegularExpression() const
{
    return mUseRegularExpression;
}

void SearchMessageJob::setUseRegularExpression(bool useRegularExpression)
{
    mUseRegularExpression = useRegularExpression;
}

int SearchMessageJob::count() const
{
    return mCount;
}

void SearchMessageJob::setCount(int count)
{
    mCount = count;
}

QString SearchMessageJob::searchText() const
{
    return mSearchText;
}

void SearchMessageJob::setSearchText(const QString &searchText)
{
    mSearchText = searchText;
}

QString SearchMessageJob::roomId() const
{
    return mRoomId;
}

void SearchMessageJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString SearchMessageJob::convertSearchText() const
{
    if (mUseRegularExpression) {
        return QStringLiteral("/%1/i").arg(mSearchText);
    }
    return mSearchText;
}

QNetworkRequest SearchMessageJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatSearch);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mRoomId);
    queryUrl.addQueryItem(QStringLiteral("searchText"), convertSearchText());
    if (mCount != -1) {
        queryUrl.addQueryItem(QStringLiteral("count"), QString::number(mCount));
    }
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool SearchMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomId is empty";
        return false;
    }
    if (mSearchText.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SearchText is empty";
        return false;
    }
    return true;
}
