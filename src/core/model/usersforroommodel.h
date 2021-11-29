/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "user.h"
#include <QAbstractListModel>
#include <QVector>
class UsersModel;
class LIBRUQOLACORE_EXPORT UsersForRoomModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UsersForRoomRoles {
        UserName = Qt::UserRole + 1,
        UserId,
        Name,
        IconStatus,
        DisplayName,
        Status,
    };
    Q_ENUM(UsersForRoomRoles)

    explicit UsersForRoomModel(QObject *parent = nullptr);
    ~UsersForRoomModel() override;

    void setUsers(const QVector<User> &users);

    void clear();
    int rowCount(const QModelIndex &parent = {}) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void parseUsersForRooms(const QJsonObject &root, UsersModel *model, bool restapi);
    void setUserStatusChanged(const User &newuser);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT bool hasFullList() const;
    void setHasFullList(bool hasFullList);
    Q_REQUIRED_RESULT int usersCount() const;

    void setLoadMoreUsersInProgress(bool inProgress);
    Q_REQUIRED_RESULT bool loadMoreUsersInProgress() const;
Q_SIGNALS:
    void hasFullListChanged();
    void userStatusChanged(const QString &userId);
    void loadingInProgressChanged();

private:
    Q_REQUIRED_RESULT QString generateDisplayName(const User &user) const;
    void checkFullList();
    QVector<User> mUsers;
    int mTotal = 0;
    int mOffset = 0;
    bool mHasFullList = false;
    bool mLoadingInProgress = false;
};

