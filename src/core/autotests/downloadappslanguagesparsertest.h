/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DownloadAppsLanguagesParserTest : public QObject
{
    Q_OBJECT
public:
    explicit DownloadAppsLanguagesParserTest(QObject *parent = nullptr);
    ~DownloadAppsLanguagesParserTest() override = default;
private Q_SLOTS:
    void shouldParseFile_data();
    void shouldParseFile();
};

