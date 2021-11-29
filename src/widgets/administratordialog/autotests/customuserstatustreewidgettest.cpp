/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "customuserstatustreewidgettest.h"
#include "administratordialog/customuserstatus/customuserstatustreewidget.h"
#include <QHeaderView>
#include <QTest>
QTEST_MAIN(CustomUserStatusTreeWidgetTest)

CustomUserStatusTreeWidgetTest::CustomUserStatusTreeWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void CustomUserStatusTreeWidgetTest::shouldHaveDefaultValues()
{
    CustomUserStatusTreeWidget w(nullptr);
    QCOMPARE(w.columnCount(), 2);
    QVERIFY(w.alternatingRowColors());
    QCOMPARE(w.contextMenuPolicy(), Qt::CustomContextMenu);
    QCOMPARE(w.selectionMode(), QTreeWidget::SingleSelection);
    QVERIFY(!w.header()->sectionsMoved());
    QVERIFY(!w.rootIsDecorated());
}
