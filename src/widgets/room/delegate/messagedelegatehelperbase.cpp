/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperbase.h"
#include "messagedelegateutils.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "textconverter.h"

#include <QAbstractTextDocumentLayout>
#include <QDrag>
#include <QListView>
#include <QMimeData>
#include <QPainter>
#include <QRect>
#include <QStyleOptionViewItem>

MessageDelegateHelperBase::~MessageDelegateHelperBase() = default;

MessageDelegateHelperBase::MessageDelegateHelperBase(QListView *view, TextSelection *textSelection)
    : mListView(view)
    , mSelection(textSelection)
{
    connect(mSelection, &TextSelection::repaintNeeded, this, &MessageDelegateHelperBase::updateView);
}

void MessageDelegateHelperBase::updateView(const QModelIndex &index)
{
    // qDebug() << " void MessageDelegateHelperBase::updateView(const QModelIndex &index)" << index;
    mListView->update(index);
}

bool MessageDelegateHelperBase::handleMouseEvent(const MessageAttachment &msgAttach,
                                                 QMouseEvent *mouseEvent,
                                                 QRect attachmentsRect,
                                                 const QStyleOptionViewItem &option,
                                                 const QModelIndex &index)
{
    Q_UNUSED(msgAttach)
    Q_UNUSED(option)

    switch (mouseEvent->type()) {
    case QEvent::MouseButtonPress:
        if (attachmentsRect.contains(mouseEvent->pos())) {
            mCurrentIndex = index;
            mMightStartDrag = true;
        } else {
            mMightStartDrag = false;
            mCurrentIndex = QModelIndex();
        }
        break;
    default:
        break;
    }
    return false;
}

bool MessageDelegateHelperBase::maybeStartDrag(const MessageAttachment &msgAttach,
                                               QMouseEvent *mouseEvent,
                                               QRect attachmentsRect,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index)
{
    if (!mMightStartDrag || index != mCurrentIndex || !attachmentsRect.contains(mouseEvent->pos())) {
        return false;
    }

    auto mimeData = new QMimeData;
    mimeData->setUrls({Ruqola::self()->rocketChatAccount()->attachmentUrlFromLocalCache(msgAttach.link())});

    auto drag = new QDrag(const_cast<QWidget *>(option.widget));
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);

    return true;
}

bool MessageDelegateHelperBase::handleHelpEvent(QHelpEvent *helpEvent,
                                                QRect messageRect,
                                                const MessageAttachment &msgAttach,
                                                const QStyleOptionViewItem &option)
{
    Q_UNUSED(helpEvent);
    Q_UNUSED(messageRect);
    Q_UNUSED(msgAttach);
    Q_UNUSED(option);
    return false;
}

void MessageDelegateHelperBase::clearTextDocumentCache()
{
    mDocumentCache.clear();
}

void MessageDelegateHelperBase::drawDescription(const MessageAttachment &msgAttach,
                                                QRect descriptionRect,
                                                QPainter *painter,
                                                int topPos,
                                                const QModelIndex &index,
                                                const QStyleOptionViewItem &option) const
{
    auto *doc = documentDescriptionForIndex(msgAttach, descriptionRect.width());
    if (!doc) {
        return;
    }

    const QVector<QAbstractTextDocumentLayout::Selection> selections = MessageDelegateUtils::selection(mSelection, doc, index, option);
    painter->save();
    // painter->drawRect(descriptionRect);
    painter->translate(descriptionRect.left(), topPos);
    const QRect clip(0, 0, descriptionRect.width(), descriptionRect.height());

    // Same as pDoc->drawContents(painter, clip) but we also set selections
    QAbstractTextDocumentLayout::PaintContext ctx;
    ctx.selections = selections;
    if (clip.isValid()) {
        painter->setClipRect(clip);
        ctx.clip = clip;
    }
    doc->documentLayout()->draw(painter, ctx);
    painter->restore();
}

QTextDocument *MessageDelegateHelperBase::documentForIndex(const MessageAttachment &msgAttach) const
{
    return documentDescriptionForIndex(msgAttach, -1);
}

QTextDocument *MessageDelegateHelperBase::documentForIndex(const QModelIndex &index) const
{
    // Unused here
    return nullptr;
}

QSize MessageDelegateHelperBase::documentDescriptionForIndexSize(const MessageAttachment &msgAttach, int width) const
{
    auto *doc = documentDescriptionForIndex(msgAttach, width);
    // Add +10 as if we use only doc->idealWidth() it's too small and it creates a new line.
    return doc ? QSize(doc->idealWidth() + 10, doc->size().height()) : QSize();
}

QTextDocument *MessageDelegateHelperBase::documentDescriptionForIndex(const MessageAttachment &msgAttach, int width) const
{
    const QString attachmentId = msgAttach.attachmentId();
    auto it = mDocumentCache.find(attachmentId);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (!qFuzzyCompare(ret->textWidth(), width)) {
            ret->setTextWidth(width);
        }
        return ret;
    }

    const QString description = msgAttach.description();

    if (description.isEmpty()) {
        return nullptr;
    }
    // Use TextConverter in case it starts with a [](URL) reply marker
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    QString needUpdateMessageId; // TODO use it ?
    const QString contextString = TextConverter::convertMessageText(description,
                                                                    rcAccount->userName(),
                                                                    {},
                                                                    rcAccount->highlightWords(),
                                                                    rcAccount->emojiManager(),
                                                                    rcAccount->messageCache(),
                                                                    needUpdateMessageId,
                                                                    {},
                                                                    {});
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
    auto ret = doc.get();
    mDocumentCache.insert(attachmentId, std::move(doc));
    return ret;
}
