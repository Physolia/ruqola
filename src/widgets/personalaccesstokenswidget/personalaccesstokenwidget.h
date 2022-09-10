/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class QLineEdit;
class PersonalAccessTokenTreeView;
class PersonalAccessTokenInfosModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PersonalAccessTokenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PersonalAccessTokenWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~PersonalAccessTokenWidget() override;

private:
    RocketChatAccount *const mRocketChatAccount;
    QLineEdit *const mSearchLineWidget;
    PersonalAccessTokenTreeView *const mPersonalAccessTokenTreeView;
    PersonalAccessTokenInfosModel *const mPersonalAccessTokenModel;
};
