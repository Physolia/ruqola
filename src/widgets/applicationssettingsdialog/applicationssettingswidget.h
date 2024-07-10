/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "applicationssettingssearchwidget.h"
#include "libruqolawidgets_private_export.h"
#include "rocketchataccount.h"
#include <QWidget>
class RocketChatAccount;
class ApplicationsSettingsListView;
class QStackedWidget;
class ApplicationsSettingsInProgressWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsWidget() override;

    void setFeature(ApplicationsSettingsSearchWidget::Feature feature);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFilterChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSortingChanged();
    RocketChatAccount *const mCurrentRocketChatAccount;
    ApplicationsSettingsSearchWidget *const mApplicationsSettingsSearchWidget;
    ApplicationsSettingsListView *const mApplicationsSettingsListView;
    ApplicationsSettingsInProgressWidget *const mApplicationsSettingsInProgressWidget;
    QStackedWidget *const mStackedWidget;
    QWidget *const mWidgetListView;
};
