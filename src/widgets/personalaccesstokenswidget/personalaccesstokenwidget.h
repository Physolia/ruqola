/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class PersonalAccessTokenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PersonalAccessTokenWidget(QWidget *parent = nullptr);
    ~PersonalAccessTokenWidget() override;
};
