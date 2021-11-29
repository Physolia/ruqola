/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT DeleteOwnAccountJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit DeleteOwnAccountJob(QObject *parent = nullptr);
    ~DeleteOwnAccountJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString password() const;
    void setPassword(const QString &password);

Q_SIGNALS:
    void deleteOwnAccountDone();

private:
    Q_DISABLE_COPY(DeleteOwnAccountJob)
    void slotDeleteOwnAccount();
    QString mPassword;
};
}
