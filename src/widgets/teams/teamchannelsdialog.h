/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "libruqolawidgets_private_export.h"
class TeamChannelsWidget;
class Room;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamChannelsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TeamChannelsDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~TeamChannelsDialog() override;
    void setRoom(Room *room);

private:
    void readConfig();
    void writeConfig();
    TeamChannelsWidget *const mTeamChannelsWidget;
};
