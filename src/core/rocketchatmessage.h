/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "user.h"
#include <QDebug>
#include <QJsonDocument>
#include <QString>
class QDateTime;
class LIBRUQOLACORE_TESTS_EXPORT RocketChatMessage
{
    Q_GADGET
public:
    RocketChatMessage();
    ~RocketChatMessage() = default;

    struct RocketChatMessageResult {
        QString method;
        QString result;
        QJsonDocument jsonDocument;
    };

    void setJsonFormat(QJsonDocument::JsonFormat jsonFormat);

    [[nodiscard]] RocketChatMessageResult generateMethod(const QString &method, const QJsonDocument &params, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult setRoomEncrypted(const QString &roomId, bool encrypted, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult joinRoom(const QString &roomId, const QString &accessCode, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult informTypingStatus(const QString &roomId, const QString &userId, bool typingStatus, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult setDefaultStatus(User::PresenceStatus status, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult openDirectChannel(const QString &username, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult createJitsiConfCall(const QString &roomId, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult userAutocomplete(const QString &searchText, const QString &exception, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult unsubscribe(quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult blockUser(const QString &rid, const QString &userId, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult unblockUser(const QString &rid, const QString &userId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    searchRoomUsers(const QString &roomId, const QString &pattern, const QString &exceptions, bool searchUser, bool searchRoom, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    inputChannelAutocomplete(const QString &roomId, const QString &pattern, const QString &exceptions, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    inputUserAutocomplete(const QString &roomId, const QString &pattern, const QString &exceptions, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult deleteFileMessage(const QString &fileId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult roomNameExists(const QString &roomName, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult streamNotifyUserOtrEnd(const QString &userFrom, const QString &userTo, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    streamNotifyUserOtrHandshake(const QString &userFrom, const QString &userTo, const QString &publicKeys, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    streamNotifyUserOtrAcknowledge(const QString &roomId, const QString &userId, const QString &publicKeys, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult deleteCustomSound(const QString &identifier, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult uploadCustomSound(const QByteArray &sound, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult listCustomSounds(quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult enable2fa(quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult disable2fa(const QString &code, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult validateTempToken2fa(const QString &code, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult regenerateCodes2fa(const QString &code, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult deleteOAuthApp(const QString &appId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult addOAuthApp(const QString &name, bool active, const QString &redirectUrl, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult updateOAuthApp(const QString &name, bool active, const QString &redirectUrl, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult setAdminStatus(const QString &userId, bool admin, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult openRoom(const QString &roomId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult getRoomById(const QString &roomId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult bannerDismiss(const QString &bannerId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult licenseGetModules(quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    videoConferenceAccepted(const QString &roomId, const QString &callId, const QString &userId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    videoConferenceRejected(const QString &roomId, const QString &callId, const QString &userId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    videoConferenceCall(const QString &roomId, const QString &callId, const QString &userId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    videoConferenceConfirmed(const QString &roomId, const QString &callId, const QString &userId, quint64 id);

private:
    Q_DISABLE_COPY(RocketChatMessage)
    [[nodiscard]] QJsonValue toJsonDateTime(const QDateTime &dateTime);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult subscribe(const QString &name, const QJsonDocument &params, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult saveRoomSettings(const QString &key, const QString &roomId, const QJsonValue &value, quint64 id);
    QJsonDocument::JsonFormat mJsonFormat = QJsonDocument::Compact;
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    generateVideoConferenceAction(const QString &action, const QString &roomId, const QString &callId, const QString &userId, quint64 id);
};

LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RocketChatMessage::RocketChatMessageResult &t);
