import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

ApplicationWindow {
    width: 1000
    minimumWidth: 700

    height: 500
    minimumHeight: 400

    visible: true
    title: qsTr("Chat App")
    id: window

    property alias pageStack: stackView

    StackView {
        id: stackView
        anchors.fill: parent

        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 200
            }
        }
    }
    Component.onCompleted: pageStack.push("Start.qml")
}
