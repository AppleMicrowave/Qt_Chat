import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Page {

    readonly property alias pageStack: stackView

    id: chat_page

    function send() {
        if (input.text !== "") {
            client.on_button_send_clicked(input.text)
            console.log("Send message: " + input.text);
            input.clear();
        }
        input.clear();
    }

    Rectangle {
        anchors.fill: parent
        color: "Sky blue"
        id: app

        RowLayout { // App's main elements layout
            anchors.fill: parent
            spacing: 20

            Rectangle { // Chats list
                id: chats
                Layout.topMargin: 20
                Layout.leftMargin: 30
                Layout.bottomMargin: 20

                Layout.minimumWidth: 100
                Layout.maximumWidth: 300

                Layout.fillWidth: true
                Layout.fillHeight: true


                ScrollView {
                    width: parent.width
                    height: parent.height
                    Column {
                        Repeater {
                            model: 50
                            Label {
                                text: "Chat"
                                font.pixelSize: 30
                            }
                        }
                    }
                }
            }

            ColumnLayout { // Chat layout

                Rectangle { // Chat

                    id: chat

                    Layout.topMargin: 20
                    Layout.rightMargin: 20
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    TextArea {
                        id: content
                        readOnly: true
                        wrapMode: TextEdit.Wrap
                        text: client.getMessages().join("\n")
                    }
                }

                RowLayout { // Message layout
                    Layout.rightMargin: 20
                    Layout.bottomMargin: 20
                    spacing: 10

                    TextField {
                        id: input
                        Layout.fillWidth: true
                        Layout.preferredHeight: 60
                        placeholderText: "Введите сообщение..."

                        Keys.onReturnPressed: {
                            send()
                        }
                    }

                    Button {
                        id: ftp
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 50
                        text: "Load"
                    }

                    Button {
                        id: btn
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 50
                        text: "Send"

                        onClicked: {
                            send()
                        }
                    }
                }
            }
        }
    }
}
