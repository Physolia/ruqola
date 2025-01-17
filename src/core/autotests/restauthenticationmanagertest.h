/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RESTAuthenticationManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit RESTAuthenticationManagerTest(QObject *parent = nullptr);
    ~RESTAuthenticationManagerTest() override = default;
};
