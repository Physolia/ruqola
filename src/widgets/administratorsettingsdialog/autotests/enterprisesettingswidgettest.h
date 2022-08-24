/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class EnterpriseSettingsWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit EnterpriseSettingsWidgetTest(QObject *parent = nullptr);
    ~EnterpriseSettingsWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};