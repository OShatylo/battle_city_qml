#include "game.h"
#include <QRandomGenerator>
#include <QTime>

Game::Game(QObject *parent) : QObject(parent)
{
    setWindowHeigth(heigth);
    setWindowHeigth(width);

    m_PlayersLifes = 3;
    m_AiTanksOnLevel = 20;

    globalTimer = new QTimer(this);
    init();

    connect(globalTimer, &QTimer::timeout,
            this, &Game::startGame);

    globalTimer->start(50);
}

int Game::lifes() const
{
    return m_PlayersLifes;
}

void Game::setLifes(int lifes)
{
    if (m_PlayersLifes != lifes)
    {
        m_PlayersLifes = lifes;
        emit lifesChanged(lifes);
    }
}

void Game::init()
{
    initTiles();
    initGameContainer();
}

void Game::initGameContainer()
{
    for (int i = 0; i < m_tileList.size(); i++)
    {
        m_gameItemsContainer.append(m_tileList.at(i));
    }
    m_base = new Shape(this);
    m_base->setYCoord(624);
    m_base->setXCoord(312);
    m_base->setHp(1);
    m_base->setType(ShapeType::Base);
    m_base->setHeight(tankHeigth);
    m_base->setWidth(tankWidth);
    m_gameItemsContainer.append(m_base);
    createPlayerTank();
    createEnemyTank();
}
Shape *Game::createPlayerTank()
{
    Shape *userTank = new Shape(this);
    userTank->setYCoord(676 - tankHeigth - step);
    userTank->setXCoord(221);
    userTank->setType(ShapeType::PlayerTank);
    userTank->setHeight(tankHeigth);
    userTank->setWidth(tankWidth);
    m_gameItemsContainer.append(userTank);
    setLifes(m_PlayersLifes - 1);

    return userTank;
}

int Game::tanksOnLevel() const
{
    return m_AiTanksOnLevel;
}

void Game::setTanksOnLevel(int value)
{
    if (m_AiTanksOnLevel != value)
    {
        m_AiTanksOnLevel = value;
        emit tankOnLevelChanged(value);
    }
}
Shape *Game::shooter() const
{
    return m_shooter;
}

void Game::setShooter(Shape *shooter)
{
    if (shooter != m_shooter)
    {
        m_shooter = shooter;
        emit shooterChanged(shooter);
    }
}

bool Game::isGameOver()
{
    if (!m_AiTanksOnLevel || !m_PlayersLifes || !m_base->hp())
    {
        emit gameOver();

        return true;
    }
    return false;
}

QList<Shape *> Game::getGameItemsContainer() const
{
    return m_gameItemsContainer;
}

bool Game::isMapLoaded()
{
    QFile fl("../Battle_city_qml/levels/level1_tile.txt");
    if (!fl.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return 0;
    }
    QTextStream in(&fl);
    QString str(fl.readLine());
    m_map = str;

    if (!m_map.isEmpty())
    {
        return true;
    }
    return false;
}

//&& (movingShape== ShapeType::PlayerTank || movingShape== ShapeType::AiTank)
bool Game::isMovePossible(Shape *movingShape, int moveDir)
{
    if ((moveDir == Direction::Up) && (((movingShape->yCoord() - step)) <= (width - tankWidth)) && (movingShape->yCoord()) > 0)
    {
        for (int i = m_gameItemsContainer.size() - 1; i >= 0; i--)
        {
            if ((m_gameItemsContainer.at(i)->type() == ShapeType::Empty) && (m_gameItemsContainer.at(i)->shapeRect().contains(movingShape->xCoord(), movingShape->yCoord() - step, false)))
            {
                return true;
            }
            else if (movingShape->type() == ShapeType::Bullet && (m_gameItemsContainer.at(i)->type() != ShapeType::Empty) && (m_gameItemsContainer.at(i)->type() != ShapeType::Bullet) &&
                     (m_gameItemsContainer.at(i)->shapeRect().contains(movingShape->xCoord(), movingShape->yCoord() - step, false) && movingShape->parent() != m_gameItemsContainer.at(i)))
            {
                m_gameItemsContainer.at(i)->setHp(0);
                m_gameItemsContainer.at(i)->setType(ShapeType::Empty);
                for (int i = m_gameItemsContainer.size() - 1; i >= 0; i--)
                {
                    if (m_gameItemsContainer.at(i) == movingShape)
                    {
                        movingShape->setHp(0);
                        m_gameItemsContainer.removeAt(i);
                        return true;
                    }
                }
                return false;
            }
        }
    }

    if ((moveDir == Direction::Down) && ((movingShape->yCoord()) < (width - tankWidth)) && ((movingShape->yCoord()) + step) > 0)
    {
        for (int i = 0; i < m_gameItemsContainer.size(); i++)
        {
            if ((m_gameItemsContainer.at(i)->type() == ShapeType::Empty) && (m_gameItemsContainer.at(i)->shapeRect().contains(movingShape->xCoord(), movingShape->yCoord() + step, false)))
            {
                return true;
            }
            else if (movingShape->type() == ShapeType::Bullet && (m_gameItemsContainer.at(i)->type() != ShapeType::Empty) && (m_gameItemsContainer.at(i)->type() != ShapeType::Bullet) &&
                     (m_gameItemsContainer.at(i)->shapeRect().contains(movingShape->xCoord(), movingShape->yCoord() + step, false) && movingShape->parent() != m_gameItemsContainer.at(i)))
            {
                m_gameItemsContainer.at(i)->setHp(0);
                m_gameItemsContainer.at(i)->setType(ShapeType::Empty);
                for (int i = m_gameItemsContainer.size() - 1; i >= 0; i--)
                {
                    if (m_gameItemsContainer.at(i) == movingShape)
                    {
                        movingShape->setHp(0);
                        m_gameItemsContainer.removeAt(i);
                        return true;
                    }
                }
                return false;
            }
        }
    }

    if ((moveDir == Direction::Left) && ((movingShape->xCoord() - step) < (width - tankWidth)) && ((movingShape->xCoord())) > 0)
    {
        for (int i = m_gameItemsContainer.size() - 1; i >= 0; i--)
        {
            if ((m_gameItemsContainer.at(i)->type() == ShapeType::Empty) && (m_gameItemsContainer.at(i)->shapeRect().contains(movingShape->xCoord() - step, movingShape->yCoord(), false)))
            {
                return true;
            }
            else if (movingShape->type() == ShapeType::Bullet && (m_gameItemsContainer.at(i)->type() != ShapeType::Empty) && (m_gameItemsContainer.at(i)->type() != ShapeType::Bullet) &&
                     (m_gameItemsContainer.at(i)->shapeRect().contains(movingShape->xCoord() - step, movingShape->yCoord(), false) && (movingShape->parent() != m_gameItemsContainer.at(i))))
            {
                m_gameItemsContainer.at(i)->setType(ShapeType::Empty);
                m_gameItemsContainer.at(i)->setHp(0);
                for (int i = m_gameItemsContainer.size() - 1; i >= 0; i--)
                {
                    if (m_gameItemsContainer.at(i) == movingShape)
                    {
                        movingShape->setHp(0);
                        m_gameItemsContainer.removeAt(i);
                        return true;
                    }
                }
                return false;
            }
        }
    }

    if ((moveDir == Direction::Right) && ((movingShape->xCoord()) < (width - tankWidth)) && ((movingShape->xCoord()) + step) > 0)
    {
        for (int i = m_gameItemsContainer.size() - 1; i >= 0; i--)
        {
            if ((m_gameItemsContainer.at(i)->type() == ShapeType::Empty) && (m_gameItemsContainer.at(i)->shapeRect().contains(movingShape->xCoord() + step, movingShape->yCoord(), false)))
            {
                return true;
            }
            else if (movingShape->type() == ShapeType::Bullet && (m_gameItemsContainer.at(i)->type() != ShapeType::Empty) && (m_gameItemsContainer.at(i)->type() != ShapeType::Bullet) &&
                     (m_gameItemsContainer.at(i)->shapeRect().contains(movingShape->xCoord() + 2 * step, movingShape->yCoord(), false) && movingShape->parent() != m_gameItemsContainer.at(i)))
            {
                m_gameItemsContainer.at(i)->setType(ShapeType::Empty);
                for (int i = m_gameItemsContainer.size() - 1; i >= 0; i--)
                {
                    if (m_gameItemsContainer.at(i) == movingShape)
                    {
                        movingShape->setHp(0);
                        m_gameItemsContainer.at(i)->setHp(0);
                        m_gameItemsContainer.removeAt(i);
                        return true;
                    }
                }
                return false;
            }
        }
    }
    return false;
}

int Game::randomMove()
{

    auto rm = QRandomGenerator::global()->bounded(4);
    return rm;
}

int Game::shotOrMove()
{
    int sOm = QRandomGenerator::global()->bounded(9);
    return sOm;
}
void Game::check()
{
    bool isAiTank = false;
    bool isPlayerTank = false;
    for (int i = 0; i < m_gameItemsContainer.size(); i++)
    {
        if (m_gameItemsContainer.at(i)->type() == ShapeType::PlayerTank)
        {
            isPlayerTank = true;
        }
        if (m_gameItemsContainer.at(i)->type() == ShapeType::AiTank)
        {
            isAiTank = true;
        }
    }
    if (isAiTank == false && m_AiTanksOnLevel)
    {
        emit createEnemy();
    }
    if ((isPlayerTank == false) && (m_PlayersLifes))
    {
        emit createPlayer();
    }
}

void Game::startGame()
{
    Shape *currentObject;
    bool isCreated = false;
    for (int i = 0; i < m_gameItemsContainer.size(); i++)
    {
        if ((ShapeType::AiTank == m_gameItemsContainer.at(i)->type()))
        {
            currentObject = m_gameItemsContainer.at(i);
            int sOm = shotOrMove();
            if (sOm == Behavior::Movement)
            {
                move(currentObject, randomMove());
            }
            if (sOm == Behavior::Shoot)
            {
                shoot(currentObject, isCreated);
                isCreated = true;
                currentObject->shoot();
            }
        }
        if (ShapeType::Bullet == m_gameItemsContainer.at(i)->type())
        {
            move(m_gameItemsContainer.at(i), m_gameItemsContainer.at(i)->direction());
        }
    }

    check();
    if (isGameOver())
    {
        globalTimer->blockSignals(true);
    }
}

bool Game::move(Shape *movingShape, int moveDir)
{
    movingShape->setDirection(moveDir);
    bool possible = isMovePossible(movingShape, moveDir);
    if (possible)
    {
        switch (moveDir)
        {
        case Direction::Up:
            movingShape->setYCoord(movingShape->yCoord() - step);
            break;
        case Direction::Down:
            movingShape->setYCoord(movingShape->yCoord() + step);
            break;
        case Direction::Left:
            movingShape->setXCoord(movingShape->xCoord() - step);
            break;
        case Direction::Right:
            movingShape->setXCoord(movingShape->xCoord() + step);
            break;
        default:
            break;
        }
        return true;
    }
    else if (movingShape->type() == ShapeType::Bullet && !possible)
    {
        for (int i = 0; i < m_gameItemsContainer.size(); i++)
        {
            if (m_gameItemsContainer.at(i) == movingShape)
            {
                movingShape->setHp(0);
                m_gameItemsContainer.removeAt(i);
            }
        }
    }
    return false;
}
Shape *Game::shoot(Shape *shooter, bool isCreated)
{
    if (!isCreated)
    {
        isCreated = true;
        Shape *bullet = new Shape(shooter);
        QQmlEngine::setObjectOwnership(bullet, QQmlEngine::CppOwnership);
        bullet->setType(ShapeType::Bullet);
        bullet->setWidth(tileWidth);
        bullet->setHeight(tileHeigth);
        bullet->setDirection(shooter->direction());
        bullet->setXCoord(shooter->shapeRect().x() + step * 2);
        bullet->setYCoord(shooter->shapeRect().y() + step * 2);
        bullet->setHp(1);
        m_gameItemsContainer.append(bullet);
        return bullet;
    }
    return nullptr;
}

bool Game::initTiles()
{
    isMapLoaded();
    int level = 0;
    for (int i = 0; i < m_map.size(); i++)
    {
        Shape *tile = new Shape(this);
        tile->setType(m_map[i].digitValue());
        tile->setHeight(tileHeigth);
        tile->setWidth(tileWidth);
        m_tileList.append(tile);
    }
    for (int i = 0; i < (m_tileList.size() / tileWidth); i++)
    {
        for (int j = 0; j < (m_tileList.size() / tileWidth); j++)
        {
            m_tileList[i * m_tileList.size() / tileWidth + j]->setXCoord(tileWidth * j);
        }
    }

    for (int i = 0; i < m_tileList.size(); i++)
    {
        m_tileList[i]->setYCoord(level * tileHeigth);
        if (i > 0 && (i + 1) % tileHeigth == 0)
        {
            level += 1;
        }
    }
    for (int i = 0; i < m_tileList.size(); i++)
    {
        m_tileList.at(i)->setShapeRect(QRect(m_tileList.at(i)->xCoord(), m_tileList.at(i)->yCoord(), m_tileList.at(i)->width(), m_tileList.at(i)->height()));
    }
    return true;
}
int Game::windowWidth() const
{
    return m_windowWidth;
}

void Game::setWindowWidth(int windowWidth)
{
    if (windowWidth != m_windowWidth)
    {
        m_windowWidth = windowWidth;
        emit windowWidthChanged(windowWidth);
    }
}

int Game::windowHeigth() const
{
    return m_windowHeigth;
}

void Game::setWindowHeigth(int windowHeigth)
{
    if (windowHeigth != m_windowHeigth)
    {
        m_windowHeigth = windowHeigth;
        emit windowsHeigthChanged(windowHeigth);
    }
}

Shape *Game::createEnemyTank()
{
    auto possition{QRandomGenerator::global()->bounded(3)};
    int startPosX[3] = {221, 312, 624};

    if (m_AiTanksOnLevel)
    {
        Shape *aiTank = new Shape(this);
        aiTank->setType(ShapeType::AiTank);
        aiTank->setYCoord(0);
        aiTank->setXCoord(startPosX[possition]);
        aiTank->setHeight(tankHeigth);
        aiTank->setWidth(tankWidth);
        m_gameItemsContainer.append(aiTank);
        setTanksOnLevel(m_AiTanksOnLevel - 1);

        return aiTank;
    }
    return nullptr;
}

QList<Shape *> Game::tankList() const
{
    return m_tankList;
}

QList<Shape *> Game::tileList() const
{
    return m_tileList;
}

Shape *Game::getTile(int index)
{
    return m_tileList[index];
}

Shape *Game::getItem(int index)
{
    return m_gameItemsContainer[index];
}
int Game::getQty() const
{
    return m_gameItemsContainer.size();
}
