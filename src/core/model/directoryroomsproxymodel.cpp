/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "directoryroomsproxymodel.h"
#include "directoryroomsmodel.h"

DirectoryRoomsProxyModel::DirectoryRoomsProxyModel(QAbstractListModel *model, QObject *parent)
    : DirectoryBaseFilterProxyModel(model, parent)
{
}

DirectoryRoomsProxyModel::~DirectoryRoomsProxyModel() = default;

bool DirectoryRoomsProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const int leftColumn{left.column()};
    if (leftColumn == DirectoryRoomsModel::LastMessageStr) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), DirectoryRoomsModel::LastMessageDateTime);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), DirectoryRoomsModel::LastMessageDateTime);
        return DirectoryBaseFilterProxyModel::lessThan(leftMessageModelIndex, rightMessageModelIndex);
    } else if (leftColumn == DirectoryRoomsModel::CreatedStr) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), DirectoryRoomsModel::CreatedDateTime);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), DirectoryRoomsModel::CreatedDateTime);
        return DirectoryBaseFilterProxyModel::lessThan(leftMessageModelIndex, rightMessageModelIndex);
    }
    return DirectoryBaseFilterProxyModel::lessThan(left, right);
}
