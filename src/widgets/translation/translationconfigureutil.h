/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QVector>

class QComboBox;
namespace TranslationConfigureUtil
{
void fillComboboxSettings(QComboBox *combo);
Q_REQUIRED_RESULT QVector<QPair<QString, QString>> languages(const QString &language);
}
