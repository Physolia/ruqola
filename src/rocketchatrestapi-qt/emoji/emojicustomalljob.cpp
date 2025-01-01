/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicustomalljob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace RocketChatRestApi;
EmojiCustomAllJob::EmojiCustomAllJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

EmojiCustomAllJob::~EmojiCustomAllJob() = default;

bool EmojiCustomAllJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start EmojiCustomAllJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("EmojiCustomAllJob: Load Emoji custom"));

    return true;
}

void EmojiCustomAllJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("EmojiCustomAllJob done: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT emojiCustomAllDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("EmojiCustomAllJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool EmojiCustomAllJob::requireHttpAuthentication() const
{
    return true;
}

bool EmojiCustomAllJob::hasQueryParameterSupport() const
{
    return true;
}

QNetworkRequest EmojiCustomAllJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomAll);

    QUrlQuery queryUrl;
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}

#include "moc_emojicustomalljob.cpp"
