/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <TextEmoticonsCore/CustomEmojiIconManager>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT CustomEmojiIconManager : public TextEmoticonsCore::CustomEmojiIconManager
{
    Q_OBJECT
public:
    explicit CustomEmojiIconManager(QObject *parent = nullptr);
    ~CustomEmojiIconManager() override;

    [[nodiscard]] QIcon generateIcon(const QString &customIdentifier) override;
    [[nodiscard]] QString fileName(const QString &customIdentifier) override;

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

    [[nodiscard]] RocketChatAccount *currentRocketChatAccount() const;

private:
    RocketChatAccount *mCurrentRocketChatAccount = nullptr;
};
