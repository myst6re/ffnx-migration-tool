import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls

import Qt.labs.platform

Window {
    visible: true
    title: qsTr("FFNx migration tool")

    ColumnLayout {
        anchors.centerIn: parent

        Button {
            Layout.alignment: Qt.AlignCenter
            text: "Choose input directory"
            onClicked: folderDialog1.open()
        }
        Button {
            Layout.alignment: Qt.AlignCenter
            text: "Choose output directory"
            onClicked: folderDialog2.open()
        }
    }

    FolderDialog {
        id: folderDialog1
        title: qsTr("Choose the source folder")
    }

    FolderDialog {
        id: folderDialog2
        title: qsTr("Choose the target folder")
    }
}
