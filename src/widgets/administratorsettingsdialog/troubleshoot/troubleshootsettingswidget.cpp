/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "troubleshootsettingswidget.h"

TroubleshootSettingsWidget::TroubleshootSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

TroubleshootSettingsWidget::~TroubleshootSettingsWidget() = default;

void TroubleshootSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}