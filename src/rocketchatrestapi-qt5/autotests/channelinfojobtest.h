/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ChannelInfoJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ChannelInfoJobTest(QObject *parent = nullptr);
    ~ChannelInfoJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldNotStarting();
};

