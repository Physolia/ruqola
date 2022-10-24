/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingswidget.h"
#include "accounts/accountsettingswidget.h"
#include "email/emailsettingswidget.h"
#include "encryption/encryptionsettingswidget.h"
#include "enterprise/enterprisesettingswidget.h"
#include "fileupload/fileuploadsettingswidget.h"
#include "general/generalsettingswidget.h"
#include "ircfederation/ircfederationwidget.h"
#include "layout/layoutsettingswidget.h"
#include "ldap/ldapsettingswidget.h"
#include "logs/logssettingswidget.h"
#include "message/messagesettingswidget.h"
#include "password/passwordsettingswidget.h"
#include "ratelimiter/ratelimiterwidget.h"
#include "retentionpolicy/retentionpolicysettingswidget.h"
#include "rocketchataccount.h"
#include "rocketchatbackend.h"
#include "slackbridge/slackbridgewidget.h"
#include "userdatadownload/userdatadownloadwidget.h"
#include "videoconference/videoconferencewidget.h"
#include "webdav/webdavsettingswidget.h"

#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>

AdministratorSettingsWidget::AdministratorSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mTabWidget(new QTabWidget(this))
    , mAccountSettingsWidget(new AccountSettingsWidget(account, this))
    , mEncryptionSettingsWidget(new EncryptionSettingsWidget(account, this))
    , mMessageSettingsWidget(new MessageSettingsWidget(account, this))
    , mUploadFileSettingsWidget(new FileUploadSettingsWidget(account, this))
    , mRetentionPolicySettingsWidget(new RetentionPolicySettingsWidget(account, this))
    , mGeneralSettingsWidget(new GeneralSettingsWidget(account, this))
    , mRateLimiterWidget(new RateLimiterWidget(account, this))
    , mPasswordSettingsWidget(new PasswordSettingsWidget(account, this))
    , mVideoConferenceWidget(new VideoConferenceWidget(account, this))
    , mIrcFederationWidget(new IrcFederationWidget(account, this))
    , mWebDavSettingsWidget(new WebDavSettingsWidget(account, this))
    , mLDapSettingsWidget(new LDapSettingsWidget(account, this))
    , mLayoutSettingsWidget(new LayoutSettingsWidget(account, this))
    , mEnterpriseSettingsWidget(new EnterpriseSettingsWidget(account, this))
    , mUserDataDownloadWidget(new UserDataDownloadWidget(account, this))
    , mSlackBridgeWidget(new SlackBridgeWidget(account, this))
    , mLogsSettingsWidget(new LogsSettingsWidget(account, this))
    , mEmailSettingsWidget(new EmailSettingsWidget(account, this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mTabWidget->setObjectName(QStringLiteral("mTabWidget"));
    mainLayout->addWidget(mTabWidget);

    mAccountSettingsWidget->setObjectName(QStringLiteral("mAccountSettingsWidget"));
    mEncryptionSettingsWidget->setObjectName(QStringLiteral("mEncryptionSettingsWidget"));
    mMessageSettingsWidget->setObjectName(QStringLiteral("mMessageSettingsWidget"));
    mUploadFileSettingsWidget->setObjectName(QStringLiteral("mUploadFileSettingsWidget"));
    mRetentionPolicySettingsWidget->setObjectName(QStringLiteral("mRetentionPolicySettingsWidget"));
    mGeneralSettingsWidget->setObjectName(QStringLiteral("mGeneralSettingsWidget"));
    mRateLimiterWidget->setObjectName(QStringLiteral("mRateLimiterWidget"));
    mPasswordSettingsWidget->setObjectName(QStringLiteral("mPasswordSettingsWidget"));
    mVideoConferenceWidget->setObjectName(QStringLiteral("mVideoConferenceWidget"));
    mIrcFederationWidget->setObjectName(QStringLiteral("mIrcFederationWidget"));
    mWebDavSettingsWidget->setObjectName(QStringLiteral("mWebDavSettingsWidget"));
    mLDapSettingsWidget->setObjectName(QStringLiteral("mLDapSettingsWidget"));
    mLayoutSettingsWidget->setObjectName(QStringLiteral("mLayoutSettingsWidget"));
    mEnterpriseSettingsWidget->setObjectName(QStringLiteral("mEnterpriseSettingsWidget"));
    mUserDataDownloadWidget->setObjectName(QStringLiteral("mUserDataDownloadWidget"));
    mSlackBridgeWidget->setObjectName(QStringLiteral("mSlackBridgeWidget"));
    mLogsSettingsWidget->setObjectName(QStringLiteral("mLogsSettingsWidget"));
    mEmailSettingsWidget->setObjectName(QStringLiteral("mEmailSettingsWidget"));
    mTabWidget->addTab(mAccountSettingsWidget, i18n("Accounts"));
    mTabWidget->addTab(mEncryptionSettingsWidget, i18n("Encryption"));
    mTabWidget->addTab(mMessageSettingsWidget, i18n("Message"));
    mTabWidget->addTab(mUploadFileSettingsWidget, i18n("File Upload"));
    mTabWidget->addTab(mRetentionPolicySettingsWidget, i18n("Retention Policy"));
    mTabWidget->addTab(mGeneralSettingsWidget, i18n("General"));
    mTabWidget->addTab(mRateLimiterWidget, i18n("Rate Limiter"));
    mTabWidget->addTab(mPasswordSettingsWidget, i18n("Password"));
    mTabWidget->addTab(mVideoConferenceWidget, i18n("Video Conference"));
    mTabWidget->addTab(mIrcFederationWidget, i18n("IRC Federation"));
    mTabWidget->addTab(mWebDavSettingsWidget, i18n("Webdav"));
    mTabWidget->addTab(mLDapSettingsWidget, i18n("LDAP"));
    mTabWidget->addTab(mLayoutSettingsWidget, i18n("Layout"));
    mTabWidget->addTab(mEnterpriseSettingsWidget, i18n("Enterprise"));
    mTabWidget->addTab(mUserDataDownloadWidget, i18n("User Data Download"));
    mTabWidget->addTab(mSlackBridgeWidget, i18n("Slack Bridge"));
    mTabWidget->addTab(mLogsSettingsWidget, i18n("Logs"));
    mTabWidget->addTab(mEmailSettingsWidget, i18n("Email"));
    if (mRocketChatAccount) {
        connect(mRocketChatAccount, &RocketChatAccount::publicSettingLoaded, this, &AdministratorSettingsWidget::initialize);
    }
    updateState(false);
}

AdministratorSettingsWidget::~AdministratorSettingsWidget() = default;

void AdministratorSettingsWidget::loadSettings()
{
    mRocketChatAccount->rocketChatBackend()->loadPublicSettings();
}

void AdministratorSettingsWidget::initialize(const QJsonObject &obj)
{
    QJsonArray configs = obj.value(QLatin1String("result")).toArray();
    // qDebug() << " obj " << obj;
    QMap<QString, QVariant> mapSettings;
    for (QJsonValueRef currentConfig : configs) {
        const QJsonObject currentConfObject = currentConfig.toObject();
        const QString id = currentConfObject[QStringLiteral("_id")].toString();
        const QVariant value = currentConfObject[QStringLiteral("value")].toVariant();
        // qDebug() << "id  " << id << " value " << value;
        mapSettings.insert(id, value);
    }

    initializeValues(mAccountSettingsWidget, mapSettings);
    initializeValues(mEncryptionSettingsWidget, mapSettings);
    initializeValues(mMessageSettingsWidget, mapSettings);
    initializeValues(mUploadFileSettingsWidget, mapSettings);
    initializeValues(mRetentionPolicySettingsWidget, mapSettings);
    initializeValues(mGeneralSettingsWidget, mapSettings);
    initializeValues(mRateLimiterWidget, mapSettings);
    initializeValues(mPasswordSettingsWidget, mapSettings);
    initializeValues(mVideoConferenceWidget, mapSettings);
    initializeValues(mIrcFederationWidget, mapSettings);
    initializeValues(mWebDavSettingsWidget, mapSettings);
    initializeValues(mLDapSettingsWidget, mapSettings);
    initializeValues(mLayoutSettingsWidget, mapSettings);
    initializeValues(mEnterpriseSettingsWidget, mapSettings);
    initializeValues(mUserDataDownloadWidget, mapSettings);
    initializeValues(mSlackBridgeWidget, mapSettings);
    initializeValues(mLogsSettingsWidget, mapSettings);
    initializeValues(mEmailSettingsWidget, mapSettings);
    updateState(true);
}

void AdministratorSettingsWidget::initializeValues(SettingsWidgetBase *widget, const QMap<QString, QVariant> &mapSettings)
{
    widget->initialize(mapSettings);
}

void AdministratorSettingsWidget::updateState(bool state)
{
    mAccountSettingsWidget->setEnabled(state);
    mEncryptionSettingsWidget->setEnabled(state);
    mMessageSettingsWidget->setEnabled(state);
    mUploadFileSettingsWidget->setEnabled(state);
    mRetentionPolicySettingsWidget->setEnabled(state);
    mRateLimiterWidget->setEnabled(state);
    mPasswordSettingsWidget->setEnabled(state);
    mVideoConferenceWidget->setEnabled(state);
    mIrcFederationWidget->setEnabled(state);
    mWebDavSettingsWidget->setEnabled(state);
    mLDapSettingsWidget->setEnabled(state);
    mLayoutSettingsWidget->setEnabled(state);
    mEnterpriseSettingsWidget->setEnabled(state);
    mUserDataDownloadWidget->setEnabled(state);
    mSlackBridgeWidget->setEnabled(state);
    mLogsSettingsWidget->setEnabled(state);
    mEmailSettingsWidget->setEnabled(state);
}
