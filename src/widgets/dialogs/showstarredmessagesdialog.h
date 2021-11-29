/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "showlistmessagebasedialog.h"
class ShowStarredMessagesDialog : public ShowListMessageBaseDialog
{
    Q_OBJECT
public:
    explicit ShowStarredMessagesDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowStarredMessagesDialog() override;

private:
    void readConfig();
    void writeConfig();
};

