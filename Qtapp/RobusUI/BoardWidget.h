#pragma once
#include <QWidget>
#include <QVector>
#include <QPoint>

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    enum Cell { Empty, Ship, Hit, Miss };

    explicit BoardWidget(QWidget *parent = nullptr);

    void setCell(int row, int col, Cell state);
    Cell cell(int row, int col) const;
    void clear();
    void setPreview(const QVector<QPoint> &cells, bool valid); // (col,row) points
    void clearPreview();

    QSize sizeHint() const override { return QSize(420, 420); }

    void setShip(int row, int col, int boatId);   // boatId >= 1

signals:
    void cellClicked(int row, int col);
    void cellHovered(int row, int col);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;

private:
    double cellSize() const;
    QPoint cellAt(const QPoint &pos) const; // (col,row) or (-1,-1)

    static constexpr int N = 10;
    Cell m_cells[N][N] = {};
    QVector<QPoint> m_preview;
    bool m_previewValid = true;

    int m_ids[N][N] = {};
    QColor shipColor(int id) const;
};
