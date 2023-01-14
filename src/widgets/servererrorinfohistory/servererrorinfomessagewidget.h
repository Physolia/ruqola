/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <KMessageWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerErrorInfoMessageWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit ServerErrorInfoMessageWidget(QWidget *parent = nullptr);
    ~ServerErrorInfoMessageWidget() override;
};