/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <plugins/pluginauthentication.h>

class GooglePluginAuthentication : public PluginAuthentication
{
    Q_OBJECT
public:
    explicit GooglePluginAuthentication(QObject *parent = nullptr, const QVariantList & = {});
    ~GooglePluginAuthentication() override;

    PluginAuthenticationInterface *createInterface(QObject *parent) override;
    AuthenticationManager::OauthType type() const override;
    QString name() const override;
    QString iconName() const override;
};

