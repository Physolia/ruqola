/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TeamRoomCompleterTest : public QObject
{
    Q_OBJECT
public:
    explicit TeamRoomCompleterTest(QObject *parent = nullptr);
    ~TeamRoomCompleterTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
