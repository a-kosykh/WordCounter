import QtQuick 2.15
import QtQuick.Dialogs 1.3

FileDialog {
    id: fileDialog
    title: "Please choose text file"
    folder: shortcuts.home
    selectMultiple: false
    defaultSuffix: "txt"
    nameFilters: [ "Text files (*.txt)" ]
    Component.onCompleted: visible = true
}
