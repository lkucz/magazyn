#include <QDebug>
#include "colordelegate.h"

ColorDelegate::ColorDelegate(QObject *parent = 0) : QItemDelegate(parent) {}

void ColorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    drawBackground(painter, option, index);
    QItemDelegate::paint(painter, option, index);
}

void ColorDelegate::setColorMap(QMap<int, QColor> colorMap)
{
    this->colorMap = colorMap;
}

void ColorDelegate::drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(colorMap.contains(index.row()))
    {
        painter->fillRect(option.rect, colorMap.value(index.row()));
    }
}
