/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DownloadFileJobTest : public QObject
{
    Q_OBJECT
public:
    explicit DownloadFileJobTest(QObject *parent = nullptr);
    ~DownloadFileJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};
