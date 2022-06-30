/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT VideoConferenceWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit VideoConferenceWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~VideoConferenceWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;
};
