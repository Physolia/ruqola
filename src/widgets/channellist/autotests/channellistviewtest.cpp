/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "channellistviewtest.h"
#include "channellist/channellistview.h"
#include <QTest>

QTEST_MAIN(ChannelListViewTest)
ChannelListViewTest::ChannelListViewTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelListViewTest::shouldHaveDefaultValues()
{
    ChannelListView w;
    QCOMPARE(w.horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QCOMPARE(w.itemDelegate()->objectName(), QStringLiteral("mChannelListDelegate"));
}
