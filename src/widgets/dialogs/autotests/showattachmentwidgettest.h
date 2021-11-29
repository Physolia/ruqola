/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ShowAttachmentWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ShowAttachmentWidgetTest(QObject *parent = nullptr);
    ~ShowAttachmentWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

