/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>

class RocketChatAccount;
class QTabWidget;
class AccountSettingsWidget;
class EncryptionSettingsWidget;
class MessageSettingsWidget;
class FileUploadSettingsWidget;
class RetentionPolicySettingsWidget;
class GeneralSettingsWidget;
class RateLimiterWidget;
class PasswordSettingsWidget;
class VideoConferenceWidget;
class IrcFederationWidget;
class WebDavSettingsWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorSettingsWidget() override;

    void loadSettings();

private:
    void initialize(const QJsonObject &obj);
    void updateState(bool state);
    QTabWidget *const mTabWidget;
    AccountSettingsWidget *const mAccountSettingsWidget;
    EncryptionSettingsWidget *const mEncryptionSettingsWidget;
    MessageSettingsWidget *const mMessageSettingsWidget;
    FileUploadSettingsWidget *const mUploadFileSettingsWidget;
    RetentionPolicySettingsWidget *const mRetentionPolicySettingsWidget;
    GeneralSettingsWidget *const mGeneralSettingsWidget;
    RateLimiterWidget *const mRateLimiterWidget;
    PasswordSettingsWidget *const mPasswordSettingsWidget;
    VideoConferenceWidget *const mVideoConferenceWidget;
    IrcFederationWidget *const mIrcFederationWidget;
    WebDavSettingsWidget *const mWebDavSettingsWidget;
    RocketChatAccount *const mRocketChatAccount;
};
