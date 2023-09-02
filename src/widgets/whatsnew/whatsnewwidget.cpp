/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewwidget.h"
#include "whatsnewcomboboxwidget.h"

#include <KLazyLocalizedString>

#include <QCryptographicHash>
#include <QLabel>
#include <QVBoxLayout>

// enter items for the "Important changes" list here:
static const KLazyLocalizedString ruqolaChanges[] = {
    kli18n("Store Message in Local Database (experimental)"),
    // TODO
};
static const int numRuqolaChanges = sizeof ruqolaChanges / sizeof *ruqolaChanges;

// enter items for the "new features" list here, so the main body of
// the welcome page can be left untouched (probably much easier for
// the translators). Note that the <li>...</li> tags are added
// automatically below:
static const KLazyLocalizedString ruqolaNewFeatures[] = {
    kli18n("Download Server Icon from Server"),
    kli18n("Show Server Error Info in Specific DialogBox"),
    kli18n("Allow to Copy Image in Clipboard"),
    kli18n("Improve debug support (Allow to show current account permissions)"),
    kli18n("Add support for quoted text (text which starts by \'>\')"),
    kli18n("Show leader/moderator/owner in channel info."),
    kli18n("Video/Sound Message support (kf6 only)."),
    kli18n("Import/Export Accounts."),
};
static const int numRuqolaNewFeatures = sizeof ruqolaNewFeatures / sizeof *ruqolaNewFeatures;

// enter items for the "Important changes" list here:
static const KLazyLocalizedString ruqolaBugfixing[] = {
    kli18n("Fix avatar support"),
    kli18n("Fix emoji support (use new ktextaddons/textemoticons)"),
    kli18n("Fix animated gif in reactions"),
    kli18n("Fix kf6 support"),
    // TODO
};
static const int numRuqolaBugfixing = sizeof ruqolaBugfixing / sizeof *ruqolaBugfixing;

WhatsNewWidget::WhatsNewWidget(QWidget *parent)
    : QWidget{parent}
    , mLabelInfo(new QLabel(this))
    , mWhatsNewComboBoxWidget(new WhatsNewComboBoxWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mWhatsNewComboBoxWidget->setObjectName(QStringLiteral("mWhatsNewComboBoxWidget"));
    mainLayout->addWidget(mWhatsNewComboBoxWidget);

    mLabelInfo->setObjectName(QStringLiteral("mLabelInfo"));
    mLabelInfo->setTextFormat(Qt::RichText);
    mLabelInfo->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    mainLayout->addWidget(mLabelInfo, 0, Qt::AlignTop);
    connect(mWhatsNewComboBoxWidget, &WhatsNewComboBoxWidget::versionChanged, this, &WhatsNewWidget::slotVersionChanged);
    mWhatsNewComboBoxWidget->initializeVersion();
}

WhatsNewWidget::~WhatsNewWidget() = default;

// static
QString WhatsNewWidget::newFeaturesMD5()
{
    QByteArray str;
    for (int i = 0; i < numRuqolaChanges; ++i) {
        str += ruqolaChanges[i].untranslatedText();
    }
    for (int i = 0; i < numRuqolaNewFeatures; ++i) {
        str += ruqolaNewFeatures[i].untranslatedText();
    }
    for (int i = 0; i < numRuqolaBugfixing; ++i) {
        str += ruqolaBugfixing[i].untranslatedText();
    }
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(str);
    return QLatin1String(md5.result().toBase64());
}

void WhatsNewWidget::slotVersionChanged(WhatsNewComboBoxWidget::VersionType type)
{
    // TODO
}

void WhatsNewWidget::updateInformations()
{
    QString message = QStringLiteral("<qt>");
    if (numRuqolaChanges > 0) {
        message += QStringLiteral("<b>") + i18n("Important changes since last version:") + QStringLiteral("</b>");
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaChanges; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaChanges[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    if (numRuqolaNewFeatures > 0) {
        message += QStringLiteral("<b>") + i18n("Some of the new features in this release of Ruqola include:") + QStringLiteral("</b>");
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaNewFeatures; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaNewFeatures[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    if (numRuqolaBugfixing > 0) {
        message += QStringLiteral("<b>") + i18n("Some bug fixing:") + QStringLiteral("</b>");
        message += QStringLiteral("<ul>");
        for (int i = 0; i < numRuqolaBugfixing; ++i) {
            message += QStringLiteral("<li>%1</li>").arg(ruqolaBugfixing[i].toString());
        }
        message += QStringLiteral("</ul>");
    }
    message += QStringLiteral("</qt>");
    mLabelInfo->setText(message);
}

#include "moc_whatsnewwidget.cpp"
