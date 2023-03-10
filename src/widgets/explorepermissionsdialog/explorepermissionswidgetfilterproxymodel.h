/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QSortFilterProxyModel>

class ExplorePermissionsWidgetFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ExplorePermissionsWidgetFilterProxyModel(QObject *parent = nullptr);
    ~ExplorePermissionsWidgetFilterProxyModel() override;

protected:
    Q_REQUIRED_RESULT bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};
