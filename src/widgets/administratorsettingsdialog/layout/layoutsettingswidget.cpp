/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "layoutsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>

LayoutSettingsWidget::LayoutSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mDisplayRoles(new QCheckBox(i18n("Display Roles"), this))
    , mUseRealName(new QCheckBox(i18n("Use Real Name"), this))
    , mUseFullNameGenerateDefaultAvatar(new QCheckBox(i18n("Use Full Name Initials to Generate Default Avatar"), this))
    , mAllowSpecialCharactersRoomNames(new QCheckBox(i18n("Allow Special Characters in Room Names"), this))
{
    mDisplayRoles->setObjectName(QStringLiteral("mDisplayRoles"));
    mMainLayout->addWidget(mDisplayRoles);
    connectCheckBox(mDisplayRoles, QStringLiteral("UI_DisplayRoles"));

    mUseRealName->setObjectName(QStringLiteral("mUseRealName"));
    mMainLayout->addWidget(mUseRealName);
    connectCheckBox(mUseRealName, QStringLiteral("UI_Use_Real_Name"));

    mUseFullNameGenerateDefaultAvatar->setObjectName(QStringLiteral("mUseFullNameGenerateDefaultAvatar"));
    mMainLayout->addWidget(mUseFullNameGenerateDefaultAvatar);
    connectCheckBox(mUseFullNameGenerateDefaultAvatar, QStringLiteral("UI_Use_Name_Avatar"));

    mAllowSpecialCharactersRoomNames->setObjectName(QStringLiteral("mAllowSpecialCharactersRoomNames"));
    mMainLayout->addWidget(mAllowSpecialCharactersRoomNames);
    connectCheckBox(mAllowSpecialCharactersRoomNames, QStringLiteral("UI_Allow_room_names_with_special_chars"));
}

LayoutSettingsWidget::~LayoutSettingsWidget() = default;

void LayoutSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mDisplayRoles, mapSettings);
    initializeWidget(mUseRealName, mapSettings);
    initializeWidget(mUseFullNameGenerateDefaultAvatar, mapSettings);
    initializeWidget(mAllowSpecialCharactersRoomNames, mapSettings);
}