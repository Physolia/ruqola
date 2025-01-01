/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ChannelUserCompleterTest : public QObject
{
    Q_OBJECT
public:
    explicit ChannelUserCompleterTest(QObject *parent = nullptr);
    ~ChannelUserCompleterTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldReturnCompleterName();
};
