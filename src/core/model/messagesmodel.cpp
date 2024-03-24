/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include <QModelIndex>
#include <QTimeZone>

#include "emoticons/emojimanager.h"
#include "loadrecenthistorymanager.h"
#include "messagesmodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include "ruqola_debug.h"
#include "textconverter.h"
#include "utils.h"

#include <KLocalizedString>

MessagesModel::MessagesModel(const QByteArray &roomID, RocketChatAccount *account, Room *room, QObject *parent)
    : QAbstractListModel(parent)
    , mRoomId(roomID)
    , mRocketChatAccount(account)
    , mRoom(room)
    , mLoadRecentHistoryManager(new LoadRecentHistoryManager)
{
    qCDebug(RUQOLA_LOG) << "Creating message Model";
    if (mRoom) {
        connect(mRoom, &Room::rolesChanged, this, &MessagesModel::refresh);
        connect(mRoom, &Room::ignoredUsersChanged, this, &MessagesModel::refresh);
        connect(mRoom, &Room::highlightsWordChanged, this, &MessagesModel::refresh);
    }
}

MessagesModel::~MessagesModel() = default;

void MessagesModel::activate()
{
    if (mRocketChatAccount) {
        connect(mRocketChatAccount, &RocketChatAccount::fileDownloaded, this, &MessagesModel::slotFileDownloaded);
    }
}

void MessagesModel::deactivate()
{
    if (mRocketChatAccount) {
        disconnect(mRocketChatAccount, &RocketChatAccount::fileDownloaded, this, &MessagesModel::slotFileDownloaded);
    }
}

Message MessagesModel::findLastMessageBefore(const QByteArray &messageId, const std::function<bool(const Message &)> &predicate) const
{
    auto it = findMessage(messageId); // if it == end, we'll start from there
    auto rit = QList<Message>::const_reverse_iterator(it); // this points to *it-1 already
    rit = std::find_if(rit, mAllMessages.rend(), predicate);
    return rit == mAllMessages.rend() ? Message() : *rit;
}

Message MessagesModel::findNextMessageAfter(const QByteArray &messageId, const std::function<bool(const Message &)> &predicate) const
{
    auto it = findMessage(messageId);
    if (it == mAllMessages.end()) {
        return Message(); // no wrap around, otherwise Alt+Key_Up would edit the oldest msg right away
    } else {
        ++it;
    }
    it = std::find_if(it, mAllMessages.end(), predicate);
    return it == mAllMessages.end() ? Message() : *it;
}

Message MessagesModel::findMessageById(const QByteArray &messageId) const
{
    auto it = findMessage(messageId);
    return it == mAllMessages.end() ? Message() : *it;
}

QModelIndex MessagesModel::indexForMessage(const QByteArray &messageId) const
{
    auto it = findMessage(messageId);
    if (it == mAllMessages.end()) {
        return {};
    }
    const QModelIndex idx = createIndex(std::distance(mAllMessages.begin(), it), 0);
    return idx;
}

QByteArray MessagesModel::messageIdFromIndex(int rowIndex)
{
    if (rowIndex >= 0 && rowIndex < mAllMessages.count()) {
        return mAllMessages.at(rowIndex).messageId();
    }
    return {};
}

void MessagesModel::refresh()
{
    beginResetModel();
    endResetModel();
}

qint64 MessagesModel::lastTimestamp() const
{
    if (!mAllMessages.isEmpty()) {
        // qCDebug(RUQOLA_LOG) << "returning timestamp" << mAllMessages.last().timeStamp();
        return mAllMessages.first().timeStamp();
    } else {
        return 0;
    }
}

int MessagesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mAllMessages.size();
}

static bool compareTimeStamps(const Message &lhs, const Message &rhs)
{
    return lhs.timeStamp() < rhs.timeStamp();
}

void MessagesModel::addMessage(const Message &message)
{
    auto it = std::upper_bound(mAllMessages.begin(), mAllMessages.end(), message, compareTimeStamps);

    auto emitChanged = [this](int rowNumber, const QList<int> &roles = QList<int>()) {
        const QModelIndex index = createIndex(rowNumber, 0);
        Q_EMIT dataChanged(index, index, roles);
    };

    // When we have 1 element.
    if (mAllMessages.count() == 1 && (*mAllMessages.begin()).messageId() == message.messageId()) {
        (*mAllMessages.begin()) = message;
        qCDebug(RUQOLA_LOG) << "Update first message";
        emitChanged(0);
    } else if (((it) != mAllMessages.begin() && (*(it - 1)).messageId() == message.messageId())) {
        qCDebug(RUQOLA_LOG) << "Update message";
        if (message.pendingMessage()) {
            // If we already have a message and we must add pending message it's that server
            // send quickly new message => replace not it by a pending message
            return;
        }
        (*(it - 1)) = message;
        emitChanged(std::distance(mAllMessages.begin(), it - 1), {OriginalMessageOrAttachmentDescription});
    } else {
        const int pos = it - mAllMessages.begin();
        beginInsertRows(QModelIndex(), pos, pos);
        mAllMessages.insert(it, message);
        endInsertRows();
    }
}

void MessagesModel::addMessages(const QList<Message> &messages, bool insertListMessages)
{
    if (messages.isEmpty()) {
        return;
    }
    if (mAllMessages.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, messages.count() - 1);
        mAllMessages = messages;
        std::sort(mAllMessages.begin(), mAllMessages.end(), compareTimeStamps);
        endInsertRows();
    } else if (insertListMessages) {
        beginResetModel();
        mAllMessages += messages;
        std::sort(mAllMessages.begin(), mAllMessages.end(), compareTimeStamps);
        endResetModel();
    } else {
        // TODO optimize this case as well?
        for (const Message &message : messages) {
            addMessage(message);
        }
    }
}

QVariant MessagesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        qCWarning(RUQOLA_LOG) << "ERROR: invalid index";
        return {};
    }
    const int idx = index.row();
    const Message &message = mAllMessages.at(idx);

    switch (role) {
    case MessagesModel::MessagePointer:
        return QVariant::fromValue(&message);
    case MessagesModel::Username:
        return message.username();
    case Qt::AccessibleTextRole:
    case MessagesModel::OriginalMessage:
        return message.text();
    case MessagesModel::DateTimeUtc:
        return QDateTime::fromMSecsSinceEpoch(message.timeStamp(), Qt::UTC).toString(Qt::ISODateWithMs);
    case MessagesModel::MessageConvertedText:
        return convertedText(message, mSearchText);
    case MessagesModel::Timestamp:
        return message.displayTime();
    case MessagesModel::UserId:
        return message.userId();
    case MessagesModel::SystemMessageType:
        return QVariant::fromValue<SystemMessageTypeUtil::SystemMessageType>(message.systemMessageType());
    case MessagesModel::MessageId:
        return message.messageId();
    case MessagesModel::Alias:
        return message.alias();
    case MessagesModel::MessageType:
        return message.messageType();
    case MessagesModel::Avatar:
        return message.avatar();
    case MessagesModel::EditedAt:
        return message.editedAt();
    case MessagesModel::EditedToolTip: {
        QLocale l;
        const QString editedDisplayTime = l.toString(QDateTime::fromMSecsSinceEpoch(message.editedAt()), QLocale::LongFormat);
        return i18n("Edited at %1 by %2", editedDisplayTime, message.editedByUsername());
    }
    case MessagesModel::Attachments: {
        QVariantList lst;
        lst.reserve(message.attachments().count());
        const auto attaches = message.attachments();
        for (const MessageAttachment &att : attaches) {
            lst.append(QVariant::fromValue(att));
        }
        return lst;
    }
    case MessagesModel::Urls: {
        QVariantList lst;
        lst.reserve(message.urls().count());
        const auto urls = message.urls();
        for (const MessageUrl &url : urls) {
            lst.append(QVariant::fromValue(url));
        }
        return lst;
    }
    case MessagesModel::Date: {
        const QDateTime currentDate = QDateTime::fromMSecsSinceEpoch(message.timeStamp());
        return QLocale().toString(currentDate.date());
    }
    case MessagesModel::DateDiffersFromPrevious:
        if (idx > 0) {
            const QDateTime currentDate = QDateTime::fromMSecsSinceEpoch(message.timeStamp(), QTimeZone::utc());
            const Message &previousMessage = mAllMessages.at(idx - 1);
            const QDateTime previousDate = QDateTime::fromMSecsSinceEpoch(previousMessage.timeStamp(), QTimeZone::utc());
            return currentDate.date() != previousDate.date();
        }
        return true; // show date at the top
    case MessagesModel::CanEditMessage:
        return mRocketChatAccount->isMessageEditable(message); // && mRoom && mRoom->hasPermission(QStringLiteral("edit-message"));
    case MessagesModel::CanDeleteMessage:
        return mRocketChatAccount->isMessageDeletable(message);
    case MessagesModel::Starred:
        return message.isStarred();
    case MessagesModel::UsernameUrl: {
        const QString username = message.username();
        if (username.isEmpty()) {
            return {};
        }
        return QStringLiteral("<a href=\'ruqola:/user/%1\'>@%1</a>").arg(message.username());
    }
    case MessagesModel::Roles: {
        const QString str = roomRoles(message.userId()).join(QLatin1Char(','));
        return str;
    }
    case MessagesModel::Reactions: {
        QVariantList lst;
        const auto reactions = message.reactions().reactions();
        lst.reserve(reactions.count());
        for (const Reaction &react : reactions) {
            // Convert reactions
            lst.append(QVariant::fromValue(react));
        }
        return lst;
    }
    case MessagesModel::Ignored:
        return mRoom && mRoom->userIsIgnored(message.userId());
    case MessagesModel::DirectChannels:
        return mRoom && (mRoom->channelType() == Room::RoomType::Direct);
    case MessagesModel::Pinned:
        return message.messagePinned().pinned();
    case MessagesModel::DiscussionCount:
        return message.discussionCount();
    case MessagesModel::DiscussionRoomId:
        return message.discussionRoomId();
    case MessagesModel::DiscussionLastMessage:
        return message.discussionLastMessage();
    case MessagesModel::ThreadCount:
        return message.threadCount();
    case MessagesModel::ThreadLastMessage:
        return message.threadLastMessage();
    case MessagesModel::ThreadMessageId:
        return message.threadMessageId();
    case MessagesModel::ThreadMessagePreview:
        return threadMessagePreview(message.threadMessageId());
    case MessagesModel::ThreadMessageFollowed:
        return threadMessageFollowed(message.threadMessageId());
    case MessagesModel::ThreadMessage: {
        const Message tm = threadMessage(message.threadMessageId());
        return QVariant::fromValue(tm);
    }
    case MessagesModel::Groupable:
        return message.groupable();
    case MessagesModel::ShowTranslatedMessage:
        return message.showTranslatedMessage();
    case MessagesModel::DisplayAttachment:
        return {}; // Unused.
    case MessagesModel::DisplayUrlPreview:
        return {}; // Unused.
    case MessagesModel::DisplayLastSeenMessage:
        if (idx > 0) {
            if (mRoom) {
                const QDateTime currentDate = QDateTime::fromMSecsSinceEpoch(message.timeStamp(), QTimeZone::utc());
                const QDateTime lastSeenDate = QDateTime::fromMSecsSinceEpoch(mRoom->lastSeenAt(), QTimeZone::utc());
                // qDebug() << " lastSeeDate" << lastSeeDate;
                if (currentDate > lastSeenDate) {
                    const Message &previousMessage = mAllMessages.at(idx - 1);
                    const QDateTime previewMessageDate = QDateTime::fromMSecsSinceEpoch(previousMessage.timeStamp(), QTimeZone::utc());
                    const bool result = (previewMessageDate <= lastSeenDate);
                    return result;
                }
            }
        }
        return false;
    case MessagesModel::Emoji:
        return message.emoji();
    case MessagesModel::AvatarInfo:
        return QVariant::fromValue(message.avatarInfo());
    case MessagesModel::PendingMessage:
        return message.pendingMessage();
    case MessagesModel::ShowIgnoredMessage:
        return message.showIgnoredMessage();
    case MessagesModel::MessageInEditMode:
        return message.isEditingMode();
    case MessagesModel::HoverHighLight:
        return message.hoverHighlight();
    case MessagesModel::LocalTranslation:
        return message.localTranslation();
    case MessagesModel::OriginalMessageOrAttachmentDescription:
        return message.originalMessageOrAttachmentDescription();
    case MessagesModel::PrivateMessage:
        return message.privateMessage();
    }

    return {};
}

QString MessagesModel::convertedText(const Message &message, const QString &searchedText) const
{
    if (message.privateMessage()) {
        return i18n("Only you can see this message");
    } else if (message.messageType() == Message::System) {
        return message.systemMessageText();
    } else {
        QStringList highlightWords;
        if (mRoom) {
            if (mRoom->channelType() != Room::RoomType::Direct) { // We can't ignore message but we can block user in direct message
                if (mRoom->userIsIgnored(message.userId()) && !message.showIgnoredMessage()) {
                    return QString(QStringLiteral("<i>") + i18n("Ignored Message") + QStringLiteral("</i>"));
                }
            }
            highlightWords = mRoom->highlightsWord();
        }
        const QString userName = mRocketChatAccount ? mRocketChatAccount->userName() : QString();
        const QStringList highlightWordsLst = mRocketChatAccount ? mRocketChatAccount->highlightWords() : highlightWords;
        return convertMessageText(message, userName, highlightWordsLst, searchedText);
    }
}

bool MessagesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        qCWarning(RUQOLA_LOG) << "ERROR: invalid index";
        return false;
    }
    const int idx = index.row();
    Message &message = mAllMessages[idx];

    switch (role) {
    case MessagesModel::DisplayAttachment: {
        const auto visibility = value.value<AttachmentAndUrlPreviewVisibility>();
        auto attachments = message.attachments();
        for (int i = 0, total = attachments.count(); i < total; ++i) {
            const MessageAttachment att = attachments.at(i);
            if (att.attachmentId() == visibility.elementId) {
                MessageAttachment changeAttachment = attachments.takeAt(i);
                changeAttachment.setShowAttachment(visibility.show);
                attachments.insert(i, changeAttachment);
                break;
            }
        }
        message.setAttachments(attachments);
        Q_EMIT dataChanged(index, index, {MessagesModel::DisplayAttachment});
        return true;
    }
    case MessagesModel::DisplayUrlPreview: {
        const auto visibility = value.value<AttachmentAndUrlPreviewVisibility>();
        auto urls = message.urls();
        for (int i = 0, total = urls.count(); i < total; ++i) {
            const MessageUrl att = urls.at(i);
            if (att.urlId() == visibility.elementId) {
                MessageUrl changeUrlPreview = urls.takeAt(i);
                changeUrlPreview.setShowPreview(visibility.show);
                urls.insert(i, changeUrlPreview);
                break;
            }
        }
        message.setUrls(urls);
        Q_EMIT dataChanged(index, index, {MessagesModel::DisplayUrlPreview});
        return true;
    }
    case MessagesModel::ShowTranslatedMessage:
        message.setShowTranslatedMessage(value.toBool());
        Q_EMIT dataChanged(index, index, {MessagesModel::ShowTranslatedMessage});
        return true;
    case MessagesModel::ShowIgnoredMessage:
        message.setShowIgnoredMessage(value.toBool());
        Q_EMIT dataChanged(index, index, {MessagesModel::ShowIgnoredMessage});
        return true;
    case MessagesModel::MessageInEditMode:
        message.setIsEditingMode(value.toBool());
        Q_EMIT dataChanged(index, index, {MessagesModel::MessageInEditMode});
        return true;
    case MessagesModel::HoverHighLight:
        message.setHoverHighlight(value.toBool());
        Q_EMIT dataChanged(index, index, {MessagesModel::HoverHighLight});
        return true;
    case MessagesModel::LocalTranslation:
        message.setLocalTranslation(value.toString());
        Q_EMIT dataChanged(index, index, {MessagesModel::LocalTranslation});
        return true;
    }
    return false;
}

QStringList MessagesModel::roomRoles(const QByteArray &userId) const
{
    if (mRoom) {
        return mRoom->rolesForUserId(userId);
    }
    return {};
}

QString MessagesModel::convertMessageText(const Message &message, const QString &userName, const QStringList &highlightWords, const QString &searchedText) const
{
    QString messageStr = message.text();
    EmojiManager *emojiManager = nullptr;
    MessageCache *messageCache = nullptr;
    int maximumRecursiveQuotedText = -1;
    if (mRocketChatAccount) {
        emojiManager = mRocketChatAccount->emojiManager();
        messageCache = mRocketChatAccount->messageCache();
        maximumRecursiveQuotedText = mRocketChatAccount->ruqolaServerConfig()->messageQuoteChainLimit();
        if (mRocketChatAccount->hasAutotranslateSupport()) {
            if (message.showTranslatedMessage()) {
                if (mRoom && mRoom->autoTranslate() && !mRoom->autoTranslateLanguage().isEmpty()) {
                    QString messageTranslation;
                    if (message.messageTranslation()) {
                        messageTranslation = message.messageTranslation()->translatedStringFromLanguage(mRoom->autoTranslateLanguage());
                    }
                    if (!messageTranslation.isEmpty()) {
                        messageStr = messageTranslation;
                    } else if (!message.localTranslation().isEmpty()) {
                        messageStr = message.localTranslation();
                    }
                }
            }
        } else if (message.showTranslatedMessage() && !message.localTranslation().isEmpty()) {
            messageStr = message.localTranslation();
        }
    }

    QByteArray needUpdateMessageId;
    const TextConverter::ConvertMessageTextSettings settings(messageStr,
                                                             userName,
                                                             mAllMessages,
                                                             highlightWords,
                                                             emojiManager,
                                                             messageCache,
                                                             message.mentions(),
                                                             message.channels(),
                                                             searchedText,
                                                             maximumRecursiveQuotedText);

    int recursiveIndex = 0;
    return TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex);
}

void MessagesModel::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

bool MessagesModel::isEmpty() const
{
    return mAllMessages.isEmpty();
}

void MessagesModel::clear()
{
    mSearchText.clear();
    if (rowCount() != 0) {
        beginResetModel();
        mAllMessages.clear();
        endResetModel();
    }
}

void MessagesModel::changeShowOriginalMessage(const QByteArray &messageId, bool showOriginal)
{
    Q_UNUSED(showOriginal)
    auto it = findMessage(messageId);
    if (it != mAllMessages.end()) {
        // TODO implement it
    }
}

void MessagesModel::slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl)
{
    auto matchesFilePath = [&](const QList<MessageAttachment> &msgAttachments) {
        return std::find_if(msgAttachments.begin(),
                            msgAttachments.end(),
                            [&](const MessageAttachment &attach) {
                                // Transform link() the way RocketChatCache::downloadFile does it
                                return mRocketChatAccount->urlForLink(attach.imageUrlPreview()).path() == filePath;
                            })
            != msgAttachments.end();
    };
    auto it = std::find_if(mAllMessages.begin(), mAllMessages.end(), [&](const Message &msg) {
        if (!msg.attachments().isEmpty()) {
            return matchesFilePath(msg.attachments());
        }
        auto *emojiManager = mRocketChatAccount->emojiManager();
        const auto reactions = msg.reactions().reactions();
        for (const Reaction &reaction : reactions) {
            const QString fileName = emojiManager->customEmojiFileName(reaction.reactionName());
            if (!fileName.isEmpty() && mRocketChatAccount->urlForLink(fileName).path() == filePath) {
                return true;
            }
        }
        const Utils::AvatarInfo info = msg.avatarInfo();
        const QUrl iconUrlStr = QUrl(mRocketChatAccount->avatarUrl(info));
        if (!iconUrlStr.isEmpty()) {
            if (iconUrlStr == cacheImageUrl) {
                return true;
            }
        }
        return false;
    });
    if (it != mAllMessages.end()) {
        const QModelIndex idx = createIndex(std::distance(mAllMessages.begin(), it), 0);
        Q_EMIT dataChanged(idx, idx);
    } else {
        qCWarning(RUQOLA_LOG) << "Attachment not found:" << filePath;
    }
}

void MessagesModel::deleteMessage(const QByteArray &messageId)
{
    auto it = findMessage(messageId);
    if (it != mAllMessages.end()) {
        const int i = std::distance(mAllMessages.begin(), it);
        beginRemoveRows(QModelIndex(), i, i);
        mAllMessages.erase(it);
        endRemoveRows();
    }
}

qint64 MessagesModel::generateNewStartTimeStamp(qint64 lastTimeStamp)
{
    return mLoadRecentHistoryManager->generateNewStartTimeStamp(lastTimeStamp);
}

Message MessagesModel::threadMessage(const QByteArray &threadMessageId) const
{
    if (!threadMessageId.isEmpty()) {
        auto it = findMessage(threadMessageId);
        if (it != mAllMessages.cend()) {
            return (*it);
        } else {
            qCDebug(RUQOLA_LOG) << "Thread message" << threadMessageId << "not found"; // could be a very old one
        }
    }
    return Message{};
}

QString MessagesModel::threadMessagePreview(const QByteArray &threadMessageId) const
{
    if (!threadMessageId.isEmpty()) {
        auto it = findMessage(threadMessageId);
        if (it != mAllMessages.cend()) {
            const QString userName = mRocketChatAccount ? mRocketChatAccount->userName() : QString();
            QString str = convertMessageText((*it), userName, mRocketChatAccount ? mRocketChatAccount->highlightWords() : QStringList(), QString());
            if (str.length() > 80) {
                str = str.left(80) + QStringLiteral("...");
            }
            return str;
        } else {
            qCDebug(RUQOLA_LOG) << "Thread message" << threadMessageId << "not found"; // could be a very old one
        }
    }
    return {};
}

bool MessagesModel::threadMessageFollowed(const QByteArray &threadMessageId) const
{
    if (!threadMessageId.isEmpty()) {
        auto it = findMessage(threadMessageId);
        if (it != mAllMessages.cend()) {
            const QByteArray userId = mRocketChatAccount ? mRocketChatAccount->userId() : QByteArray();
            if (!userId.isEmpty()) {
                // TODO convert replies to QList<QByteArray>
                return (*it).replies().contains(QString::fromLatin1(userId));
            }
        } else {
            qCDebug(RUQOLA_LOG) << "Thread message" << threadMessageId << "not found"; // could be a very old one
        }
    }
    return false;
}

QList<Message>::iterator MessagesModel::findMessage(const QByteArray &messageId)
{
    return std::find_if(mAllMessages.begin(), mAllMessages.end(), [&](const Message &msg) {
        return msg.messageId() == messageId;
    });
}

QList<Message>::const_iterator MessagesModel::findMessage(const QByteArray &messageId) const
{
    return std::find_if(mAllMessages.cbegin(), mAllMessages.cend(), [&](const Message &msg) {
        return msg.messageId() == messageId;
    });
}

QByteArray MessagesModel::roomId() const
{
    return mRoomId;
}

QString MessagesModel::searchText() const
{
    return mSearchText;
}

void MessagesModel::setSearchText(const QString &searchText)
{
    mSearchText = searchText;
}

#include "moc_messagesmodel.cpp"
