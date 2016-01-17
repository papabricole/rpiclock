#include "RpiClock.h"

#include <QTime>
#include <QTimerEvent>
#include <QPainter>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

RpiClock::RpiClock(QWidget *parent) : QWidget(parent)
{
    connect(&forecast, SIGNAL(finished(QNetworkReply*)), SLOT(fileDownloaded(QNetworkReply*)));

    timeDateTimer = startTimer(30*1000);
    forecastTimer = startTimer(30*60*1000);

    updateTemperature();
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
    p.drawText(date_rect, Qt::AlignCenter, date_text + " " + temp_text);

    font.setPixelSize(170);
    p.setFont(font);
    p.drawText(time_rect, Qt::AlignCenter, time_text);
}

void RpiClock::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == timeDateTimer) {
        update();
    }
    if (event->timerId() == forecastTimer) {
        updateTemperature();
    }
}

void RpiClock::updateTemperature()
{
    // Forecast Berlin: http://api.openweathermap.org/data/2.5/weather?id=2950159
    // API: http://openweathermap.org/wiki/API/1.0/JSON_API
    const QString APP_ID = "e2b075d68c39dc43e16995653fcd6fd0";
    const QUrl url("http://api.openweathermap.org/data/2.5/weather?id=2950159&appid=" + APP_ID);

    QNetworkRequest request(url);
    forecast.get(request);
}

void RpiClock::fileDownloaded(QNetworkReply * pReply)
{
    const QByteArray downloadedData = pReply->readAll();

    pReply->deleteLater();

    QRegExp rx("\"temp\":([-+]?[0-9]*.?[0-9]+),");
    if( rx.indexIn( downloadedData ) >= 0 ) {
        int temperature = rx.cap(1).toFloat() - 273.15f;
        temp_text = QString::number(temperature) + QChar(0260);
    } else {
        temp_text = QString("--") + QChar(0260);
    }
    update();
}
