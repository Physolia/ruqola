/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT SetTopicDmJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SetTopicDmJob(QObject *parent = nullptr);
    ~SetTopicDmJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString directUserId() const;
    void setDirectUserId(const QString &userId);

Q_SIGNALS:
    void setTopicDmDone();

private:
    Q_DISABLE_COPY(SetTopicDmJob)
    void slotSetTopicDmFinished();
    QString mDirectUserId;
    QString mTopic;
};
}
