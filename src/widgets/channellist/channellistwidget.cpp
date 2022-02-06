/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channellistwidget.h"
#include "channellistview.h"
#include "model/roomfilterproxymodel.h"
#include "ruqolawidgets_debug.h"

#include "accountmanager.h"
#include "parsemessageurlutils.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolautils.h"

#include <KLocalizedString>
#include <QAction>
#include <QKeyEvent>
#include <QLineEdit>
#include <QVBoxLayout>

ChannelListWidget::ChannelListWidget(QWidget *parent)
    : QWidget(parent)
    , mChannelView(new ChannelListView(this))
    , mSearchRoomLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins({});

    mChannelView->setObjectName(QStringLiteral("mChannelView"));
    mainLayout->addWidget(mChannelView);
    connect(mChannelView, &ChannelListView::roomSelected, this, &ChannelListWidget::roomSelected);

    // dummy action just for getting the icon)
    mSearchRoomLineEdit->addAction(QIcon::fromTheme(QStringLiteral("view-filter")), QLineEdit::LeadingPosition);
    mSearchRoomLineEdit->setObjectName(QStringLiteral("mSearchRoom"));
    mSearchRoomLineEdit->setPlaceholderText(i18n("Filter Channels (CTRL + K)"));
    mSearchRoomLineEdit->setClearButtonEnabled(true);
    mSearchRoomLineEdit->installEventFilter(this);
    mainLayout->addWidget(mSearchRoomLineEdit);
    connect(mSearchRoomLineEdit, &QLineEdit::textChanged, this, &ChannelListWidget::slotSearchRoomTextChanged);

    // BEGIN: Actions
    auto searchRoomAction = new QAction(i18n("Search Channels"), this);
    searchRoomAction->setShortcut(Qt::CTRL | Qt::Key_K);
    connect(searchRoomAction, &QAction::triggered, this, [this]() {
        mSearchRoomLineEdit->setFocus();
    });
    addAction(searchRoomAction); // TODO: Add to MainWindow's action collection instead?

    auto previousRoomAction = new QAction(i18n("Previous Channel"), this);
    previousRoomAction->setShortcut(Qt::CTRL | Qt::Key_Up);
    connect(previousRoomAction, &QAction::triggered, this, [this]() {
        selectNextChannel(Qt::Key_Up);
        applyChannelSelection();
    });
    addAction(previousRoomAction); // TODO: Add to MainWindow's action collection instead?

    auto nextRoomAction = new QAction(i18n("Next Channel"), this);
    nextRoomAction->setShortcut(Qt::CTRL | Qt::Key_Down);
    connect(nextRoomAction, &QAction::triggered, this, [this]() {
        selectNextChannel(Qt::Key_Down);
        applyChannelSelection();
    });
    addAction(nextRoomAction); // TODO: Add to MainWindow's action collection instead?
    // END: Actions
}

ChannelListWidget::~ChannelListWidget() = default;

void ChannelListWidget::clearFilterChannel()
{
    if (auto *model = mChannelView->filterModel()) {
        model->setFilterString(QString());
        mSearchRoomLineEdit->clear();
    }
}

void ChannelListWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    clearFilterChannel();
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, nullptr, this, nullptr);
    }
    mCurrentRocketChatAccount = account;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::accountInitialized, this, &ChannelListWidget::slotAccountInitialized);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::openLinkRequested, this, &ChannelListWidget::slotOpenLinkRequested);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::openTeamNameRequested, this, &ChannelListWidget::slotOpenTeamRequested);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::selectRoomByRoomNameRequested, mChannelView, &ChannelListView::selectChannelByRoomNameRequested);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::selectRoomByRoomIdRequested, mChannelView, &ChannelListView::selectChannelRequested);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::selectChannelAndMessage, this, &ChannelListWidget::slotSelectMessageRequested);

    mChannelView->setCurrentRocketChatAccount(account);
    mChannelView->setFilterModel(mCurrentRocketChatAccount->roomFilterProxyModel());
}

ChannelListView *ChannelListWidget::channelListView() const
{
    return mChannelView;
}

bool ChannelListWidget::eventFilter(QObject *object, QEvent *event)
{
    if (object == mSearchRoomLineEdit && event->type() == QEvent::KeyPress) {
        const auto keyEvent = static_cast<QKeyEvent *>(event);
        const int keyValue = keyEvent->key();
        if (keyValue == Qt::Key_Return || keyValue == Qt::Key_Enter) {
            applyChannelSelection();
        } else if (keyValue == Qt::Key_Up || keyValue == Qt::Key_Down) {
            selectNextChannel(keyValue);
            return true; // eat event
        }
    }

    return QWidget::eventFilter(object, event);
}

void ChannelListWidget::slotAccountInitialized()
{
    mChannelView->selectChannelRequested(mCurrentRocketChatAccount->settings()->lastSelectedRoom());
}

void ChannelListWidget::slotSearchRoomTextChanged()
{
    mChannelView->filterModel()->setFilterString(mSearchRoomLineEdit->text());
}

void ChannelListWidget::slotOpenTeamRequested(const QString &identifier)
{
    const QModelIndex selectedIndex = mChannelView->selectionModel()->currentIndex();
    if (selectedIndex.isValid()) {
        const QString currentRoomId = selectedIndex.data(RoomModel::RoomId).toString();
        if (identifier == currentRoomId) {
            return;
        }
    }
    if (!mChannelView->selectChannelByRoomIdRequested(identifier)) {
        mCurrentRocketChatAccount->openChannel(identifier, RocketChatAccount::ChannelTypeInfo::RoomId);
    }
}

void ChannelListWidget::slotSelectMessageRequested(const QString &messageId,
                                                   const QString &roomId,
                                                   ParseMessageUrlUtils::RoomIdType roomType,
                                                   ParseMessageUrlUtils::ChannelType channelType)
{
    switch (roomType) {
    case ParseMessageUrlUtils::RoomIdType::Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << "Room type undefined!";
        break;
    case ParseMessageUrlUtils::RoomIdType::RoomId: {
        const QModelIndex selectedIndex = mChannelView->selectionModel()->currentIndex();
        if (selectedIndex.isValid()) {
            const QString currentRoomId = selectedIndex.data(RoomModel::RoomId).toString();
            if (roomId == currentRoomId) {
                // TODO select messageId
                return;
            }
            switch (channelType) {
            case ParseMessageUrlUtils::ChannelType::Channel: {
                if (!mChannelView->selectChannelByRoomIdRequested(roomId)) {
                    mCurrentRocketChatAccount->openChannel(roomId, RocketChatAccount::ChannelTypeInfo::RoomId);
                }
                // TODO select message id
                break;
            }
            case ParseMessageUrlUtils::ChannelType::Direct: {
                if (!mChannelView->selectChannelByRoomIdRequested(roomId)) {
                    // TODO add support for roomId or roomName
                    mCurrentRocketChatAccount->openDirectChannel(roomId /*, RocketChatAccount::ChannelTypeInfo::RoomId*/);
                }
                // TODO select message id
                break;
            }
            case ParseMessageUrlUtils::ChannelType::Unknown: {
                qCWarning(RUQOLAWIDGETS_LOG) << "ChannelType undefined!";
                break;
            }
            }
        }
        break;
    }
    case ParseMessageUrlUtils::RoomIdType::RoomName: {
        const QModelIndex selectedIndex = mChannelView->selectionModel()->currentIndex();
        if (selectedIndex.isValid()) {
            const QString currentRoomName = selectedIndex.data(RoomModel::RoomName).toString();
            if (roomId == currentRoomName) {
                // TODO select message id
                return;
            }
            switch (channelType) {
            case ParseMessageUrlUtils::ChannelType::Channel: {
                if (!mChannelView->selectChannelByRoomNameRequested(roomId)) {
                    mCurrentRocketChatAccount->openChannel(roomId, RocketChatAccount::ChannelTypeInfo::RoomName);
                }
                // TODO select message id
                break;
            }
            case ParseMessageUrlUtils::ChannelType::Direct: {
                if (!mChannelView->selectChannelByRoomNameRequested(roomId)) {
                    // TODO add support for roomId or roomName
                    mCurrentRocketChatAccount->openDirectChannel(roomId /*, RocketChatAccount::ChannelTypeInfo::RoomName*/);
                }
                // TODO select message id
                break;
            }
            case ParseMessageUrlUtils::ChannelType::Unknown: {
                qCWarning(RUQOLAWIDGETS_LOG) << "ChannelType undefined!";
                break;
            }
            }
        }
        break;
    }
    }
}

void ChannelListWidget::slotOpenLinkRequested(const QString &link)
{
    if (link.startsWith(QLatin1String("ruqola:"))) {
        const QString roomOrUser = RuqolaUtils::self()->extractRoomUserFromUrl(link);
        const QModelIndex selectedIndex = mChannelView->selectionModel()->currentIndex();
        if (selectedIndex.isValid()) {
            const QString currentRoomName = selectedIndex.data(RoomModel::RoomName).toString();
            if (roomOrUser == currentRoomName) {
                return;
            }
        }
        if (link.startsWith(QLatin1String("ruqola:/room/"))) {
            if (!mChannelView->selectChannelByRoomNameRequested(roomOrUser)) {
                mCurrentRocketChatAccount->openChannel(roomOrUser, RocketChatAccount::ChannelTypeInfo::RoomName);
            }
        } else if (link.startsWith(QLatin1String("ruqola:/user/"))) {
            if (roomOrUser == QLatin1String("here") || roomOrUser == QLatin1String("all")) {
                return;
            }
            if (!mChannelView->selectChannelByRoomNameRequested(roomOrUser)) {
                if (roomOrUser != mCurrentRocketChatAccount->userName()) {
                    mCurrentRocketChatAccount->openDirectChannel(roomOrUser);
                }
            }
        } else if (link == QLatin1String("ruqola:/jitsicall/")) {
            const QModelIndex jitsiSelectedIndex = mChannelView->selectionModel()->currentIndex();
            if (jitsiSelectedIndex.isValid()) {
                const QString roomId = jitsiSelectedIndex.data(RoomModel::RoomId).toString();
                mCurrentRocketChatAccount->joinJitsiConfCall(roomId);
            }
        }
    } else {
        ParseMessageUrlUtils parseUrl;
        if (parseUrl.parseUrl(link)) {
            if (Ruqola::self()->accountManager()->showMessage(parseUrl)) {
                return;
            }
        }
        RuqolaUtils::self()->openUrl(link);
    }
}

void ChannelListWidget::setLayoutSpacing(int spacing)
{
    layout()->setSpacing(spacing);
}

void ChannelListWidget::selectNextChannel(int direction)
{
    Q_ASSERT(direction == Qt::Key_Up || direction == Qt::Key_Down);

    const auto *model = mChannelView->model();
    const QModelIndex currentIndex = mChannelView->selectionModel()->currentIndex();
    const auto rowCount = model->rowCount();
    int selectRow = -1;
    if (direction == Qt::Key_Up) {
        if (!currentIndex.isValid()) {
            selectRow = rowCount - 1;
        } else if (currentIndex.row() - 1 >= 0) {
            selectRow = currentIndex.row() - 1;
        }
    } else { // Qt::Key_Down
        if (!currentIndex.isValid() || (currentIndex.row() + 1) == rowCount) {
            selectRow = 0;
        } else if (currentIndex.row() + 1 < rowCount) {
            selectRow = currentIndex.row() + 1;
        }
    }

    while (selectRow != -1 && selectRow != currentIndex.row() && !model->index(selectRow, 0).flags().testFlag(Qt::ItemIsSelectable)) {
        if (direction == Qt::Key_Up) {
            --selectRow;
            if (selectRow == -1) {
                selectRow = rowCount - 1;
            }
        } else { // Qt::Key_Down
            ++selectRow;
            if (selectRow == rowCount) {
                selectRow = 0;
            }
        }
    }

    if (selectRow != -1) {
        mChannelView->selectionModel()->setCurrentIndex(model->index(selectRow, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
}

void ChannelListWidget::applyChannelSelection()
{
    const auto selectedIndex = mChannelView->selectionModel()->currentIndex();
    if (selectedIndex.isValid()) {
        mChannelView->channelSelected(selectedIndex);
        mSearchRoomLineEdit->setText({});
    }
}
