/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messageattachmentdelegatehelperbase.h"
#include <QModelIndex>
#include <QRect>
class QTextDocument;
class QHelpEvent;
class QListView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperText : public MessageAttachmentDelegateHelperBase
{
public:
    explicit MessageAttachmentDelegateHelperText(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageAttachmentDelegateHelperText() override;
    void
    draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] QSize sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] bool handleMouseEvent(const MessageAttachment &msgAttach,
                                        QMouseEvent *mouseEvent,
                                        QRect attachmentsRect,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) override;

    [[nodiscard]] bool
    handleHelpEvent(QHelpEvent *helpEvent, QRect messageRect, const MessageAttachment &msgAttach, const QStyleOptionViewItem &option) override;

private:
    struct TextLayout {
        QString title;
        QRect hideShowButtonRect;
        QRectF titleRect;
        QSize textSize;
        QFont textFont;
        bool isShown = true;
    };
    [[nodiscard]] TextLayout
    layoutText(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth, int attachmentsHeight) const;
    [[nodiscard]] QTextDocument *documentAttachmentForIndex(const MessageAttachment &msgAttach, int width) const;
    [[nodiscard]] QPoint
    adaptMousePosition(const QPoint &pos, const MessageAttachment &msgAttach, QRect attachmentsRect, const QStyleOptionViewItem &option) override;
};
