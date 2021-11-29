/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ServerInfoJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ServerInfoJobTest(QObject *parent = nullptr);
    ~ServerInfoJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};

