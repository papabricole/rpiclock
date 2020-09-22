import QtQuick 2.11
import QtQuick.Window 2.11
import com.dht11 1.0

Window {
    visible: true
    width: 480
    height: 320
    color: "black"
    title: "RpiClock"
    objectName: "window"
    property string exterior_temperature: "--"

    DHT11 {
        id: dht11
    }

    Timer {
        interval: 1000; running: true; repeat: true; triggeredOnStart: true
        onTriggered: {
            interior_text.text = dht11.temperature() + "° " +  dht11.humidity() + "%"
            exterior_text.text = Qt.formatDateTime(new Date(), "dd MMM ") + exterior_temperature + "°"
            time.text = Qt.formatDateTime(new Date(), "hh:mm")
        }
    }

    Timer {
        id: interior
        interval: 10000; running: true; repeat: true; triggeredOnStart: true
        onTriggered: dht11.update()
    }

    Timer {
        id: forecast
        interval: 30 * 60 * 1000; running: true; repeat: true; triggeredOnStart: true
        onTriggered: {
            var xmlhttp = new XMLHttpRequest();
            var url = "http://rpi.local";

            xmlhttp.onreadystatechange=function() {
                if (xmlhttp.readyState == XMLHttpRequest.DONE && xmlhttp.status == 200) {
                    var jsn = JSON.parse(xmlhttp.responseText);
                    exterior_temperature = Math.round(jsn['current']['temp'])
                }
            }
            xmlhttp.open("GET", url, true);
            xmlhttp.send();
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
