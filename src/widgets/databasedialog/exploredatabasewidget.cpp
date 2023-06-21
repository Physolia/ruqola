/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/messagemodel.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include <KMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QVBoxLayout>
// Same as in localmessagedatabase.cpp
enum class Fields {
    MessageId,
    TimeStamp,
    Json,
}; // in the same order as the table
// debug dialogbox => don't translate it
ExploreDatabaseWidget::ExploreDatabaseWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mMessageListView(new MessageListView(account, MessageListView::Mode::Viewing, this))
    , mLocalMessageDatabase(new LocalMessageDatabase())
    , mRoomName(new QLineEdit(this))
    , mMessageModel(new MessageModel(QStringLiteral("no_room"), account, nullptr, this))
{
    mMessageListView->setObjectName(QStringLiteral("mMessageListView"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    mainLayout->addLayout(hboxLayout);

    mRoomName->setObjectName(QStringLiteral("mRoomName"));
    mRoomName->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mRoomName, this);
    auto label = new QLabel(QStringLiteral("Room name:"), this);
    label->setObjectName(QStringLiteral("label"));
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(mRoomName);

    auto pushButton = new QPushButton(QStringLiteral("Load"), this);
    pushButton->setObjectName(QStringLiteral("pushButton"));
    hboxLayout->addWidget(pushButton);
    connect(pushButton, &QPushButton::clicked, this, &ExploreDatabaseWidget::slotLoad);
    connect(mRoomName, &QLineEdit::returnPressed, this, &ExploreDatabaseWidget::slotLoad);

    mMessageListView->setModel(mMessageModel);
    mainLayout->addWidget(mMessageListView);
}

ExploreDatabaseWidget::~ExploreDatabaseWidget() = default;

void ExploreDatabaseWidget::slotLoad()
{
    const QString roomName = mRoomName->text().trimmed();
    if (!roomName.isEmpty()) {
        auto tableModel = mLocalMessageDatabase->createMessageModel(mRocketChatAccount->accountName(), roomName);
        QVector<Message> listMessages;
        if (tableModel) {
            int rows = tableModel->rowCount();
            for (int row = 0; row < rows; ++row) {
                const QSqlRecord record = tableModel->record(row);
                // const QDateTime timeStamp = QDateTime::fromMSecsSinceEpoch(record.value(int(Fields::TimeStamp)).toULongLong());
                const QString json = record.value(int(Fields::Json)).toString();
                // qDebug() << " json111 " << json.toUtf8();
                const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
                // qDebug() << " doc " << doc;
                // qDebug() << " json " << QCborValue::fromCbor(json.toUtf8()).toMap();
                const Message msg = Message::deserialize(doc.object());
                // qDebug() << " msg " << msg;
                listMessages.append(std::move(msg));
                if (row == rows - 1 && tableModel->canFetchMore()) {
                    tableModel->fetchMore();
                    rows = tableModel->rowCount();
                }
            }
            // qDebug() << " listMessages " << listMessages.count();
            mMessageModel->clear();
            mMessageModel->addMessages(listMessages);
        } else {
            KMessageBox::error(this, QStringLiteral("Room '%1' does not have database").arg(roomName), QStringLiteral("Database empty"));
        }
    }
}

#include "moc_exploredatabasewidget.cpp"
