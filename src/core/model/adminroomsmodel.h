/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#ifndef ADMINROOMSMODEL_H
#define ADMINROOMSMODEL_H

#include <QAbstractListModel>
#include "libruqolacore_export.h"
#include "administrator/adminrooms.h"

// Model showing all emojis
class LIBRUQOLACORE_EXPORT AdminRoomsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AdminRoomsRoles {
        Name = Qt::UserRole + 1,
        Total,
        MessagesCount,
        UsersCount,
        Topic,
        Identifier,
        ReadOnly,
        DefaultRoom,
    };
    Q_ENUM(AdminRoomsRoles)

    explicit AdminRoomsModel(QObject *parent = nullptr);
    ~AdminRoomsModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    Q_REQUIRED_RESULT AdminRooms adminRooms() const;

    void setAdminRooms(const AdminRooms &adminrooms);

private:
    Q_DISABLE_COPY(AdminRoomsModel)
    AdminRooms mAdminRooms;
};

#endif // ADMINROOMSMODEL_H
