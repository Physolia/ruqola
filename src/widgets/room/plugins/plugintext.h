/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"
#include <QObject>
class PluginTextInterface;
class LIBRUQOLAWIDGETS_EXPORT PluginText : public QObject
{
    Q_OBJECT
public:
    explicit PluginText(QObject *parent = nullptr);
    ~PluginText() override;

    virtual PluginTextInterface *createInterface(QObject *parent) = 0;
};

