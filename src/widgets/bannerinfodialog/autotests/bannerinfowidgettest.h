/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class BannerInfoWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit BannerInfoWidgetTest(QObject *parent = nullptr);
    ~BannerInfoWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};