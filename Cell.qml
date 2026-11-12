import QtQuick 2.5
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
    }
    Connections {
        target: cell
        function onTypeChanged() {
            img.source = "";
            root.destroy();
        }
    }
}
