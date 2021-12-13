/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountconfigurewidget.h"
#include "myaccount2e2configurewidget.h"
#include "myaccount2faconfigurewidget.h"
#include "myaccountpreferenceconfigurewidget.h"
#include "myaccountprofileconfigurewidget.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>

MyAccountConfigureWidget::MyAccountConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mMyAccount2FaConfigureWidget(new MyAccount2FaConfigureWidget(account, this))
    , mMyAccount2ProfileConfigureWidget(new MyAccountProfileConfigureWidget(account, this))
    , mMyAccountPreferenceConfigureWidget(new MyAccountPreferenceConfigureWidget(account, this))
    , mMyAccount2e2ConfigureWidget(new MyAccount2e2ConfigureWidget(account, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto tabWidget = new QTabWidget(this);
    tabWidget->setObjectName(QStringLiteral("tabWidget"));
    mainLayout->addWidget(tabWidget);

    mMyAccount2ProfileConfigureWidget->setObjectName(QStringLiteral("mMyAccount2ProfileConfigureWidget"));
    tabWidget->addTab(mMyAccount2ProfileConfigureWidget, i18n("Profile"));

    mMyAccount2FaConfigureWidget->setObjectName(QStringLiteral("mMyAccount2FaConfigureWidget"));
    tabWidget->addTab(mMyAccount2FaConfigureWidget, i18n("Two Authentication Factor"));
    if (account && !account->allowProfileChange()) {
        mMyAccount2ProfileConfigureWidget->setVisible(false);
    }

    mMyAccount2e2ConfigureWidget->setObjectName(QStringLiteral("mMyAccount2e2ConfigureWidget"));
    int index = tabWidget->addTab(mMyAccount2e2ConfigureWidget, i18n("E2E Encryption"));
    // TODO
    // if (account && !account->allowProfileChange()) {
    tabWidget->setTabVisible(index, false);
    // }

    mMyAccountPreferenceConfigureWidget->setObjectName(QStringLiteral("mMyAccountPreferenceConfigureWidget"));
    tabWidget->addTab(mMyAccountPreferenceConfigureWidget, i18n("Preference"));
}

MyAccountConfigureWidget::~MyAccountConfigureWidget() = default;

void MyAccountConfigureWidget::save()
{
    mMyAccount2ProfileConfigureWidget->save();
    mMyAccount2FaConfigureWidget->save();
    mMyAccountPreferenceConfigureWidget->save();
    mMyAccount2e2ConfigureWidget->save();
}

void MyAccountConfigureWidget::load()
{
    mMyAccount2ProfileConfigureWidget->load();
    mMyAccount2FaConfigureWidget->load();
    mMyAccountPreferenceConfigureWidget->load();
    mMyAccount2e2ConfigureWidget->load();
}

void MyAccountConfigureWidget::initialize()
{
    mMyAccount2ProfileConfigureWidget->initialize();
}
