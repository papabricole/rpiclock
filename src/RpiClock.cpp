#include "RpiClock.h"

#include <QTime>
#include <QTimerEvent>
#include <QPainter>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <cmath>

#include "dht11/pi_2_dht_read.h"

RpiClock::RpiClock(QWidget *parent) 
    : QWidget(parent)
    , mHumidity(0.f)
    , mTemperature(0.f)
    , mHomeIcon(":/images/home.png")
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
        float humidity = 0, temperature = 0;
        if (pi_2_dht_read(DHT11, 12, &humidity, &temperature) == DHT_SUCCESS) {
            mHumidity = humidity;
            mTemperature = temperature;
        }

        p.drawPixmap(QRect(0, 30, mHomeIcon.width(), mHomeIcon.height()), mHomeIcon);
        p.drawText(date_rect.adjusted(mHomeIcon.width(), 0, 0, 0), Qt::AlignCenter, " " + QString::number(mTemperature) + QChar(0260) + " " + QString::number(mHumidity) + "%");
    }
    font.setPixelSize(170);
    p.setFont(font);
    p.drawText(time_rect, Qt::AlignCenter, time_text);
}

void RpiClock::updateTemperature()
{
#ifdef WUNDERGRUND
    // Forecast Berlin: http://api.wunderground.com/api/xxxxxxxxxxxx/conditions/q/pws:IBERLIN2709.json
    const QString APP_ID = "01748a07052b5f92";
    const QUrl url("http://api.wunderground.com/api/" + APP_ID + "/conditions/q/pws:IBERLIN15.json");
#else
    const QUrl url("https://io.adafruit.com/api/v2/morganleborgne/feeds/temperature");
#endif
    QNetworkRequest request(url);
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    forecast.get(request);
}

void RpiClock::fileDownloaded(QNetworkReply * pReply)
{
    const QByteArray downloadedData = pReply->readAll();

    pReply->deleteLater();
#ifdef WUNDERGRUND
    QRegExp rx("\"temp_c\":([-+]?[0-9]*.?[0-9]+),");
#else
    QRegExp rx("\"last_value\":\"([-+]?[0-9]*.?[0-9]+)\",");
#endif
    if( rx.indexIn( downloadedData ) >= 0 ) {
        int temperature = round(rx.cap(1).toFloat());
        temp_text = QString::number(temperature) + QChar(0260);
    } else {
        temp_text = QString("--") + QChar(0260);
    }
    update();
}


