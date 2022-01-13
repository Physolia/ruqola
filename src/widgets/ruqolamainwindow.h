/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "libruqolawidgets_export.h"
#include <KXmlGuiWindow>
#include <QPointer>
class StatusCombobox;
class RuqolaCentralWidget;
class QAction;
class QWidgetAction;
class ServerMenu;
class RocketChatAccount;
class QLabel;
class AccountsOverviewWidget;
class Notification;
class StatusModelFilterProxyModel;
class LIBRUQOLAWIDGETS_EXPORT RuqolaMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit RuqolaMainWindow(QWidget *parent = nullptr);
    ~RuqolaMainWindow() override;

    void slotActivateRequested(const QStringList &arguments, const QString &workingDirectory);

protected:
    Q_REQUIRED_RESULT bool queryClose() override;

private:
    void slotRegisterUserSuccessed();
    void setupActions();
    void readConfig();
    void setupStatusBar();
    void slotAddServer();
    void slotServerInfo();
    void slotConfigure();
    void slotLogout();
    void slotSearchChannel();
    void slotSelectNextUnreadChannel();
    void slotCreateNewChannel();
    void slotTypingNotificationChanged(const QString &roomId, const QString &notificationStr);
    void slotClearNotification();
    void slotAccountChanged();
    void slotUnreadOnTop(bool checked);
    void slotShowLog();
    void updateActions();
    void slotMissingChannelPassword(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void slotClearAccountAlerts();
    void slotLoginPageActivated(bool loginPageActivated);
    void slotConfigureNotifications();
    void slotRaiseWindow();
    void slotRegisterNewUser();
    void slotAdministrator();
    void slotConfigureMyAccount();
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    void logout(const QString &accountName);
    void slotRoomNeedAttention();
    void slotCreateDirectMessages();
    void slotClose();
    void slotStatusChanged();
    void createSystemTray();
    void slotUpdateCustomUserStatus();
    void slotCreateDiscussion();
    void slotCreateTeam();
    void slotPermissionChanged();
    void slotDirectory();
    void showNextView();
    void showPreviousView();

    bool mReallyClose{false};

    RuqolaCentralWidget *const mMainWidget;
    QAction *mServerInfo = nullptr;
    QAction *mLogout = nullptr;
    QAction *mSearchChannel = nullptr;
    QAction *mNextUnreadChannel = nullptr;
    QAction *mCreateNewChannel = nullptr;
    QAction *mCreateDirectMessages = nullptr;
    QAction *mUnreadOnTop = nullptr;
    QAction *mShowLog = nullptr;
    QAction *mClearAlerts = nullptr;
    QAction *mRegisterNewUser = nullptr;
    QAction *mMyAccount = nullptr;
    QAction *mAdministrator = nullptr;
    QAction *mCreateDiscussion = nullptr;
    QAction *mCreateTeam = nullptr;
    QAction *mDirectory = nullptr;
    QAction *mNextTab = nullptr;
    QAction *mPreviewTab = nullptr;
    QWidgetAction *mStatus = nullptr;
    ServerMenu *mServerMenu = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QLabel *mStatusBarTypingMessage = nullptr;
    AccountsOverviewWidget *mAccountOverviewWidget = nullptr;
    Notification *mNotification = nullptr;
    StatusCombobox *mStatusComboBox = nullptr;
    StatusModelFilterProxyModel *mStatusProxyModel = nullptr;
};

