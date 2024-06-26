/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "directorycontainerwidgettest.h"
#include "directory/directorycontainerwidget.h"
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(DirectoryContainerWidgetTest)
DirectoryContainerWidgetTest::DirectoryContainerWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void DirectoryContainerWidgetTest::shouldHaveDefaultValues()
{
    DirectoryContainerWidget w(nullptr, DirectoryWidget::DirectoryType::Room);
    // TODO
}
