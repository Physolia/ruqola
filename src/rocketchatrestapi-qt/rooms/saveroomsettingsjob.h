/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT SaveRoomSettingsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT SaveRoomSettingsInfo {
        enum SettingChanged {
            Unknown = 0,
            SystemMessages = 1,
            RoomName = 2,
            RoomTopic = 4,
            RoomAnnouncement = 8,
            RoomDescription = 16,
            RetentionEnabled = 32,
            RetentionExcludePinned = 64,
            RetentionFilesOnly = 128,
            RetentionIgnoreThreads = 256,
            RetentionOverrideGlobal = 512,
            RetentionMaxAge = 1024,
            Favorite = 2048,
            RoomType = 4096,
            JoinCode = 8192,
            ReadOnly = 16384,
            Encrypted = 32768,
            RoomAvatar = 65536,
            ReactWhenReadOnly = 131072,
            Featured = 262144,
            DefaultValue = 524288,
        };
        Q_DECLARE_FLAGS(SettingsChanged, SettingChanged)

        QStringList systemMessages;
        QString roomAvatar;
        QByteArray roomId;
        QString roomName;
        QString roomTopic;
        QString roomAnnouncement;
        QString roomDescription;
        QString roomType;
        QString joinCode;
        bool retentionEnabled = false;
        bool retentionExcludePinned = false;
        bool retentionFilesOnly = false;
        bool retentionIgnoreThreads = false;
        bool retentionOverrideGlobal = false;
        bool favorite = false;
        bool readOnly = false;
        bool encrypted = false;
        bool reactWhenReadOnly = false;
        bool featured = false;
        bool defaultValue = false;
        int retentionMaxAge = 32;
        //'featured','roomCustomFields',
        //'reactWhenReadOnly', 'default','tokenpass', 'streamingOptions'
        SettingsChanged mSettingsWillBeChanged = SettingChanged::Unknown;
        [[nodiscard]] bool isValid() const;
    };

    explicit SaveRoomSettingsJob(QObject *parent = nullptr);
    ~SaveRoomSettingsJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] SaveRoomSettingsInfo saveRoomSettingsInfo() const;
    void setSaveRoomSettingsInfo(const SaveRoomSettingsInfo &saveRoomSettingsInfo);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &detail) override;

Q_SIGNALS:
    void saveRoomSettingsDone(const QString &roomId);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

    SaveRoomSettingsInfo mSaveRoomSettingsInfo;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::SettingChanged)
Q_DECLARE_TYPEINFO(RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo, Q_RELOCATABLE_TYPE);
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo &t);
