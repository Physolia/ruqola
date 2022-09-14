/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorywidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/notificationhistorymodel.h"
#include "model/notificationhistorymodelfilterproxymodel.h"
#include "notificationhistorylistview.h"
#include "notificationhistorymanager.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QApplication>
#include <QClipboard>
#include <QLineEdit>
#include <QListView>
#include <QMenu>
#include <QVBoxLayout>

NotificationHistoryWidget::NotificationHistoryWidget(QWidget *parent)
    : QWidget{parent}
    , mListNotificationsListView(new NotificationHistoryListView(this))
    , mSearchLineEdit(new QLineEdit(this))
    , mNotificationFilterProxyModel(new NotificationHistoryModelFilterProxyModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto searchLayout = new QHBoxLayout;
    searchLayout->setObjectName(QStringLiteral("searchLayout"));
    searchLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search..."));
    searchLayout->addWidget(mSearchLineEdit);
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);

    mainLayout->addLayout(searchLayout);

    mListNotificationsListView->setObjectName(QStringLiteral("mListNotifications"));
    mainLayout->addWidget(mListNotificationsListView);

    auto model = NotificationHistoryManager::self()->notificationHistoryModel();

    mNotificationFilterProxyModel->setObjectName(QStringLiteral("mNotificationFilterProxyModel"));
    mNotificationFilterProxyModel->setSourceModel(model);
    mListNotificationsListView->setModel(mNotificationFilterProxyModel);

    connect(mListNotificationsListView, &QListView::doubleClicked, this, &NotificationHistoryWidget::slotShowMessage);
    connect(mListNotificationsListView, &NotificationHistoryListView::showMessage, this, &NotificationHistoryWidget::slotShowMessage);

    connect(model, &QAbstractItemModel::rowsAboutToBeInserted, mListNotificationsListView, &MessageListViewBase::checkIfAtBottom);
    connect(model, &QAbstractItemModel::rowsAboutToBeRemoved, mListNotificationsListView, &MessageListViewBase::checkIfAtBottom);
    connect(model, &QAbstractItemModel::modelAboutToBeReset, mListNotificationsListView, &MessageListViewBase::checkIfAtBottom);

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &NotificationHistoryWidget::slotTextChanged);
}

NotificationHistoryWidget::~NotificationHistoryWidget() = default;

void NotificationHistoryWidget::slotTextChanged(const QString &str)
{
    mNotificationFilterProxyModel->setFilterString(str);
    mListNotificationsListView->setSearchText(str);
}

void NotificationHistoryWidget::slotShowMessage(const QModelIndex &index)
{
    if (index.isValid()) {
        const QString roomId = index.data(NotificationHistoryModel::RoomId).toString();
        const QString messageId = index.data(NotificationHistoryModel::MessageId).toString();
        const QString accountName = index.data(NotificationHistoryModel::AccountName).toString();
        if (!accountName.isEmpty() && !roomId.isEmpty() && !messageId.isEmpty()) {
            Q_EMIT showNotifyMessage(accountName, messageId, roomId);
        } else {
            qCWarning(RUQOLAWIDGETS_LOG) << " Problem with index. AccountName " << accountName << " roomId : " << roomId << "messageId " << messageId;
        }
    }
}
