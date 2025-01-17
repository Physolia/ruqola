/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RetentionPolicySettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit RetentionPolicySettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~RetentionPolicySettingsWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mEnabled;
    QCheckBox *const mUseAdvancedRetentionPolicyConfiguration;
    QCheckBox *const mDontPruneThreads;
    QCheckBox *const mDontPruneDiscussion;
    QCheckBox *const mDontPrunePinnedMessages;
    QCheckBox *const mOnlyDeleteFiles;
    QCheckBox *const mAppliesChannels;
    QCheckBox *const mAppliesGroups;
    QCheckBox *const mAppliesMessages;
    QSpinBox *const mMaximumMessageDirectMessages;
    QSpinBox *const mMaximumMessagePrivateGroups;
    QSpinBox *const mMaximumMessageChannels;
    QComboBox *const mTimerPrecision;
};
