/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QLineEdit>

#include "libruqolawidgets_private_export.h"
class QTimer;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchWithDelayLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit SearchWithDelayLineEdit(QWidget *parent = nullptr);
    ~SearchWithDelayLineEdit() override;

    void setDelayMs(int ms);

Q_SIGNALS:
    void searchRequested(const QString &str);
    void searchCleared();

private:
    void slotSearchTimerFired();
    void slotSearchTextEdited();
    QTimer *const mSearchTimer;
    int mMilliseconds = 1000;
};

