/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "administratorcustomuserstatuswidgettest.h"
#include "administratordialog/customuserstatus/administratorcustomuserstatuswidget.h"
#include <KTreeWidgetSearchLineWidget>
#include <QTest>
#include <QTreeWidget>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorCustomUserStatusWidgetTest)

AdministratorCustomUserStatusWidgetTest::AdministratorCustomUserStatusWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorCustomUserStatusWidgetTest::shouldHaveDefaultValues()
{
    AdministratorCustomUserStatusWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mCustomUserStatusTreeWidget = w.findChild<QTreeWidget *>(QStringLiteral("mCustomUserStatusTreeWidget"));
    QVERIFY(mCustomUserStatusTreeWidget);

    auto mSearchLineWidget = w.findChild<KTreeWidgetSearchLineWidget *>(QStringLiteral("mSearchLineWidget"));
    QVERIFY(mSearchLineWidget);
}
