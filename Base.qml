import QtQuick 2.0
import shape 1.0

Item {
    property MyObject cell

    id: root
    width: cell.width
    height: cell.height

    x: cell.xCoord
    y: cell.yCoord

    Image {
        id: img
        source: cell.source
        width: root.width
        height: root.height
        anchors.fill: root
    }
    Connections {
        target: cell
        function onHpChanged() {
            if(cell.hp === 0) {
                root.destroy();
            }
        }
    }
}
