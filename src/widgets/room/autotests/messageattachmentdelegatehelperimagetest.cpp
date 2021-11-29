/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperimagetest.h"
#include "managerdatapaths.h"
#include "messages/message.h"
#include "messages/messageattachment.h"
#include "rocketchataccount.h"
#include "room/delegate/messageattachmentdelegatehelperimage.h"
#include "ruqola.h"
#include "testdata.h"

#include <QStandardPaths>
#include <QStyleOptionViewItem>
#include <QTest>

QTEST_MAIN(MessageDelegateHelperImageTest)

MessageDelegateHelperImageTest::MessageDelegateHelperImageTest(QObject *parent)
    : QObject(parent)
{
    initTestAccount();
}

void MessageDelegateHelperImageTest::shouldExtractMessageData()
{
    MessageAttachmentDelegateHelperImage helper;
    QStyleOptionViewItem option;
    QWidget fakeWidget;
    option.widget = &fakeWidget;
    const MessageAttachment msgAttach = testAttachment();

    const MessageAttachmentDelegateHelperImage::ImageLayout layout = helper.layoutImage(msgAttach, option, 500, 500);
    QCOMPARE(layout.title, msgAttach.title());
    QCOMPARE(layout.description, msgAttach.description());
    QVERIFY(layout.isShown);
    QVERIFY(!layout.isAnimatedImage);
    QVERIFY(layout.imagePreviewPath.endsWith(QLatin1String("/testfile.png")));
    QVERIFY(qAbs(layout.imageSize.height() - layout.imageSize.width() * 2) <= 1); // aspect ratio is preserved, allow for one pixel rounding diff
    QCOMPARE(layout.pixmap.devicePixelRatioF(), fakeWidget.devicePixelRatioF());
    QVERIFY(layout.imageSize.height() < 500 * fakeWidget.devicePixelRatioF());
}
