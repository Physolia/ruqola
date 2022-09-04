/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class TranslationConfigureLanguageListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TranslationConfigureLanguageListWidget(QWidget *parent = nullptr);
    ~TranslationConfigureLanguageListWidget() override;
};
