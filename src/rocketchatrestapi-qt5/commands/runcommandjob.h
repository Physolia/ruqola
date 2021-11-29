/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
#include <QDebug>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT RunCommandJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT RunCommandInfo {
        QString commandName;
        QString roomId;
        QString threadMessageId;
        QString triggerId;
        QString params;
        Q_REQUIRED_RESULT bool isValid() const;
    };
    explicit RunCommandJob(QObject *parent = nullptr);
    ~RunCommandJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT RunCommandInfo runCommandInfo() const;
    void setRunCommandInfo(const RunCommandInfo &runCommandInfo);

    static RunCommandInfo parseString(const QString &str, const QString &roomId, const QString &tmid);
Q_SIGNALS:
    void runCommandDone();

private:
    Q_DISABLE_COPY(RunCommandJob)
    void slotRunCommand();
    RunCommandInfo mRunCommandInfo;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::RunCommandJob::RunCommandInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::RunCommandJob::RunCommandInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::RunCommandJob::RunCommandInfo &t);

