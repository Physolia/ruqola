/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MarkRoomAsReadJobTest : public QObject
{
    Q_OBJECT
public:
    explicit MarkRoomAsReadJobTest(QObject *parent = nullptr);
    ~MarkRoomAsReadJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};

