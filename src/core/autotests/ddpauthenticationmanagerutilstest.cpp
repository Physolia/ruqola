/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ddpauthenticationmanagerutilstest.h"
#include "ddpapi/ddpauthenticationmanagerutils.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(DDPAuthenticationManagerUtilsTest)
DDPAuthenticationManagerUtilsTest::DDPAuthenticationManagerUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void DDPAuthenticationManagerUtilsTest::shouldTestLoginResume()
{
    {
        const QString token = QStringLiteral(R"(dsdf##56;)");
        QCOMPARE(QJsonDocument(DDPAuthenticationManagerUtils::loginResume(token)).toJson(QJsonDocument::Compact), QByteArray("[{\"resume\":\"dsdf##56;\"}]"));
    }

    {
        const QString token = QStringLiteral(R"(foo42";)");
        QCOMPARE(QJsonDocument(DDPAuthenticationManagerUtils::loginResume(token)).toJson(QJsonDocument::Compact), QByteArray("[{\"resume\":\"foo42\\\";\"}]"));
    }
}

void DDPAuthenticationManagerUtilsTest::shouldTestLdapLogin()
{
    {
        const QString user = QStringLiteral(R"(username42)");
        const QString password = QStringLiteral(R"(blafoo4)");
        QCOMPARE(QJsonDocument(DDPAuthenticationManagerUtils::loginLdap(user, password)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"ldap\":true,\"ldapOptions\":{},\"ldapPass\":\"blafoo4\",\"username\":\"username42\"}]"));
    }
    {
        const QString user = QStringLiteral(R"(username42)");
        const QString password = QStringLiteral(R"(45p";)");
        QCOMPARE(QJsonDocument(DDPAuthenticationManagerUtils::loginLdap(user, password)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"ldap\":true,\"ldapOptions\":{},\"ldapPass\":\"45p\\\";\",\"username\":\"username42\"}]"));
    }
}

void DDPAuthenticationManagerUtilsTest::shouldTestloginOAuth()
{
    {
        const QString credentialToken = QStringLiteral(R"(username42)");
        const QString credentialSecret = QStringLiteral(R"(blafoo4)");
        QCOMPARE(QJsonDocument(DDPAuthenticationManagerUtils::loginOAuth(credentialToken, credentialSecret)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"oauth\":{\"credentialSecret\":\"blafoo4\",\"credentialToken\":\"username42\"}}]"));
    }
}
