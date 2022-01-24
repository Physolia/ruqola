/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "model/directorybasefilterproxymodel.h"

class AdministratorInvitesFilterProxyModel : public DirectoryBaseFilterProxyModel
{
    Q_OBJECT
public:
    explicit AdministratorInvitesFilterProxyModel(QAbstractListModel *model = nullptr, QObject *parent = nullptr);
    ~AdministratorInvitesFilterProxyModel() override;

    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
