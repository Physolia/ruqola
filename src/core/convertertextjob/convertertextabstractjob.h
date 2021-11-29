/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QObject>

class LIBRUQOLACORE_TESTS_EXPORT ConverterTextAbstractJob : public QObject
{
    Q_OBJECT
public:
    explicit ConverterTextAbstractJob(QObject *parent = nullptr);
    ~ConverterTextAbstractJob() override;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &messageId);

    virtual void start() = 0;

private:
    QString mMessageId;
};

