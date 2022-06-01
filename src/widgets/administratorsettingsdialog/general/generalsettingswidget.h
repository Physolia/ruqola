/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT GeneralSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit GeneralSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~GeneralSettingsWidget() override;

    void initialize() override;

private:
    QCheckBox *const mEnableFavoriteRooms;
    QLineEdit *const mSiteUrl;
};
