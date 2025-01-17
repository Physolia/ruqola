/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "teams/teamroom.h"
#include <QAbstractListModel>
#include <QVector>
class LIBRUQOLACORE_EXPORT TeamRoomsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum TeamRoomsRoles {
        Name = Qt::UserRole + 1,
        AutoJoin,
        Identifier,
    };
    Q_ENUM(TeamRoomsRoles)

    explicit TeamRoomsModel(QObject *parent = nullptr);
    ~TeamRoomsModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QVector<TeamRoom> teamRooms() const;
    void setTeamRooms(const QVector<TeamRoom> &teamRooms);

    void setRoomChanged(const TeamRoom &teamRoom);

    void insertRooms(const QVector<TeamRoom> &teamRooms);

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    [[nodiscard]] bool isCheckable() const;
    void setIsCheckable(bool isCheckable);

private:
    Q_DISABLE_COPY(TeamRoomsModel)
    QVector<TeamRoom> mTeamRooms;
    QStringList mRoomSelected;
    bool mIsCheckable = false;
};
