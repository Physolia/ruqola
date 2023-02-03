/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QString>

class LocalDatabaseBase
{
public:
    explicit LocalDatabaseBase(const QString &basePath);
    ~LocalDatabaseBase();
    Q_REQUIRED_RESULT QString dbFileName(const QString &accountName, const QString &roomName) const;

protected:
    const QString mBasePath;
};
