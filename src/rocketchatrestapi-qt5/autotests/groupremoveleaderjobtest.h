/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GroupRemoveLeaderJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GroupRemoveLeaderJobTest(QObject *parent = nullptr);
    ~GroupRemoveLeaderJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
};

