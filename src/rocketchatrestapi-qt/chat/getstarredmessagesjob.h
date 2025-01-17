/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT GetStarredMessagesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetStarredMessagesJob(QObject *parent = nullptr);
    ~GetStarredMessagesJob() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    [[nodiscard]] bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void getStarredMessagesDone(const QJsonObject &obj, const QString &roomId);

private:
    Q_DISABLE_COPY(GetStarredMessagesJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
};
}
