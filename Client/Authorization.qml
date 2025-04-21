import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Dialogs 6.3

Page {

    readonly property alias pageStack: stackView

    id: authorization_page

    Component.onCompleted: {
        client.authResult.connect(authOperate);
    }

    MessageDialog {
        id: loginDialog
        title: "Notification"

        text: ""
        onAccepted: {
            if (authorization_page.currentDialogContext === "Login succeed.") {
                root.stackView.push("Chat.qml")
            }
        }
    }
    MessageDialog {
        id: regDialog
        title: "Notification"
        text: ""
    }

    function authOperate(result) {
        if (result === "AUTH_OK") {
            loginDialog.title = "Login succeed.";
            loginDialog.text = "Welcome, " + client.getClientName();
            loginDialog.open();
        } else if (result === "AUTH_FAIL") {
            loginDialog.title = "Login error.";
            loginDialog.text = "Invalid login/password / server is not avaliable.";
            loginDialog.accepted()
            loginDialog.open();
        } else if (result === "REG_OK") {
            regDialog.title = "Registration is complete.";
            regDialog.text = "New user is created. Now you can login.";
            regDialog.open();
        } else if (result === "REG_FAIL") {
            regDialog.title = "Register failure.";
            regDialog.text= "User already exists.";
            regDialog.open();
        }
    }


    ColumnLayout {
        id: authentication_panel

        anchors.centerIn: parent

        TextField {
            id: login

            Layout.preferredWidth: 150
            Layout.preferredHeight: 60
            placeholderText: "Login"
        }

        TextField {
            id: password

            Layout.preferredWidth: 150
            Layout.preferredHeight: 60
            placeholderText: "Password"

            echoMode: TextInput.Password
        }
        Button {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            text: "Login"

            onClicked: {
                client.on_button_authorize_clicked(login.text, password.text, 0);
            }
        }
        Button {
            id: reg_btn

            Layout.fillWidth: true
            Layout.preferredHeight: 30

            text: "Register"

            onClicked: {
                client.on_button_authorize_clicked(login.text, password.text, 1);
            }
        }

    }
}
