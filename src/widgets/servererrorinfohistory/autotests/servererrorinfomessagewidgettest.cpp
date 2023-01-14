/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagewidgettest.h"
#include "servererrorinfohistory/servererrorinfomessagewidget.h"
#include <QTest>
QTEST_MAIN(ServerErrorInfoMessageWidgetTest)

ServerErrorInfoMessageWidgetTest::ServerErrorInfoMessageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ServerErrorInfoMessageWidgetTest::shouldHaveDefaultValues()
{
    ServerErrorInfoMessageWidget w;
    QVERIFY(!w.isVisible());
    QVERIFY(w.isCloseButtonVisible());
    QCOMPARE(w.messageType(), KMessageWidget::Error);
}