/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2021 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <QByteArray>


// common scene states

enum SceneInfoState
{
	SIS_Select = 0,
	SIS_Hover,
	SIS_Drag,
	SIS_Hover_Port,
	SIS_Edit_Label
};


// common scene attributes
const QByteArray class_scene = QByteArrayLiteral("");
const QByteArray class_item = QByteArrayLiteral("item");
const QByteArray class_node = QByteArrayLiteral("node");
const QByteArray class_edge = QByteArrayLiteral("edge");

const QByteArray attr_id = QByteArrayLiteral("id");
const QByteArray attr_size = QByteArrayLiteral("size");
const QByteArray attr_weight = QByteArrayLiteral("weight");
const QByteArray attr_color = QByteArrayLiteral("color");
const QByteArray attr_style = QByteArrayLiteral("style");
const QByteArray attr_shape = QByteArrayLiteral("shape");
const QByteArray attr_stroke_color = QByteArrayLiteral("stroke.color");
const QByteArray attr_stroke_style = QByteArrayLiteral("stroke.style");
const QByteArray attr_stroke_size = QByteArrayLiteral("stroke.size");
const QByteArray attr_label = QByteArrayLiteral("label");
const QByteArray attr_label_font = QByteArrayLiteral("label.font");
const QByteArray attr_label_color = QByteArrayLiteral("label.color");
const QByteArray attr_label_position = QByteArrayLiteral("label.position");
const QByteArray attr_labels_policy = QByteArrayLiteral("labels.policy");
const QByteArray attr_labels_visIds = QByteArrayLiteral("labels.visibleIds");
const QByteArray attr_edge_direction = QByteArrayLiteral("direction");
