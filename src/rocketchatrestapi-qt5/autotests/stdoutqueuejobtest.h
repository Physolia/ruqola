/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class StdoutQueueJobTest : public QObject
{
    Q_OBJECT
public:
    explicit StdoutQueueJobTest(QObject *parent = nullptr);
    ~StdoutQueueJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
