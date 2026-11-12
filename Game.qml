import QtQuick 2.0
import shape 1.0

Item{
    property MyGame gamedata: data

    id:root
    MyGame {
        id: data
    }
}
