/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "myaccountprofileconfigurewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "dialogs/asktwoauthenticationpassworddialog.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "misc/passwordconfirmwidget.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <KPasswordDialog>
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>

MyAccountProfileConfigureWidget::MyAccountProfileConfigureWidget(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mName = new QLineEdit(this);
    mName->setObjectName(QStringLiteral("mName"));
    new LineEditCatchReturnKey(mName, this);
    mainLayout->addRow(i18n("Name:"), mName);

    mUserName = new QLineEdit(this);
    mUserName->setObjectName(QStringLiteral("mUserName"));
    new LineEditCatchReturnKey(mUserName, this);
    mainLayout->addRow(i18n("Username:"), mUserName);

    mEmail = new QLineEdit(this);
    mEmail->setObjectName(QStringLiteral("mEmail"));
    new LineEditCatchReturnKey(mEmail, this);
    mainLayout->addRow(i18n("Email:"), mEmail);

    mNickName = new QLineEdit(this);
    mNickName->setObjectName(QStringLiteral("mNickName"));
    new LineEditCatchReturnKey(mNickName, this);
    mainLayout->addRow(i18n("Nickname:"), mNickName);

    mStatusText = new QLineEdit(this);
    new LineEditCatchReturnKey(mStatusText, this);
    mStatusText->setObjectName(QStringLiteral("mStatusText"));
    mainLayout->addRow(i18n("Status Text:"), mStatusText);

    mPasswordConfirmWidget = new PasswordConfirmWidget(this);
    mPasswordConfirmWidget->setObjectName(QStringLiteral("mPasswordConfirmWidget"));
    mainLayout->addRow(mPasswordConfirmWidget);

    mDeleteMyAccount = new QPushButton(i18n("Delete my Account"), this);
    mDeleteMyAccount->setObjectName(QStringLiteral("mDeleteMyAccount"));
    mainLayout->addWidget(mDeleteMyAccount);
    connect(mDeleteMyAccount, &QPushButton::clicked, this, &MyAccountProfileConfigureWidget::slotDeleteMyAccount);
    init();
}

MyAccountProfileConfigureWidget::~MyAccountProfileConfigureWidget()
{
}

void MyAccountProfileConfigureWidget::slotDeleteMyAccount()
{
    if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Do you really delete your account ?"), i18n("Delete my Account"))) {
        QPointer<KPasswordDialog> dlg = new KPasswordDialog(this);
        dlg->setPrompt(i18n("Current Password"));
        if (dlg->exec()) {
            qDebug() << " void MyAccountProfileConfigureWidget::slotDeleteMyAccount() not implemented yet";
            Ruqola::self()->rocketChatAccount()->deleteOwnAccount(dlg->password());
        }
        delete dlg;
    }
}

void MyAccountProfileConfigureWidget::init()
{
    mUserName->setReadOnly(!Ruqola::self()->rocketChatAccount()->allowUsernameChange());
    mEmail->setReadOnly(!Ruqola::self()->rocketChatAccount()->allowEmailChange());
    mPasswordConfirmWidget->setVisible(Ruqola::self()->rocketChatAccount()->allowPasswordChange());
    mDeleteMyAccount->setVisible(Ruqola::self()->rocketChatAccount()->allowDeleteOwnAccount());
}

void MyAccountProfileConfigureWidget::load()
{
    mOwnUser = Ruqola::self()->rocketChatAccount()->ownUser();
    mEmail->setText(mOwnUser.email());
    mName->setText(mOwnUser.name());
    mUserName->setText(mOwnUser.userName());
    mNickName->setText(mOwnUser.nickName());
    mStatusText->setText(mOwnUser.statusText());
}

void MyAccountProfileConfigureWidget::save()
{
    RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo updateInfo;
    if (!mEmail->isReadOnly() && (mOwnUser.email() != mEmail->text())) {
        updateInfo.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::Email;
        updateInfo.email = mEmail->text();
    }
    if (!mNickName->isReadOnly() && (mOwnUser.nickName() != mNickName->text())) {
        updateInfo.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::NickName;
        updateInfo.nickName = mNickName->text();
    }
    if (!mUserName->isReadOnly() && (mOwnUser.userName() != mUserName->text())) {
        updateInfo.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::UserName;
        updateInfo.userName = mUserName->text();
    }
    if (!mStatusText->isReadOnly() && (mOwnUser.statusText() != mStatusText->text())) {
        updateInfo.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::StatusText;
        updateInfo.statusText = mStatusText->text();
    }
    if (!mName->isReadOnly() && (mOwnUser.name() != mName->text())) {
        updateInfo.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::Name;
        updateInfo.name = mName->text();
    }
    if (mPasswordConfirmWidget->isVisible() && mPasswordConfirmWidget->isNewPasswordConfirmed()) {
        updateInfo.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::Password;
        updateInfo.newPassword = mPasswordConfirmWidget->password(); //TODO use ssha256 ???
    }
    if (Ruqola::self()->rocketChatAccount()->ownUser().servicePassword().email2faEnabled()) { //TODO verify it
        QPointer<AskTwoAuthenticationPasswordDialog> dlg = new AskTwoAuthenticationPasswordDialog(this);
        QString code;
        if (dlg->exec()) {
            code = dlg->code();
        }
    }

    //TODO add more.
    if (updateInfo.isValid()) {
        Ruqola::self()->rocketChatAccount()->updateOwnBasicInfo(updateInfo);
    }
}
