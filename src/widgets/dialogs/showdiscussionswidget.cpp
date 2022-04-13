/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showdiscussionswidget.h"
#include "discussion/listdiscussiondelegate.h"
#include "misc/lineeditcatchreturnkey.h"
#include "misc/messagelistviewbase.h"
#include "model/discussionsfilterproxymodel.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

ShowDiscussionsWidget::ShowDiscussionsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchDiscussionLineEdit(new QLineEdit(this))
    , mDiscussionInfoLabel(new QLabel(this))
    , mListDiscussions(new MessageListViewBase(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchDiscussionLineEdit->setObjectName(QStringLiteral("mSearchDiscussionLineEdit"));
    mSearchDiscussionLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchDiscussionLineEdit, this);
    mSearchDiscussionLineEdit->setPlaceholderText(i18n("Search Discussion..."));
    connect(mSearchDiscussionLineEdit, &QLineEdit::textChanged, this, &ShowDiscussionsWidget::slotSearchMessageTextChanged);
    mainLayout->addWidget(mSearchDiscussionLineEdit);

    mDiscussionInfoLabel->setObjectName(QStringLiteral("mInfo"));
    mDiscussionInfoLabel->setTextFormat(Qt::RichText);
    mainLayout->addWidget(mDiscussionInfoLabel);
    QFont labFont = mDiscussionInfoLabel->font();
    labFont.setBold(true);
    mDiscussionInfoLabel->setFont(labFont);
    connect(mDiscussionInfoLabel, &QLabel::linkActivated, this, &ShowDiscussionsWidget::loadMoreDiscussion);

    mListDiscussions->setObjectName(QStringLiteral("mListDiscussions"));
    mainLayout->addWidget(mListDiscussions);
    auto listDiscussionDelegate = new ListDiscussionDelegate(mRocketChatAccount, this);
    mListDiscussions->setItemDelegate(listDiscussionDelegate);
    connect(listDiscussionDelegate, &ListDiscussionDelegate::openDiscussion, this, &ShowDiscussionsWidget::slotOpenDiscussion);
}

ShowDiscussionsWidget::~ShowDiscussionsWidget() = default;

void ShowDiscussionsWidget::slotSearchMessageTextChanged(const QString &str)
{
    mDiscussionModel->setFilterString(str);
    updateLabel();
}

void ShowDiscussionsWidget::setModel(DiscussionsFilterProxyModel *model)
{
    mListDiscussions->setModel(model);
    mDiscussionModel = model;
    connect(mDiscussionModel, &DiscussionsFilterProxyModel::hasFullListChanged, this, &ShowDiscussionsWidget::updateLabel);
    connect(mDiscussionModel, &DiscussionsFilterProxyModel::loadingInProgressChanged, this, &ShowDiscussionsWidget::updateLabel);
    updateLabel();
}

void ShowDiscussionsWidget::updateLabel()
{
    if (mDiscussionModel->loadMoreDiscussionsInProgress()) {
        mDiscussionInfoLabel->setText(i18n("Loading..."));
    } else {
        mDiscussionInfoLabel->setText(mDiscussionModel->rowCount() == 0 ? i18n("No Discussion found") : displayShowDiscussionInRoom());
    }
}

QString ShowDiscussionsWidget::displayShowDiscussionInRoom() const
{
    QString displayMessageStr =
        i18np("%1 Discussion in room (Total: %2)", "%1 Discussions in room (Total: %2)", mDiscussionModel->rowCount(), mDiscussionModel->total());
    if (!mDiscussionModel->hasFullList()) {
        displayMessageStr += QStringLiteral(" <a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)"));
    }
    return displayMessageStr;
}

void ShowDiscussionsWidget::slotOpenDiscussion(const QString &roomDiscussionId)
{
    if (mRocketChatAccount) {
        mRocketChatAccount->ddp()->openRoom(roomDiscussionId);
    }
}
