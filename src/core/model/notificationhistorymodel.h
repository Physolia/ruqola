/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractItemModel>

#include "libruqolacore_export.h"
#include "notificationinfo.h"
#include <QVector>

class LIBRUQOLACORE_EXPORT NotificationHistoryModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum NotificationHistoryRoles {
        AccountName = Qt::UserRole + 1,
    };
    Q_ENUM(NotificationHistoryRoles)

    explicit NotificationHistoryModel(QObject *parent = nullptr);
    ~NotificationHistoryModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void clear();

    void insertNotifications(const QVector<NotificationInfo> &infos);

private:
    QVector<NotificationInfo> mNotificationInfo;
};