/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "directoryteamsproxymodel.h"
#include "directoryteamsmodel.h"

DirectoryTeamsProxyModel::DirectoryTeamsProxyModel(QAbstractListModel *model, QObject *parent)
    : DirectoryBaseFilterProxyModel(model, parent)
{
}

DirectoryTeamsProxyModel::~DirectoryTeamsProxyModel() = default;

bool DirectoryTeamsProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const int leftColumn{left.column()};
    if (leftColumn == DirectoryTeamsModel::CreateTeam) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), DirectoryTeamsModel::CreateTeamDateTime);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), DirectoryTeamsModel::CreateTeamDateTime);
        return DirectoryBaseFilterProxyModel::lessThan(leftMessageModelIndex, rightMessageModelIndex);
    }
    return DirectoryBaseFilterProxyModel::lessThan(left, right);
}
