/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"

#include <QObject>

class LIBRUQOLACORE_EXPORT AccountManager : public QObject
{
    Q_OBJECT
public:
    struct LIBRUQOLACORE_EXPORT AccountManagerInfo {
        QString displayName;
        QString accountName;
        QString userName;
        QString serverUrl;
        QString password;
        bool enabled = true;
    };

    explicit AccountManager(QObject *parent = nullptr);
    ~AccountManager() override;

    void removeAccount(const QString &accountName, bool removeLogFiles = false);
    void addAccount(const AccountManagerInfo &info);

    RocketChatAccount *account() const;
    RocketChatAccountModel *rocketChatAccountModel() const;

    RocketChatAccountFilterProxyModel *rocketChatAccountProxyModel() const;

    void addAccount(RocketChatAccount *account);

    void setCurrentAccount(const QString &accountName);
    [[nodiscard]] QString currentAccount() const;
    void selectAccount(const QString &accountName);

    void modifyAccount(const AccountManagerInfo &info);
    [[nodiscard]] QStringList accountsName() const;

    [[nodiscard]] int accountNumber() const;
    void openMessageUrl(const QString &messageUrl);
    [[nodiscard]] bool showMessage(const ParseMessageUrlUtils &parseUrl);
    [[nodiscard]] RocketChatAccount *accountFromName(const QString &accountName);

    [[nodiscard]] QStringList accountNamesSorted() const;

Q_SIGNALS:
    void logoutAccountDone(const QString &accountName);
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    void currentAccountChanged();
    void roomNeedAttention();
    void messageUrlNotFound(const QString &str);

private:
    Q_DISABLE_COPY(AccountManager)

    LIBRUQOLACORE_NO_EXPORT void removeDatabaseAccount(const QString &accountName);
    LIBRUQOLACORE_NO_EXPORT void removeDirectory(const QString &directory);
    LIBRUQOLACORE_NO_EXPORT void removeLogs(const QString &accountName);
    LIBRUQOLACORE_NO_EXPORT void loadAccount();
    LIBRUQOLACORE_NO_EXPORT void connectToAccount(RocketChatAccount *account);
    LIBRUQOLACORE_NO_EXPORT void slotSwitchToAccountAndRoomName(const QString &accountName, const QString &roomName, const QString &channelType);
    RocketChatAccount *mCurrentAccount = nullptr;
    RocketChatAccountModel *const mRocketChatAccountModel;
    RocketChatAccountFilterProxyModel *const mRocketChatAccountProxyModel;
};
