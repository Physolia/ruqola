/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ratelimiterwidgettest.h"
#include "administratorsettingsdialog/ratelimiter/ratelimiterwidget.h"
#include "settingswidgetshelper.h"
#include <QLabel>
#include <QTest>
QTEST_MAIN(RateLimiterWidgetTest)
RateLimiterWidgetTest::RateLimiterWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RateLimiterWidgetTest::shouldHaveDefaultValues()
{
    RateLimiterWidget w(nullptr);

    auto apiRateLimiterLabel = w.findChild<QLabel *>(QStringLiteral("apiRateLimiterLabel"));
    QVERIFY(apiRateLimiterLabel);
    QVERIFY(!apiRateLimiterLabel->text().isEmpty());

    auto mEnableRateLimiter = w.findChild<QCheckBox *>(QStringLiteral("mEnableRateLimiter"));
    QVERIFY(mEnableRateLimiter);
    QVERIFY(!mEnableRateLimiter->isChecked());
    QVERIFY(!mEnableRateLimiter->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableRateLimiter), QStringLiteral("API_Enable_Rate_Limiter"));

    auto ddpRateLimiterLabel = w.findChild<QLabel *>(QStringLiteral("ddpRateLimiterLabel"));
    QVERIFY(ddpRateLimiterLabel);
    QVERIFY(!ddpRateLimiterLabel->text().isEmpty());

    auto mLimitByIP = w.findChild<QCheckBox *>(QStringLiteral("mLimitByIP"));
    QVERIFY(mLimitByIP);
    QVERIFY(!mLimitByIP->isChecked());
    QVERIFY(!mLimitByIP->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimitByIP), QStringLiteral("DDP_Rate_Limit_IP_Enabled"));

    auto mLimitByUser = w.findChild<QCheckBox *>(QStringLiteral("mLimitByUser"));
    QVERIFY(mLimitByUser);
    QVERIFY(!mLimitByUser->isChecked());
    QVERIFY(!mLimitByUser->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimitByUser), QStringLiteral("DDP_Rate_Limit_User_Enabled"));

    auto mLimitByConnection = w.findChild<QCheckBox *>(QStringLiteral("mLimitByConnection"));
    QVERIFY(mLimitByConnection);
    QVERIFY(!mLimitByConnection->isChecked());
    QVERIFY(!mLimitByConnection->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLimitByConnection), QStringLiteral("DDP_Rate_Limit_Connection_Enabled"));
}