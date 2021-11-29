/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ChannelDeleteJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ChannelDeleteJobTest(QObject *parent = nullptr);
    ~ChannelDeleteJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateUserIdJson();
    void shouldGenerateUserNameJson();
};

