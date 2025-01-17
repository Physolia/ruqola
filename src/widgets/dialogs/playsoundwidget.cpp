/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "playsoundwidget.h"
#include "ruqolaglobalconfig.h"

#include <KLocalizedString>
#include <KMessageWidget>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QAudioDevice>
#endif
#include <QComboBox>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QLabel>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QMediaDevices>
#endif
#include <QPushButton>
#include <QSlider>
#include <QStyle>
#include <QTime>
#include <QToolButton>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QAudioOutput>
#endif

PlaySoundWidget::PlaySoundWidget(QWidget *parent)
    : QWidget(parent)
    , mMediaPlayer(new QMediaPlayer(this))
    , mPlayButton(new QPushButton(this))
    , mSoundButton(new QToolButton(this))
    , mSoundSlider(new QSlider(Qt::Horizontal, this))
    , mPositionSlider(new QSlider(Qt::Horizontal, this))
    , mLabelDuration(new QLabel(this))
    , mMessageWidget(new KMessageWidget(this))
    , mLabelPercentSound(new QLabel(this))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    , mAudioOutput(new QAudioOutput(this))
    , mDeviceComboBox(new QComboBox(this))
#endif
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    mMediaPlayer->setAudioOutput(mAudioOutput);
    mDeviceComboBox->setObjectName(QStringLiteral("mDeviceComboBox"));
    initializeAudioOutput();
#endif

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    mainLayout->addWidget(mDeviceComboBox);
#endif

    auto playerLayout = new QHBoxLayout;
    playerLayout->setObjectName(QStringLiteral("playerLayout"));
    playerLayout->setContentsMargins({});
    mainLayout->addLayout(playerLayout);
    mMessageWidget->setObjectName(QStringLiteral("mMessageWidget"));
    mainLayout->addWidget(mMessageWidget);
    mMessageWidget->setVisible(false);
    mMessageWidget->setCloseButtonVisible(false);
    mMessageWidget->setMessageType(KMessageWidget::Information);
    mMessageWidget->setWordWrap(true);
    mLabelDuration->setObjectName(QStringLiteral("mLabelDuration"));
    mLabelPercentSound->setObjectName(QStringLiteral("mLabelPercentSound"));

    mLabelDuration->setTextFormat(Qt::PlainText);
    mLabelPercentSound->setTextFormat(Qt::PlainText);

    mMediaPlayer->setObjectName(QStringLiteral("mMediaPlayer"));

    mPositionSlider->setObjectName(QStringLiteral("mPositionSlider"));
    mPositionSlider->setRange(0, 100);
    mPositionSlider->setValue(100);
    connect(mPositionSlider, &QAbstractSlider::sliderMoved, this, &PlaySoundWidget::setPosition);

    mSoundSlider->setObjectName(QStringLiteral("mSoundSlider"));
    mSoundSlider->setRange(0, 100);
    mSoundSlider->setValue(RuqolaGlobalConfig::self()->soundVolume());
    mSoundSlider->setTickPosition(QSlider::TicksAbove);

    connect(mSoundSlider, &QAbstractSlider::sliderMoved, this, &PlaySoundWidget::slotVolumeChanged);

    connect(mMediaPlayer, &QMediaPlayer::positionChanged, this, &PlaySoundWidget::slotPositionChanged);
    connect(mMediaPlayer, &QMediaPlayer::durationChanged, this, &PlaySoundWidget::slotDurationChanged);

    // Allow to change volume

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(mMediaPlayer, &QMediaPlayer::stateChanged, this, &PlaySoundWidget::mediaStateChanged);
#else
    connect(mMediaPlayer, &QMediaPlayer::playbackStateChanged, this, &PlaySoundWidget::mediaStateChanged);
#endif
    mPlayButton->setObjectName(QStringLiteral("mPlayButton"));
    mPlayButton->setEnabled(false);
    mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playerLayout->addWidget(mPlayButton);
    connect(mPlayButton, &QAbstractButton::clicked, this, &PlaySoundWidget::play);

    mSoundButton->setCheckable(true);
    mSoundButton->setObjectName(QStringLiteral("mSoundButton"));
    mSoundButton->setIcon(QIcon::fromTheme(QStringLiteral("player-volume")));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(mSoundButton, &QToolButton::clicked, mMediaPlayer, &QMediaPlayer::setMuted);
    connect(mMediaPlayer, &QMediaPlayer::mutedChanged, this, &PlaySoundWidget::muteChanged);
#else
    connect(mSoundButton, &QToolButton::clicked, mAudioOutput, &QAudioOutput::setMuted);
    connect(mAudioOutput, &QAudioOutput::mutedChanged, this, &PlaySoundWidget::muteChanged);
#endif
    playerLayout->addWidget(mPositionSlider);

    playerLayout->addWidget(mSoundButton);

    playerLayout->addWidget(mLabelDuration);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(mSoundSlider, &QAbstractSlider::valueChanged, mMediaPlayer, &QMediaPlayer::setVolume);
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(mMediaPlayer, qOverload<QMediaPlayer::Error>(&QMediaPlayer::error), this, &PlaySoundWidget::handleError);
#else
    connect(mMediaPlayer, &QMediaPlayer::errorChanged, this, &PlaySoundWidget::handleError);
#endif

    playerLayout->addWidget(mSoundSlider);
    playerLayout->addWidget(mLabelPercentSound);

    QFontMetrics f(font());
    mLabelPercentSound->setFixedWidth(f.horizontalAdvance(QStringLiteral("MMM")));
    slotVolumeChanged(mSoundSlider->value());
}

PlaySoundWidget::~PlaySoundWidget()
{
    RuqolaGlobalConfig::self()->setSoundVolume(mSoundSlider->value());
    RuqolaGlobalConfig::self()->save();
}

void PlaySoundWidget::initializeAudioOutput()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    mDeviceComboBox->addItem(i18n("Default"), QVariant::fromValue(QAudioDevice()));
    for (const auto &deviceInfo : QMediaDevices::audioOutputs()) {
        mDeviceComboBox->addItem(deviceInfo.description(), QVariant::fromValue(deviceInfo));
    }
    connect(mDeviceComboBox, &QComboBox::activated, this, &PlaySoundWidget::audioOutputChanged);
#endif
}

void PlaySoundWidget::audioOutputChanged(int index)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const auto device = mDeviceComboBox->itemData(index).value<QAudioDevice>();
    mMediaPlayer->audioOutput()->setDevice(device);
#endif
}

void PlaySoundWidget::slotPositionChanged(qint64 progress)
{
    if (!mPositionSlider->isSliderDown())
        mPositionSlider->setValue(progress / 1000);

    updateDurationInfo(progress / 1000);
}

void PlaySoundWidget::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || mDuration) {
        const QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60, currentInfo % 60, (currentInfo * 1000) % 1000);
        const QTime totalTime((mDuration / 3600) % 60, (mDuration / 60) % 60, mDuration % 60, (mDuration * 1000) % 1000);
        QString format = QStringLiteral("mm:ss");
        if (mDuration > 3600)
            format = QStringLiteral("hh:mm:ss");
        tStr = currentTime.toString(format) + QStringLiteral(" / ") + totalTime.toString(format);
    }
    mLabelDuration->setText(tStr);
}

void PlaySoundWidget::slotVolumeChanged(int position)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    mMediaPlayer->setVolume(position);
#else
    mAudioOutput->setVolume(position / 100.0);
#endif
    mLabelPercentSound->setText(QStringLiteral("%1%").arg(position));
}

void PlaySoundWidget::slotDurationChanged(qint64 duration)
{
    mDuration = duration / 1000;
    mPositionSlider->setMaximum(mDuration);
}

void PlaySoundWidget::setPosition(int position)
{
    mMediaPlayer->setPosition(position * 1000);
}

void PlaySoundWidget::muteChanged(bool state)
{
    mSoundButton->setIcon(state ? QIcon::fromTheme(QStringLiteral("player-volume-muted")) : QIcon::fromTheme(QStringLiteral("player-volume")));
}

QUrl PlaySoundWidget::audioUrl() const
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    // Code not used in qt5 :)
    return {};
#else
    return mMediaPlayer->source();
#endif
}

void PlaySoundWidget::setAudioUrl(const QUrl &url)
{
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    mMediaPlayer->setMedia(url);
#else
    mMediaPlayer->setSource(url);
#endif
    mPlayButton->setEnabled(true);
}

void PlaySoundWidget::play()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    switch (mMediaPlayer->state()) {
    case QMediaPlayer::PlayingState:
        mMediaPlayer->pause();
        break;
    default:
        mMediaPlayer->play();
        break;
    }
#else
    switch (mMediaPlayer->playbackState()) {
    case QMediaPlayer::PlayingState:
        mMediaPlayer->pause();
        break;
    default:
        mMediaPlayer->play();
        break;
    }
#endif
}
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void PlaySoundWidget::mediaStateChanged(QMediaPlayer::State state)
#else
void PlaySoundWidget::mediaStateChanged(QMediaPlayer::PlaybackState state)
#endif
{
    switch (state) {
    case QMediaPlayer::PlayingState:
        mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void PlaySoundWidget::handleError()
{
    mPlayButton->setEnabled(false);
    const QString errorString = mMediaPlayer->errorString();
    QString message = i18n("Error: "); // i18n ?
    if (errorString.isEmpty()) {
        message += QStringLiteral(" #") + QString::number(int(mMediaPlayer->error()));
    } else {
        message += errorString;
    }
    mMessageWidget->setText(message);
    mMessageWidget->animatedShow();
}

#include "moc_playsoundwidget.cpp"
