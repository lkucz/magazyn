#ifndef COLORDELEGATE_H
#define COLORDELEGATE_H

#include <QItemDelegate>
#include <QPainter>
#include <QColor>
#include <QMap>

class ColorDelegate: public QItemDelegate
{
public:
    ColorDelegate(QObject *parent);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setColorMap(QMap<int, QColor> colorMap);

protected:
    virtual void drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QMap<int, QColor> colorMap;
};


#endif // COLORDELEGATE_H

