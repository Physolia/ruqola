/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDateTime>
#include <QDebug>
#include <QWidget>
class QDateEdit;
class QToolButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorModerationRangeWidget : public QWidget
{
    Q_OBJECT
public:
    struct DateTimeRange {
        QDateTime fromDate;
        QDateTime toDate;
    };
    explicit AdministratorModerationRangeWidget(QWidget *parent = nullptr);
    ~AdministratorModerationRangeWidget() override;

    [[nodiscard]] DateTimeRange range() const;

Q_SIGNALS:
    void rangeChanged();

private:
    void initializeMenu();
    void slotRangeChanged();
    QDateEdit *const mFromDate;
    QDateEdit *const mToDate;
    QToolButton *const mFilterDate;
};
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AdministratorModerationRangeWidget::DateTimeRange &t);
