/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"

#include "librestapi_private_export.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT PrivateInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit PrivateInfoJob(QObject *parent = nullptr);
    ~PrivateInfoJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;
Q_SIGNALS:
    void privateInfoDone(const QJsonObject &data);

private:
    Q_DISABLE_COPY(PrivateInfoJob)
    void slotPrivateInfoDone();
};
}
