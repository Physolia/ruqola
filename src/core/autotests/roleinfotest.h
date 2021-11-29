/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoleInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit RoleInfoTest(QObject *parent = nullptr);
    ~RoleInfoTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadRoles_data();
    void shouldLoadRoles();
};
