/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deviceinfo.h"
#include "utils.h"

DeviceInfo::DeviceInfo() = default;

DeviceInfo::~DeviceInfo() = default;

QDebug operator<<(QDebug d, const DeviceInfo &t)
{
    d.space() << "Identifier" << t.identifier();
    d.space() << "Host" << t.host();
    d.space() << "SessionId" << t.sessionId();
    d.space() << "Ip" << t.ip();
    d.space() << "UserId" << t.userId();
    d.space() << "LoginAt" << t.loginAt();
    return d;
}

void DeviceInfo::parseDeviceInfo(const QJsonObject &obj)
{
    mIdentifier = obj[QLatin1StringView("_id")].toString();
    mHost = obj[QLatin1StringView("host")].toString();
    mSessionId = obj[QLatin1StringView("sessionId")].toString();
    mIp = obj[QLatin1StringView("ip")].toString();
    mUserId = obj[QLatin1StringView("userId")].toString();
    const QJsonObject deviceObj = obj[QLatin1StringView("device")].toObject();
    if (!deviceObj.isEmpty()) {
        const QJsonObject osObj = deviceObj[QLatin1StringView("os")].toObject();
        if (!osObj.isEmpty()) {
            mOs = osObj[QLatin1StringView("name")].toString() + QLatin1Char(' ') + osObj[QLatin1StringView("version")].toString();
        }
        mClient = deviceObj[QLatin1StringView("name")].toString();
    }
    setLoginAt(Utils::parseIsoDate(QStringLiteral("loginAt"), obj));
    //"device":{"longVersion":"103.0","name":"Firefox","os":{"name":"Linux","version":"x86_64"},"type":"browser","version":"103.0"}
    // TODO LoginAt
    // TODO
}

bool DeviceInfo::operator==(const DeviceInfo &other) const
{
    return mIdentifier == other.identifier() && mIp == other.ip() && mHost == other.host() && mSessionId == other.sessionId() && mUserId == other.userId()
        && mLoginAt == other.loginAt() && mOs == other.os() && mClient == other.client();
}

const QString &DeviceInfo::identifier() const
{
    return mIdentifier;
}

void DeviceInfo::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

const QString &DeviceInfo::host() const
{
    return mHost;
}

void DeviceInfo::setHost(const QString &newHost)
{
    mHost = newHost;
}

const QString &DeviceInfo::sessionId() const
{
    return mSessionId;
}

void DeviceInfo::setSessionId(const QString &newSessionId)
{
    mSessionId = newSessionId;
}

const QString &DeviceInfo::ip() const
{
    return mIp;
}

void DeviceInfo::setIp(const QString &newIp)
{
    mIp = newIp;
}

const QString &DeviceInfo::userId() const
{
    return mUserId;
}

void DeviceInfo::setUserId(const QString &newUserId)
{
    mUserId = newUserId;
}

qint64 DeviceInfo::loginAt() const
{
    return mLoginAt;
}

void DeviceInfo::setLoginAt(qint64 newLoginAt)
{
    mLoginAt = newLoginAt;
    QLocale l;
    mLoginAtDateTimeStr = l.toString(QDateTime::fromMSecsSinceEpoch(mLoginAt), QLocale::LongFormat);
}

QString DeviceInfo::loginAtDisplay() const
{
    return mLoginAtDateTimeStr;
}

const QString &DeviceInfo::client() const
{
    return mClient;
}

void DeviceInfo::setClient(const QString &newClient)
{
    mClient = newClient;
}

const QString &DeviceInfo::os() const
{
    return mOs;
}

void DeviceInfo::setOs(const QString &newOs)
{
    mOs = newOs;
}
