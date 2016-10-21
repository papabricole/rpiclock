#include "RpiClock.h"

#include <QTime>
#include <QTimerEvent>
#include <QPainter>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <cmath>

RpiClock::RpiClock(QWidget *parent) : QWidget(parent)
{
    connect(&forecast, SIGNAL(finished(QNetworkReply*)), SLOT(fileDownloaded(QNetworkReply*)));

    timeDateTimer.setInterval(7*1000);
    forecastTimer.setInterval(30*60*1000);

    connect(&timeDateTimer, SIGNAL(timeout()), this, SLOT(update()));
    connect(&forecastTimer, SIGNAL(timeout()), this, SLOT(updateTemperature()));

    updateTemperature();

    timeDateTimer.start();
    forecastTimer.start();
}

void RpiClock::paintEvent(QPaintEvent *event)
{
    //480x320
    QRect date_rect = QRect(0, 30, 480, 80);
    QRect time_rect = QRect(0, 80, 480, 320-80);

    const QString date_text = QDate::currentDate().toString("dd MMM");
    const QString time_text = QTime::currentTime().toString("hh:mm");

    QFont font;
    font.setFamily("Helvetica");

    QPainter p(this);
    p.fillRect(rect(), Qt::black);
    p.setPen(Qt::white);

    font.setPixelSize(80);
    p.setFont(font);
    if (timeDateTimer.interval() == 3*1000) {
        timeDateTimer.setInterval(7*1000);
        p.drawText(date_rect, Qt::AlignCenter, date_text + " " + temp_text);
    } else {
        timeDateTimer.setInterval(3*1000);
        p.drawText(date_rect, Qt::AlignCenter, QString("20") + QChar(0260) + " 60%");
    }
    font.setPixelSize(170);
    p.setFont(font);
    p.drawText(time_rect, Qt::AlignCenter, time_text);
}

void RpiClock::updateTemperature()
{
    // Forecast Berlin: http://api.wunderground.com/api/xxxxxxxxxxxx/conditions/q/pws:IBERLIN2709.json
    const QString APP_ID = "---";
    const QUrl url("http://api.wunderground.com/api/" + APP_ID + "/conditions/q/pws:IBERLIN2709.json");

    QNetworkRequest request(url);
    forecast.get(request);
}

void RpiClock::fileDownloaded(QNetworkReply * pReply)
{
    const QByteArray downloadedData = pReply->readAll();

    pReply->deleteLater();

    QRegExp rx("\"temp_c\":([-+]?[0-9]*.?[0-9]+),");
    if( rx.indexIn( downloadedData ) >= 0 ) {
        int temperature = round(rx.cap(1).toFloat());
        temp_text = QString::number(temperature) + QChar(0260);
    } else {
        temp_text = QString("--") + QChar(0260);
    }
    update();
}


