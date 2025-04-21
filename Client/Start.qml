import QtQuick 2.15
import QtQuick.Controls 2.15

Page {

    readonly property alias pageStack: stackView

    Button {
        id: start
        width: 100
        height: 100
        text: "Press"
        anchors.centerIn: parent
        onClicked: {
            stackView.push("Authorization.qml")
        }
    }
}
