/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sendmessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SendMessageJob::SendMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SendMessageJob::~SendMessageJob() = default;

bool SendMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SendMessageJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &SendMessageJob::slotSendMessageDone);
    return true;
}

bool SendMessageJob::requireHttpAuthentication() const
{
    return true;
}

void SendMessageJob::slotSendMessageDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SendMessageJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT sendMessageDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("SendMessageJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

SendMessageJob::SendMessageArguments SendMessageJob::sendMessageArguments() const
{
    return mSendMessageArguments;
}

void SendMessageJob::setSendMessageArguments(const SendMessageArguments &sendMessageArguments)
{
    mSendMessageArguments = sendMessageArguments;
}

QNetworkRequest SendMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatSendMessage);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool SendMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mSendMessageArguments.message.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Text is empty";
        return false;
    }
    if (mSendMessageArguments.roomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "roomId is not defined";
        return false;
    }
    return true;
}

QJsonDocument SendMessageJob::json() const
{
    QJsonObject message;
    QJsonObject jsonObj;
    if (!mSendMessageArguments.messageId.isEmpty()) {
        jsonObj[QLatin1String("_id")] = mSendMessageArguments.messageId;
    }
    if (!mSendMessageArguments.threadMessageId.isEmpty()) {
        jsonObj[QLatin1String("tmid")] = mSendMessageArguments.threadMessageId;
    }
    jsonObj[QLatin1String("rid")] = mSendMessageArguments.roomId;
    jsonObj[QLatin1String("msg")] = mSendMessageArguments.message;
    message[QLatin1String("message")] = jsonObj;
    const QJsonDocument postData = QJsonDocument(message);
    return postData;
}
