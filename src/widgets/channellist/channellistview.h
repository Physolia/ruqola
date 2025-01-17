/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "room.h"
#include <QTreeView>

#include "libruqolawidgets_private_export.h"

class RoomFilterProxyModel;
class RoomListHeadingsProxyModel;
class ChannelListDelegate;
class RocketChatAccount;

class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelListView : public QTreeView
{
    Q_OBJECT
public:
    explicit ChannelListView(QWidget *parent = nullptr);
    ~ChannelListView() override;

    [[nodiscard]] RoomFilterProxyModel *filterModel() const;

    void selectChannelRequested(const QString &channelId, const QString &messageId);
    void selectNextUnreadChannel();

    enum class Direction {
        Up,
        Down,
    };
    void selectNextChannel(Direction direction = Direction::Down, bool switchToNextUnreadChannel = false);

    [[nodiscard]] bool selectChannelByRoomNameRequested(const QString &selectedRoomName);

    void channelSelected(const QModelIndex &index);

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);
    [[nodiscard]] bool selectChannelByRoomIdRequested(const QString &identifier);

Q_SIGNALS:
    void roomSelected(const QString &roomName, const QString &roomId, Room::RoomType roomType);
    void roomPressed(const QString &roomId);
    void selectMessageIdRequested(const QString &messageId);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void slotClicked(const QModelIndex &index);
    void slotHideChannel(const QModelIndex &index, Room::RoomType roomType);
    void slotLeaveChannel(const QModelIndex &index, Room::RoomType roomType);
    void slotChangeFavorite(const QModelIndex &index, bool isFavorite);
    void slotMarkAsChannel(const QModelIndex &index, bool markAsRead);
    void slotConvertToTeam(const QModelIndex &index, Room::RoomType roomType);
    void slotChannelConvertToTeamDone(const QJsonObject &obj);
    void slotGroupConvertToTeamDone(const QJsonObject &obj);
    void slotMoveToTeam(const QModelIndex &index);
    void slotConvertToChannel(const QModelIndex &index);
    void slotConfigureNotification(Room *room);
    [[nodiscard]] bool selectChannelByRoomIdOrRoomName(const QString &id, bool roomId);
    void slotRoomRemoved(const QString &roomId);
    void slotPressed(const QModelIndex &index);

    ChannelListDelegate *const mChannelListDelegate;
    RoomListHeadingsProxyModel *const mRoomListHeadingsProxyModel;
    RoomFilterProxyModel *const mRoomFilterProxyModel;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QMetaObject::Connection mUpdateChannelViewConnect;
};
