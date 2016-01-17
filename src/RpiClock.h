#pragma once

#include <QWidget>
#include <QScopedPointer>
#include <QNetworkAccessManager>

class RpiClock : public QWidget
{
    Q_OBJECT
public:
    RpiClock(QWidget *parent=0);
protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent*);
private slots:
    void fileDownloaded(QNetworkReply * pReply);
private:
    void updateTemperature();

    QNetworkAccessManager forecast;

    QString temp_text;

    int timeDateTimer;
    int forecastTimer;
};
