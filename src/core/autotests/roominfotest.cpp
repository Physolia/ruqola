/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "roominfotest.h"
#include "roominfo/roominfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(RoomInfoTest)

RoomInfoTest::RoomInfoTest(QObject *parent)
    : QObject(parent)
{
}

void RoomInfoTest::shouldHaveDefaultValues()
{
    RoomInfo w;
    QVERIFY(!w.defaultRoom());
    QCOMPARE(w.usersCount(), -1);
    QCOMPARE(w.messageCount(), -1);
    QVERIFY(w.channelType().isEmpty());
    QVERIFY(w.topic().isEmpty());
    QVERIFY(w.identifier().isEmpty());
    QVERIFY(!w.readOnly());
    QVERIFY(w.name().isEmpty());
    QCOMPARE(w.lastMessage(), -1);
    QVERIFY(!w.featured());
}
