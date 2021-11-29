/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CustomUserStatusDeleteTestJob : public QObject
{
    Q_OBJECT
public:
    explicit CustomUserStatusDeleteTestJob(QObject *parent = nullptr);
    ~CustomUserStatusDeleteTestJob() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};

