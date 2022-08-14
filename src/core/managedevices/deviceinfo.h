/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_EXPORT DeviceInfo
{
public:
    DeviceInfo();
    ~DeviceInfo();

    void parseDeviceInfo(const QJsonObject &object);

    Q_REQUIRED_RESULT bool operator==(const DeviceInfo &other) const;

    Q_REQUIRED_RESULT const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

    Q_REQUIRED_RESULT const QString &host() const;
    void setHost(const QString &newHost);

    Q_REQUIRED_RESULT const QString &sessionId() const;
    void setSessionId(const QString &newSessionId);

    Q_REQUIRED_RESULT const QString &ip() const;
    void setIp(const QString &newIp);

    Q_REQUIRED_RESULT const QString &userId() const;
    void setUserId(const QString &newUserId);

    Q_REQUIRED_RESULT qint64 loginAt() const;
    void setLoginAt(qint64 newLoginAt);

private:
    QString mIdentifier;
    QString mHost;
    QString mSessionId;
    QString mIp;
    QString mUserId;
    qint64 mLoginAt = -1;
};

Q_DECLARE_METATYPE(DeviceInfo)
Q_DECLARE_TYPEINFO(DeviceInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const DeviceInfo &t);
