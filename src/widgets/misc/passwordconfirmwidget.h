/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class KPasswordLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PasswordConfirmWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PasswordConfirmWidget(QWidget *parent = nullptr);
    ~PasswordConfirmWidget() override;
    Q_REQUIRED_RESULT bool isNewPasswordConfirmed() const;
    Q_REQUIRED_RESULT QString password() const;

private:
    KPasswordLineEdit *const mNewPasswordLineEdit;
    KPasswordLineEdit *const mConfirmPasswordLineEdit;
};

