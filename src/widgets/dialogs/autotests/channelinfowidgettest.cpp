/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "channelinfowidgettest.h"
#include "dialogs/channelinfowidget.h"
#include <QStackedWidget>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ChannelInfoWidgetTest)
ChannelInfoWidgetTest::ChannelInfoWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelInfoWidgetTest::shouldHaveDefaultValues()
{
    ChannelInfoWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);
}
