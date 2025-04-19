import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    id: authorization_page
    //property Client client

    Button {
        id: start
        width: 100
        height: 100
        text: "Press"
        anchors.centerIn: parent
        onClicked: {
            client.on_button_authorize_clicked()
            stackView.push("Chat.qml")
        }
    }
}
