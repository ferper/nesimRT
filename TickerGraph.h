#ifndef TICKERGRAPH_H
#define TICKERGRAPH_H

#include <QDateTime>
#include <QHash>
#include <QPen>
#include <QQueue>
#include <QWidget>

struct DataSeriesValue {
    QDateTime mTimeStamp;
    double mValue;
};

struct DataSeries {
    QString mLabel;
    QQueue<DataSeriesValue> mData;
    QPen mDataLinePen;
    QColor mFillColor;
};

class TickerGraph : public QWidget
{
    Q_OBJECT
public:
    explicit TickerGraph(QWidget * parent = nullptr);

    void paintEvent(QPaintEvent * pEvent);

    // Data-related parameters
    QString units() const;
    void setUnits(const QString & unit);

    QPair<double, double> range() const;
    void setRange(const QPair<double, double> & p);
    void setRange(double min, double max);

    bool autoRange() const;
    void setAutoRange(bool value);

    int pointWidth() const;
    void setPointWidth(int w);

    QList<double> referencePoints() const;
    void setReferencePoints(const QList<double> & points);

    // Style-related parameters
    QBrush backgroundBrush() const;
    void setBackgroundBrush(const QBrush & brush);

    QPen gridPen() const;
    void setGridPen(const QPen & pen);

    double gridPitch() const;
    void setGridPitch(double pitch);

    QColor borderColor() const;
    void setBorderColor(const QColor & borderColor);

    // Series-related
    void addSeries(const QString & series);
    void removeSeries(const QString & series);
    bool hasSeries(const QString & series) const;

    QPen dataLinePen(const QString & seriesName) const;
    void setDataLinePen(const QString & seriesName, const QPen & pen);

    QColor fillColor(const QString & series) const;
    void setFillColor(const QString & series, const QColor & fillColor);

    QColor axisColor() const;
    void setAxisColor(const QColor & color);

    QFont axisFont() const;
    void setAxisFont(const QFont & font);

    QColor labelColor() const;
    void setLabelColor(const QColor & color);

    QFont labelFont() const;
    void setLabelFont(const QFont & font);

    void enableLabel();
    void disableLabel();

    // Adding and clearing data points
    void appendPoint(const QString & seriesName, double point, QDateTime tstamp = QDateTime::currentDateTime());
    void clear();

    int timespan() const;
    void setTimespan(int timespan);

    qreal tickSeparation() const;
    void setTickSeparation(qreal tickSeparation);

    void enableTicks();
    void disableTicks();

private:
    const int kQueueCapacity = 2048;

    QHash<QString, DataSeries> mSeries;

    int mPointWidth;
    QString mUnits;

    // Reference points
    QList<double> mReferencePoints;

    // Data range visible in graph
    double mMin;
    double mMax;

    bool mAutoRange = false;

    bool mShowLabel = true;
    bool mShowTicks = true;

    // Style

    QColor mBorderColor;
    QBrush mBackgroundBrush;
    QPen mGridPen;
    double mGridPitch;

    QColor mAxisColor;
    QFont mAxisFont;
    QColor mLabelColor;
    QFont mLabelFont;

    qreal mLastRightmostTickPosition = 0;
    qreal mTickSeparation = 40;

    /// Graph time span in seconds
    int mTimespan = 120;

    QDateTime mPreviousRightmostTimestamp;
};

#endif // TICKERGRAPH_H
