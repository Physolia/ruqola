/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class AddUsersWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateDirectMessagesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateDirectMessagesWidget(QWidget *parent = nullptr);
    ~CreateDirectMessagesWidget() override;
    Q_REQUIRED_RESULT QStringList userNames() const;
Q_SIGNALS:
    void updateOkButton(bool state);

private:
    AddUsersWidget *const mUsers;
};

