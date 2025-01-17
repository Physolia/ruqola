/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecalldialogtest.h"
#include "conferencecalldialog/conferencecalldialog.h"
#include "conferencecalldialog/conferencecallwidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConferenceCallDialogTest)
ConferenceCallDialogTest::ConferenceCallDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ConferenceCallDialogTest::shouldHaveDefaultValues()
{
    ConferenceCallDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mConferenceCallWidget = d.findChild<ConferenceCallWidget *>(QStringLiteral("mConferenceCallWidget"));
    QVERIFY(mConferenceCallWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    auto info = d.conferenceCallInfo();
    QVERIFY(!info.useCamera);
    QVERIFY(!info.useMic);
}

#include "moc_conferencecalldialogtest.cpp"
