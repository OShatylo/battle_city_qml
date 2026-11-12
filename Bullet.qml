import QtQuick 2.0
import shape 1.0
Item{

    property MyObject cell
    id: root
    width: cell.width
    height: cell.height

    x: cell.xCoord - 13
    y: cell.yCoord - 13

    Image {
        id: img
        source: "qrc:/img/BulletUp.png"
        width: root.width
        height: root.height
        rotation: cell.direction * 90
    }

    Connections{
        target: cell
        function onHpChanged() {
            if (cell.hp === 0) {
                root.destroy();
            }
        }
    }
}
