/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class WhatsNewComboBoxWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit WhatsNewComboBoxWidgetTest(QObject *parent = nullptr);
    ~WhatsNewComboBoxWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldInitializeComboBox();
    void shouldEmitVersionChanged();
};