/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef ROCKETCHATACCOUNTMODEL_H
#define ROCKETCHATACCOUNTMODEL_H

#include <QAbstractListModel>
#include "libruqolacore_export.h"
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT RocketChatAccountModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int accountNumber READ accountNumber NOTIFY accountNumberChanged)
public:
    enum AccountRoles {
        Name = Qt::UserRole + 1,
        SiteUrl,
        UserName,
    };
    Q_ENUM(AccountRoles)

    explicit RocketChatAccountModel(QObject *parent = nullptr);
    ~RocketChatAccountModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = {}) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void insertAccount(RocketChatAccount *account);
    void removeAccount(const QString &name);
    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;
    Q_REQUIRED_RESULT int indexOf(RocketChatAccount *account) const;

    void clear();

    void setAccounts(const QVector<RocketChatAccount *> &accounts);

    Q_REQUIRED_RESULT RocketChatAccount *account(int index) const;

    Q_REQUIRED_RESULT RocketChatAccount *account(const QString &accountName) const;

    Q_REQUIRED_RESULT int accountNumber() const;
    Q_REQUIRED_RESULT QStringList accountsName() const;

Q_SIGNALS:
    void accountNumberChanged();

private:
    Q_DISABLE_COPY(RocketChatAccountModel)
    QVector<RocketChatAccount *> mRocketChatAccount;
};

Q_DECLARE_METATYPE(RocketChatAccountModel*);

#endif // ROCKETCHATACCOUNTMODEL_H
