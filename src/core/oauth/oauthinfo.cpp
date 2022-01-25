/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthinfo.h"

OauthInfo::OauthInfo() = default;

QDebug operator<<(QDebug d, const OauthInfo &t)
{
    d << "Identifier " << t.identifier();
    d << "Active " << t.active();
    d << "Name " << t.name();
    d << "ClientId " << t.clientId();
    d << "ClientSecret " << t.clientSecret();
    d << "RedirectUri " << t.redirectUri();
    // TODO
    return d;
}

bool OauthInfo::operator==(const OauthInfo &other) const
{
    return mIdentifier == other.identifier() && mActive == other.active() && mName == other.name() && mClientId == other.clientId()
        && mClientSecret == other.clientSecret() && mRedirectUri == other.redirectUri();
}

void OauthInfo::parseOauthInfo(const QJsonObject &replyObject)
{
    mIdentifier = replyObject[QLatin1String("_id")].toString();
    mActive = replyObject[QLatin1String("active")].toBool();
    mName = replyObject[QLatin1String("name")].toString();
    mClientId = replyObject[QLatin1String("clientId")].toString();
    mClientSecret = replyObject[QLatin1String("clientSecret")].toString();
    mRedirectUri = replyObject[QLatin1String("redirectUri")].toString();
    // TODO _createdAt / _updatedAt / _updatedAt
#if 0
    if (replyObject.contains(QLatin1String("createdAt"))) {
        setCreateDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(QStringLiteral("createdAt"), replyObject)));
    }
    if (replyObject.contains(QLatin1String("expires"))) {
        setExpireDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(QStringLiteral("expires"), replyObject)));
    }
#endif
}

const QString &OauthInfo::identifier() const
{
    return mIdentifier;
}

void OauthInfo::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

bool OauthInfo::active() const
{
    return mActive;
}

void OauthInfo::setActive(bool newActive)
{
    mActive = newActive;
}

const QString &OauthInfo::name() const
{
    return mName;
}

void OauthInfo::setName(const QString &newName)
{
    mName = newName;
}

const QString &OauthInfo::clientId() const
{
    return mClientId;
}

void OauthInfo::setClientId(const QString &newClientId)
{
    mClientId = newClientId;
}

const QString &OauthInfo::clientSecret() const
{
    return mClientSecret;
}

void OauthInfo::setClientSecret(const QString &newClientSecret)
{
    mClientSecret = newClientSecret;
}

const QString &OauthInfo::redirectUri() const
{
    return mRedirectUri;
}

void OauthInfo::setRedirectUri(const QString &newRedirectUri)
{
    mRedirectUri = newRedirectUri;
}
