/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
#include <QMap>
class LIBRUQOLACORE_TESTS_EXPORT MessageTranslation
{
public:
    MessageTranslation();
    void parse(const QJsonObject &o);
    Q_REQUIRED_RESULT bool operator==(const MessageTranslation &other) const;

    Q_REQUIRED_RESULT QMap<QString, QString> translatedString() const;
    void setTranslatedString(const QMap<QString, QString> &translatedString);

    Q_REQUIRED_RESULT QString translatedStringFromLanguage(const QString &lang);

private:
    QMap<QString, QString> mTranslatedString;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageTranslation &t);
