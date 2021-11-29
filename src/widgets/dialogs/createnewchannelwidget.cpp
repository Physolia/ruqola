/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "createnewchannelwidget.h"
#include "channelnamevalidlineedit.h"
#include "misc/adduserswidget.h"
#include <KAuthorized>
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

CreateNewChannelWidget::CreateNewChannelWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mChannelName(new ChannelNameValidLineEdit(account, this))
    , mUsers(new AddUsersWidget(this))
    , mReadOnly(new QCheckBox(this))
    , mBroadcast(new QCheckBox(this))
    , mPrivate(new QCheckBox(this))
    , mEncryptedRoom(new QCheckBox(this))
    , mTopicLineEdit(new QLineEdit(this))
    , mMainLayout(new QFormLayout(this))
{
    mMainLayout->setObjectName(QStringLiteral("mainLayout"));
    mMainLayout->setContentsMargins({});

    mChannelName->setObjectName(QStringLiteral("mChannelName"));
    mMainLayout->addRow(i18n("Name:"), mChannelName);

    mTopicLineEdit->setObjectName(QStringLiteral("mTopicLineEdit"));
    mMainLayout->addRow(i18n("Topic:"), mTopicLineEdit);

    mUsers->setObjectName(QStringLiteral("mUsers"));
    mUsers->setPlaceholderText(i18n("Invite Users..."));
    mMainLayout->addRow(i18n("Users:"), mUsers);

    mReadOnly->setObjectName(QStringLiteral("mReadOnly"));
    mReadOnly->setChecked(false);
    mReadOnly->setToolTip(i18n("All users in this team can write messages"));
    mMainLayout->addRow(i18n("Read-Only:"), mReadOnly);

    mBroadcast->setObjectName(QStringLiteral("mBroadcast"));
    mBroadcast->setChecked(false);
    mBroadcast->setToolTip(i18n("Only authorized users can write new messages, but the other users will be able to reply"));
    mMainLayout->addRow(i18n("Broadcast:"), mBroadcast);

    mPrivate->setObjectName(QStringLiteral("mPrivate"));
    mPrivate->setChecked(false);
    mPrivate->setToolTip(i18n("Only invited people can join"));

    mMainLayout->addRow(i18n("Private Room:"), mPrivate);

    mEncryptedRoom->setObjectName(QStringLiteral("mEncryptedRoom"));
    mEncryptedRoom->setChecked(false);
    mMainLayout->addRow(i18n("Encrypted Room:"), mEncryptedRoom);

    connect(mChannelName, &ChannelNameValidLineEdit::channelIsValid, this, &CreateNewChannelWidget::slotChangeOkButtonEnabled);
}

CreateNewChannelWidget::~CreateNewChannelWidget() = default;

void CreateNewChannelWidget::slotChangeOkButtonEnabled(bool state)
{
    Q_EMIT updateOkButton(/*!mChannelName->text().trimmed().isEmpty()*/ state);
}

QString CreateNewChannelWidget::channelName() const
{
    return mChannelName->text();
}

QStringList CreateNewChannelWidget::users() const
{
    return mUsers->userIds();
}

bool CreateNewChannelWidget::readOnly() const
{
    return mReadOnly->isChecked();
}

bool CreateNewChannelWidget::broadCast() const
{
    return mBroadcast->isChecked();
}

bool CreateNewChannelWidget::privateChannel() const
{
    return mPrivate->isChecked();
}

bool CreateNewChannelWidget::encryptedRoom() const
{
    return mEncryptedRoom->isChecked();
}

QString CreateNewChannelWidget::topic() const
{
    return mTopicLineEdit->text();
}

void CreateNewChannelWidget::setFeatures(CreateNewChannelWidget::Features features)
{
    bool visible = features & CreateNewChannelWidget::Feature::BroadCast;

    mBroadcast->setVisible(visible);
    mMainLayout->labelForField(mBroadcast)->setVisible(visible);

    visible = features & CreateNewChannelWidget::Feature::Encrypted;

    mEncryptedRoom->setVisible(visible);
    mMainLayout->labelForField(mEncryptedRoom)->setVisible(visible);
}
