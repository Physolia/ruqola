/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "inputtextmanager.h"
#include <KTextEdit>

#include "libruqolawidgets_private_export.h"
#include <QPointer>
class RocketChatAccount;
class CompletionListView;
class UserAndChannelCompletionDelegate;
/**
 * @brief The MessageTextEdit class is the widget used for typing messages to be sent.
 */
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageTextEdit : public KTextEdit
{
    Q_OBJECT
public:
    explicit MessageTextEdit(QWidget *parent = nullptr);
    ~MessageTextEdit() override;

    void setCurrentRocketChatAccount(RocketChatAccount *account, bool threadMessageDialog);

    void insertEmoji(const QString &text);
    [[nodiscard]] QString text() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void changeText(const QString &str, int cursorPosition);

    QMenu *mousePopupMenu() override;

    void setRoomId(const QString &roomId);
    [[nodiscard]] QString roomId() const;

Q_SIGNALS:

    void sendMessage(const QString &str);
    void keyPressed(QKeyEvent *ev);
    void textEditing(bool clearNotification);
    void textClicked();
    void handleMimeData(const QMimeData *mimeData);

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void dropEvent(QDropEvent *e) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;

private:
    void slotCompletionTypeChanged(InputTextManager::CompletionForType type);
    void slotCompletionAvailable();
    void slotComplete(const QModelIndex &index);
    void slotSetAsBold();
    void slotSetAsItalic();
    void slotSetAsStrikeOut();
    void insertFormat(QChar formatChar);
    void slotInsertCodeBlock();
    void loadSpellCheckingSettings();
    void slotLanguageChanged(const QString &lang);
    void slotSpellCheckingEnableChanged(bool b);
    void slotInsertMarkdownUrl();
    void slotLoginChanged();
    void switchAutoCorrectionLanguage(const QString &lang);
    void slotUpdateMessageFailed(const QString &str);
    void slotSelectFirstTextCompleter();

    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    InputTextManager *mCurrentInputTextManager = nullptr;
    CompletionListView *const mUserAndChannelCompletionListView;
    CompletionListView *const mEmojiCompletionListView;
    CompletionListView *const mCommandCompletionListView;
    UserAndChannelCompletionDelegate *mUserAndChannelCompletionDelegate = nullptr;
    QString mRoomId;
};
