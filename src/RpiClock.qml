import QtQuick 2.11
import QtQuick.Window 2.11
import MqttClient 1.0

Window {
    visible: true
    width: 480
    height: 320
    color: "black"
    title: "RpiClock"
    objectName: "window"
    property string interior_temperature: "--"
    property string interior_humidity: "--"
    property string exterior_temperature: "--"
    property var interiorTemperatureSub: 0
    property var interiorHumiditySub: 0
    property var exteriorTemperatureSub: 0

    MqttClient {
        id: client
        hostname: "quadra.local"
        username: "mqtt"
        password: "mqtt"
        Component.onCompleted: {
            console.log("connecting...")
            client.connectToHost()
        }
        onConnected: {
            console.log("connected!!!")
            interiorTemperatureSub = client.subscribe("sensors/A4:C1:38:24:F2:46/temperature")
            interiorTemperatureSub.messageReceived.connect(interiorTemperatureReceived)
            interiorHumiditySub = client.subscribe("sensors/A4:C1:38:24:F2:46/humidity")
            interiorHumiditySub.messageReceived.connect(interiorHumidityReceived)
            exteriorTemperatureSub = client.subscribe("sensors/A4:C1:38:2A:2D:90/temperature")
            exteriorTemperatureSub.messageReceived.connect(exteriorTemperatureReceived)
        }
        onDisconnected: {
            console.log("disconnected")
            interior_temperature = "--"
            interior_humidity = "--"
            exterior_temperature = "--"
        }
        onErrorChanged: {
            console.log("error: " + error)
        }
    }

    function interiorTemperatureReceived(payload)
    {
        interior_temperature = Math.floor(payload)
    }

    function interiorHumidityReceived(payload)
    {
        interior_humidity = payload
    } 

    function exteriorTemperatureReceived(payload)
    {
        exterior_temperature = Math.floor(payload)
    }

    Timer {
        interval: 1000; running: true; repeat: true; triggeredOnStart: true
        onTriggered: {
            if (client.state === MqttClient.Disconnected)
            {
                console.log("connecting...")
                client.connectToHost()
            }
            interior_text.text = interior_temperature + "° " + interior_humidity + "%"
            exterior_text.text = Qt.formatDateTime(new Date(), "dd MMM ") + exterior_temperature + "°"
            time.text = Qt.formatDateTime(new Date(), "hh:mm")
        }
    }

    FontLoader { id: localFont; source: "DejaVuSans.ttf" }

    Item {
        property int speed: 600
        property var easing: Easing.InOutCirc
        id: container
        y: 30
        clip: true
        anchors.left: parent.left
        anchors.right: parent.right
        height: fontMetrics.height

        FontMetrics {
            id: fontMetrics
            font.family: localFont.name
            font.pointSize: 70
        }

        Text {
            id: exterior_text
            y: 3
            color: "white"
            anchors.horizontalCenter: parent.horizontalCenter
            text: ""
            font.family: localFont.name
            font.pointSize: 65; font.bold: false
        }

        Image {
            id: home_icon
            height: 65
            fillMode: Image.PreserveAspectFit
            source: "home.svg"
            anchors.bottom: interior_text.baseline
            anchors.left: parent.left
        }

        Text {
            id: interior_text
            color: "white"
            height: fontMetrics.height
            horizontalAlignment: Text.AlignHCenter
            anchors.top: exterior_text.bottom
            anchors.left: home_icon.right
            anchors.right: parent.right
            text: ""
            font.family: localFont.name
            font.pointSize: 65; font.bold: false
        }

        SequentialAnimation {
            running: true
            loops: Animation.Infinite

            NumberAnimation { target: exterior_text; property: "y"; from: 3; to: -fontMetrics.height; duration: container.speed; easing.type: container.easing }
            PauseAnimation { duration: 3000 }
            NumberAnimation { target: exterior_text; property: "y"; from: -fontMetrics.height; to: 3; duration: container.speed; easing.type: container.easing }
            PauseAnimation { duration: 7000 }
        }
    }

    Text {
        id: time
        color: "white"
        y: 120
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: localFont.name
        font.pointSize: 130; font.bold: false
        text: ""
    }
}
