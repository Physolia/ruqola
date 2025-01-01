/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class MyAccount2FaTotpWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit MyAccount2FaTotpWidgetTest(QObject *parent = nullptr);
    ~MyAccount2FaTotpWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
