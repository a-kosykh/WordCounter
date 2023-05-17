import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    id: mainWin
    width: 640
    height: 480
    visible: true
    title: qsTr("Histogram")

    Item {
        anchors.fill: parent
        Rectangle {
            id: root
            anchors.fill: parent
            property color backgroundColor: "white"
            property color gridColor: "slategray"
            property color legendColor: "lightgray"
            property bool legendEnabled: true
            property int gridSpacing: 10000
            property double maxValue: 100000
            property int dataMargin: 5 * (mainWin.width / 640)

            Rectangle {
                id: grid
                anchors.fill: root
                anchors.leftMargin: 50
                anchors.rightMargin: 50 + legend.width
                anchors.topMargin: 10
                anchors.bottomMargin: 10
                color: "white"
                Repeater {
                    model: root.maxValue / root.gridSpacing
                    Rectangle {
                        width: grid.width
                        height: 1
                        opacity: 0.5
                        color: root.gridColor
                        y: grid.height - index * grid.height / ( root.maxValue / root.gridSpacing )
                    }
                }
                Repeater {
                    model: root.maxValue / root.gridSpacing
                    //y: grid.height - index * grid.height / ( root.maxValue / root.gridSpacing )

                    Text {
                        text: index * root.gridSpacing
                        y: grid.height - index * grid.height / ( root.maxValue / root.gridSpacing ) - height / 2
                        anchors.right: grid.left
                    }
                }
                Repeater {
                    id: dataRep
                    model: histogram_ctrl.topWords
                    Rectangle {
                        height: modelData.count * grid.height / root.maxValue
                        width: grid.width / dataRep.count - 2 * root.dataMargin
                        color: "lightblue"
                        x: index * (width + root.dataMargin * 2) + root.dataMargin
                        y: grid.height - height
                        border.width: 1
                    }
                }
            }
            Rectangle {
                id: legend
                width: root.legendEnabled ? root.width * 0.1: 0
                color: root.legendColor
                anchors.right: root.right
                anchors.top: root.top
                anchors.topMargin: 10
                height: grid.height
                visible: root.legendEnabled
                Column {
                    anchors.centerIn: legend
                    anchors.left: legend.left
                    Repeater {
                        id: legendRep
                        model: root.model

                        Row {
                            Rectangle {
                                width: height
                                height: legendText.height
                                color: model.color
                            }
                            Text {
                                id: legendText
                                text: model.legend
                            }
                        }
                    }
                }
            }
        }
        Button {
            id: btn
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
            text: "adasd"
            onClicked: {
                histogram_ctrl.buttonClicked()
            }
        }
    }
}
