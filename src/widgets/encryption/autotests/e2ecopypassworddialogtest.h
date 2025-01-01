/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QObject>

class E2eCopyPasswordDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit E2eCopyPasswordDialogTest(QObject *parent = nullptr);
    ~E2eCopyPasswordDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
