/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
class QNetworkRequest;
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ServerInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ServerInfoJob(QObject *parent = nullptr);
    ~ServerInfoJob() override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool useDeprecatedVersion() const;

    void setUseDeprecatedVersion(bool useDeprecatedVersion);

    [[nodiscard]] bool forceRequiresAuthentication() const;
    void setForceRequiresAuthentication(bool forceRequiresAuthentication);

Q_SIGNALS:
    void serverInfoDone(const QString &versionInfo, const QJsonObject &obj);
    void serverInfoFailed(bool useDeprectedVersion);

private:
    Q_DISABLE_COPY(ServerInfoJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    bool mUseDeprecatedVersion = true;
    bool mForceRequiresAuthentication = false;
};
}
