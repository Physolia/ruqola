/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class SearchChannelWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchChannelDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SearchChannelDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SearchChannelDialog() override;

private:
    void readConfig();
    void writeConfig();
    SearchChannelWidget *const mSearchChannelWidget;
};

