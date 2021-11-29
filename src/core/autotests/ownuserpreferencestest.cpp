/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "ownuserpreferencestest.h"
#include "ownuser/ownuserpreferences.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(OwnUserPreferencesTest)
OwnUserPreferencesTest::OwnUserPreferencesTest(QObject *parent)
    : QObject(parent)
{
}

void OwnUserPreferencesTest::shouldHaveDefaultValues()
{
    OwnUserPreferences p;
    QVERIFY(p.highlightWords().isEmpty());
    QVERIFY(p.emailNotificationMode().isEmpty());
    QVERIFY(p.desktopNotifications().isEmpty());
    QVERIFY(p.mobileNotifications().isEmpty());
    QVERIFY(p.convertAsciiEmoji());
    QVERIFY(p.useEmojis());
    QVERIFY(!p.hideRoles());
    QVERIFY(!p.hideAvatars());
}

void OwnUserPreferencesTest::shouldLoadOwnUserPreferencesInfo_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<OwnUserPreferences>("ownUserPreferences");
    {
        OwnUserPreferences preferences;
        QTest::addRow("empty") << QStringLiteral("empty") << preferences;
    }
    {
        OwnUserPreferences preferences;
        preferences.setEmailNotificationMode(QStringLiteral("mentions"));
        preferences.setMobileNotifications(QStringLiteral("nothing"));
        preferences.setDesktopNotifications(QStringLiteral("mentions"));
        preferences.setConvertAsciiEmoji(false);
        preferences.setUseEmojis(false);
        preferences.setIdleTimeLimit(300);
        QTest::addRow("ownuserpreferences1") << QStringLiteral("ownuserpreferences1") << preferences;
    }
}

void OwnUserPreferencesTest::shouldLoadOwnUserPreferencesInfo()
{
    QFETCH(QString, fileName);
    QFETCH(OwnUserPreferences, ownUserPreferences);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/ownuserpreferences/") + fileName + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    OwnUserPreferences r;
    r.parsePreferences(obj);
    const bool result = (r == ownUserPreferences);
    if (!result) {
        qDebug() << "result " << r;
        qDebug() << "expected " << ownUserPreferences;
    }
    QVERIFY(result);
}
