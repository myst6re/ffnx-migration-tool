import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QMain

import Qt.labs.platform

Window {
    visible: true
    title: qsTr("FFNx migration tool")

    QMain {
        id: 'qmain'
    }

    ColumnLayout {
        anchors.centerIn: parent

        Button {
            Layout.alignment: Qt.AlignCenter
            text: "Choose input directory"
            onClicked: folderDialog1.open()
            id: 'inDir'
        }
        Button {
            Layout.alignment: Qt.AlignCenter
            text: "Choose output directory"
            onClicked: folderDialog2.open()
            id: 'outDir'
        }
        ComboBox {
            Layout.alignment: Qt.AlignCenter
            model: ["Tonberry", "Remaster/Demaster"]
            id: 'modType'
        }
        Button {
            Layout.alignment: Qt.AlignCenter
            text: "Convert"
            onClicked: qmain.runConvert(modType.currentIndex, folderDialog1.folder, folderDialog2.folder)
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
