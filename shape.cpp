#include "shape.h"

Shape::Shape(QObject *parent) : QObject(parent)
{
    m_type = 0;
    m_direction = 0;
    m_source = "";
    m_hp = 1;
    m_xCoord = 0;
    m_yCoord = 0;
    m_height = 0;
    m_width = 0;
}

int Shape::yCoord() const
{
    return m_yCoord;
}

int Shape::xCoord() const
{
    return m_xCoord;
}

QString Shape::source() const
{
    return m_source;
}

int Shape::type() const
{
    return m_type;
}

int Shape::direction() const
{
    return m_direction;
}

bool Shape::traversable() const
{
    return m_traversable;
}

void Shape::setTraversable(bool traversable)
{
    if (traversable != m_traversable){
        m_traversable = traversable;
        emit traversableChanged(traversable);
    }
}

void Shape::setShapeRect(const QRect &shapeRect)
{
    if (m_shapeRect != shapeRect) {
        m_shapeRect = shapeRect;
        emit shapeRectChanged();
    }
}

void Shape::shoot()
{
    emit shooting();
}

int Shape::shapeRectCenterX()
{
    return shapeRect().center().x();
}

int Shape::shapeRectCenterY()
{
    return shapeRect().center().y();
}

int Shape::width() const
{
    return m_width;
}

void Shape::setWidth(int width)
{
    if (m_width != width) {
        m_width = width;
        setShapeRect(QRect(m_xCoord, m_yCoord, m_width, m_height));
        emit widthChanged(width);
    }
}

int Shape::height() const
{
    return m_height;
}

void Shape::setHeight(int height)
{
    if (m_height != height) {
        m_height = height;
        setShapeRect(QRect(m_xCoord, m_yCoord, m_width, m_height));
        emit heightChanged(height);
    }
}

QRect Shape::shapeRect() const
{
    return m_shapeRect;
}

int Shape::hp() const
{
    return m_hp;
}

void Shape::setHp(int hp)
{
    if(m_hp != hp) {
     m_hp = hp;
     emit hpChanged(hp);
    }
}

void Shape::setYCoord(const int &yCoord)
{
    if (m_yCoord != yCoord) {
        m_yCoord = yCoord;
        setShapeRect(QRect(m_xCoord, m_yCoord, m_width, m_height));
        emit yCoordChanged(yCoord);
        emit shapeRectChanged();
    }
}

void Shape::setDirection(int direction)
{
    if (m_direction != direction) {
        m_direction = direction;
        emit directionChanged(direction);
    }
}

void Shape::setSource(const QString &source)
{
    if (m_source != source) {
        m_source = source;
        emit sourceChanged(source);
    }
}

void Shape::setType(int type)
{
    if(m_type != type){
        m_type = type;
        emit typeChanged(type);
    }
}

void Shape::setXCoord(const int &xCoord)
{
    if (m_xCoord != xCoord) {
        m_xCoord = xCoord;
        setShapeRect(QRect(m_xCoord,m_yCoord,m_width,m_height));
        emit xCoordChanged(xCoord);
    }
}
