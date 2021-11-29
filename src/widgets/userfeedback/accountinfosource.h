/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KUserFeedback/AbstractDataSource>

class AccountInfoSource : public KUserFeedback::AbstractDataSource
{
public:
    AccountInfoSource();
    ~AccountInfoSource() override;

    Q_REQUIRED_RESULT QString name() const override;
    Q_REQUIRED_RESULT QString description() const override;
    Q_REQUIRED_RESULT QVariant data() override;
};

