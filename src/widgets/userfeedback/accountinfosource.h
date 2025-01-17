/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#ifdef USE_KUSERFEEDBACK_QT6
#include <KUserFeedbackQt6/AbstractDataSource>
#else
#include <KUserFeedback/AbstractDataSource>
#endif
class AccountInfoSource : public KUserFeedback::AbstractDataSource
{
public:
    AccountInfoSource();
    ~AccountInfoSource() override;

    [[nodiscard]] QString name() const override;
    [[nodiscard]] QString description() const override;
    [[nodiscard]] QVariant data() override;
};
