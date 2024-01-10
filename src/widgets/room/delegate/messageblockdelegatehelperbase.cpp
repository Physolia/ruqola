/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageblockdelegatehelperbase.h"
#include "delegateutils/messagedelegateutils.h"

#include <QAbstractItemView>
#include <QDrag>
#include <QHelpEvent>
#include <QMimeData>
#include <QToolTip>

MessageBlockDelegateHelperBase::MessageBlockDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageBlockDelegateHelperBase::~MessageBlockDelegateHelperBase() = default;

bool MessageBlockDelegateHelperBase::handleHelpEvent(QHelpEvent *helpEvent, QRect messageRect, const Block &block, const QStyleOptionViewItem &option)
{
    Q_UNUSED(option)
    if (helpEvent->type() != QEvent::ToolTip) {
        return false;
    }
    const auto *doc = documentDescriptionForIndex(block, messageRect.width());
    if (!doc) {
        return false;
    }
#if 0
    const QPoint relativePos = adaptMousePosition(helpEvent->pos(), block, messageRect, option);
    QString formattedTooltip;
    if (MessageDelegateUtils::generateToolTip(doc, relativePos, formattedTooltip)) {
        QToolTip::showText(helpEvent->globalPos(), formattedTooltip, mListView);
        return true;
    }
#endif
    return false;
}

bool MessageBlockDelegateHelperBase::maybeStartDrag(const Block &block,
                                                    QMouseEvent *mouseEvent,
                                                    QRect blocksRect,
                                                    const QStyleOptionViewItem &option,
                                                    const QModelIndex &index)
{
    Q_UNUSED(block)
    Q_UNUSED(mouseEvent)
    Q_UNUSED(blocksRect)
    Q_UNUSED(option)
    Q_UNUSED(index)
    return false;
}

QTextDocument *MessageBlockDelegateHelperBase::documentDescriptionForIndex(const Block &block, int width) const
{
    const QString blockId = block.blockId();
    auto it = mDocumentCache.find(blockId);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (width != -1 && !qFuzzyCompare(ret->textWidth(), width)) {
            ret->setTextWidth(width);
        }
        return ret;
    }
#if 0
    const QString description = block.description();

    if (description.isEmpty()) {
        return nullptr;
    }
    // Use TextConverter in case it starts with a [](URL) reply marker
    QString needUpdateMessageId; // TODO use it ?
    // Laurent Ruqola::self()->rocketChatAccount() only for test.
    auto account = mRocketChatAccount ? mRocketChatAccount : Ruqola::self()->rocketChatAccount();
    int recursiveIndex = 0;
    const QString contextString = TextConverter::convertMessageText(description,
                                                                    account->userName(),
                                                                    {},
                                                                    account->highlightWords(),
                                                                    account->emojiManager(),
                                                                    account->messageCache(),
                                                                    needUpdateMessageId,
                                                                    {},
                                                                    {});
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
    auto ret = doc.get();
    mDocumentCache.insert(attachmentId, std::move(doc));
    return ret;
#else
    return nullptr;
#endif
}

QSize MessageBlockDelegateHelperBase::documentDescriptionForIndexSize(const Block &block, int width) const
{
    auto *doc = documentDescriptionForIndex(block, width);
    // Add +10 as if we use only doc->idealWidth() it's too small and it creates a new line.
    return doc ? QSize(doc->idealWidth() + 10, doc->size().height()) : QSize();
}
