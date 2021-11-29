/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QComboBox>

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoomComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit UsersInRoomComboBox(QWidget *parent = nullptr);
    ~UsersInRoomComboBox() override;

private:
    void fillCombobox();
};

