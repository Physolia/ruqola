/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT FileUploadSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit FileUploadSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~FileUploadSettingsWidget() override;

public:
    void initialize();

private:
    QCheckBox *const mFileUploadsEnabled;
    QCheckBox *const mProtectUploadedFiles;
};
