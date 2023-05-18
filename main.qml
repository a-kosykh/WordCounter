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
    title: qsTr("Histogram")

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
                text: "Open File"
                onClicked: {
                    histogram_ctrl.buttonClicked()
                }
            }
            Button {
                id: anotherBtn
                Layout.fillWidth: true
                text: "Open File"
                onClicked: {
                    histogram_ctrl.buttonClicked()
                }
            }
        }
        Rectangle {
            id: histogramRoot
            anchors.left: root.left
            anchors.right: root.right
            height: root.height - buttonsLayout.height - buttonsLayout.height

            property color backgroundColor: "white"
            property color gridColor: "slategray"
            property color legendColor: "white"
            property bool legendEnabled: true
            property int gridSpacing: 10000
            property double maxValue: 100000
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
                    }
                }
            }
            Rectangle {
                width: histogramRoot.legendEnabled ? histogramRoot.width * 0.1: 0
                anchors.right: histogramRoot.right
                anchors.verticalCenter: histogramRoot.verticalCenter
                anchors.topMargin: 10
                height: grid.height
                visible: histogramRoot.legendEnabled
                color: histogramRoot.legendColor
                border.color: "black"
                border.width: 2
                Item {
                    id: legend
                    anchors.fill: parent
                    anchors.margins: 5

                    ColumnLayout {
                        anchors.fill: legend
                        spacing: 5
                        Repeater {
                            id: legendRep
                            model: histogramRoot.model

                            Row {
                                spacing: 5
                                Rectangle {
                                    width: height
                                    height: legendText.height
                                    color: modelData.color
                                }
                                Text {
                                    id: legendText
                                    text: modelData.word
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
