/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include "localdatabase/localmessagedatabase.h"
#include <QWidget>
class RocketChatAccount;
class MessageListView;
class QLineEdit;
class MessagesModel;
class QSpinBox;
class QDateTimeEdit;
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseWidget() override;

private:
    void slotLoad();
    RocketChatAccount *const mRocketChatAccount;
    MessageListView *const mMessageListView;
    std::unique_ptr<LocalMessageDatabase> mLocalMessageDatabase;
    QLineEdit *const mRoomName;
    QSpinBox *const mNumberOfMessages;
    QCheckBox *const mUseStartDateTime;
    QDateTimeEdit *const mStartDateTime;
    QCheckBox *const mUseEndDateTime;
    QDateTimeEdit *const mEndDateTime;
    MessagesModel *const mMessageModel;
};
