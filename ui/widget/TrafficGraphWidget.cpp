#include "TrafficGraphWidget.h"
#include <QPainterPath>

TrafficGraphWidget::TrafficGraphWidget(QWidget *parent) : QWidget(parent) {
    setMinimumSize(100, 30);
    setMaximumWidth(150);
}

void TrafficGraphWidget::addData(quint64 upSpeed, quint64 downSpeed) {
    m_data.append(qMakePair(upSpeed, downSpeed));
    if (m_data.size() > m_maxDataPoints) {
        m_data.removeFirst();
    }
    
    // Auto scale
    m_maxSpeed = 1024; // reset to 1KB/s
    for (const auto &pair : m_data) {
        if (pair.first > m_maxSpeed) m_maxSpeed = pair.first;
        if (pair.second > m_maxSpeed) m_maxSpeed = pair.second;
    }
    
    update();
}

void TrafficGraphWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int w = width();
    int h = height();

    // Draw background
    QPainterPath bgPath;
    bgPath.addRoundedRect(0, 0, w, h, 4, 4);
    painter.fillPath(bgPath, QColor(40, 44, 52, 180)); // sleek dark background
    
    if (m_data.isEmpty()) return;
    
    float stepX = (float)w / (m_maxDataPoints - 1);
    
    QPainterPath upPath, downPath;
    QPainterPath upFill, downFill;
    
    upFill.moveTo(0, h);
    downFill.moveTo(0, h);
    
    for (int i = 0; i < m_data.size(); ++i) {
        float x = i * stepX;
        // Clamp visually
        float upY = h - ((float)m_data[i].first / m_maxSpeed) * (h - 4) - 2;
        float downY = h - ((float)m_data[i].second / m_maxSpeed) * (h - 4) - 2;
        
        if (i == 0) {
            upPath.moveTo(x, upY);
            downPath.moveTo(x, downY);
            upFill.lineTo(x, upY);
            downFill.lineTo(x, downY);
        } else {
            upPath.lineTo(x, upY);
            downPath.lineTo(x, downY);
            upFill.lineTo(x, upY);
            downFill.lineTo(x, downY);
        }
    }
    
    float lastX = (m_data.size() - 1) * stepX;
    upFill.lineTo(lastX, h);
    downFill.lineTo(lastX, h);
    
    // Fill Upload (Soft Red/Purple)
    painter.fillPath(upFill, QColor(255, 100, 150, 40));
    // Fill Download (Soft Cyan/Green)
    painter.fillPath(downFill, QColor(100, 255, 200, 40));
    
    // Draw Upload Line
    painter.setPen(QPen(QColor(255, 100, 150), 1.5));
    painter.drawPath(upPath);
    
    // Draw Download Line
    painter.setPen(QPen(QColor(100, 255, 200), 1.5));
    painter.drawPath(downPath);
}
