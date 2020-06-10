#pragma once

#include <QObject>

#include <atomic>

class DHT11 : public QObject
{
    Q_OBJECT
  public:
    explicit DHT11(QObject* parent = 0);

    Q_INVOKABLE void update();
    Q_INVOKABLE int temperature() const { return m_temperature; }
    Q_INVOKABLE int humidity() const { return m_humidity; }

  private:
    std::atomic<int> m_temperature;
    std::atomic<int> m_humidity;
};
