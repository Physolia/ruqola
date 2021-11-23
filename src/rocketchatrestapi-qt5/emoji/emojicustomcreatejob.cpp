/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

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

#include "emojicustomcreatejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QFile>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMimeDatabase>
#include <QNetworkReply>
using namespace RocketChatRestApi;
EmojiCustomCreateJob::EmojiCustomCreateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

EmojiCustomCreateJob::~EmojiCustomCreateJob() = default;

bool EmojiCustomCreateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("EmojiCustomCreateJob::start");
    const QString fileNameAsLocalFile = mEmojiInfo.fileNameUrl.toLocalFile();
    auto file = new QFile(fileNameAsLocalFile);
    if (!file->open(QIODevice::ReadOnly)) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << " Impossible to open filename " << mEmojiInfo.fileNameUrl;
        Q_EMIT failed(i18n("File not found \'%1\'", fileNameAsLocalFile));
        delete file;
        deleteLater();
        return false;
    }
    QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileNameAsLocalFile);

    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeType.name()));
    const QString filePartInfo = QStringLiteral("form-data; name=\"emoji\"; filename=\"%1\"").arg(mEmojiInfo.fileNameUrl.fileName());
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(filePartInfo));

    filePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
    multiPart->append(filePart);

    QHttpPart namePart;
    namePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"name\"")));
    namePart.setBody(mEmojiInfo.name.toUtf8());
    multiPart->append(namePart);

    QHttpPart aliasesPart;
    aliasesPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"aliases\"")));
    aliasesPart.setBody(mEmojiInfo.alias.toUtf8());
    multiPart->append(aliasesPart);

    QNetworkReply *reply = networkAccessManager()->post(request(), multiPart);
    // connect(reply, &QNetworkReply::uploadProgress, this, &UploadFileJob::slotUploadProgress);
    connect(reply, &QNetworkReply::finished, this, &EmojiCustomCreateJob::slotEmojiCustomCreateFinished);
    multiPart->setParent(reply); // delete the multiPart with the reply
    return true;
}

void EmojiCustomCreateJob::slotEmojiCustomCreateFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());

    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("EmojiCustomCreateJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT emojiCustomCreateDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("EmojiCustomCreateJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

const EmojiCustomCreateJob::EmojiInfo &EmojiCustomCreateJob::emojiInfo() const
{
    return mEmojiInfo;
}

void EmojiCustomCreateJob::setEmojiInfo(const EmojiInfo &newEmojiInfo)
{
    mEmojiInfo = newEmojiInfo;
}

bool EmojiCustomCreateJob::requireHttpAuthentication() const
{
    return true;
}

bool EmojiCustomCreateJob::canStart() const
{
    if (!mEmojiInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "EmojiCustomCreateJob: mEmojiInfo is not valid";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest EmojiCustomCreateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);
    return request;
}

bool EmojiCustomCreateJob::EmojiInfo::isValid() const
{
    // Alias is optional
    return !name.isEmpty() && !fileNameUrl.isEmpty();
}

QString EmojiCustomCreateJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == QLatin1String("Custom_Emoji_Error_Name_Or_Alias_Already_In_Use")) {
        return i18n("The custom emoji or one of its aliases is already in use.");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}
