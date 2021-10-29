/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "administratorwidget.h"
#include "administratordialog/logs/viewlogwidget.h"
#include "administratordialog/permissions/permissionswidget.h"
#include "administratordialog/roles/administratorroleswidget.h"
#include "customemoji/administratorcustomemojiwidget.h"
#include "customsounds/administratorcustomsoundswidget.h"
#include "customuserstatus/administratorcustomuserstatuswidget.h"
#include "invites/administratorinviteswidget.h"
#include "permissions/permissionmanager.h"
#include "rocketchataccount.h"
#include "rooms/administratorroomswidget.h"
#include "ruqola.h"
#include "serverinfo/administratorserverinfowidget.h"
#include "users/administratoruserswidget.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>

AdministratorWidget::AdministratorWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mTabWidget(new QTabWidget(this))
    , mAdministratorRoomsWidget(new AdministratorRoomsWidget(account, this))
    , mAdministratorCustomUserStatusWidget(new AdministratorCustomUserStatusWidget(account, this))
    , mAdministratorCustomSoundsWidget(new AdministratorCustomSoundsWidget(account, this))
    , mAdministratorServerInfoWidget(new AdministratorServerInfoWidget(account, this))
    , mAdministratorUsersWidget(new AdministratorUsersWidget(account, this))
    , mAdministratorInvitesWidget(new AdministratorInvitesWidget(account, this))
    , mAdministratorCustomEmojiWidget(new AdministratorCustomEmojiWidget(account, this))
    , mViewLogWidget(new ViewLogWidget(account, this))
    , mPermissionsWidget(new PermissionsWidget(account, this))
    , mRolesWidget(new AdministratorRolesWidget(account, this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mTabWidget->setObjectName(QStringLiteral("mTabWidget"));
    mainLayout->addWidget(mTabWidget);

    mAdministratorServerInfoWidget->setObjectName(QStringLiteral("mAdministratorServerInfoWidget"));
    mServerInfoTabIndex = mTabWidget->addTab(mAdministratorServerInfoWidget, i18n("Server Info"));

    mAdministratorRoomsWidget->setObjectName(QStringLiteral("mAdministratorRoomsWidget"));
    mTabWidget->addTab(mAdministratorRoomsWidget, i18n("Rooms"));

    mAdministratorCustomUserStatusWidget->setObjectName(QStringLiteral("mAdministratorCustomUserStatusWidget"));
    mTabWidget->addTab(mAdministratorCustomUserStatusWidget, i18n("Custom User Status"));

    mAdministratorCustomSoundsWidget->setObjectName(QStringLiteral("mAdministratorCustomSoundsWidget"));
    mCustomSoundsTabIndex = mTabWidget->addTab(mAdministratorCustomSoundsWidget, i18n("Custom Sounds"));

    mAdministratorCustomEmojiWidget->setObjectName(QStringLiteral("mAdministratorCustomEmojiWidget"));
    mTabWidget->addTab(mAdministratorCustomEmojiWidget, i18n("Custom Emojis"));

    mAdministratorUsersWidget->setObjectName(QStringLiteral("mAdministratorUsersWidget"));
    mTabWidget->addTab(mAdministratorUsersWidget, i18n("Users"));

    mAdministratorInvitesWidget->setObjectName(QStringLiteral("mAdministratorInvitesWidget"));
    mTabWidget->addTab(mAdministratorInvitesWidget, i18n("Invites"));

    mViewLogWidget->setObjectName(QStringLiteral("mViewLogWidget"));
    mViewLogTagIndex = mTabWidget->addTab(mViewLogWidget, i18n("View Log"));

    mPermissionsWidget->setObjectName(QStringLiteral("mPermissionsWidget"));
    mTabWidget->addTab(mPermissionsWidget, i18n("Permissions"));

    mRolesWidget->setObjectName(QStringLiteral("mRolesWidget"));
    mTabWidget->addTab(mRolesWidget, i18n("Roles"));
}

AdministratorWidget::~AdministratorWidget()
{
}

void AdministratorWidget::initialize()
{
    mAdministratorUsersWidget->initialize();
    mAdministratorRoomsWidget->initialize();
    mAdministratorCustomSoundsWidget->initialize();
    mAdministratorCustomEmojiWidget->initialize();
    mPermissionsWidget->initialize();
    mAdministratorCustomUserStatusWidget->initialize();
    mRolesWidget->initialize();
    mAdministratorServerInfoWidget->initialize();
    mAdministratorInvitesWidget->initialize();
}

void AdministratorWidget::updateUiFromPermission()
{
    if (!mRocketChatAccount->hasPermission(QStringLiteral("manage-sounds"))) {
        mTabWidget->setTabVisible(mCustomSoundsTabIndex, false);
    }

    if (!mRocketChatAccount->hasPermission(QStringLiteral("view-statistics"))) {
        mTabWidget->setTabVisible(mServerInfoTabIndex, false);
    }
    if (!mRocketChatAccount->hasPermission(QStringLiteral("view-logs"))) {
        mTabWidget->setTabVisible(mViewLogTagIndex, false);
    }
}
