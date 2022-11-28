#include "TickerGraph.h"
#include <QDebug>
#include <QtGui/QPainter>

#include <QDebug>

namespace {
    const double DEFAULT_MIN = 0.0;
    const double DEFAULT_MAX = 100.0;
    const double DEFAULT_GRID_PITCH = 10.0;
    const char DEFAULT_LABEL_FONT_FAMILY[] = "Arial";
    const int DEFAULT_LABEL_FONT_SIZE = 12;
    const Qt::GlobalColor DEFAULT_LABEL_COLOR = Qt::blue;
    const int LABEL_MARGIN = 2;
}

TickerGraph::TickerGraph(QWidget * parent)
    : QWidget(parent)
    , mPointWidth(1)
    , mMin(DEFAULT_MIN)
    , mMax(DEFAULT_MAX)
    , mBorderColor(QColor(0, 128, 64))
    , mBackgroundBrush(QBrush(Qt::black))
    , mGridPen(QColor(0, 128, 64))
    , mGridPitch(DEFAULT_GRID_PITCH)
    , mAxisColor(DEFAULT_LABEL_COLOR)
    , mLabelColor(DEFAULT_LABEL_COLOR)
    , mLabelFont(DEFAULT_LABEL_FONT_FAMILY, DEFAULT_LABEL_FONT_SIZE)
{
    mAxisFont.setPointSize(8);
}

/**
 * Paint event handler
 */
void TickerGraph::paintEvent(QPaintEvent *)
{
    int graphWidth = width();
    int graphHeight;

    if (mShowTicks)
        graphHeight = height() - 30;
    else
        graphHeight = height();

    QRectF graphRect = rect();
    graphRect.setWidth(graphWidth);
    graphRect.setHeight(graphHeight);

    // Vertical scale
    const double scale = (mMax > mMin) ? (graphHeight / (mMax - mMin)) : 1.0;
    Q_UNUSED(scale);

    // Setup painter
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Draw the background
    painter.fillRect(graphRect, mBackgroundBrush);

    // Draw the border
    painter.setPen(mBorderColor);
    painter.drawRect(graphRect.adjusted(0.5, 0.5, -0.5, -0.5));

    // Draw the grid lines
    const int pitchInPixels = mGridPitch;

    if (pitchInPixels > 0) {
        painter.setPen(mGridPen);
        for (int x = 0; x < graphWidth; x += pitchInPixels)
            painter.drawLine(QPointF(x + 0.5, 0 + 0.5), QPoint(x + 0.5, graphHeight + 0.5));

        for (int y = 0; y < graphHeight; y += pitchInPixels)
            painter.drawLine(QPointF(0 + 0.5, y + 0.5), QPoint(graphWidth + 0.5, y + 0.5));
    }
    // Plot the data lines
    // We'll update the autorange on the fly
    bool rangeInitialized = false;
    double nextMin=0.0;
    double nextMax=0.0;

    QDateTime rightmostTimestamp;

    for (DataSeries & series : mSeries.values()) {
        // Make sure there's data in the series
        if (series.mData.size() == 0)
            continue;

        // Setup pen
        QPen seriesPen = series.mDataLinePen;
        painter.setPen(seriesPen);

        // Calculate the graph scale
        int seriesPenWidth = series.mDataLinePen.width();
        int availableHeight = graphHeight - seriesPenWidth * 2;
        const double scale = availableHeight / (mMax - mMin);
        Q_UNUSED(scale);

        QPolygonF dataPoints;
        qreal lastHorizontalPosition = 1;
        qreal horizontalPosition=0;
        qreal verticalPosition;

        // We'll draw the elements from right to left (newer to older)
        auto dataIterator = series.mData.crbegin();

        // Keep track of rightmost timestamp
        if (rightmostTimestamp.isNull())
            rightmostTimestamp = dataIterator->mTimeStamp;

        while (dataIterator != series.mData.crend()) {
            // The horizontal separation depends on the time delta between the current
            // point and the most recent point
            qreal msDelta = dataIterator->mTimeStamp.msecsTo(series.mData.crbegin()->mTimeStamp);
            horizontalPosition = graphWidth * (1 - msDelta / (mTimespan * 1000));

            // The vertical position takes into account the pen width
            verticalPosition = seriesPenWidth + availableHeight - scale * (dataIterator->mValue - mMin);
            dataPoints << QPointF(horizontalPosition, verticalPosition);

            // Reset autorange if needed
            if (mAutoRange) {
                if (!rangeInitialized) {
                    nextMin = dataIterator->mValue;
                    nextMax = dataIterator->mValue;
                    rangeInitialized = true;
                }
                else if (dataIterator->mValue > nextMax)
                    nextMax = dataIterator->mValue;
                else if (dataIterator->mValue < nextMin)
                    nextMin = dataIterator->mValue;
            }

            // Stop drawing once we reach the left border (x=0)
            if (lastHorizontalPosition > 0 && horizontalPosition <= 0)
                break;
            lastHorizontalPosition = horizontalPosition;
            dataIterator++;
        }

        // Close the polygon
        dataPoints.push_back(QPointF(horizontalPosition, graphHeight));
        dataPoints.push_front(QPointF(graphWidth, graphHeight));

        // Draw the outline
        painter.drawPolyline(dataPoints);

        // Draw the fill
        painter.setPen(Qt::NoPen);
        painter.setBrush(series.mFillColor);
        painter.drawConvexPolygon(dataPoints);
        painter.setBrush(QBrush());
    }

    // Update range if enabled
    if (mAutoRange) {
        mMin = nextMin - 1;
        mMax = 1.10 * nextMax;
    }

    // Draw the horizontal axis ticks

    if (mShowTicks) {
        // Base case, last timestamp is null, initialize it
        if (mPreviousRightmostTimestamp.isNull())
            mPreviousRightmostTimestamp = rightmostTimestamp;

        // Calculate left limit of the visible range of dates
        QDateTime leftmostTimestamp = rightmostTimestamp.addSecs(-mTimespan);

        // Container for ticks
        QSet<QDateTime> ticks;

        if (leftmostTimestamp.msecsTo(mPreviousRightmostTimestamp) >= mTickSeparation * 1000) {
            QDateTime aux = mPreviousRightmostTimestamp;
            while (aux > leftmostTimestamp) {
                ticks << aux;
                aux = aux.addMSecs(-mTickSeparation * 1000);
            }
        }
        if (mPreviousRightmostTimestamp.msecsTo(rightmostTimestamp) >= mTickSeparation * 1000) {
            QDateTime aux = mPreviousRightmostTimestamp;

            while (aux < rightmostTimestamp) {
                ticks << aux;
                aux = aux.addMSecs(mTickSeparation * 1000);
            }
        }

        if (mPreviousRightmostTimestamp < leftmostTimestamp)
            mPreviousRightmostTimestamp = rightmostTimestamp;

        // Setup the pen to draw the ticks
        painter.setPen(mBorderColor);
        painter.setFont(mAxisFont);

        qreal tickSeparationInPx = mTickSeparation * graphWidth / (qreal)mTimespan;

        for (const QDateTime & tick : ticks) {
            qreal tickDeltaMs = tick.msecsTo(rightmostTimestamp);
            qreal tickDeltaPerc = tickDeltaMs / (mTimespan * 1000);
            qreal tickX = graphWidth * (1 - tickDeltaPerc);

            // Draw the tick
            painter.drawLine(tickX, graphHeight, tickX, graphHeight + 5);

            // Compute the tick text
            QString tickText = tick.toString("mm:ss.zzz");

            // Draw the tick text
            painter.drawText(tickX - tickSeparationInPx / 2, graphHeight + 5, tickSeparationInPx, graphHeight,
                             Qt::AlignHCenter, tickText);
        }
    }

    // Draw the min and max values in the left margin
    painter.setPen(mAxisColor);
    painter.setFont(mAxisFont);
    QString minLabel = QString("%1 %2").arg(mMin).arg(mUnits);
    QString maxLabel = QString("%1 %2").arg(mMax).arg(mUnits);
    painter.drawText(graphRect.adjusted(LABEL_MARGIN, LABEL_MARGIN, -LABEL_MARGIN, -LABEL_MARGIN),
                     Qt::AlignTop | Qt::AlignLeft, maxLabel);
    painter.drawText(graphRect.adjusted(LABEL_MARGIN, LABEL_MARGIN, -LABEL_MARGIN, -LABEL_MARGIN),
                     Qt::AlignBottom | Qt::AlignLeft, minLabel);

    QString ecuadorLabel = QString("%1 %2").arg(mMax-(abs(mMax-mMin))/2).arg(mUnits);
    painter.drawText(graphRect.adjusted(LABEL_MARGIN, LABEL_MARGIN, -LABEL_MARGIN, -LABEL_MARGIN),
                     Qt::AlignVCenter | Qt::AlignLeft, ecuadorLabel);

    QString uppermiddleLabel = QString("%1 %2").arg(mMax-abs((abs(mMax-mMin))/4)).arg(mUnits);
    painter.drawText(0, graphRect.height()/4  , uppermiddleLabel);


    QString lowermiddleLabel = QString("%1 %2").arg(mMin+abs(((abs(mMax-mMin))/4))).arg(mUnits);
    painter.drawText(0, graphRect.height()-abs(((abs(graphRect.height())-abs(0))/4)) , lowermiddleLabel);


    // Draw the reference labels, if any
    const int axisLabelHeight = QFontMetrics(mAxisFont).height();
    foreach (double point, mReferencePoints) {
        double y = graphHeight - scale * (point - mMin);

        QString label = QString("%1 %2").arg(point).arg(mUnits);
        painter.drawText(QRect(LABEL_MARGIN, y - axisLabelHeight / 2, graphWidth - LABEL_MARGIN, axisLabelHeight),
                         Qt::AlignVCenter | Qt::AlignLeft, label);
    }
}

// The units to be displayed along with the current value
QString TickerGraph::units() const {
    return mUnits;
}

/**
 * Specifies the units to be displayed along with the current value.
 * For example, if the graph is to display voltage:
 *     graph->setUnits("V")
 */
void TickerGraph::setUnits(const QString & units) {
    if (units != mUnits){
        mUnits = units;
        if (!mSeries.isEmpty())
            update();
    }
}

/**
 * The range of the data that the graph is meant to display.
 */
QPair<double, double> TickerGraph::range() const {
    return QPair<double, double>(mMin, mMax);
}

void TickerGraph::setRange(double min, double max) {
    if ((min != mMin || max != mMax) && (mMax > mMin)) {
        mMin = min;
        mMax = max;
        if (!mSeries.isEmpty())
            update();
    }
}

bool TickerGraph::autoRange() const {
    return mAutoRange;
}

void TickerGraph::setAutoRange(bool value) {
    mAutoRange = value;
}

/**
 * Sets the range of data that this graph is meant to display.
 */
void TickerGraph::setRange(const QPair<double, double> & range) {
    setRange(range.first, range.second);
}

/**
 * The horizontal distance in pixels between consecutive data points.
 */
int TickerGraph::pointWidth() const {
    return mPointWidth;
}

/**
 * Sets the horizontal distance in pixels between consecutive data points.
 */
void TickerGraph::setPointWidth(int w) {
    if (w != mPointWidth) {
        mPointWidth = w;
        if (!mSeries.isEmpty())
            update();
    }
}

/**
 * A list of levels, aside from the min and max, that should be labelled on the graph.
 */
QList<double> TickerGraph::referencePoints() const {
    return mReferencePoints;
}

/**
 * Sets the list of levels, aside from the min and max, that should be labelled on the graph.
 */
void TickerGraph::setReferencePoints(const QList<double> & points) {
    if (points != mReferencePoints) {
        mReferencePoints = points;
        update();
    }
}

/**
 * The brush used to fill the background of the graph.
 */
QBrush TickerGraph::backgroundBrush() const {
    return mBackgroundBrush;
}

/**
 * Sets the brush used to fill the background of the graph.
 * The default brush is a solid fill black brush.
 */
void TickerGraph::setBackgroundBrush(const QBrush & brush) {
    if (brush != mBackgroundBrush) {
        mBackgroundBrush = brush;
        update();
    }
}

/**
 * The pen used to draw the grid lines.
 */
QPen TickerGraph::gridPen() const {
    return mGridPen;
}

/**
 * Sets the pen used to draw the grid lines.
 * The default pen is a single pixel wide and green.
 */
void TickerGraph::setGridPen(const QPen & pen) {
    if (pen != mGridPen) {
        mGridPen = pen;
        update();
    }
}

/**
 * The spacing between adjacent horizontal or vertical grid lines,
 * in the same units as the data values. The default is 10.
 */
double TickerGraph::gridPitch() const {
    return mGridPitch;
}

/**
 * Sets the spacing between adjacent horizontal or vertical grid lines,
 * in a fixed pixel size. The default is 10.
 */
void TickerGraph::setGridPitch(double pitch) {
    if (pitch != mGridPitch) {
        mGridPitch = pitch;
        update();
    }
}

/**
 * The pen used for drawing grid lines on the graph.
 */
QPen TickerGraph::dataLinePen(const QString & seriesName) const {
    const DataSeries & series = mSeries[seriesName];

    return series.mDataLinePen;
}

/**
 * Sets the pen used for drawing the data lines on the graph.
 * The default pen is a single pixel wide and green.
 */
void TickerGraph::setDataLinePen(const QString & seriesName, const QPen & pen) {
    DataSeries & series = mSeries[seriesName];

    if (pen != series.mDataLinePen) {
        series.mDataLinePen = pen;

        if (!series.mData.isEmpty())
            update();
    }
}

/**
 * The color for the value label.
 */
QColor TickerGraph::labelColor() const {
    return mLabelColor;
}

/**
 * Sets the color for the value label. The default color is white.
 */
void TickerGraph::setLabelColor(const QColor & color) {
    if (color != mLabelColor) {
        mLabelColor = color;
        update();
    }
}

/**
 * The font used for drawing the current value label.
 */
QFont TickerGraph::labelFont() const {
    return mLabelFont;
}

/**
 * Sets the font used for drawing the current value label at
 * the top left corner of the graph.
 * The default font is 12pt Arial.
 */
void TickerGraph::setLabelFont(const QFont & font) {
    if (font != mLabelFont) {
        mLabelFont = font;

        if (!mSeries.isEmpty())
            update();
    }
}

/**
 * The color used to draw the min and max value labels on the vertical axis.
 */
QColor TickerGraph::axisColor() const {
    return mAxisColor;
}

/**
 * Sets the color used for the min and max value labels on the vertical
 * axis. The default color used is white.
 */
void TickerGraph::setAxisColor(const QColor & color) {
    if (color != mAxisColor) {
        mAxisColor = color;
        update();
    }
}

/**
 * The font used for the min and max value labels on the vertical axis.
 */
QFont TickerGraph::axisFont() const {
    return mAxisFont;
}

/**
 * Sets the font used for the min and max value labels on the vertical axis.
 * The default font is 8pt Arial.
 */
void TickerGraph::setAxisFont(const QFont & font) {
    if (font != mAxisFont) {
        mAxisFont = font;
        update();
    }
}

/**
 * Appends a new data point to the graph, and redraws the graph with
 * the new point added to the right edge.
 */
void TickerGraph::appendPoint(const QString & seriesName, double point, QDateTime tstamp) {
    DataSeries & series = mSeries[seriesName];
    DataSeriesValue value;

    value.mTimeStamp = tstamp;
    value.mValue = point;
    series.mData.enqueue(value);

    while (series.mData.size() > kQueueCapacity)
        series.mData.dequeue();

    update();
}

/**
 * Clears all of the data from the graph and redraws the graph with no data
 */
void TickerGraph::clear() {
    mSeries.clear();
    update();
}

int TickerGraph::timespan() const {
    return mTimespan;
}

void TickerGraph::setTimespan(int timespan) {
    mTimespan = timespan;
}

qreal TickerGraph::tickSeparation() const {
    return mTickSeparation;
}

void TickerGraph::setTickSeparation(qreal tickSeparation) {
    mTickSeparation = tickSeparation;
}

void TickerGraph::enableTicks() {
    mShowTicks = true;
}

void TickerGraph::disableTicks() {
    mShowTicks = false;
}

void TickerGraph::enableLabel() {
    mShowLabel = true;
}

void TickerGraph::disableLabel() {
    mShowLabel = false;
}

QColor TickerGraph::borderColor() const {
    return mBorderColor;
}

void TickerGraph::setBorderColor(const QColor & borderColor) {
    mBorderColor = borderColor;
}

void TickerGraph::addSeries(const QString & series) {
    DataSeries newSeries;

    newSeries.mLabel = series;
    mSeries[series] = newSeries;
}

void TickerGraph::removeSeries(const QString & series) {
    mSeries.remove(series);
}

bool TickerGraph::hasSeries(const QString & series) const {
    return mSeries.contains(series);
}

QColor TickerGraph::fillColor(const QString & series) const {
    return mSeries[series].mFillColor;
}

void TickerGraph::setFillColor(const QString & series, const QColor & fillColor) {
    mSeries[series].mFillColor = fillColor;
}
