/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UsersPresenceJobTest : public QObject
{
    Q_OBJECT
public:
    explicit UsersPresenceJobTest(QObject *parent = nullptr);
    ~UsersPresenceJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};

