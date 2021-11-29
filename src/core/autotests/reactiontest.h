/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ReactionTest : public QObject
{
    Q_OBJECT
public:
    explicit ReactionTest(QObject *parent = nullptr);
    ~ReactionTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldReturnCount();
    void shouldShowReactionsToolTip();
};

