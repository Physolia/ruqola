/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QDialog>

class NotificationHistoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NotificationHistoryDialog(QWidget *parent = nullptr);
    ~NotificationHistoryDialog() override;
};
