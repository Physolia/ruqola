/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class ApplicationsSettingsDescriptionWidget;
class ApplicationsSettingsVersionWidget;
class ApplicationsSettingsLogsWidget;
class QTabWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsDescriptionTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsDescriptionTabWidget(QWidget *parent = nullptr);
    ~ApplicationsSettingsDescriptionTabWidget() override;
    void setDescription(const QString &desc);

private:
    QTabWidget *const mTabWidget;
    ApplicationsSettingsDescriptionWidget *const mDescriptionWidget;
    ApplicationsSettingsVersionWidget *const mVersionWidget;
    ApplicationsSettingsLogsWidget *const mLogsWidget;
};
