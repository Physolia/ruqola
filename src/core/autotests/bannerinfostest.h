/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class BannerInfosTest : public QObject
{
    Q_OBJECT
public:
    explicit BannerInfosTest(QObject *parent = nullptr);
    ~BannerInfosTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadRoles_data();
    void shouldLoadRoles();
};