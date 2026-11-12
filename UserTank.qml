import QtQuick 2.5
import shape 1.0

FocusScope{

    signal moveLeft()
    signal moveRight()
    signal moveUp()
    signal moveDown()
    signal shooted()
    
    property MyObject cell
    property MyGame game
    focus: true

    id: root
    width: 52
    height: 52
    x: cell.xCoord
    y: cell.yCoord


    Image {
        id: img
        source: root.cell.source
        width: root.cell.width
        height: root.cell.height
        anchors.centerIn: root
        rotation: root.cell.direction * 90
    }

    Keys.onPressed: function(event) {
        switch (event.key) {
        case Qt.Key_Up:
            moveUp()
            break

        case Qt.Key_Right:
            moveRight()
            break

        case Qt.Key_Down:
            moveDown()
            break

        case Qt.Key_Left:
            moveLeft()
            break

        case Qt.Key_Space:
            shooted()
            break

        default:
            return
        }

        event.accepted = true
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
