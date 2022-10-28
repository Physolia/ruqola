/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emailsettingswidgettest.h"
#include "administratorsettingsdialog/email/emailsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTest>
QTEST_MAIN(EmailSettingsWidgetTest)
EmailSettingsWidgetTest::EmailSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void EmailSettingsWidgetTest::shouldHaveDefaultValues()
{
    EmailSettingsWidget w(nullptr);
    auto smtpLabel = w.findChild<QLabel *>(QStringLiteral("smtpLabel"));
    QVERIFY(smtpLabel);
    QVERIFY(!smtpLabel->text().isEmpty());

    auto mSmtpHost = w.findChild<QLineEdit *>(QStringLiteral("mSmtpHost"));
    QVERIFY(mSmtpHost);
    QVERIFY(mSmtpHost->text().isEmpty());
    QVERIFY(mSmtpHost->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSmtpHost), QStringLiteral("SMTP_Host"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("SMTP_Host"));

    auto mSmtpPort = w.findChild<QSpinBox *>(QStringLiteral("mSmtpPort"));
    QVERIFY(mSmtpPort);
    QVERIFY(mSmtpPort->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSmtpPort), QStringLiteral("SMTP_Port"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("SMTP_Port"));

    auto mIgnoreTls = w.findChild<QCheckBox *>(QStringLiteral("mIgnoreTls"));
    QVERIFY(mIgnoreTls);
    QVERIFY(!mIgnoreTls->isChecked());
    QVERIFY(!mIgnoreTls->text().isEmpty());
    QVERIFY(mIgnoreTls->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mIgnoreTls), QStringLiteral("SMTP_IgnoreTLS"));

    auto mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    QVERIFY(mUserName->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUserName), QStringLiteral("SMTP_Username"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("SMTP_Username"));

    auto mFromEmail = w.findChild<QLineEdit *>(QStringLiteral("mFromEmail"));
    QVERIFY(mFromEmail);
    QVERIFY(mFromEmail->text().isEmpty());
    QVERIFY(mFromEmail->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFromEmail), QStringLiteral("From_Email"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("From_Email"));

    auto privacyLabel = w.findChild<QLabel *>(QStringLiteral("privacyLabel"));
    QVERIFY(privacyLabel);
    QVERIFY(!privacyLabel->text().isEmpty());

    auto mShowMessageEmailNotification = w.findChild<QCheckBox *>(QStringLiteral("mShowMessageEmailNotification"));
    QVERIFY(mShowMessageEmailNotification);
    QVERIFY(!mShowMessageEmailNotification->isChecked());
    QVERIFY(!mShowMessageEmailNotification->text().isEmpty());
    QVERIFY(mShowMessageEmailNotification->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mShowMessageEmailNotification), QStringLiteral("Email_notification_show_message"));

    auto mAddSenderReplyTo = w.findChild<QCheckBox *>(QStringLiteral("mAddSenderReplyTo"));
    QVERIFY(mAddSenderReplyTo);
    QVERIFY(!mAddSenderReplyTo->isChecked());
    QVERIFY(!mAddSenderReplyTo->text().isEmpty());
    QVERIFY(mAddSenderReplyTo->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAddSenderReplyTo), QStringLiteral("Add_Sender_To_ReplyTo"));

    auto directReplyLabel = w.findChild<QLabel *>(QStringLiteral("directReplyLabel"));
    QVERIFY(directReplyLabel);
    QVERIFY(!directReplyLabel->text().isEmpty());

    auto mEnableDirectReply = w.findChild<QCheckBox *>(QStringLiteral("mEnableDirectReply"));
    QVERIFY(mEnableDirectReply);
    QVERIFY(!mEnableDirectReply->isChecked());
    QVERIFY(!mEnableDirectReply->text().isEmpty());
    QVERIFY(!mEnableDirectReply->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableDirectReply), QStringLiteral("Direct_Reply_Enable"));

    auto mDebugDirectReply = w.findChild<QCheckBox *>(QStringLiteral("mDebugDirectReply"));
    QVERIFY(mDebugDirectReply);
    QVERIFY(!mDebugDirectReply->isChecked());
    QVERIFY(!mDebugDirectReply->text().isEmpty());
    QVERIFY(!mDebugDirectReply->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDebugDirectReply), QStringLiteral("Direct_Reply_Debug"));

    auto forgotPasswordLabel = w.findChild<QLabel *>(QStringLiteral("forgotPasswordLabel"));
    QVERIFY(forgotPasswordLabel);
    QVERIFY(!forgotPasswordLabel->text().isEmpty());
}