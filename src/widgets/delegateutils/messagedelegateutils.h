/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "messages/messageattachment.h"

#include <QAbstractTextDocumentLayout>
#include <QString>
class QTextDocument;
#include <QVector>
#include <memory>

class TextSelection;
class QStyleOptionViewItem;
namespace MessageDelegateUtils
{
[[nodiscard]] std::unique_ptr<QTextDocument> createTextDocument(bool useItalic, const QString &text, int width);

[[nodiscard]] bool generateToolTip(const QTextDocument *doc, const QPoint &pos, QString &formattedTooltip);

void generateToolTip(const QString &toolTip, const QString &href, QString &formattedTooltip);

[[nodiscard]] bool useItalicsForMessage(const QModelIndex &index);

[[nodiscard]] bool pendingMessage(const QModelIndex &index);
[[nodiscard]] QVector<QAbstractTextDocumentLayout::Selection> selection(TextSelection *selection,
                                                                        QTextDocument *doc,
                                                                        const QModelIndex &index,
                                                                        const QStyleOptionViewItem &option,
                                                                        const MessageAttachment &msgAttach = {},
                                                                        bool isAMessage = true);

void drawSelection(QTextDocument *doc,
                   QRect rect,
                   int top,
                   QPainter *painter,
                   const QModelIndex &index,
                   const QStyleOptionViewItem &option,
                   TextSelection *selection,
                   const MessageAttachment &msgAttach,
                   bool isAMessage = true);

void setClipboardSelection(TextSelection *selection);

[[nodiscard]] QSizeF dprAwareSize(const QPixmap &pixmap);

[[nodiscard]] qreal basicMargin();
[[nodiscard]] QSize timeStampSize(const QString &timeStampText, const QStyleOptionViewItem &option);
[[nodiscard]] QSize textSizeHint(QTextDocument *doc, qreal *pBaseLine);
[[nodiscard]] bool showIgnoreMessages(const QModelIndex &index);
}
