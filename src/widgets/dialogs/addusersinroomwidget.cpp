/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "addusersinroomwidget.h"
#include "adduserscompletionlineedit.h"
#include "misc/adduserswidget.h"

#include <KLocalizedString>
#include <QVBoxLayout>

AddUsersInRoomWidget::AddUsersInRoomWidget(QWidget *parent)
    : QWidget(parent)
    , mAddUsersWidget(new AddUsersWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mAddUsersWidget->setObjectName(QStringLiteral("mAddUsersWidget"));
    mAddUsersWidget->setPlaceholderText(i18n("Search Users..."));
    connect(mAddUsersWidget, &AddUsersWidget::userListChanged, this, &AddUsersInRoomWidget::updateOkButton);
    mainLayout->addWidget(mAddUsersWidget);
    mainLayout->addStretch(1);
}

AddUsersInRoomWidget::~AddUsersInRoomWidget() = default;

QStringList AddUsersInRoomWidget::userIds() const
{
    return mAddUsersWidget->userIds();
}

QStringList AddUsersInRoomWidget::userNames() const
{
    return mAddUsersWidget->userNames();
}
