/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TeamsListJobTest : public QObject
{
    Q_OBJECT
public:
    explicit TeamsListJobTest(QObject *parent = nullptr);
    ~TeamsListJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateRequestUsername();
};
