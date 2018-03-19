/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "restapimethod.h"
#include "restapirequest.h"
#include "ruqola_restapi_debug.h"
#include "serverinfojob.h"
#include "uploadfilejob.h"
#include "owninfojob.h"
#include "getavatarjob.h"
#include "logoutjob.h"
#include "privateinfojob.h"
#include "channellistjob.h"
#include "loginjob.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QJsonArray>
#include <QHttpMultiPart>
#include <QFile>
#include <QMimeDatabase>

RestApiRequest::RestApiRequest(QObject *parent)
    : QObject(parent)
{
    mRestApiMethod = new RestApiMethod;
    mCookieJar = new QNetworkCookieJar;
    mNetworkAccessManager = new QNetworkAccessManager(this);
    mNetworkAccessManager->setCookieJar(mCookieJar);
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &RestApiRequest::slotResult);
    connect(mNetworkAccessManager, &QNetworkAccessManager::sslErrors, this, &RestApiRequest::slotSslErrors);
}

RestApiRequest::~RestApiRequest()
{
    delete mRestApiMethod;
}

void RestApiRequest::initializeCookies()
{
    QString url = serverUrl();
    if (!url.isEmpty()) {
        QString host;
        QStringList lsthost = url.split(QStringLiteral("//"));
        if (lsthost.count() < 2) {
            host = url;
        } else {
            host = lsthost.at(1);
        }

        if (!mUserId.isEmpty()) {
            QNetworkCookie userIdCookie;
            userIdCookie.setDomain(host);
            userIdCookie.setName(QByteArrayLiteral("rc_uid"));
            userIdCookie.setValue(mUserId.toLocal8Bit());
            mCookieJar->insertCookie(userIdCookie);
        }

        if (!mAuthToken.isEmpty()) {
            QNetworkCookie tokenCookie;
            tokenCookie.setDomain(host);
            tokenCookie.setName(QByteArrayLiteral("rc_token"));
            tokenCookie.setValue(mAuthToken.toLocal8Bit());
            mCookieJar->insertCookie(tokenCookie);
        }
    } else {
        qCWarning(RUQOLA_RESTAPI_LOG) << "We can not initialize cookies as server url is empty.";
    }
}

void RestApiRequest::setAuthToken(const QString &authToken)
{
    const bool isChanged = (mAuthToken != authToken);
    mAuthToken = authToken;
    if (isChanged) {
        if (!mAuthToken.isEmpty()) {
            initializeCookies();
        }
    }
}

void RestApiRequest::setUserId(const QString &userId)
{
    const bool isChanged = (mUserId != userId);
    mUserId = userId;
    if (isChanged) {
        if (!mUserId.isEmpty()) {
            initializeCookies();
        }
    }
}

void RestApiRequest::parseGet(const QByteArray &data, const QUrl &url, bool storeInCache, const QUrl &localFileUrl)
{
    qCDebug(RUQOLA_RESTAPI_LOG) << "RestApiRequest::parseGet: url " << url;
    Q_EMIT getDataDone(data, url, storeInCache, localFileUrl);
}

void RestApiRequest::parsePost(const QByteArray &data)
{
    qCDebug(RUQOLA_RESTAPI_LOG) << "RestApiRequest::parsePost: " << data;
}

void RestApiRequest::slotResult(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        //Exclude it until we port to new api
        const RestMethod restMethod = reply->property("method").value<RestMethod>();
        if (restMethod == ServerInfo
            || restMethod == UploadFile
            || restMethod == Me
            || restMethod == GetAvatar
            || restMethod == Logout
            || restMethod == PrivateInfo
            || restMethod == ChannelList
            || restMethod == Login) {
            return;
        }
        const QByteArray data = reply->readAll();
        switch (restMethod) {
        case Login:
        case ChannelList:
        case PrivateInfo:
        case Logout:
        case GetAvatar:
        case ServerInfo:
        case Me:
        case UploadFile:
            return;
        case Get:
        {
            const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (status == 200) {
                bool storeInCache = true;
                QUrl localFile;
                QVariant var = reply->property("storeInCache");
                if (var.isValid()) {
                    storeInCache = var.toBool();
                }
                var = reply->property("localFile");
                if (var.isValid()) {
                    localFile = var.toUrl();
                }
                parseGet(data, reply->url(), storeInCache, localFile);
            } else {
                qCWarning(RUQOLA_RESTAPI_LOG) << "Unable to download " << reply->url();
            }
            break;
        }
        case Post:
            parsePost(data);
            break;
        case Unknown:
            qCWarning(RUQOLA_RESTAPI_LOG) << " Unknown restapi method" << data;
            break;
        }
    } else {
        const RestMethod restMethod = reply->property("method").value<RestMethod>();
        qCWarning(RUQOLA_RESTAPI_LOG) << " Error reply - "<<reply->errorString() << " restMethod "<<restMethod;
    }
    reply->deleteLater();
}

void RestApiRequest::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)
{
    qCDebug(RUQOLA_RESTAPI_LOG) << " void RestApiRequest::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)" << error.count();
    reply->ignoreSslErrors(error);
}

void RestApiRequest::setPassword(const QString &password)
{
    mPassword = password;
}

void RestApiRequest::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString RestApiRequest::serverUrl() const
{
    return mRestApiMethod->serverUrl();
}

void RestApiRequest::setServerUrl(const QString &serverUrl)
{
    mRestApiMethod->setServerUrl(serverUrl);
}

QString RestApiRequest::authToken() const
{
    return mAuthToken;
}

QString RestApiRequest::userId() const
{
    return mUserId;
}

void RestApiRequest::login()
{
    LoginJob *job = new LoginJob(this);
    connect(job, &LoginJob::loginDone, this, &RestApiRequest::slotLogin);
    job->setNetworkAccessManager(mNetworkAccessManager);
    job->setRestApiMethod(mRestApiMethod);
    job->setPassword(mPassword);
    job->setUserName(mUserName);
    job->start();
}

void RestApiRequest::slotLogin(const QString &authToken, const QString &userId)
{
    mAuthToken = authToken;
    mUserId = userId;
}

void RestApiRequest::slotLogout()
{
    mUserId.clear();
    mAuthToken.clear();
    Q_EMIT logoutDone();
}

void RestApiRequest::logout()
{
    LogoutJob *job = new LogoutJob(this);
    connect(job, &LogoutJob::logoutDone, this, &RestApiRequest::slotLogout);
    job->setNetworkAccessManager(mNetworkAccessManager);
    job->setRestApiMethod(mRestApiMethod);
    job->setAuthToken(mAuthToken);
    job->setUserId(mUserId);
    job->start();
}

void RestApiRequest::channelList()
{
    ChannelListJob *job = new ChannelListJob(this);
    //TODO connect(job, &ChannelListJob::channelListDone, this, &RestApiRequest::slotLogout);
    job->setNetworkAccessManager(mNetworkAccessManager);
    job->setRestApiMethod(mRestApiMethod);
    job->setAuthToken(mAuthToken);
    job->setUserId(mUserId);
    job->start();
}

void RestApiRequest::getAvatar(const QString &userId)
{
    GetAvatarJob *job = new GetAvatarJob(this);
    connect(job, &GetAvatarJob::avatar, this, &RestApiRequest::avatar);
    job->setNetworkAccessManager(mNetworkAccessManager);
    job->setRestApiMethod(mRestApiMethod);
    job->setAvatarUserId(userId);
    job->start();
}

void RestApiRequest::getPrivateSettings()
{
    PrivateInfoJob *job = new PrivateInfoJob(this);
    connect(job, &PrivateInfoJob::privateInfoDone, this, &RestApiRequest::privateInfoDone);
    job->setNetworkAccessManager(mNetworkAccessManager);
    job->setRestApiMethod(mRestApiMethod);
    job->setAuthToken(mAuthToken);
    job->setUserId(mUserId);
    job->start();
}

void RestApiRequest::getOwnInfo()
{
    OwnInfoJob *job = new OwnInfoJob(this);
    connect(job, &OwnInfoJob::ownInfoDone, this, &RestApiRequest::getOwnInfoDone);
    job->setNetworkAccessManager(mNetworkAccessManager);
    job->setRestApiMethod(mRestApiMethod);
    job->setAuthToken(mAuthToken);
    job->setUserId(mUserId);
    job->start();
}

void RestApiRequest::post(const QUrl &url, const QByteArray &data, const QString &mimeType)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, mimeType);
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    QNetworkReply *reply = mNetworkAccessManager->post(request, data);
    reply->setProperty("method", QVariant::fromValue(RestMethod::Post));
}

QNetworkReply *RestApiRequest::get(const QUrl &url, const QString &mimeType)
{
    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
    request.setHeader(QNetworkRequest::ContentTypeHeader, mimeType);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    QNetworkReply *reply = mNetworkAccessManager->get(request);
    reply->setProperty("method", QVariant::fromValue(RestMethod::Get));
    return reply;
}

void RestApiRequest::serverInfo()
{
    ServerInfoJob *job = new ServerInfoJob(this);
    job->setNetworkAccessManager(mNetworkAccessManager);
    job->setRestApiMethod(mRestApiMethod);
    connect(job, &ServerInfoJob::serverInfoDone, this, &RestApiRequest::getServerInfoDone);
    job->start();
}

void RestApiRequest::uploadFile(const QString &roomId, const QString &description, const QString &text, const QUrl &filename)
{
    UploadFileJob *job = new UploadFileJob(this);
    job->setNetworkAccessManager(mNetworkAccessManager);
    job->setRestApiMethod(mRestApiMethod);
    job->setDescription(description);
    job->setMessageText(text);
    job->setFilenameUrl(filename);
    job->setRoomId(roomId);
    job->setAuthToken(mAuthToken);
    job->setUserId(mUserId);
    job->start();
}
