import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    id: mainWin
    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("Счётчик слов")

    Column {
        id: root
        anchors.fill: parent
        RowLayout {
            id: buttonsLayout
            anchors.left: root.left
            anchors.right: root.right
            anchors.margins: 5
            height: openFileBtn.height
            Button {
                id: openFileBtn
                Layout.fillWidth: true
                text: "Открыть текстовый файл..."
                enabled: histogram_ctrl.topWords.length === 0
                onClicked: {
                    var component = Qt.createComponent("filedialog.qml")
                    var win = component.createObject(mainWin)
                    win.accepted.connect(function(){
                        histogram_ctrl.filepathChanged(win.fileUrl)
                    })
                }
            }
            Button {
                id: anotherBtn
                Layout.fillWidth: true
                text: "Очистить содержимое"
                enabled: histogram_ctrl.topWords.length !== 0 && histogram_ctrl.progress === 1.0
                onClicked: {
                    histogram_ctrl.filepathChanged("")
                }

            }
        }
        Rectangle {
            id: histogramRoot
            anchors.left: root.left
            anchors.right: root.right
            anchors.rightMargin: 10
            height: root.height - buttonsLayout.height - buttonsLayout.height

            property color backgroundColor: "white"
            property color gridColor: "slategray"
            property color legendColor: "white"
            property bool legendEnabled: true
            property int gridSpacing: maxValue / 10
            property double maxValue: histogram_ctrl.maxCount
            property int dataMargin: 5 * (mainWin.width / 640)

            property var model: histogram_ctrl.topWords

            Rectangle {
                id: grid
                anchors.fill: histogramRoot
                anchors.leftMargin: 50
                anchors.rightMargin: 50 + legend.width
                anchors.topMargin: 10
                anchors.bottomMargin: 10
                color: "white"
                Repeater {
                    model: histogramRoot.maxValue / histogramRoot.gridSpacing
                    Rectangle {
                        width: grid.width
                        height: 1
                        opacity: 0.5
                        color: histogramRoot.gridColor
                        y: grid.height - index * grid.height / ( histogramRoot.maxValue / histogramRoot.gridSpacing )
                    }
                }
                Repeater {
                    model: histogramRoot.maxValue / histogramRoot.gridSpacing

                    Text {
                        text: index * histogramRoot.gridSpacing
                        y: grid.height - index * grid.height / ( histogramRoot.maxValue / histogramRoot.gridSpacing ) - height / 2
                        anchors.right: grid.left
                    }
                }
                Repeater {
                    id: dataRep
                    model: histogramRoot.model

                    delegate: Rectangle
                    {
                        id: bar
                        height: modelData.count * grid.height / histogramRoot.maxValue
                        width: grid.width / dataRep.count - 2 * histogramRoot.dataMargin
                        color: modelData.color
                        x: index * (width + histogramRoot.dataMargin * 2) + histogramRoot.dataMargin
                        y: grid.height - height
                        border.width: 1
                        ToolTip.text: modelData.count
                        ToolTip.visible: modelData.count ? ma.containsMouse : false
                        MouseArea {
                            id: ma
                            anchors.fill: parent
                            hoverEnabled: true
                        }
                    }
                }
            }
            Rectangle {
                width: histogramRoot.legendEnabled ? histogramRoot.width * 0.2: 0
                anchors.right: histogramRoot.right
                anchors.verticalCenter: histogramRoot.verticalCenter
                anchors.topMargin: 10
                height: grid.height
                visible: histogramRoot.legendEnabled
                color: histogramRoot.legendColor
                Item {
                    id: legend
                    anchors.fill: parent
                    anchors.margins: 5

                    ColumnLayout {
                        id: legendCl
                        anchors.top: legend.top
                        anchors.right: legend.right
                        anchors.left: legend.left
                        spacing: 5
                        Repeater {
                            id: legendRep
                            model: histogramRoot.model

                            RowLayout {
                                spacing: 5
                                width: legendCl.width
                                Rectangle {
                                    width: height
                                    height: legendText.height
                                    color: modelData.color
                                }

                                Text {
                                    id: legendText
                                    Layout.fillWidth: true
                                    text: modelData.word
                                    elide: Text.ElideRight
                                    ToolTip.text: text
                                    ToolTip.visible: text ? ma.containsMouse : false
                                    MouseArea {
                                        id: ma
                                        anchors.fill: parent
                                        hoverEnabled: true
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        ProgressBar {
            anchors.left: root.left
            anchors.right: root.right
            anchors.margins: 5
            height: buttonsLayout.height
            value: histogram_ctrl.progress

            Behavior on value {
                NumberAnimation{}
            }
        }
    }
}
