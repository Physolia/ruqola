/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageTextEditorTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageTextEditorTest(QObject *parent = nullptr);
    ~MessageTextEditorTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
