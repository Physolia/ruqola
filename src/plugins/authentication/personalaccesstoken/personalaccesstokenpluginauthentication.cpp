/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenpluginauthentication.h"
#include "personalaccesstokenauthenticationinterface.h"
#include <KLocalizedString>
#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(PersonalAccessTokenPluginAuthentication, "ruqola_personalaccesstokenauthenticationplugin.json")

PersonalAccessTokenPluginAuthentication::PersonalAccessTokenPluginAuthentication(QObject *parent, const QVariantList &)
    : PluginAuthentication(parent)
{
}

PersonalAccessTokenPluginAuthentication::~PersonalAccessTokenPluginAuthentication() = default;

PluginAuthenticationInterface *PersonalAccessTokenPluginAuthentication::createInterface(QObject *parent)
{
    return new PersonalAccessTokenAuthenticationInterface(parent);
}

AuthenticationManager::AuthMethodType PersonalAccessTokenPluginAuthentication::authenticationType() const
{
    return AuthenticationManager::AuthMethodType::PersonalAccessToken;
}

QString PersonalAccessTokenPluginAuthentication::name() const
{
    return i18n("Personal Access Token");
}

QString PersonalAccessTokenPluginAuthentication::iconName() const
{
    return QStringLiteral("dialog-password");
}

#include "moc_personalaccesstokenpluginauthentication.cpp"
#include "personalaccesstokenpluginauthentication.moc"
