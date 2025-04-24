import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQml.Models

Page {

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
                Layout.topMargin: 20
                Layout.leftMargin: 30
                Layout.bottomMargin: 20

                Layout.minimumWidth: 100
                Layout.maximumWidth: 300

                Layout.fillWidth: true
                Layout.fillHeight: true


                ListView {
                    id: chatList
                    anchors.fill: parent
                    Layout.margins: 5
                    model: client !== null && client.chats !== undefined ? client.chats : []
                    clip: true

                    delegate: Item {
                        width: chatList.width
                        height: 60

                        Rectangle {
                            id: background
                            anchors.fill: parent
                            radius: 15
                            color: "#85ffee"
                            border.color: "Black"
                            border.width: 1

                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 16
                                text: modelData
                                font.pixelSize: 18
                                color: "#263238"
                            }

                            MouseArea {
                                id: mouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                onPressed: {
                                    background.color = "SkyBlue"
                                    //client.selectChat(modelData)
                                }
                                onReleased: background.color = "#85ffee"
                                onEntered: background.border.width = 2;
                                onExited: background.border.width = 1;
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
                        text: client !== null && client.chatMessages !== undefined ? client.chatMessages.join("\n") : ""
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
