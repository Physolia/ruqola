/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "teamroomcompletermodel.h"
#include "ruqola_debug.h"
#include <QModelIndex>

TeamRoomCompleterModel::TeamRoomCompleterModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

TeamRoomCompleterModel::~TeamRoomCompleterModel() = default;

void TeamRoomCompleterModel::clear()
{
    if (!mRooms.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        mRooms.clear();
        endRemoveRows();
    }
}

void TeamRoomCompleterModel::insertRooms(const QVector<TeamRoomCompleter> &rooms)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mRooms.count() - 1);
        mRooms.clear();
        endRemoveRows();
    }
    if (!rooms.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, rooms.count() - 1);
        mRooms = rooms;
        endInsertRows();
    }
}

int TeamRoomCompleterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mRooms.count();
}

QVariant TeamRoomCompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mRooms.count()) {
        return {};
    }
    const TeamRoomCompleter &room = mRooms.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case TeamName:
        return room.name();
    case TeamId:
        return room.identifier();
    case Qt::DecorationRole:
    case TeamIcon:
        return {};
    }

    return {};
}
