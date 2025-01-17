/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TextPluginManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit TextPluginManagerTest(QObject *parent = nullptr);
    ~TextPluginManagerTest() override = default;
};
