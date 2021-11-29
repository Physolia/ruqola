/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DiscussionsFilterProxyModelTest : public QObject
{
    Q_OBJECT
public:
    explicit DiscussionsFilterProxyModelTest(QObject *parent = nullptr);
    ~DiscussionsFilterProxyModelTest() override = default;
private Q_SLOTS:
    void shouldAssignValue();
    void shouldHaveDefaultValues();
};

