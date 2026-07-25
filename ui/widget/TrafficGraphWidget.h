#pragma once

#include <QWidget>
#include <QPainter>
#include <QList>
#include <QPair>

class TrafficGraphWidget : public QWidget {
    Q_OBJECT
public:
    explicit TrafficGraphWidget(QWidget *parent = nullptr);
    void addData(quint64 upSpeed, quint64 downSpeed);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<QPair<quint64, quint64>> m_data;
    int m_maxDataPoints = 60;
    quint64 m_maxSpeed = 1024;
};
