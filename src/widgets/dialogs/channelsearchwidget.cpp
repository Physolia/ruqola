/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "channelsearchwidget.h"
#include "channelsearchnamelineresultwidget.h"
#include <QStackedWidget>
#include <QVBoxLayout>
ChannelSearchWidget::ChannelSearchWidget(QWidget *parent)
    : QWidget(parent)
    , mStackedWidget(new QStackedWidget(this))
    , mChannelSearchNameLineEdit(new ChannelSearchNameLineEdit(this))
    , mChannelSearchLineResult(new ChannelSearchNameLineResultWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mainLayout->addWidget(mStackedWidget);
    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mChannelSearchNameLineEdit->setObjectName(QStringLiteral("mChannelSearchNameLineEdit"));
    mStackedWidget->addWidget(mChannelSearchNameLineEdit);

    mChannelSearchLineResult->setObjectName(QStringLiteral("mChannelSearchLineResult"));
    mStackedWidget->addWidget(mChannelSearchLineResult);

    connect(mChannelSearchNameLineEdit, &ChannelSearchNameLineEdit::newRoomName, this, &ChannelSearchWidget::slotSelectedRoom);
    connect(mChannelSearchLineResult, &ChannelSearchNameLineResultWidget::clearRoomName, this, &ChannelSearchWidget::slotClearRoom);
}

ChannelSearchWidget::~ChannelSearchWidget() = default;

void ChannelSearchWidget::setChannelInfo(const QString &name, const QString &channelId)
{
    mUserInfo.channelName = name;
    mUserInfo.channelId = channelId;
    slotSelectedRoom(mUserInfo);
    mChannelSearchLineResult->setReadOnly(true);
}

QString ChannelSearchWidget::channelName() const
{
    return mUserInfo.channelName;
}

QString ChannelSearchWidget::channelId() const
{
    return mUserInfo.channelId;
}

void ChannelSearchWidget::slotSelectedRoom(const ChannelSearchNameLineEdit::ChannelCompletionInfo &userInfo)
{
    mUserInfo = userInfo;
    mStackedWidget->setCurrentWidget(mChannelSearchLineResult);
    mChannelSearchLineResult->setRoomName(userInfo.channelName);
    Q_EMIT updateRoomName(true);
}

void ChannelSearchWidget::slotClearRoom()
{
    mUserInfo = ChannelSearchNameLineEdit::ChannelCompletionInfo();
    mStackedWidget->setCurrentWidget(mChannelSearchNameLineEdit);
    Q_EMIT updateRoomName(false);
}
