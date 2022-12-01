/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

OauthSettingsWidget::OauthSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mSignWithApple(new QCheckBox(i18n("Sign in with Apple"), this))
    , mAccountsOAuthAppleId(new QLineEdit(this))
    , mAccountsOAuthAppleIss(new QLineEdit(this))
    , mAccountsOAuthAppleKid(new QLineEdit(this))
    , mTwitterLogin(new QCheckBox(i18n("Twitter Login"), this))
    , mTwitterId(new QLineEdit(this))
    , mTwitterSecret(new QLineEdit(this))
    , mTwitterCallbackURL(new QLineEdit(this))
    , mLinkedInLogin(new QCheckBox(i18n("LinkedIn Login"), this))
    , mLinkedInId(new QLineEdit(this))
    , mLinkedInSecret(new QLineEdit(this))
    , mLinkedInCallbackURL(new QLineEdit(this))
    , mProxyHost(new QLineEdit(this))
    , mProxyServices(new QLineEdit(this))
    , mMeteorLogin(new QCheckBox(i18n("Meteor Login"), this))
    , mMeteorId(new QLineEdit(this))
    , mMeteorSecret(new QLineEdit(this))
    , mMeteorCallbackURL(new QLineEdit(this))
    , mFacebookLogin(new QCheckBox(i18n("Facebook Login"), this))
    , mFacebookId(new QLineEdit(this))
    , mFacebookSecret(new QLineEdit(this))
    , mFacebookCallbackURL(new QLineEdit(this))
    , mGithubLogin(new QCheckBox(i18n("Github Login"), this))
    , mGithubId(new QLineEdit(this))
    , mGithubSecret(new QLineEdit(this))
    , mGithubCallbackURL(new QLineEdit(this))
{
    auto appleLabel = createBoldLabel(i18n("Apple"));
    appleLabel->setObjectName(QStringLiteral("appleLabel"));
    mMainLayout->addWidget(appleLabel);

    mSignWithApple->setObjectName(QStringLiteral("mSignWithApple"));
    mMainLayout->addWidget(mSignWithApple);
    mSignWithApple->setToolTip(i18n("If you want Apple login enabled only on mobile, you can leave all fields empty."));
    connectCheckBox(mSignWithApple, QStringLiteral("Accounts_OAuth_Apple"));

    mAccountsOAuthAppleId->setObjectName(QStringLiteral("mAccountsOAuthAppleId"));
    addLineEdit(i18n("OAuth Apple Id"), mAccountsOAuthAppleId, QStringLiteral("Accounts_OAuth_Apple_id"));

    mAccountsOAuthAppleIss->setObjectName(QStringLiteral("mAccountsOAuthAppleIss"));
    addLineEdit(i18n("OAuth Apple Iss"), mAccountsOAuthAppleIss, QStringLiteral("Accounts_OAuth_Apple_iss"));

    mAccountsOAuthAppleKid->setObjectName(QStringLiteral("AccountsOAuthAppleKid"));
    addLineEdit(i18n("OAuth Apple Kid"), mAccountsOAuthAppleKid, QStringLiteral("Accounts_OAuth_Apple_kid"));

    auto dolphinLabel = createBoldLabel(i18n("Dolphin"));
    dolphinLabel->setObjectName(QStringLiteral("dolphinLabel"));
    mMainLayout->addWidget(dolphinLabel);
    // TODO

    auto twitterLabel = createBoldLabel(i18n("Twitter"));
    twitterLabel->setObjectName(QStringLiteral("twitterLabel"));
    mMainLayout->addWidget(twitterLabel);

    mTwitterLogin->setObjectName(QStringLiteral("mTwitterLogin"));
    mMainLayout->addWidget(mTwitterLogin);
    connectCheckBox(mTwitterLogin, QStringLiteral("Accounts_OAuth_Twitter"));

    mTwitterId->setObjectName(QStringLiteral("mTwitterId"));
    addLineEdit(i18n("Twitter Id"), mTwitterId, QStringLiteral("Accounts_OAuth_Twitter_id"));

    mTwitterSecret->setObjectName(QStringLiteral("mTwitterSecret"));
    addLineEdit(i18n("Twitter Secret"), mTwitterSecret, QStringLiteral("Accounts_OAuth_Twitter_secret"));

    mTwitterCallbackURL->setObjectName(QStringLiteral("mTwitterCallbackURL"));
    addLineEdit(i18n("Twitter Secret"), mTwitterCallbackURL, QStringLiteral("Accounts_OAuth_Twitter_callback_url"), true);

    auto linkedInLabel = createBoldLabel(i18n("LinkedIn"));
    linkedInLabel->setObjectName(QStringLiteral("linkedInLabel"));
    mMainLayout->addWidget(linkedInLabel);

    mLinkedInLogin->setObjectName(QStringLiteral("mLinkedInLogin"));
    mMainLayout->addWidget(mLinkedInLogin);
    connectCheckBox(mLinkedInLogin, QStringLiteral("Accounts_OAuth_Linkedin"));

    mLinkedInId->setObjectName(QStringLiteral("mLinkedInId"));
    addLineEdit(i18n("LinkedIn Id"), mLinkedInId, QStringLiteral("Accounts_OAuth_Linkedin_id"));

    mLinkedInSecret->setObjectName(QStringLiteral("mLinkedInSecret"));
    addLineEdit(i18n("LinkedIn Secret"), mLinkedInSecret, QStringLiteral("Accounts_OAuth_Linkedin_secret"));

    mLinkedInCallbackURL->setObjectName(QStringLiteral("mLinkedInCallbackURL"));
    addLineEdit(i18n("LinkedIn Secret"), mLinkedInCallbackURL, QStringLiteral("Accounts_OAuth_Linkedin_callback_url"), true);

    auto proxyLabel = createBoldLabel(i18n("Proxy"));
    proxyLabel->setObjectName(QStringLiteral("proxyLabel"));
    mMainLayout->addWidget(proxyLabel);

    mProxyHost->setObjectName(QStringLiteral("mProxyHost"));
    addLineEdit(i18n("Proxy Host"), mProxyHost, QStringLiteral("Accounts_OAuth_Proxy_host"));

    mProxyServices->setObjectName(QStringLiteral("mProxyServices"));
    addLineEdit(i18n("Proxy Services"), mProxyServices, QStringLiteral("Accounts_OAuth_Proxy_services"));

    // Meteor
    auto meteorLabel = createBoldLabel(i18n("Meteor"));
    meteorLabel->setObjectName(QStringLiteral("meteorLabel"));
    mMainLayout->addWidget(meteorLabel);

    mMeteorLogin->setObjectName(QStringLiteral("mMeteorLogin"));
    mMainLayout->addWidget(mMeteorLogin);
    connectCheckBox(mMeteorLogin, QStringLiteral("Accounts_OAuth_Meteor"));

    mMeteorId->setObjectName(QStringLiteral("mMeteorId"));
    addLineEdit(i18n("Meteor Id"), mMeteorId, QStringLiteral("Accounts_OAuth_Meteor_id"));

    mMeteorSecret->setObjectName(QStringLiteral("mMeteorSecret"));
    addLineEdit(i18n("Meteor Secret"), mMeteorSecret, QStringLiteral("Accounts_OAuth_Meteor_secret"));

    mMeteorCallbackURL->setObjectName(QStringLiteral("mMeteorCallbackURL"));
    addLineEdit(i18n("Meteor Secret"), mMeteorCallbackURL, QStringLiteral("Accounts_OAuth_Meteor_callback_url"), true);

    // Facebook
    auto facebookLabel = createBoldLabel(i18n("Facebook"));
    facebookLabel->setObjectName(QStringLiteral("facebookLabel"));
    mMainLayout->addWidget(facebookLabel);

    mFacebookLogin->setObjectName(QStringLiteral("mFacebookLogin"));
    mMainLayout->addWidget(mFacebookLogin);
    connectCheckBox(mFacebookLogin, QStringLiteral("Accounts_OAuth_Facebook"));

    mFacebookId->setObjectName(QStringLiteral("mFacebookId"));
    addLineEdit(i18n("Facebook Id"), mFacebookId, QStringLiteral("Accounts_OAuth_Facebook_id"));

    mFacebookSecret->setObjectName(QStringLiteral("mFacebookSecret"));
    addLineEdit(i18n("Facebook Secret"), mFacebookSecret, QStringLiteral("Accounts_OAuth_Facebook_secret"));

    mFacebookCallbackURL->setObjectName(QStringLiteral("mFacebookCallbackURL"));
    addLineEdit(i18n("Facebook Secret"), mFacebookCallbackURL, QStringLiteral("Accounts_OAuth_Facebook_callback_url"), true);

    // Github
    auto githubLabel = createBoldLabel(i18n("Github"));
    githubLabel->setObjectName(QStringLiteral("githubLabel"));
    mMainLayout->addWidget(githubLabel);

    mGithubLogin->setObjectName(QStringLiteral("mGithubLogin"));
    mMainLayout->addWidget(mGithubLogin);
    connectCheckBox(mGithubLogin, QStringLiteral("Accounts_OAuth_Github"));

    mGithubId->setObjectName(QStringLiteral("mGithubId"));
    addLineEdit(i18n("Github Id"), mGithubId, QStringLiteral("Accounts_OAuth_Github_id"));

    mGithubSecret->setObjectName(QStringLiteral("mGithubSecret"));
    addLineEdit(i18n("Github Secret"), mGithubSecret, QStringLiteral("Accounts_OAuth_Github_secret"));

    mGithubCallbackURL->setObjectName(QStringLiteral("mGithubCallbackURL"));
    addLineEdit(i18n("Github Secret"), mGithubCallbackURL, QStringLiteral("Accounts_OAuth_Github_callback_url"), true);
}

OauthSettingsWidget::~OauthSettingsWidget() = default;

void OauthSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mSignWithApple, mapSettings, false);
    initializeWidget(mAccountsOAuthAppleId, mapSettings, QString());
    initializeWidget(mAccountsOAuthAppleIss, mapSettings, QString());
    initializeWidget(mAccountsOAuthAppleKid, mapSettings, QString());
    initializeWidget(mTwitterLogin, mapSettings, false);
    initializeWidget(mTwitterId, mapSettings, QString());
    initializeWidget(mTwitterSecret, mapSettings, QString());
    initializeWidget(mTwitterCallbackURL, mapSettings, urlFromRelativePath(QStringLiteral("_oauth/twitter")));

    initializeWidget(mLinkedInLogin, mapSettings, false);
    initializeWidget(mLinkedInId, mapSettings, QString());
    initializeWidget(mLinkedInSecret, mapSettings, QString());
    initializeWidget(mLinkedInCallbackURL, mapSettings, urlFromRelativePath(QStringLiteral("_oauth/linkedin")));

    initializeWidget(mProxyHost, mapSettings, QStringLiteral("https://oauth-proxy.rocket.chat"));
    initializeWidget(mProxyServices, mapSettings, QString());

    initializeWidget(mMeteorLogin, mapSettings, false);
    initializeWidget(mMeteorId, mapSettings, QString());
    initializeWidget(mMeteorSecret, mapSettings, QString());
    initializeWidget(mMeteorCallbackURL, mapSettings, urlFromRelativePath(QStringLiteral("_oauth/meteor")));

    initializeWidget(mFacebookLogin, mapSettings, false);
    initializeWidget(mFacebookId, mapSettings, QString());
    initializeWidget(mFacebookSecret, mapSettings, QString());
    initializeWidget(mFacebookCallbackURL, mapSettings, urlFromRelativePath(QStringLiteral("_oauth/facebook")));

    initializeWidget(mGithubLogin, mapSettings, false);
    initializeWidget(mGithubId, mapSettings, QString());
    initializeWidget(mGithubSecret, mapSettings, QString());
    initializeWidget(mGithubCallbackURL, mapSettings, urlFromRelativePath(QStringLiteral("_oauth/github")));
}