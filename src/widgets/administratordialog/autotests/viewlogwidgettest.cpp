/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "viewlogwidgettest.h"
#include "administratordialog/logs/viewlogwidget.h"
#include <QPlainTextEdit>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ViewLogWidgetTest)

ViewLogWidgetTest::ViewLogWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ViewLogWidgetTest::shouldHaveDefaultValues()
{
    ViewLogWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mPlainTextEdit = w.findChild<QPlainTextEdit *>(QStringLiteral("mPlainTextEdit"));
    QVERIFY(mPlainTextEdit);
    QVERIFY(mPlainTextEdit->isReadOnly());
}

#include "moc_viewlogwidgettest.cpp"
