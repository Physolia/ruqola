/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "showimagewidget.h"
#include "common/delegateutil.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <QDoubleSpinBox>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QScopedValueRollback>
#include <QSlider>
#include <QTimer>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QtMath>

namespace
{
constexpr qreal defaultMinimumZoomScale = (qreal)0.1;
constexpr qreal defaultMaximumZoomScale = (qreal)10.0;

qreal fitToViewZoomScale(QSize imageSize, QSize widgetSize)
{
    if (imageSize.width() > widgetSize.width() || imageSize.height() > widgetSize.height()) {
        // Make sure it fits, we care only about the first two decimal points, so round to the smaller value
        const qreal hZoom = (qreal)widgetSize.width() / imageSize.width();
        const qreal vZoom = (qreal)widgetSize.height() / imageSize.height();
        return std::max((int)(std::min(hZoom, vZoom) * 100) / 100.0, defaultMinimumZoomScale);
    }

    return 1.0;
}

}

ImageGraphicsView::ImageGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
    , mAnimatedLabel(new QLabel)
    , mMinimumZoom(defaultMinimumZoomScale)
    , mMaximumZoom(defaultMaximumZoomScale)
{
    setDragMode(QGraphicsView::ScrollHandDrag);

    auto scene = new QGraphicsScene(this);
    setScene(scene);

    mAnimatedLabel->setObjectName(QStringLiteral("mAnimatedLabel"));
    mAnimatedLabel->setBackgroundRole(QPalette::Base);
    mAnimatedLabel->setAlignment(Qt::AlignCenter);

    mGraphicsProxyWidget = scene->addWidget(mAnimatedLabel);
    mGraphicsProxyWidget->setObjectName(QStringLiteral("mGraphicsProxyWidget"));
    mGraphicsProxyWidget->setFlag(QGraphicsItem::ItemIsMovable, true);

    mGraphicsPixmapItem = scene->addPixmap({});
    mGraphicsPixmapItem->setTransformationMode(Qt::SmoothTransformation);

    updateRanges();
}

ImageGraphicsView::~ImageGraphicsView()
{
}

void ImageGraphicsView::updatePixmap(const QPixmap &pix, const QString &path)
{
    clearContents();
    if (!mImageInfo.isAnimatedImage) {
        mGraphicsPixmapItem->setPixmap(pix);
        QTimer::singleShot(0, this, [=] {
            updateRanges();

            fitToView();
        });
    } else {
        mMovie.reset(new QMovie(this));
        mMovie->setFileName(path);
        mMovie->start();
        mMovie->stop();
        mAnimatedLabel->setMovie(mMovie.data());

        QTimer::singleShot(0, this, [=] {
            mOriginalMovieSize = mMovie->currentPixmap().size();
            updateRanges();

            fitToView();
            mMovie->start();
        });
    }
}

void ImageGraphicsView::setImageInfo(const ShowImageWidget::ImageInfo &info)
{
    mImageInfo = info;
    if (info.needToDownloadBigImage) {
        // qDebug() << " Download big image " << info.needToDownloadBigImage;
        // We just need to download image not get url as it will be empty as we need to download it.
        (void)Ruqola::self()->rocketChatAccount()->attachmentUrlFromLocalCache(info.bigImagePath);
        updatePixmap(mImageInfo.pixmap, mImageInfo.bigImagePath); // TODO use correct path. big or not.
    } else {
        // Use big image.
        // qDebug() << " Ruqola::self()->rocketChatAccount()->attachmentUrlFromLocalCache(mImageInfo.bigImagePath).toLocalFile() " <<
        // Ruqola::self()->rocketChatAccount()->attachmentUrlFromLocalCache(mImageInfo.bigImagePath).toLocalFile(); qDebug() << " mImageInfo." <<
        // mImageInfo.previewImagePath;
        const QPixmap pix(Ruqola::self()->rocketChatAccount()->attachmentUrlFromLocalCache(mImageInfo.bigImagePath).toLocalFile());
        updatePixmap(pix, mImageInfo.bigImagePath);
    }
    // qDebug() << "ShowImageWidget::ImageInfo  " << info;
}

void ImageGraphicsView::zoomIn(QPointF centerPos)
{
    setZoom(zoom() * 1.1, centerPos);
}

void ImageGraphicsView::zoomOut(QPointF centerPos)
{
    setZoom(zoom() * 0.9, centerPos);
}

void ImageGraphicsView::clearContents()
{
    mOriginalMovieSize = {};
    mAnimatedLabel->setMovie(nullptr);
    mMovie.reset();

    mGraphicsPixmapItem->setPixmap({});
}

qreal ImageGraphicsView::minimumZoom() const
{
    return mMinimumZoom;
}

qreal ImageGraphicsView::maximumZoom() const
{
    return mMaximumZoom;
}

void ImageGraphicsView::updateRanges()
{
    const auto newMinimumZoom = fitToViewZoomScale(originalImageSize(), size());
    if (!qFuzzyCompare(mMinimumZoom, newMinimumZoom)) {
        mMinimumZoom = fitToViewZoomScale(originalImageSize(), size());
        Q_EMIT minimumZoomChanged(mMinimumZoom);
    }
    // note: mMaximumZoom is constant for now
}

void ImageGraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() == Qt::ControlModifier) {
        const int y = e->angleDelta().y();
        if (y < 0) {
            zoomOut(e->position());
        } else if (y > 0) {
            zoomIn(e->position());
        } // else: y == 0 => horizontal scroll => do not handle
    } else {
        QGraphicsView::wheelEvent(e);
    }
}

QSize ImageGraphicsView::originalImageSize() const
{
    if (mOriginalMovieSize.isValid()) {
        return mOriginalMovieSize;
    }

    return mGraphicsPixmapItem->pixmap().size();
}

const ShowImageWidget::ImageInfo &ImageGraphicsView::imageInfo() const
{
    return mImageInfo;
}

qreal ImageGraphicsView::zoom() const
{
    return transform().m11();
}

void ImageGraphicsView::setZoom(qreal zoom, QPointF centerPos)
{
    // clamp value
    zoom = qBound(minimumZoom(), zoom, maximumZoom());

    if (qFuzzyCompare(this->zoom(), zoom)) {
        return;
    }

    if (mIsUpdatingZoom) {
        return;
    }

    QScopedValueRollback<bool> guard(mIsUpdatingZoom, true);

    QPointF targetScenePos;
    if (!centerPos.isNull()) {
        targetScenePos = mapToScene(centerPos.toPoint());
    } else {
        targetScenePos = sceneRect().center();
    }

    ViewportAnchor oldAnchor = this->transformationAnchor();
    setTransformationAnchor(QGraphicsView::NoAnchor);

    QTransform matrix;
    matrix.translate(targetScenePos.x(), targetScenePos.y()).scale(zoom, zoom).translate(-targetScenePos.x(), -targetScenePos.y());
    setTransform(matrix);

    setTransformationAnchor(oldAnchor);
    Q_EMIT zoomChanged(zoom);
}

void ImageGraphicsView::fitToView()
{
    setZoom(fitToViewZoomScale(originalImageSize(), size()));
    centerOn(mGraphicsPixmapItem);
}

ShowImageWidget::ShowImageWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mImageGraphicsView(new ImageGraphicsView(this))
    , mZoomControls(new QWidget(this))
    , mZoomSpin(new QDoubleSpinBox(this))
    , mSlider(new QSlider(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mImageGraphicsView->setObjectName(QStringLiteral("mImageGraphicsView"));
    mainLayout->addWidget(mImageGraphicsView);
    connect(mImageGraphicsView, &ImageGraphicsView::zoomChanged, this, [this](qreal zoom) {
        mSlider->setValue(static_cast<int>(zoom * 100));
        mZoomSpin->setValue(zoom);
    });
    connect(mImageGraphicsView, &ImageGraphicsView::minimumZoomChanged, this, &ShowImageWidget::updateRanges);
    connect(mImageGraphicsView, &ImageGraphicsView::maximumZoomChanged, this, &ShowImageWidget::updateRanges);

    mZoomControls->setObjectName(QStringLiteral("zoomControls"));
    auto zoomLayout = new QHBoxLayout;
    zoomLayout->setObjectName(QStringLiteral("zoomLayout"));
    mZoomControls->setLayout(zoomLayout);
    mainLayout->addWidget(mZoomControls);

    auto mLabel = new QLabel(i18n("Zoom:"), this);
    mLabel->setObjectName(QStringLiteral("zoomLabel"));
    zoomLayout->addWidget(mLabel);

    mZoomSpin->setObjectName(QStringLiteral("mZoomSpin"));

    mZoomSpin->setValue(1);
    mZoomSpin->setDecimals(1);
    mZoomSpin->setSingleStep(0.1);
    zoomLayout->addWidget(mZoomSpin);

    mSlider->setObjectName(QStringLiteral("mSlider"));
    mSlider->setOrientation(Qt::Horizontal);
    zoomLayout->addWidget(mSlider);
    mSlider->setValue(mZoomSpin->value() * 100.0);

    auto resetButton = new QPushButton(i18n("100%"), this);
    resetButton->setObjectName(QStringLiteral("resetButton"));
    zoomLayout->addWidget(resetButton);
    connect(resetButton, &QPushButton::clicked, this, [=] {
        mImageGraphicsView->setZoom(1.0);
    });

    auto fitToViewButton = new QPushButton(i18n("Fit to View"), this);
    fitToViewButton->setObjectName(QStringLiteral("fitToViewButton"));
    zoomLayout->addWidget(fitToViewButton);
    connect(fitToViewButton, &QPushButton::clicked, mImageGraphicsView, &ImageGraphicsView::fitToView);

    connect(mZoomSpin, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value) {
        mImageGraphicsView->setZoom(static_cast<qreal>(value));
    });
    connect(mSlider, &QSlider::valueChanged, this, [this](int value) {
        mImageGraphicsView->setZoom(static_cast<qreal>(value) / 100);
    });

    if (mRocketChatAccount) {
        connect(mRocketChatAccount, &RocketChatAccount::fileDownloaded, this, &ShowImageWidget::slotFileDownloaded);
    }
    updateRanges();
}

ShowImageWidget::~ShowImageWidget()
{
}

void ShowImageWidget::slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl)
{
    const ImageInfo info = imageInfo();
    if (filePath == info.bigImagePath) {
        const QString cacheImageUrlPath{cacheImageUrl.toLocalFile()};
        const QPixmap pixmap(cacheImageUrlPath);
        mImageGraphicsView->updatePixmap(pixmap, cacheImageUrlPath);
    }
}

void ShowImageWidget::updateRanges()
{
    const auto min = mImageGraphicsView->minimumZoom();
    const auto max = mImageGraphicsView->maximumZoom();
    mZoomSpin->setRange(min, max);
    mSlider->setRange(min * 100.0, max * 100.0);
}

void ShowImageWidget::setImageInfo(const ShowImageWidget::ImageInfo &info)
{
    mImageGraphicsView->setImageInfo(info);
}

const ShowImageWidget::ImageInfo &ShowImageWidget::imageInfo() const
{
    return mImageGraphicsView->imageInfo();
}

void ShowImageWidget::saveAs()
{
    DelegateUtil::saveFile(this, mImageGraphicsView->imageInfo().bigImagePath, i18n("Save Image"));
}

QDebug operator<<(QDebug d, const ShowImageWidget::ImageInfo &t)
{
    d << "bigImagePath : " << t.bigImagePath;
    d << "previewImagePath : " << t.previewImagePath;
    d << "isAnimatedImage : " << t.isAnimatedImage;
    d << " pixmap is null ? " << t.pixmap.isNull();
    d << " needToDownloadBigImage ? " << t.needToDownloadBigImage;
    return d;
}
