/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT ModerationInfo
{
public:
    ModerationInfo();
    ~ModerationInfo() = default;

    Q_REQUIRED_RESULT bool operator==(const ModerationInfo &other) const;

    void parseModerationInfo(const QJsonObject &replyObject);
};

Q_DECLARE_METATYPE(ModerationInfo)
Q_DECLARE_TYPEINFO(ModerationInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationInfo &t);
