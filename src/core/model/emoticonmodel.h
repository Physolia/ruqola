/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "emoticons/customemoji.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>
#include <TextEmoticonsCore/UnicodeEmoticon>

class RocketChatAccount;
// Model showing all emojis
class LIBRUQOLACORE_EXPORT EmoticonModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum EmoticonsRoles {
        UnicodeEmoji = Qt::UserRole + 1,
        CompleterName, // keep value in sync with InputCompleterModel
        Identifier,
        Category,
        Icon,
    };
    Q_ENUM(EmoticonsRoles)

    explicit EmoticonModel(RocketChatAccount *account, QObject *parent = nullptr);
    ~EmoticonModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    Q_REQUIRED_RESULT QList<TextEmoticonsCore::UnicodeEmoticon> unicodeEmoticons() const;

    void setUnicodeEmoticons(const QList<TextEmoticonsCore::UnicodeEmoticon> &emoticons);

    Q_REQUIRED_RESULT const QVector<CustomEmoji> &customEmojiList() const;
    void setCustomEmojiList(const QVector<CustomEmoji> &newCustomEmojiList);

    void deleteEmojiCustom(const QStringList &lst);

    void addEmojiCustomList(const QVector<CustomEmoji> &newCustomEmojiList);

private:
    Q_REQUIRED_RESULT QIcon createCustomIcon(const QString &name) const;
    Q_DISABLE_COPY(EmoticonModel)
    QList<TextEmoticonsCore::UnicodeEmoticon> mEmoticons;
    QVector<CustomEmoji> mCustomEmojiList;
    // first int is an index into mEmoticons
    // second is -1 for the emoticon identifier or otherwise an index into the alias list
    QVector<QPair<int, int>> mUnicodeRows;
    QVector<QPair<int, int>> mCustomRows;
    RocketChatAccount *const mRocketChatAccount;
};
