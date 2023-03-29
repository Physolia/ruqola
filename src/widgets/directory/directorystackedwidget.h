/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "directory/directorywidget.h"
#include "libruqolawidgets_private_export.h"
#include <QStackedWidget>
class RocketChatAccount;
class DirectoryNotAuthorizedWidget;

class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectoryStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit DirectoryStackedWidget(RocketChatAccount *account, DirectoryWidget::DirectoryType type, QWidget *parent = nullptr);
    ~DirectoryStackedWidget() override;

    Q_REQUIRED_RESULT bool isAutorized() const;
    void setIsAutorized(bool newIsAutorized);

    void fillDirectory();

    Q_REQUIRED_RESULT DirectoryWidget *directoryWidget() const;

    Q_REQUIRED_RESULT DirectoryNotAuthorizedWidget *directoryNotAutorizedWidget() const;

private:
    DirectoryWidget *const mDirectoryWidget;
    DirectoryNotAuthorizedWidget *const mDirectoryNotAutorizedWidget;
    bool mIsAutorized = true;
};