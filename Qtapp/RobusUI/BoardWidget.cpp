#include "BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>

BoardWidget::BoardWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);   // needed for hover without clicking
    setMinimumSize(300, 300);
}

double BoardWidget::cellSize() const
{
    return qMin(width(), height()) / double(N + 1);   // +1 for the A-J / 1-10 labels
}

void BoardWidget::setCell(int r, int c, Cell s) { m_cells[r][c] = s; update(); }
BoardWidget::Cell BoardWidget::cell(int r, int c) const { return m_cells[r][c]; }

void BoardWidget::clear()
{
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c) { m_cells[r][c] = Empty; m_ids[r][c] = 0; }
    update();
}

void BoardWidget::setPreview(const QVector<QPoint> &cells, bool valid)
{
    m_preview = cells; m_previewValid = valid; update();
}
void BoardWidget::clearPreview() { m_preview.clear(); update(); }

QPoint BoardWidget::cellAt(const QPoint &pos) const
{
    double cs = cellSize();
    int col = int((pos.x() - cs) / cs);
    int row = int((pos.y() - cs) / cs);
    if (pos.x() < cs || pos.y() < cs || col >= N || row >= N) return QPoint(-1, -1);
    return QPoint(col, row);
}

void BoardWidget::mousePressEvent(QMouseEvent *e)
{
    QPoint p = cellAt(e->pos());
    if (p.x() >= 0) emit cellClicked(p.y(), p.x());
}

void BoardWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = cellAt(e->pos());
    if (p.x() >= 0) emit cellHovered(p.y(), p.x());
}

void BoardWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    const double cs = cellSize();

    // labels
    p.setPen(QColor("#8fa3b8"));
    QFont f = font(); f.setPixelSize(int(cs * 0.4)); p.setFont(f);
    for (int i = 0; i < N; ++i) {
        p.drawText(QRectF((i + 1) * cs, 0, cs, cs), Qt::AlignCenter, QString::number(i + 1));
        p.drawText(QRectF(0, (i + 1) * cs, cs, cs), Qt::AlignCenter, QString(QChar('A' + i)));
    }

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            QRectF cell((c + 1) * cs, (r + 1) * cs, cs, cs);
            QRectF inner = cell.adjusted(1.5, 1.5, -1.5, -1.5);

            // water
            p.setPen(Qt::NoPen);
            p.setBrush(QColor("#1b4f7a"));
            p.drawRoundedRect(inner, 4, 4);

            switch (m_cells[r][c]) {
            case Ship: {
                int id = m_ids[r][c];
                auto same = [&](int rr, int cc) {
                    return rr >= 0 && rr < N && cc >= 0 && cc < N &&
                           m_cells[rr][cc] == Ship && m_ids[rr][cc] == id;
                };
                // extend toward neighbours of the same boat so it looks like one piece
                const double gap = 4;
                QRectF boat = cell.adjusted(same(r, c - 1) ? 0 : gap,
                                            same(r - 1, c) ? 0 : gap,
                                            same(r, c + 1) ? 0 : -gap,
                                            same(r + 1, c) ? 0 : -gap);
                p.setBrush(shipColor(id));
                p.setPen(Qt::NoPen);
                p.drawRoundedRect(boat, 6, 6);
                break;
            }
            case Hit:
                p.setBrush(QColor("#d64545"));
                p.setPen(Qt::NoPen);
                p.drawEllipse(cell.center(), cs * 0.28, cs * 0.28);
                break;
            case Miss:
                p.setBrush(QColor("#e8f1f8"));
                p.setPen(Qt::NoPen);
                p.drawEllipse(cell.center(), cs * 0.1, cs * 0.1);
                break;
            default: break;
            }
        }
    }

    // placement preview (green = valid, red = invalid)
    QColor pc = m_previewValid ? QColor(80, 200, 120, 140) : QColor(220, 70, 70, 140);
    p.setPen(Qt::NoPen); p.setBrush(pc);
    for (const QPoint &pt : m_preview) {
        if (pt.x() < 0 || pt.x() >= N || pt.y() < 0 || pt.y() >= N) continue;
        p.drawRoundedRect(QRectF((pt.x() + 1) * cs + 1.5, (pt.y() + 1) * cs + 1.5, cs - 3, cs - 3), 4, 4);
    }
}

void BoardWidget::setShip(int r, int c, int boatId)
{
    m_cells[r][c] = Ship;
    m_ids[r][c] = boatId;
    update();
}

QColor BoardWidget::shipColor(int id) const
{
    static const QColor palette[] = {
        "#8a9bab", "#e0a84a", "#6cc08b", "#c77dba", "#d9786c", "#5fb3c9"
    };
    return palette[(qMax(id, 1) - 1) % 6];
}
