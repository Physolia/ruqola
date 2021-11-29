/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "messagetexteditortest.h"
#include "dialogs/messagetexteditor.h"
#include <QTest>
QTEST_MAIN(MessageTextEditorTest)
MessageTextEditorTest::MessageTextEditorTest(QObject *parent)
    : QObject(parent)
{
}

void MessageTextEditorTest::shouldHaveDefaultValues()
{
    MessageTextEditor d;
    QVERIFY(!d.acceptRichText());
    QVERIFY(d.toPlainText().isEmpty());
}
