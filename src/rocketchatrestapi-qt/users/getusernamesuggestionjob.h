/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT GetUsernameSuggestionJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // Since 0.65
    explicit GetUsernameSuggestionJob(QObject *parent = nullptr);
    ~GetUsernameSuggestionJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

Q_SIGNALS:
    void getUsernameSuggestionDone(const QString &result);

private:
    Q_DISABLE_COPY(GetUsernameSuggestionJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}