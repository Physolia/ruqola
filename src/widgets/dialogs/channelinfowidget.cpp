/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinfowidget.h"
#include "channelinfoeditablewidget.h"
#include "channelinfoprunewidget.h"
#include "channelinforeadonlywidget.h"
#include "connection.h"
#include "misc/systemmessagescombobox.h"
#include "permissions/permissionmanager.h"
#include "rocketchataccount.h"
#include "room.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

#include <QCheckBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

ChannelInfoWidget::ChannelInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mStackedWidget(new QStackedWidget(this))
    , mChannelInfoEditableWidget(new ChannelInfoEditableWidget(account, this))
    , mChannelInfoReadOnlyWidget(new ChannelInfoReadOnlyWidget(account, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    mStackedWidget->addWidget(mChannelInfoReadOnlyWidget);
    mStackedWidget->addWidget(mChannelInfoEditableWidget);
    connect(mChannelInfoEditableWidget, &ChannelInfoEditableWidget::channelDeleted, this, &ChannelInfoWidget::channelDeleted);
    connect(mChannelInfoEditableWidget, &ChannelInfoEditableWidget::fnameChanged, this, &ChannelInfoWidget::fnameChanged);
    connect(mChannelInfoEditableWidget, &ChannelInfoEditableWidget::roomNameValid, this, &ChannelInfoWidget::roomNameValid);
}

ChannelInfoWidget::~ChannelInfoWidget() = default;

void ChannelInfoWidget::updateUiFromPermission()
{
    mChannelInfoEditableWidget->updateUiFromPermission();
}

RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo ChannelInfoWidget::saveRoomSettingsInfo() const
{
    if (mRoom->canBeModify()) {
        return mChannelInfoEditableWidget->saveRoomSettingsInfo();
    }
    return {};
}

void ChannelInfoWidget::setRoom(Room *room)
{
    mRoom = room;
    if (mRoom->canBeModify()) {
        mChannelInfoEditableWidget->setRoom(mRoom);
        mStackedWidget->setCurrentWidget(mChannelInfoEditableWidget);
        mChannelInfoEditableWidget->updateEditableChannelInfo();
        mChannelInfoEditableWidget->updateRetentionValue();
        mChannelInfoEditableWidget->connectEditableWidget();
    } else {
        mChannelInfoReadOnlyWidget->setRoom(mRoom);
        mStackedWidget->setCurrentWidget(mChannelInfoReadOnlyWidget);
    }
}


