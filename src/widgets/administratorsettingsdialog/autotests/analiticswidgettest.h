/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AnaliticsWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AnaliticsWidgetTest(QObject *parent = nullptr);
    ~AnaliticsWidgetTest() override = default;
};
