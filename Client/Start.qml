import QtQuick 2.15
import QtQuick.Controls 2.15

Page {

    Column {
        anchors.centerIn: parent
        spacing: 30

        Label {
            text: "Local chat"
            font.pixelSize: 32
            font.bold: true
            color: "#263238"
        }

        Button {
            id: start
            width: 120
            height: 50
            text: "Press"
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: {
                stackView.push("Authorization.qml")
            }
        }
    }
}
