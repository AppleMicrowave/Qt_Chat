import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

ApplicationWindow {
    width: 1000
    minimumWidth: 700

    height: 500
    minimumHeight: 400

    visible: true
    title: qsTr("Chat App")
    id: window

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Authorization {}
    }
}
