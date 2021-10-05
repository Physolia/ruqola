/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "channelactionpopupmenu.h"
#include "rocketchataccount.h"
#include "room.h"
#include <KLocalizedString>
#include <QMenu>

ChannelActionPopupMenu::ChannelActionPopupMenu(QObject *parent)
    : QObject(parent)
    , mMenu(new QMenu)
{
    mMenu->setObjectName(QStringLiteral("mMenu"));
    connect(mMenu, &QMenu::aboutToShow, this, &ChannelActionPopupMenu::slotUpdateMenu);
    createMenu();
}

ChannelActionPopupMenu::~ChannelActionPopupMenu()
{
    delete mMenu;
}

void ChannelActionPopupMenu::createMenu()
{
    mPruneMessages = new QAction(i18n("Prune Messages..."), this);
    mMenu->addAction(mPruneMessages);
    connect(mPruneMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::PruneMessages);
    });

    mPruneMessagesSeparator = mMenu->addSeparator();

    mShowMentions = new QAction(i18n("Show Mentions..."), this);
    mMenu->addAction(mShowMentions);
    connect(mShowMentions, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowMentions);
    });

    mShowPinnedMessages = new QAction(i18n("Show Pinned Messages..."), this);
    mMenu->addAction(mShowPinnedMessages);
    connect(mShowPinnedMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowPinned);
    });

    mShowStarredMessages = new QAction(i18n("Show Starred Messages..."), this);
    mMenu->addAction(mShowStarredMessages);
    connect(mShowStarredMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowStarred);
    });

    mShowSnipperedMessages = new QAction(i18n("Show Snippered Messages..."), this);
    mMenu->addAction(mShowSnipperedMessages);
    connect(mShowSnipperedMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowSnippered);
    });

    mShowFileAttachments = new QAction(QIcon::fromTheme(QStringLiteral("document-send-symbolic")), i18n("Show File Attachments..."), this);
    mMenu->addAction(mShowFileAttachments);
    connect(mShowFileAttachments, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowAttachment);
    });

    mShowDiscussions = new QAction(i18n("Show Discussions..."), this);
    mMenu->addAction(mShowDiscussions);
    connect(mShowDiscussions, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowDiscussions);
    });

    mShowThreads = new QAction(i18n("Show Threads..."), this);
    mMenu->addAction(mShowThreads);
    connect(mShowThreads, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowThreads);
    });

    mMenu->addSeparator();

    mConfigureNotification = new QAction(QIcon::fromTheme(QStringLiteral("preferences-desktop-notification")), i18n("Configure Notification..."), this);
    mMenu->addAction(mConfigureNotification);
    connect(mConfigureNotification, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::Notification);
    });

    mAutoTranslateSeparator = mMenu->addSeparator();
    mAutoTranslate = new QAction(i18n("Configure Auto-Translate..."), this);
    mMenu->addAction(mAutoTranslate);
    connect(mAutoTranslate, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::AutoTranslate);
    });

    mInviteUsersGenerateUrlSeparator = mMenu->addSeparator();
    mMenu->addAction(mInviteUsersGenerateUrlSeparator);
    mInviteUsersGenerateUrl = new QAction(i18n("Invite Users"), this);
    mMenu->addAction(mInviteUsersGenerateUrl);
    connect(mInviteUsersGenerateUrl, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::InviteUsers);
    });

    mAddUserInRoomsSeparator = mMenu->addSeparator();
    mAddUserInRooms = new QAction(i18n("Add Users in Channel..."), this);
    mMenu->addAction(mAddUserInRoomsSeparator);
    connect(mAddUserInRooms, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::AddUsersInRoom);
    });
    mMenu->addAction(mAddUserInRooms);

    mMenu->addSeparator();
    mStartVideoChat = new QAction(QIcon::fromTheme(QStringLiteral("camera-video")), i18n("Video Chat"), this);
    mMenu->addAction(mStartVideoChat);
    connect(mStartVideoChat, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::VideoChat);
    });

    mMenu->addSeparator();
    mExportMessages = new QAction(i18n("Export Messages..."), this);
    mMenu->addAction(mExportMessages);
    connect(mExportMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ExportMessages);
    });

    mMenu->addSeparator();
    mFollowMessage = new QAction(i18n("Follow Message..."), this);
    mMenu->addAction(mFollowMessage);
    connect(mFollowMessage, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::FollowMessages);
    });

    mMenu->addSeparator();
    mOffTheRecordMessages = new QAction(i18n("OTR"), this);
    mMenu->addAction(mOffTheRecordMessages);
    connect(mOffTheRecordMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::OtrMessages);
    });
}

QAction *ChannelActionPopupMenu::createSeparator()
{
    return mMenu->addSeparator();
}

QMenu *ChannelActionPopupMenu::menu() const
{
    return mMenu;
}

void ChannelActionPopupMenu::setRoom(Room *room)
{
    mRoom = room;
}

void ChannelActionPopupMenu::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mCurrentRocketChatAccount = account;
}

void ChannelActionPopupMenu::slotUpdateMenu()
{
    mShowPinnedMessages->setVisible(mCurrentRocketChatAccount->allowMessagePinningEnabled());
    mShowStarredMessages->setVisible(mCurrentRocketChatAccount->hasStarredMessagesSupport() && mCurrentRocketChatAccount->allowMessageStarringEnabled());
    mShowSnipperedMessages->setVisible(mCurrentRocketChatAccount->hasSnippetedMessagesSupport() && mCurrentRocketChatAccount->allowMessageSnippetingEnabled());
    mAutoTranslate->setVisible(mCurrentRocketChatAccount->hasAutotranslateSupport());
    mAutoTranslateSeparator->setVisible(mCurrentRocketChatAccount->autoTranslateEnabled());

    bool hasInviteUserSupport = mCurrentRocketChatAccount->hasInviteUserSupport();
    bool hasPermissionInviteUserSupport = mRoom && mRoom->hasPermission(QStringLiteral("create-invite-links"));
    mInviteUsersGenerateUrl->setVisible(hasInviteUserSupport && hasPermissionInviteUserSupport);
    mInviteUsersGenerateUrlSeparator->setVisible(hasInviteUserSupport && hasPermissionInviteUserSupport);
    mStartVideoChat->setVisible(mCurrentRocketChatAccount->jitsiEnabled());

    mAddUserInRoomsSeparator->setVisible(mRoom && mRoom->canBeModify());
    mAddUserInRooms->setVisible(mRoom && mRoom->canBeModify());

    const bool showPruneMessage = mCurrentRocketChatAccount->hasPermission(QStringLiteral("clean-channel-history"));
    mPruneMessages->setVisible(showPruneMessage);
    mPruneMessagesSeparator->setVisible(showPruneMessage);

    mExportMessages->setVisible(mCurrentRocketChatAccount->hasPermission(QStringLiteral("mail-messages")));

    // FIXME Disable for the moment
    mFollowMessage->setVisible(false && mCurrentRocketChatAccount->threadsEnabled());

    // FIXME Disable for the moment
    mOffTheRecordMessages->setVisible(false && mCurrentRocketChatAccount->otrEnabled() && mRoom->channelType() == Room::RoomType::Direct);
}
