/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "messagedelegatehelperbase.h"

#include <QIcon>
#include <QRect>
#include <QString>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperFile : public MessageDelegateHelperBase
{
public:
    MessageAttachmentDelegateHelperFile();
    ~MessageAttachmentDelegateHelperFile() override;
    void draw(const MessageAttachment &msgAttach,
              QPainter *painter,
              QRect attachmentsRect,
              const QModelIndex &index,
              const QStyleOptionViewItem &option) const override;
    QSize sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    bool handleMouseEvent(const MessageAttachment &msgAttach,
                          QMouseEvent *mouseEvent,
                          QRect attachmentsRect,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) override;

private:
    struct FileLayout {
        QString title;
        QString description;
        QSize titleSize;
        QSize descriptionSize;
        QRect downloadButtonRect;
        int y; // relative
        int height;
        QString link;
    };
    Q_REQUIRED_RESULT FileLayout doLayout(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth) const;
    void handleDownloadClicked(const QString &link, QWidget *widget);
    friend class MessageDelegateHelperFileTest;
    const QIcon mDownloadIcon;
};

