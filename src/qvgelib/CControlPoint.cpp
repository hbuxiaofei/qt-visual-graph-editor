/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2021 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include <QGraphicsSceneContextMenuEvent>

#include "CControlPoint.h"
#include "CItem.h"


CControlPoint::CControlPoint(CItem *parent) : 
	Shape(dynamic_cast<QGraphicsItem*>(parent)),
	m_parentItem(parent)
{
 	Q_ASSERT(parent != NULL);

	setRect(-4, -4, 8, 8);
	setBrush(Qt::black);
	setPen(QPen(Qt::gray, 1));
}


// reimp 

QVariant CControlPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	//if (change == ItemPositionChange)
	//{
	//	if (auto editScene = dynamic_cast<CEditorScene*>(scene()))
	//	{
	//		return editScene->getSnapped(value.toPointF());
	//	}

	//	return value;
	//}

	if (change == ItemPositionHasChanged)
	{
		m_parentItem->onControlPointMoved(this, value.toPointF());

		return value;
	}

	return Shape::itemChange(change, value);
}


// menu

void CControlPoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	event->accept();

	QMenu menu;
	menu.addAction(tr("Delete point"), this, SLOT(onActionDelete()));
	menu.exec(event->screenPos());
}


void CControlPoint::onActionDelete()
{
	m_parentItem->onControlPointDelete(this);
}
