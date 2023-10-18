/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ArchiveGroupsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ArchiveGroupsJobTest(QObject *parent = nullptr);
    ~ArchiveGroupsJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};