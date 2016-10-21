#pragma once

#include <QWidget>
#include <QTimer>
#include <QScopedPointer>
#include <QNetworkAccessManager>

class RpiClock : public QWidget
{
    Q_OBJECT
public:
    RpiClock(QWidget *parent=0);
protected:
    void paintEvent(QPaintEvent *event);
private slots:
    void updateTemperature();
    void fileDownloaded(QNetworkReply * pReply);
private:

    QNetworkAccessManager forecast;

    QString temp_text;

    QTimer timeDateTimer;
    QTimer forecastTimer;
};
