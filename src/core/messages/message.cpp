/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "message.h"
#include "ruqola_debug.h"
#include <KLocalizedString>
#include <QCborValue>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>

Message::Message(EmojiManager *emojiManager)
    : mEmojiManager(emojiManager)
{
}

Message::~Message() = default;

void Message::parseMessage(const QJsonObject &o, bool restApi)
{
    const QString roomId = o.value(QLatin1StringView("rid")).toString();

    // t ? I can't find it.
    const QString type = o.value(QLatin1StringView("t")).toString();

    mMessageId = o.value(QLatin1StringView("_id")).toString();
    mRoomId = roomId;
    mText = o.value(QLatin1StringView("msg")).toString();
    if (restApi) {
        mUpdatedAt = Utils::parseIsoDate(QStringLiteral("_updatedAt"), o);
        setEditedAt(Utils::parseIsoDate(QStringLiteral("editedAt"), o));
        setTimeStamp(Utils::parseIsoDate(QStringLiteral("ts"), o));
        mThreadLastMessage = Utils::parseIsoDate(QStringLiteral("tlm"), o);
        mDiscussionLastMessage = Utils::parseIsoDate(QStringLiteral("dlm"), o);
    } else {
        setTimeStamp(Utils::parseDate(QStringLiteral("ts"), o));
        mUpdatedAt = Utils::parseDate(QStringLiteral("_updatedAt"), o);
        setEditedAt(Utils::parseDate(QStringLiteral("editedAt"), o));
        // Verify if a day we will use not restapi for it.
        mThreadLastMessage = Utils::parseDate(QStringLiteral("tlm"), o);
        // Verify if a day we will use not restapi for it.
        mDiscussionLastMessage = Utils::parseDate(QStringLiteral("dlm"), o);
    }

    QStringList lst;
    const QJsonArray replieArray = o.value(QLatin1StringView("replies")).toArray();
    const auto nbReplieArrayCount{replieArray.count()};
    lst.reserve(nbReplieArrayCount);
    for (auto i = 0; i < nbReplieArrayCount; ++i) {
        lst.append(replieArray.at(i).toVariant().toString());
    }
    mReplies = lst;

    const auto userObject = o.value(QLatin1StringView("u")).toObject();
    mUsername = userObject.value(QLatin1StringView("username")).toString();
    mName = userObject.value(QLatin1StringView("name")).toString();
    mUserId = userObject.value(QLatin1StringView("_id")).toString();
    mEditedByUsername = o.value(QLatin1StringView("editedBy")).toObject().value(QLatin1StringView("username")).toString();
    mEditedByUserId = o.value(QLatin1StringView("editedBy")).toObject().value(QLatin1StringView("_id")).toString();
    mAlias = o.value(QLatin1StringView("alias")).toString();
    mAvatar = o.value(QLatin1StringView("avatar")).toString();
    mGroupable = o.value(QLatin1StringView("groupable")).toBool(/*true*/ false); // Laurent, disable for the moment groupable
    mParseUrls = o.value(QLatin1StringView("parseUrls")).toBool();
    mRole = o.value(QLatin1StringView("role")).toString();
    mThreadCount = o.value(QLatin1StringView("tcount")).toInt();
    mDiscussionCount = o.value(QLatin1StringView("dcount")).toInt();
    mDiscussionRoomId = o.value(QLatin1StringView("drid")).toString();
    mThreadMessageId = o.value(QLatin1StringView("tmid")).toString();
    mEmoji = o.value(QLatin1StringView("emoji")).toString();
    mMessageStarred.parse(o);
    mMessagePinned.parse(o);
    mMessageTranslation.parse(o);

    mMessageType = Message::MessageType::NormalText;
    if (!type.isEmpty()) {
        if (type == QLatin1StringView("videoconf")) {
            mMessageType = VideoConference;
            // qDebug() << " VIDEO " << o;
        } else {
            mSystemMessageType = type;
            mMessageType = System;
        }
    }
    parseBlocks(o.value(QLatin1StringView("blocks")).toArray());
    parseMentions(o.value(QLatin1StringView("mentions")).toArray());

    parseAttachment(o.value(QLatin1StringView("attachments")).toArray());
    parseUrls(o.value(QLatin1StringView("urls")).toArray());
    parseReactions(o.value(QLatin1StringView("reactions")).toObject());
    parseChannels(o.value(QLatin1StringView("channels")).toArray());
    // TODO unread element
}

void Message::parseReactions(const QJsonObject &reacts)
{
    if (!reacts.isEmpty()) {
        mReactions.parseReactions(reacts, mEmojiManager);
    }
}

QString Message::editedDisplayTime() const
{
    return mEditedDisplayTime;
}

bool Message::isEditingMode() const
{
    return mIsEditingMode;
}

void Message::setIsEditingMode(bool isEditingMode)
{
    mIsEditingMode = isEditingMode;
}

bool Message::showIgnoredMessage() const
{
    return mShowIgnoredMessage;
}

void Message::setShowIgnoredMessage(bool showIgnoredMessage)
{
    mShowIgnoredMessage = showIgnoredMessage;
}

bool Message::pendingMessage() const
{
    return mPendingMessage;
}

void Message::setPendingMessage(bool pendingMessage)
{
    mPendingMessage = pendingMessage;
}

QString Message::emoji() const
{
    return mEmoji;
}

void Message::setEmoji(const QString &emoji)
{
    mEmoji = emoji;
}

QStringList Message::replies() const
{
    return mReplies;
}

void Message::setReplies(const QStringList &replies)
{
    mReplies = replies;
}

QString Message::name() const
{
    return mName;
}

void Message::setName(const QString &name)
{
    mName = name;
}

bool Message::isAutoTranslated() const
{
    return !mMessageTranslation.translatedString().isEmpty();
}

bool Message::showTranslatedMessage() const
{
    return mShowTranslatedMessage;
}

void Message::setShowTranslatedMessage(bool showOriginalMessage)
{
    mShowTranslatedMessage = showOriginalMessage;
}

MessageTranslation Message::messageTranslation() const
{
    return mMessageTranslation;
}

void Message::setMessageTranslation(const MessageTranslation &messageTranslation)
{
    mMessageTranslation = messageTranslation;
}

QString Message::displayTime() const
{
    return mDisplayTime;
}

QString Message::threadMessageId() const
{
    return mThreadMessageId;
}

void Message::setThreadMessageId(const QString &threadMessageId)
{
    mThreadMessageId = threadMessageId;
}

QString Message::discussionRoomId() const
{
    return mDiscussionRoomId;
}

void Message::setDiscussionRoomId(const QString &discussionRoomId)
{
    mDiscussionRoomId = discussionRoomId;
}

int Message::discussionCount() const
{
    return mDiscussionCount;
}

void Message::setDiscussionCount(int discussionCount)
{
    mDiscussionCount = discussionCount;
}

qint64 Message::discussionLastMessage() const
{
    return mDiscussionLastMessage;
}

void Message::setDiscussionLastMessage(qint64 discussionLastMessage)
{
    mDiscussionLastMessage = discussionLastMessage;
}

qint64 Message::threadLastMessage() const
{
    return mThreadLastMessage;
}

void Message::setThreadLastMessage(qint64 threadLastMessage)
{
    mThreadLastMessage = threadLastMessage;
}

int Message::threadCount() const
{
    return mThreadCount;
}

void Message::setThreadCount(int threadCount)
{
    mThreadCount = threadCount;
}

MessageStarred Message::messageStarred() const
{
    return mMessageStarred;
}

void Message::setMessageStarred(MessageStarred messageStarred)
{
    mMessageStarred = messageStarred;
}

MessagePinned Message::messagePinned() const
{
    return mMessagePinned;
}

void Message::setMessagePinned(const MessagePinned &messagePinned)
{
    mMessagePinned = messagePinned;
}

bool Message::unread() const
{
    return mUnread;
}

void Message::setUnread(bool unread)
{
    mUnread = unread;
}

QString Message::role() const
{
    return mRole;
}

void Message::setRole(const QString &role)
{
    mRole = role;
}

void Message::parseChannels(const QJsonArray &channels)
{
    mChannels.clear();
    for (int i = 0, total = channels.size(); i < total; ++i) {
        const QJsonObject mention = channels.at(i).toObject();
        mChannels.insert(mention.value(QLatin1StringView("name")).toString(), mention.value(QLatin1StringView("_id")).toString());
    }
}

QList<Block> Message::blocks() const
{
    return mBlocks;
}

void Message::setBlocks(const QList<Block> &newBlocks)
{
    mBlocks = newBlocks;
}

QString Message::originalMessageOrAttachmentDescription() const
{
    if (attachments().empty()) {
        return text();
    }
    return attachments().constFirst().description();
}

const QString &Message::localTranslation() const
{
    return mLocalTranslation;
}

void Message::setLocalTranslation(const QString &newLocalTranslation)
{
    mLocalTranslation = newLocalTranslation;
}

bool Message::hoverHighlight() const
{
    return mHoverHighlight;
}

void Message::setHoverHighlight(bool newShowReactionIcon)
{
    mHoverHighlight = newShowReactionIcon;
}

const QMap<QString, QString> &Message::channels() const
{
    return mChannels;
}

void Message::setChannels(const QMap<QString, QString> &newChannels)
{
    mChannels = newChannels;
}

void Message::parseBlocks(const QJsonArray &blocks)
{
    mBlocks.clear();
    for (int i = 0, total = blocks.count(); i < total; ++i) {
        const QJsonObject blockObject = blocks.at(i).toObject();
        Block b;
        b.parseBlock(blockObject);
        if (b.isValid()) {
            mBlocks.append(std::move(b));
        }
    }
}

ModerationMessage Message::moderationMessage() const
{
    return mModerationMessage;
}

void Message::setModerationMessage(const ModerationMessage &newModerationMessage)
{
    mModerationMessage = newModerationMessage;
}

QColor Message::goToMessageBackgroundColor() const
{
    return mGoToMessageBackgroundColor;
}

void Message::setGoToMessageBackgroundColor(const QColor &newGoToMessageBackgroundColor)
{
    mGoToMessageBackgroundColor = newGoToMessageBackgroundColor;
}

void Message::setVideoConferenceInfo(const VideoConferenceInfo &info)
{
    auto it = std::find_if(mBlocks.cbegin(), mBlocks.cend(), [info](const auto &block) {
        return block.blockId() == info.blockId();
    });
    if (it != mBlocks.cend()) {
        mBlocks.removeAll(*it);
        Block b(*it);
        b.setVideoConferenceInfo(info);
        mBlocks.append(b);
    }
}

void Message::parseMentions(const QJsonArray &mentions)
{
    mMentions.clear();
    for (int i = 0; i < mentions.size(); i++) {
        const QJsonObject mention = mentions.at(i).toObject();
        mMentions.insert(mention.value(QLatin1StringView("username")).toString(), mention.value(QLatin1StringView("_id")).toString());
    }
}

void Message::parseUrls(const QJsonArray &urls)
{
    mUrls.clear();
    if (!urls.isEmpty()) {
        qCDebug(RUQOLA_LOG) << " void Message::urls(const QJsonObject &attachments)" << urls;
        for (int i = 0; i < urls.size(); i++) {
            const QJsonObject url = urls.at(i).toObject();
            MessageUrl messageUrl;
            messageUrl.setUrlId(Message::generateUniqueId(mMessageId, i));
            messageUrl.parseUrl(url);
            if (!messageUrl.isEmpty()) {
                mUrls.append(messageUrl);
            }
        }
    }
}

Reactions Message::reactions() const
{
    return mReactions;
}

void Message::setReactions(const Reactions &reactions)
{
    mReactions = reactions;
}

bool Message::isPinned() const
{
    return mMessagePinned.pinned();
}

bool Message::isStarred() const
{
    return mMessageStarred.isStarred();
}

void Message::setIsStarred(bool starred)
{
    mMessageStarred.setIsStarred(starred);
}

QMap<QString, QString> Message::mentions() const
{
    return mMentions;
}

void Message::setMentions(const QMap<QString, QString> &mentions)
{
    mMentions = mentions;
}

void Message::parseAttachment(const QJsonArray &attachments)
{
    mAttachments.clear();
    if (!attachments.isEmpty()) {
        // qDebug() << " void Message::parseAttachment(const QJsonObject &attachments)"<<attachments;
        for (int i = 0; i < attachments.size(); i++) {
            const QJsonObject attachment = attachments.at(i).toObject();
            MessageAttachment messageAttachement;
            messageAttachement.parseAttachment(attachment);
            messageAttachement.setAttachmentId(Message::generateUniqueId(messageId(), i));
            if (messageAttachement.isValid()) {
                mAttachments.append(messageAttachement);
            }
        }
    }
}

bool Message::operator==(const Message &other) const
{
    return (mMessageId == other.messageId()) && (mRoomId == other.roomId()) && (mText == other.text()) && (mTimeStamp == other.timeStamp())
        && (mUsername == other.username()) && (mName == other.name()) && (mUserId == other.userId()) && (mUpdatedAt == other.updatedAt())
        && (mEditedAt == other.editedAt()) && (mEditedByUsername == other.editedByUsername()) && (mEditedByUserId == other.editedByUserId())
        && (mAlias == other.alias()) && (mAvatar == other.avatar()) && (mSystemMessageType == other.systemMessageType()) && (mGroupable == other.groupable())
        && (mParseUrls == other.parseUrls()) && (mUrls == other.urls()) && (mAttachments == other.attachments()) && (mMentions == other.mentions())
        && (mRole == other.role()) && (mReactions == other.reactions()) && (mUnread == other.unread()) && (mMessagePinned == other.messagePinned())
        && (mMessageStarred == other.messageStarred()) && (mThreadCount == other.threadCount()) && (mThreadLastMessage == other.threadLastMessage())
        && (mDiscussionCount == other.discussionCount()) && (mDiscussionLastMessage == other.discussionLastMessage())
        && (mDiscussionRoomId == other.discussionRoomId()) && (mThreadMessageId == other.threadMessageId())
        && (mMessageTranslation == other.messageTranslation()) && (mShowTranslatedMessage == other.showTranslatedMessage()) && (mReplies == other.replies())
        && (mEmoji == other.emoji()) && (mPendingMessage == other.pendingMessage()) && (mShowIgnoredMessage == other.showIgnoredMessage())
        && (mChannels == other.channels()) && (mLocalTranslation == other.localTranslation()) && (mBlocks == other.blocks())
        && (mDisplayTime == other.mDisplayTime);
}

bool Message::operator<(const Message &other) const
{
    return mTimeStamp < other.mTimeStamp;
}

QString Message::systemMessageType() const
{
    return mSystemMessageType;
}

void Message::setSystemMessageType(const QString &systemMessageType)
{
    mSystemMessageType = systemMessageType;
}

Message::MessageType Message::messageType() const
{
    return mMessageType;
}

QString Message::systemMessageText() const
{
    if (mSystemMessageType == QLatin1StringView("uj")) {
        return i18n("%1 has joined the channel", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("ul")) {
        return i18n("%1 has left the channel", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("ult")) {
        return i18n("%1 left this Team", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("room_changed_topic")) {
        if (mText.isEmpty()) {
            return i18n("Topic was cleared by: %1", mUsername);
        } else {
            return i18n("%2 changed topic to: <i>%1</i>", mText, mUsername);
        }
    } else if (mSystemMessageType == QLatin1StringView("au")) {
        return i18n("%2 added %1 to the conversation", mText, mUsername);
    } else if (mSystemMessageType == QLatin1Char('r')) {
        return i18n("%2 changed room name to <a href=\"ruqola:/room/%1\">#%1</a>", mText, mUsername);
    } else if (mSystemMessageType == QLatin1StringView("ru")) {
        return i18n("%2 removed user %1", mText, mUsername);
    } else if (mSystemMessageType == QLatin1StringView("room_changed_description")) {
        if (mText.isEmpty()) {
            return i18n("Description was cleared by %1", mUsername);
        } else {
            return i18n("%2 changed room description to %1", mText, mUsername);
        }
    } else if (mSystemMessageType == QLatin1StringView("room_changed_announcement")) {
        if (mText.isEmpty()) {
            return i18n("Announcement was cleared by %1", mUsername);
        } else {
            return i18n("%2 changed room announcement to %1", mText, mUsername);
        }
    } else if (mSystemMessageType == QLatin1StringView("room_changed_privacy")) {
        return i18n("%2 changed room privacy to %1", mText, mUsername);
    } else if (mSystemMessageType == QLatin1StringView("jitsi_call_started")) {
        return QStringLiteral("<a href=\"ruqola:/jitsicall/\">") + i18n("Click to join to video") + QStringLiteral("</a>");
    } else if (mSystemMessageType == QLatin1StringView("rm")) {
        // TODO encrypted message
        return i18n("Message Deleted");
    } else if (mSystemMessageType == QLatin1StringView("message_pinned")) {
        return i18n("Message Pinned");
    } else if (mSystemMessageType == QLatin1StringView("otr")) {
        return i18n("Encrypted Message");
    } else if (mSystemMessageType == QLatin1StringView("user-unmuted")) {
        return i18n("%1 was unmuted by %2", mText, mUsername);
    } else if (mSystemMessageType == QLatin1StringView("user-muted")) {
        return i18n("%1 was muted by %2", mText, mUsername);
    } else if (mSystemMessageType == QLatin1StringView("subscription-role-added")) {
        return i18n("Role \'%3\' was added to %1 by %2", mText, mUsername, mRole);
    } else if (mSystemMessageType == QLatin1StringView("subscription-role-removed")) {
        return i18n("Role \'%3\' was removed to %1 by %2", mText, mUsername, mRole);
    } else if (mSystemMessageType == QLatin1StringView("e2e")) {
        // TODO need to unencrypt it
        // return i18n("Encrypted message: %1", mText);
        return i18n("This message is end-to-end encrypted. To view it, you must enter your encryption key in your account settings.");
    } else if (mSystemMessageType == QLatin1StringView("discussion-created")) {
        return i18n("Discussion created about \"%1\"", mText);
    } else if (mSystemMessageType == QLatin1StringView("ut")) {
        return i18n("%1 has joined the conversation", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("room-archived")) {
        return i18n("This room has been archived by %1", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("room-unarchived")) {
        return i18n("This room has been unarchived by %1", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("rtc")) {
        qCWarning(RUQOLA_LOG) << "Need to implement : " << mSystemMessageType << " mText " << mText;
        return i18n("Unknown action!");
    } else if (mSystemMessageType == QLatin1StringView("wm")) {
        // TODO verify
        qCWarning(RUQOLA_LOG) << "Need to implement : " << mSystemMessageType << " mText " << mText;
        return i18n("Welcome %1!", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("room_changed_avatar")) {
        return i18n("Room avatar changed by %1", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("room_e2e_enabled")) {
        return i18n("This room's encryption has been enabled by %1", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("room_e2e_disabled")) {
        return i18n("This room's encryption has been disabled by %1", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("room-set-read-only")) {
        return i18n("Room set as Read Only by  %1", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("room-removed-read-only")) {
        return i18n("Room added writing permission by %1", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("added-user-to-team")) {
        return i18n("%1 added @%2 to this Team", mUsername, mText);
    } else if (mSystemMessageType == QLatin1StringView("removed-user-from-team")) {
        return i18n("%1 removed @%2 from this Team", mUsername, mText);
    } else if (mSystemMessageType == QLatin1StringView("user-converted-to-team")) {
        return i18n("%1 converted #%2 to a Team", mUsername, mText);
    } else if (mSystemMessageType == QLatin1StringView("user-converted-to-channel")) {
        return i18n("%1 converted #%2 to a Channel", mUsername, mText);
    } else if (mSystemMessageType == QLatin1StringView("user-removed-room-from-team")) {
        return i18n("%1 removed #%2 from this Team", mUsername, mText);
    } else if (mSystemMessageType == QLatin1StringView("user-deleted-room-from-team")) {
        return i18n("%1 deleted #%2", mUsername, mText);
    } else if (mSystemMessageType == QLatin1StringView("user-added-room-to-team")) {
        return i18n("%1 added #%2 to this Team", mUsername, mText);
    } else if (mSystemMessageType == QLatin1StringView("room-allowed-reacting")) {
        return i18n("Room allowed reacting by %1", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("room-disallowed-reacting")) {
        return i18n("Room disallowed reacting by %1", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("ujt")) {
        return i18n("%1 joined this Team", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("user_joined_otr")) {
        return i18n("%1 has joined OTR chat.", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("user_key_refreshed_successfully")) {
        return i18n("%1 key refreshed successfully", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("user_requested_otr_key_refresh")) {
        return i18n("%1 has requested key refresh.", mUsername);
    } else if (mSystemMessageType == QLatin1StringView("room_changed_topic")) {
        return i18n("Room topic changed to: %1 by %2", mUsername, mText); // TODO verify
    } else if (mSystemMessageType == QLatin1StringView("videoconf")) {
        return i18n("Conference Call");
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown type for message: type: " << mSystemMessageType << " mText " << mText;
        return i18n("Unknown action!");
    }
}

void Message::setMessageType(MessageType messageType)
{
    mMessageType = messageType;
}

QList<MessageAttachment> Message::attachments() const
{
    return mAttachments;
}

void Message::setAttachments(const QList<MessageAttachment> &attachments)
{
    mAttachments = attachments;
}

QList<MessageUrl> Message::urls() const
{
    return mUrls;
}

void Message::setUrls(const QList<MessageUrl> &urls)
{
    mUrls = urls;
}

QString Message::alias() const
{
    return mAlias;
}

void Message::setAlias(const QString &alias)
{
    mAlias = alias;
}

QString Message::editedByUserId() const
{
    return mEditedByUserId;
}

void Message::setEditedByUserId(const QString &editedByUserId)
{
    mEditedByUserId = editedByUserId;
}

QString Message::editedByUsername() const
{
    return mEditedByUsername;
}

void Message::setEditedByUsername(const QString &editedByUsername)
{
    mEditedByUsername = editedByUsername;
}

bool Message::wasEdited() const
{
    return !mEditedByUsername.isEmpty();
}

qint64 Message::editedAt() const
{
    return mEditedAt;
}

void Message::setEditedAt(qint64 editedAt)
{
    if (mEditedAt != editedAt) {
        mEditedAt = editedAt;
        QLocale l;
        mEditedDisplayTime = l.toString(QDateTime::fromMSecsSinceEpoch(mEditedAt), QLocale::LongFormat);
    }
}

qint64 Message::updatedAt() const
{
    return mUpdatedAt;
}

void Message::setUpdatedAt(qint64 updatedAt)
{
    mUpdatedAt = updatedAt;
}

QString Message::userId() const
{
    return mUserId;
}

void Message::setUserId(const QString &userId)
{
    mUserId = userId;
}

QString Message::username() const
{
    return mUsername;
}

void Message::setUsername(const QString &username)
{
    mUsername = username;
}

qint64 Message::timeStamp() const
{
    return mTimeStamp;
}

void Message::setTimeStamp(qint64 timeStamp)
{
    if (mTimeStamp != timeStamp) {
        mTimeStamp = timeStamp;
        mDisplayTime = QDateTime::fromMSecsSinceEpoch(mTimeStamp).time().toString(QStringLiteral("hh:mm"));
    }
}

QString Message::text() const
{
    return mText;
}

void Message::setText(const QString &text)
{
    mText = text;
}

QString Message::messageId() const
{
    return mMessageId;
}

void Message::setMessageId(const QString &messageId)
{
    mMessageId = messageId;
}

QString Message::roomId() const
{
    return mRoomId;
}

void Message::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString Message::avatar() const
{
    return mAvatar;
}

void Message::setAvatar(const QString &avatar)
{
    mAvatar = avatar;
}

bool Message::parseUrls() const
{
    return mParseUrls;
}

void Message::setParseUrls(bool parseUrls)
{
    mParseUrls = parseUrls;
}

bool Message::groupable() const
{
    return mGroupable;
}

void Message::setGroupable(bool groupable)
{
    mGroupable = groupable;
}

QString Message::generateUniqueId(const QString &messageId, int index)
{
    return QStringLiteral("%1_%2").arg(messageId, QString::number(index));
}

Utils::AvatarInfo Message::avatarInfo() const
{
    Utils::AvatarInfo info; // Optimize ???
    info.avatarType = Utils::AvatarType::User;
    info.identifier = mUsername;
    return info;
}

Message Message::deserialize(const QJsonObject &o, EmojiManager *emojiManager)
{
    Message message(emojiManager);
    message.mThreadCount = o[QLatin1StringView("tcount")].toInt();
    message.mDiscussionCount = o[QLatin1StringView("dcount")].toInt();
    message.mDiscussionRoomId = o[QLatin1StringView("drid")].toString();
    message.mThreadMessageId = o[QLatin1StringView("tmid")].toString();

    if (o.contains(QLatin1StringView("tlm"))) {
        message.mThreadLastMessage = static_cast<qint64>(o[QLatin1StringView("tlm")].toDouble());
    }
    if (o.contains(QLatin1StringView("dlm"))) {
        message.mDiscussionLastMessage = static_cast<qint64>(o[QLatin1StringView("dlm")].toDouble());
    }

    message.mMessageId = o[QLatin1StringView("messageID")].toString();
    message.mRoomId = o[QLatin1StringView("roomID")].toString();
    message.mText = o[QLatin1StringView("message")].toString();
    message.setTimeStamp(static_cast<qint64>(o[QLatin1StringView("timestamp")].toDouble()));
    message.mUsername = o[QLatin1StringView("username")].toString();
    message.mName = o[QLatin1StringView("name")].toString();
    message.mUserId = o[QLatin1StringView("userID")].toString();
    message.mUpdatedAt = static_cast<qint64>(o[QLatin1StringView("updatedAt")].toDouble());
    message.setEditedAt(static_cast<qint64>(o[QLatin1StringView("editedAt")].toDouble()));
    message.mEditedByUsername = o[QLatin1StringView("editedByUsername")].toString();
    message.mEditedByUserId = o[QLatin1StringView("editedByUserID")].toString();
    message.mAlias = o[QLatin1StringView("alias")].toString();
    message.mAvatar = o[QLatin1StringView("avatar")].toString();
    message.mGroupable = o[QLatin1StringView("groupable")].toBool();
    message.mParseUrls = o[QLatin1StringView("parseUrls")].toBool();
    message.mMessageStarred.setIsStarred(o[QLatin1StringView("starred")].toBool());

    message.mMessagePinned = MessagePinned::deserialize(o[QLatin1StringView("pinnedMessage")].toObject());
    message.mRole = o[QLatin1StringView("role")].toString();
    message.mSystemMessageType = o[QLatin1StringView("type")].toString();
    message.mEmoji = o[QLatin1StringView("emoji")].toString();
    message.mMessageType = o[QLatin1StringView("messageType")].toVariant().value<MessageType>();
    const QJsonArray attachmentsArray = o.value(QLatin1StringView("attachments")).toArray();
    for (int i = 0; i < attachmentsArray.count(); ++i) {
        const QJsonObject attachment = attachmentsArray.at(i).toObject();
        MessageAttachment att = MessageAttachment::deserialize(attachment);
        att.setAttachmentId(Message::generateUniqueId(message.messageId(), i));
        if (att.isValid()) {
            message.mAttachments.append(std::move(att));
        }
    }
    const QJsonArray urlsArray = o.value(QLatin1StringView("urls")).toArray();
    for (int i = 0; i < urlsArray.count(); ++i) {
        const QJsonObject urlObj = urlsArray.at(i).toObject();
        MessageUrl url = MessageUrl::deserialize(urlObj);
        url.setUrlId(Message::generateUniqueId(message.messageId(), i));
        if (!url.isEmpty()) {
            message.mUrls.append(std::move(url));
        }
    }
    const QJsonObject reactionsArray = o.value(QLatin1StringView("reactions")).toObject();
    message.setReactions(Reactions::deserialize(reactionsArray, emojiManager));

    const QJsonArray repliesArray = o.value(QLatin1StringView("replies")).toArray();
    QStringList replies;
    replies.reserve(repliesArray.count());
    for (int i = 0, total = repliesArray.count(); i < total; ++i) {
        replies.append(repliesArray.at(i).toString());
    }
    message.setReplies(replies);

    QMap<QString, QString> mentions;
    const QJsonArray mentionsArray = o.value(QLatin1StringView("mentions")).toArray();
    for (int i = 0, total = mentionsArray.count(); i < total; ++i) {
        const QJsonObject mention = mentionsArray.at(i).toObject();
        mentions.insert(mention.value(QLatin1StringView("username")).toString(), mention.value(QLatin1StringView("_id")).toString());
    }
    message.setMentions(mentions);

    QMap<QString, QString> channels;
    const QJsonArray channelsArray = o.value(QLatin1StringView("channels")).toArray();
    for (int i = 0, total = channelsArray.count(); i < total; ++i) {
        const QJsonObject channel = channelsArray.at(i).toObject();
        channels.insert(channel.value(QLatin1StringView("channel")).toString(), channel.value(QLatin1StringView("_id")).toString());
    }
    message.setChannels(channels);

    const QJsonArray blocksArray = o.value(QLatin1StringView("blocks")).toArray();
    for (int i = 0, total = blocksArray.count(); i < total; ++i) {
        const Block block = Block::deserialize(blocksArray.at(i).toObject());
        message.mBlocks.append(std::move(block));
    }

    message.mLocalTranslation = o[QLatin1StringView("localTransation")].toString();

    message.mMessageTranslation = MessageTranslation::deserialize(o[QLatin1StringView("messageTranslation")].toArray());

    return message;
}

QByteArray Message::serialize(const Message &message, bool toBinary)
{
    QJsonDocument d;
    QJsonObject o;

    o[QLatin1StringView("messageID")] = message.mMessageId;
    o[QLatin1StringView("roomID")] = message.mRoomId;
    o[QLatin1StringView("message")] = message.mText;
    o[QLatin1StringView("timestamp")] = message.mTimeStamp;
    o[QLatin1StringView("username")] = message.mUsername;
    if (!message.mName.isEmpty()) {
        o[QLatin1StringView("name")] = message.mName;
    }
    o[QLatin1StringView("userID")] = message.mUserId;
    o[QLatin1StringView("updatedAt")] = message.mUpdatedAt;
    o[QLatin1StringView("editedAt")] = message.mEditedAt;
    if (message.mThreadLastMessage > -1) {
        o[QLatin1StringView("tlm")] = message.mThreadLastMessage;
    }

    o[QLatin1StringView("editedByUsername")] = message.mEditedByUsername;
    o[QLatin1StringView("editedByUserID")] = message.mEditedByUserId;
    o[QLatin1StringView("alias")] = message.mAlias;
    o[QLatin1StringView("avatar")] = message.mAvatar;
    o[QLatin1StringView("groupable")] = message.mGroupable;
    o[QLatin1StringView("parseUrls")] = message.mParseUrls;
    o[QLatin1StringView("starred")] = message.mMessageStarred.isStarred();

    o[QLatin1StringView("pinnedMessage")] = MessagePinned::serialize(message.mMessagePinned);

    if (!message.mRole.isEmpty()) {
        o[QLatin1StringView("role")] = message.mRole;
    }
    if (!message.mEmoji.isEmpty()) {
        o[QLatin1StringView("emoji")] = message.mEmoji;
    }

    o[QLatin1StringView("type")] = message.mSystemMessageType;
    o[QLatin1StringView("messageType")] = QJsonValue::fromVariant(QVariant::fromValue<Message::MessageType>(message.mMessageType));

    // Attachments
    if (!message.mAttachments.isEmpty()) {
        QJsonArray array;
        const int nbAttachment = message.mAttachments.count();
        for (int i = 0; i < nbAttachment; ++i) {
            array.append(MessageAttachment::serialize(message.mAttachments.at(i)));
        }
        o[QLatin1StringView("attachments")] = array;
    }

    // Mentions
    if (!message.mentions().isEmpty()) {
        QMapIterator<QString, QString> i(message.mentions());
        QJsonArray array;
        while (i.hasNext()) {
            i.next();
            QJsonObject mention;
            mention.insert(QLatin1StringView("_id"), i.value());
            mention.insert(QLatin1StringView("username"), i.key());
            array.append(std::move(mention));
        }
        o[QLatin1StringView("mentions")] = array;
    }

    // Channels
    if (!message.channels().isEmpty()) {
        QMapIterator<QString, QString> j(message.channels());
        QJsonArray array;
        while (j.hasNext()) {
            j.next();
            QJsonObject channel;
            channel.insert(QLatin1StringView("_id"), j.value());
            channel.insert(QLatin1StringView("channel"), j.key());
            array.append(std::move(channel));
        }
        o[QLatin1StringView("channels")] = array;
    }
    // Urls
    if (!message.mUrls.isEmpty()) {
        QJsonArray array;
        const int nbUrls = message.mUrls.count();
        for (int i = 0; i < nbUrls; ++i) {
            array.append(MessageUrl::serialize(message.mUrls.at(i)));
        }
        o[QLatin1StringView("urls")] = array;
    }

    if (!message.reactions().isEmpty()) {
        o[QLatin1StringView("reactions")] = Reactions::serialize(message.reactions());
    }

    if (message.mThreadCount > 0) {
        o[QLatin1StringView("tcount")] = message.mThreadCount;
        o[QLatin1StringView("tlm")] = message.mThreadLastMessage;
    }

    if (message.mDiscussionCount > 0) {
        o[QLatin1StringView("dcount")] = message.mDiscussionCount;
        o[QLatin1StringView("dlm")] = message.mDiscussionLastMessage;
    }
    if (!message.mDiscussionRoomId.isEmpty()) {
        o[QLatin1StringView("drid")] = message.mDiscussionRoomId;
    }

    if (!message.mThreadMessageId.isEmpty()) {
        o[QLatin1StringView("tmid")] = message.mThreadMessageId;
    }
    if (!message.mReplies.isEmpty()) {
        o[QLatin1StringView("replies")] = QJsonArray::fromStringList(message.mReplies);
    }

    if (!message.mBlocks.isEmpty()) {
        QJsonArray blockArray;
        const int nBlocks = message.mBlocks.count();
        for (int i = 0; i < nBlocks; ++i) {
            blockArray.append(Block::serialize(message.mBlocks.at(i)));
        }
        o[QLatin1StringView("blocks")] = blockArray;
    }
    if (!message.mLocalTranslation.isEmpty()) {
        o[QLatin1StringView("localTransation")] = message.mLocalTranslation;
    }
    if (!message.mMessageTranslation.isEmpty()) {
        o[QLatin1StringView("messageTranslation")] = MessageTranslation::serialize(message.mMessageTranslation);
    }

    if (toBinary) {
        return QCborValue::fromJsonValue(o).toCbor();
    }

    d.setObject(o);
    return d.toJson(QJsonDocument::Indented);
}

QDebug operator<<(QDebug d, const Message &t)
{
    d.space() << "mMessageId:" << t.messageId();
    d.space() << "mText:" << t.text();
    d.space() << "mTimeStamp:" << t.timeStamp();
    d.space() << "mUsername:" << t.username();
    d.space() << "mName:" << t.name();
    d.space() << "mUserId:" << t.userId();
    d.space() << "mUpdatedAt:" << t.updatedAt();
    d.space() << "mEditedAt:" << t.editedAt();
    d.space() << "mEditedByUsername:" << t.editedByUsername();
    d.space() << "mEditedByUserId:" << t.editedByUserId();
    d.space() << "mAlias:" << t.alias();
    d.space() << "mSystemMessageType:" << t.systemMessageType();
    d.space() << "mRoomId:" << t.roomId();
    d.space() << "mAvatar:" << t.avatar();
    d.space() << "mGroupable:" << t.groupable();
    d.space() << "mParseUrls:" << t.parseUrls();
    for (int i = 0, total = t.attachments().count(); i < total; ++i) {
        d.space() << "Attachment:" << t.attachments().at(i);
    }
    for (int i = 0, total = t.urls().count(); i < total; ++i) {
        d.space() << "Urls:" << t.urls().at(i);
    }
    d.space() << "Mentions:" << t.mentions();
    d.space() << "mMessageType:" << t.messageType();
    d.space() << "mRole:" << t.role();
    d.space() << "mReaction:" << t.reactions();
    d.space() << "mUnread:" << t.unread();
    d.space() << "starred" << t.messageStarred();
    d.space() << "pinned" << t.messagePinned();
    d.space() << "threadcount" << t.threadCount();
    d.space() << "threadlastmessage" << t.threadLastMessage();
    d.space() << "discussioncount" << t.discussionCount();
    d.space() << "discussionlastmessage" << t.discussionLastMessage();
    d.space() << "discussionRoomId" << t.discussionRoomId();
    d.space() << "threadMessageId" << t.threadMessageId();
    d.space() << "messagetranslation" << t.messageTranslation();
    d.space() << "mShowOriginalMessage" << t.showTranslatedMessage();
    d.space() << "mReplies" << t.replies();
    d.space() << "mEmoji" << t.emoji();
    d.space() << "mPendingMessage" << t.pendingMessage();
    d.space() << "mShowIgnoredMessage" << t.showIgnoredMessage();
    d.space() << "mChannels" << t.channels();
    d.space() << "mLocalTranslation" << t.localTranslation();
    d.space() << "mDiscussionCount" << t.discussionCount();
    d.space() << "mDiscussionLastMessage" << t.discussionLastMessage();

    for (int i = 0, total = t.blocks().count(); i < total; ++i) {
        d.space() << "block:" << t.blocks().at(i);
    }

    return d;
}

QString Message::dateTime() const
{
    const QDateTime currentDate = QDateTime::fromMSecsSinceEpoch(timeStamp());
    return currentDate.toString();
}

#include "moc_message.cpp"
