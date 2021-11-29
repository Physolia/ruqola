/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GetUsersInRoleJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GetUsersInRoleJobTest(QObject *parent = nullptr);
    ~GetUsersInRoleJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldNotStarting();
};
