/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class AddUsersWidget;
class RocketChatAccount;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateDirectMessagesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateDirectMessagesWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CreateDirectMessagesWidget() override;
    [[nodiscard]] QStringList userNames() const;
Q_SIGNALS:
    void updateOkButton(bool state);

private:
    void checkMaximumUsers(bool state);
    AddUsersWidget *const mUsers;
    RocketChatAccount *const mRocketChatAccount;
    QLabel *const mTooManyUsers;
};
