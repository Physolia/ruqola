/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ReactionsTest : public QObject
{
    Q_OBJECT
public:
    explicit ReactionsTest(QObject *parent = nullptr);
    ~ReactionsTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldParseReactions_data();
    void shouldParseReactions();
};

