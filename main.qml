
import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import QtMultimedia

import shape 1.0

ApplicationWindow {
    id: mainWindow

    visible: true
    width: 900
    height: 800
    color: "black"
    title: "Battle City"

    onActiveFocusItemChanged: {
        console.log("activeFocusItem:", activeFocusItem)
    }

    SoundEffect {
        id: shootSound
        source: "qrc:/music/shoot.wav"
    }

    SoundEffect {
        id: explosionSound
        source: "qrc:/music/fexplosion.wav"
    }

    SoundEffect {
        id: gameOverSound
        source: "qrc:/music/gameover.wav"
    }

    Row {
        anchors.centerIn: parent

        Rectangle {
            id: root

            width: 800
            height: 800
            color: "gray"

            Rectangle {
                id: scene

                property MyGame gameContainer
                property Item playerTank
                property Item aITank
                property var game

                width: 676
                height: 676
                color: "black"
                anchors.centerIn: parent

                function createComponent(fileName, properties) {
                    const component = Qt.createComponent(fileName)

                    if (component.status !== Component.Ready) {
                        console.error(
                            "Failed to load", fileName,
                            component.errorString()
                        )
                        return null
                    }

                    const object = component.createObject(
                        scene, properties
                    )

                    if (!object) {
                        console.error(
                            "Failed to create", fileName,
                            component.errorString()
                        )
                    }

                    return object
                }

                Component.onCompleted: {
                    game = createComponent("Game.qml", {})

                    if (!game) {
                        return
                    }

                    gameContainer = game.gamedata

                    for (let i = 0; i < gameContainer.qty; ++i) {
                        const cell = gameContainer.getItem(i)

                        switch (cell.type) {
                        case ShapeType.Empty:
                            break

                        case ShapeType.Brick:
                            cell.source = "qrc:/img/Brick_quarter.png"
                            createComponent("Cell.qml", {
                                "cell": cell
                            })
                            break

                        case ShapeType.Concrete:
                            cell.source = "qrc:/img/Сoncrete_quarter.png"
                            createComponent("Cell.qml", {
                                "cell": cell
                            })
                            break

                        case ShapeType.AiTank:
                            cell.source = "qrc:/img/EnemyUp.png"

                            aITank = createComponent("aiTank.qml", {
                                "cell": cell
                            })
                            break

                        case ShapeType.PlayerTank:
                            cell.source = "qrc:/img/PlayerUp.png"

                            playerTank = createComponent("UserTank.qml", {
                                "cell": cell,
                                "game": gameContainer
                            })
                            break

                        case ShapeType.Base:
                            cell.source = "qrc:/img/base.png"
                            createComponent("Base.qml", {
                                "cell": cell
                            })
                            break
                        }
                    }
                }

                Connections {
                    target: scene.playerTank

                    function onShooted() {
                        const bullet = scene.gameContainer.shoot(
                            scene.playerTank.cell, 0
                        )

                        if (bullet) {
                            scene.createComponent("Bullet.qml", {
                                "cell": bullet
                            })
                        }

                        shootSound.play()
                    }
                }

                Connections {
                    target: scene.aITank

                    function onShooting() {
                        const bullet = scene.gameContainer.shoot(
                            scene.aITank.cell, 0
                        )

                        if (bullet) {
                            scene.createComponent("Bullet.qml", {
                                "cell": bullet
                            })
                        }
                    }
                }

                Connections {
                    target: scene.gameContainer

                    function onCreatePlayer() {
                        explosionSound.play()

                        const tank =
                            scene.gameContainer.createPlayerTank()

                        tank.source = "qrc:/img/PlayerUp.png"

                        scene.playerTank = scene.createComponent(
                            "UserTank.qml", {
                                "cell": tank,
                                "game": scene.gameContainer,
                                "focus": true
                            }
                        )
                    }

                    function onCreateEnemy() {
                        const tank =
                            scene.gameContainer.createEnemyTank()

                        tank.source = "qrc:/img/EnemyUp.png"

                        scene.aITank = scene.createComponent(
                            "aiTank.qml", {
                                "cell": tank
                            }
                        )
                    }

                    function onTankOnLevelChanged() {
                        console.log(
                            "Tanks left:",
                            scene.gameContainer.tanksOnLevel
                        )
                    }

                    function onGameOver() {
                        console.log("Game over!")
                        gameOverSound.play()
                    }
                }

                Connections {
                    target: scene.playerTank

                    function onMoveUp() {
                        scene.gameContainer.move(
                            scene.playerTank.cell, Direction.Up
                        )
                    }

                    function onMoveRight() {
                        scene.gameContainer.move(
                            scene.playerTank.cell, Direction.Right
                        )
                    }

                    function onMoveDown() {
                        scene.gameContainer.move(
                            scene.playerTank.cell, Direction.Down
                        )
                    }

                    function onMoveLeft() {
                        scene.gameContainer.move(
                            scene.playerTank.cell, Direction.Left
                        )
                    }
                }
            }
        }

        Rectangle {
            width: 100
            height: 800
            color: "gray"

            Rectangle {
                id: tanksLeft

                width: 100
                height: 100
                color: "green"

                Text {
                    anchors.centerIn: parent

                    text: scene.gameContainer
                          ? "Enemy left: "
                            + scene.gameContainer.lifes
                            + "\n"
                            + scene.gameContainer.tanksOnLevel
                          : "Loading..."

                    font.family: "Helvetica"
                    font.pointSize: 16
                }
            }
        }
    }
}
