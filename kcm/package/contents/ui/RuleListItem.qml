import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

MouseArea {
    id: itemRoot
    height: 48
    anchors.left: parent.left
    anchors.right: parent.right

    hoverEnabled: true

    signal move(int from, int to)
    signal edit(int index)
    signal remove(int index)

    z: dragArea.pressed ? 100 : 0

    Rectangle {
        id: background
        anchors.fill: parent
        opacity: 0
        color: theme.highlightColor

        visible: itemRoot.containsMouse
    }

    RowLayout {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        MouseArea {
            id: dragArea

            Layout.fillHeight: true
            Layout.leftMargin: 12
            Layout.minimumWidth: 18
            Layout.minimumHeight: 18

            drag.target: itemRoot
            PlasmaCore.IconItem {
                anchors.fill: parent

                source: "show-grid"
            }

            drag.onActiveChanged: {
                if (!dragArea.drag.active) {
                    var newIndex = (itemRoot.y + (itemRoot.height / 2)) / itemRoot.height
                    move(index, newIndex)
                }
            }

            cursorShape: dragArea.pressed ? Qt.DragMoveCursor : Qt.OpenHandCursor
        }

        PlasmaComponents.Label {
            Layout.minimumWidth: 120
            Layout.fillHeight: true
            Layout.leftMargin: 4
            text: model.action
        }
        PlasmaComponents.Label {
            Layout.minimumWidth: 160
            text: model.from
        }
        PlasmaComponents.Label {
            Layout.minimumWidth: 160
            text: model.to
        }
        PlasmaComponents.Label {
            Layout.minimumWidth: 60
            text: model.ipv6 ? "IPv6" : ""
        }
        PlasmaComponents.Label {
            Layout.leftMargin: 12
            text: model.logging
        }
    }

    PlasmaComponents.ToolButton {
        anchors.right: parent.right
        anchors.rightMargin: 24
        anchors.verticalCenter: parent.verticalCenter
        visible: itemRoot.containsMouse

        height: 48
        iconSource: "entry-delete"
        onClicked: itemRoot.remove(index)
    }

    states: [
        State {
            name: "grabed"
            when: dragArea.pressed
            PropertyChanges {
                target: background
                opacity: 1
            }
        }
    ]
    transitions: [
        Transition {
            to: "grabed"
            NumberAnimation {
                properties: "opacity"
                easing.type: Easing.InOutQuad
            }
        },
        Transition {
            from: "grabed"
            NumberAnimation {
                properties: "opacity"
                easing.type: Easing.InOutQuad
            }
        }
    ]

    onClicked: edit(index)
}
