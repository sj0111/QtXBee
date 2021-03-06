#ifndef TEMPMONITOR_H
#define TEMPMONITOR_H

#include <QObject>
#include <QTimer>

namespace QtXBee {
class XBee;
namespace Wpan {
class RxResponse16;
}
}

class TempMonitor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float currentTemperature READ currentTemperature NOTIFY currentTemperatureChanged)
    Q_PROPERTY(float minTemperature READ minTemperature NOTIFY minTemperatureChanged)
    Q_PROPERTY(float maxTemperature READ maxTemperature NOTIFY maxTemperatureChanged)
public:
    explicit TempMonitor(const QString & serialPort, QObject * parent = 0);
    ~TempMonitor();

    void setTempSensorAddress(const quint16 address);
    float currentTemperature() const;
    float minTemperature() const;
    float maxTemperature() const;

signals:
    void temperatureReceived(float temperature);
    void currentTemperatureChanged();
    void minTemperatureChanged();
    void maxTemperatureChanged();

public slots:
    void start(const uint pollingTime = 1000);
    void stop();

private slots:
    void timerTriggered();
    void onPacketReceived(QtXBee::Wpan::RxResponse16*);

private:
    QtXBee::XBee *m_xbee;
    quint16 m_sensorAddr;
    QTimer m_pollingTimer;
    float m_currentTemp;
    float m_minTemp;
    float m_maxTemp;
    bool m_firstFrame;
};

#endif // TEMPMONITOR_H
