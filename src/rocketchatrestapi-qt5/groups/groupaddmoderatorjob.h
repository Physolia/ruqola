/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GroupAddModeratorJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit GroupAddModeratorJob(QObject *parent = nullptr);
    ~GroupAddModeratorJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString addModeratorUserId() const;
    void setAddModeratorUserId(const QString &addModeratorUserId);

Q_SIGNALS:
    void addModeratorDone();

private:
    Q_DISABLE_COPY(GroupAddModeratorJob)
    void slotAddModeratorFinished();
    QString mAddModeratorUserId;
};
}
