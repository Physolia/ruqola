/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationoptionstest.h"
#include "notifications/notificationoptions.h"
#include <QTest>
QTEST_GUILESS_MAIN(NotificationOptionsTest)

NotificationOptionsTest::NotificationOptionsTest(QObject *parent)
    : QObject(parent)
{
}

void NotificationOptionsTest::shouldHaveDefaultValue()
{
    NotificationOptions w;
    QVERIFY(w.audioNotifications().isEmpty());
    QVERIFY(w.desktopNotifications().isEmpty());
    QVERIFY(w.mobilePushNotification().isEmpty());
    QVERIFY(w.emailNotifications().isEmpty());
    QVERIFY(w.unreadTrayIconAlert().isEmpty());
    QVERIFY(!w.disableNotifications());
    QVERIFY(!w.hideUnreadStatus());
    QVERIFY(!w.muteGroupMentions());
    QVERIFY(w.audioNotificationValue().isEmpty());
}

void NotificationOptionsTest::shouldAssignValue()
{
    NotificationOptions w;
    QString audioNotifications = QStringLiteral("foo");
    w.setAudioNotifications(audioNotifications);

    QString desktopNotifications = QStringLiteral("bla");
    w.setDesktopNotifications(desktopNotifications);

    QString mobilePushNotification = QStringLiteral("bli");
    w.setMobilePushNotification(mobilePushNotification);

    QString emailNotifications = QStringLiteral("blu");
    w.setEmailNotifications(emailNotifications);

    QString unreadTrayIconAlert = QStringLiteral("Ablu");
    w.setUnreadTrayIconAlert(unreadTrayIconAlert);

    QString audioNotificationValue = QStringLiteral("ZZZZ");
    w.setAudioNotificationValue(audioNotificationValue);

    bool disableNotifications = true;
    w.setDisableNotifications(disableNotifications);

    bool hideUnreadStatus = true;
    w.setHideUnreadStatus(hideUnreadStatus);

    bool muteGroupMentions = true;
    w.setMuteGroupMentions(muteGroupMentions);

    QCOMPARE(w.audioNotifications(), audioNotifications);
    QCOMPARE(w.desktopNotifications(), desktopNotifications);
    QCOMPARE(w.mobilePushNotification(), mobilePushNotification);
    QCOMPARE(w.emailNotifications(), emailNotifications);
    QCOMPARE(w.unreadTrayIconAlert(), unreadTrayIconAlert);
    QCOMPARE(w.audioNotificationValue(), audioNotificationValue);
    QCOMPARE(w.disableNotifications(), disableNotifications);
    QCOMPARE(w.hideUnreadStatus(), hideUnreadStatus);
    QCOMPARE(w.muteGroupMentions(), muteGroupMentions);

    NotificationOptions t;
    t = w;
    QCOMPARE(t, w);
}
