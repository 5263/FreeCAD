/***************************************************************************
 *   Copyright (c) 2004 Werner Mayer <wmayer[at]users.sourceforge.net>     *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"

#ifndef _PreComp_
# include <algorithm>
# include <QComboBox>
# include <QLayout>
# include <QPixmap>
# include <QSpinBox>
#endif

#include <App/Application.h>
#include <App/Document.h>
#include <App/DocumentObject.h>
#include <App/PropertyGeo.h>
#include <App/PropertyFile.h>
#include <Gui/Application.h>
#include <Gui/Document.h>
#include <Gui/ViewProviderDocumentObject.h>
#include <Gui/Widgets.h>
#include <Gui/Placement.h>

#include "PropertyItem.h"

using namespace Gui::PropertyEditor;

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyItem, Base::BaseClass);

PropertyItem::PropertyItem() : parentItem(0), readonly(false)
{
}

PropertyItem::~PropertyItem()
{
    qDeleteAll(childItems);
}

void PropertyItem::reset()
{
    qDeleteAll(childItems);
    childItems.clear();
}

void PropertyItem::setPropertyData(const std::vector<App::Property*>& items)
{
    propertyItems = items;
    bool ro = true;
    for (std::vector<App::Property*>::const_iterator it = items.begin();
        it != items.end(); ++it) {
        App::PropertyContainer* parent = (*it)->getContainer();
        if (parent)
            ro &= parent->isReadOnly(*it);
    }
    this->setReadOnly(ro);
}

const std::vector<App::Property*>& PropertyItem::getPropertyData() const
{
    return propertyItems;
}

void PropertyItem::setParent(PropertyItem* parent)
{
    parentItem = parent;
}

PropertyItem *PropertyItem::parent() const
{
    return parentItem;
}

void PropertyItem::appendChild(PropertyItem *item)
{
    childItems.append(item);
}

PropertyItem *PropertyItem::child(int row)
{
    return childItems.value(row);
}

int PropertyItem::childCount() const
{
    return childItems.count();
}

int PropertyItem::columnCount() const
{
    return 2;
}

void PropertyItem::setReadOnly(bool ro)
{
    readonly = ro;
}

bool PropertyItem::isReadOnly() const
{
    return readonly;
}

QVariant PropertyItem::toolTip(const App::Property* prop) const
{
    return QVariant(QString::fromUtf8(prop->getDocumentation()));
}

QVariant PropertyItem::decoration(const App::Property* /*prop*/) const
{
    return QVariant();
}

QVariant PropertyItem::toString(const QVariant& prop) const
{
    return prop;
}

QVariant PropertyItem::value(const App::Property* /*prop*/) const
{
    return QVariant();
}

void PropertyItem::setValue(const QVariant& /*value*/)
{
}

QString PropertyItem::pythonIdentifier(const App::Property* prop) const
{
    App::PropertyContainer* parent = prop->getContainer();
    if (parent->getTypeId() == App::Document::getClassTypeId()) {
        App::Document* doc = static_cast<App::Document*>(parent);
        QString docName = QString::fromAscii(App::GetApplication().getDocumentName(doc));
        QString propName = QString::fromAscii(parent->getName(prop));
        return QString::fromAscii("FreeCAD.getDocument(\"%1\").%2").arg(docName).arg(propName);
    }
    if (parent->getTypeId().isDerivedFrom(App::DocumentObject::getClassTypeId())) {
        App::DocumentObject* obj = static_cast<App::DocumentObject*>(parent);
        App::Document* doc = obj->getDocument();
        QString docName = QString::fromAscii(App::GetApplication().getDocumentName(doc));
        QString objName = QString::fromAscii(obj->getNameInDocument());
        QString propName = QString::fromAscii(parent->getName(prop));
        return QString::fromAscii("FreeCAD.getDocument(\"%1\").getObject(\"%2\").%3")
            .arg(docName).arg(objName).arg(propName);
    }
    if (parent->getTypeId().isDerivedFrom(Gui::ViewProviderDocumentObject::getClassTypeId())) {
        App::DocumentObject* obj = static_cast<Gui::ViewProviderDocumentObject*>(parent)->getObject();
        App::Document* doc = obj->getDocument();
        QString docName = QString::fromAscii(App::GetApplication().getDocumentName(doc));
        QString objName = QString::fromAscii(obj->getNameInDocument());
        QString propName = QString::fromAscii(parent->getName(prop));
        return QString::fromAscii("FreeCADGui.getDocument(\"%1\").getObject(\"%2\").%3")
            .arg(docName).arg(objName).arg(propName);
    }
    return QString();
}

QWidget* PropertyItem::createEditor(QWidget* /*parent*/, const QObject* /*receiver*/, const char* /*method*/) const
{
    return 0;
}

void PropertyItem::setEditorData(QWidget * /*editor*/, const QVariant& /*data*/) const
{
}

QVariant PropertyItem::editorData(QWidget * /*editor*/) const
{
    return QVariant();
}

QString PropertyItem::propertyName() const
{
    if (propName.isEmpty())
        return QLatin1String("<empty>");
    return propName;
}

void PropertyItem::setPropertyName(const QString& name)
{
    setObjectName(name);
    QString display;
    bool upper = false;
    for (int i=0; i<name.length(); i++) {
        if (name[i].isUpper() && !display.isEmpty()) {
            // if there is a sequence of capital letters do not insert spaces
            if (!upper)
                display += QLatin1String(" ");
        }
        upper = name[i].isUpper();
        display += name[i];
    }

    propName = display;
}

void PropertyItem::setPropertyValue(const QString& value)
{
    for (std::vector<App::Property*>::const_iterator it = propertyItems.begin();
        it != propertyItems.end(); ++it) {
        App::PropertyContainer* parent = (*it)->getContainer();
        if (parent && !parent->isReadOnly(*it)) {
            QString cmd = QString::fromAscii("%1 = %2").arg(pythonIdentifier(*it)).arg(value);
            Gui::Application::Instance->runPythonCode((const char*)cmd.toAscii());
        }
    }
}

QVariant PropertyItem::data(int column, int role) const
{
    // property name
    if (column == 0) {
        if (role == Qt::DisplayRole)
            return propertyName();
        // no properties set
        if (propertyItems.empty())
            return QVariant();
        else if (role == Qt::ToolTipRole)
            return toolTip(propertyItems[0]);
        else
            return QVariant();
    }
    else {
        // no properties set
        if (propertyItems.empty()) {
            PropertyItem* parent = this->parent();
            if (!parent || !parent->parent())
                return QVariant();
            if (role == Qt::EditRole)
                return parent->property(qPrintable(objectName()));
            else if (role == Qt::DisplayRole) {
                QVariant val = parent->property(qPrintable(objectName()));
                return toString(val);

            }
            else
                return QVariant();
        }
        if (role == Qt::EditRole)
            return value(propertyItems[0]);
        else if (role == Qt::DecorationRole)
            return decoration(propertyItems[0]);
        else if (role == Qt::DisplayRole)
            return toString(value(propertyItems[0]));
        else if (role == Qt::ToolTipRole)
            return toolTip(propertyItems[0]);
        else
            return QVariant();
    }
}

bool PropertyItem::setData (const QVariant& value)
{
    if (propertyItems.empty()) {
        PropertyItem* parent = this->parent();
        if (!parent || !parent->parent())
            return false;
        parent->setProperty(qPrintable(objectName()),value);
        return true;
    }
    else {
        setValue(value);
        return true;
    }
}

Qt::ItemFlags PropertyItem::flags(int column) const
{
    Qt::ItemFlags basicFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (column == 1 && !isReadOnly())
        return basicFlags | Qt::ItemIsEditable;
    else
        return basicFlags;
}

int PropertyItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<PropertyItem*>(this));

    return 0;
}

// --------------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyStringItem, Gui::PropertyEditor::PropertyItem);

PropertyStringItem::PropertyStringItem()
{
}

QVariant PropertyStringItem::value(const App::Property* prop) const
{
    assert(prop && prop->getTypeId().isDerivedFrom(App::PropertyString::getClassTypeId()));

    std::string value = static_cast<const App::PropertyString*>(prop)->getValue();
    return QVariant(QString::fromUtf8(value.c_str()));
}

void PropertyStringItem::setValue(const QVariant& value)
{
    if (!value.canConvert(QVariant::String))
        return;
    QString val = value.toString();
    QString data = QString::fromAscii("\"%1\"").arg(val);
    setPropertyValue(data);
}

QWidget* PropertyStringItem::createEditor(QWidget* parent, const QObject* receiver, const char* method) const
{
    QLineEdit *le = new QLineEdit(parent);
    le->setFrame(false);
    QObject::connect(le, SIGNAL(textChanged(const QString&)), receiver, method);
    return le;
}

void PropertyStringItem::setEditorData(QWidget *editor, const QVariant& data) const
{
    QLineEdit *le = qobject_cast<QLineEdit*>(editor);
    le->setText(data.toString());
}

QVariant PropertyStringItem::editorData(QWidget *editor) const
{
    QLineEdit *le = qobject_cast<QLineEdit*>(editor);
    return QVariant(le->text());
}

// --------------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertySeparatorItem, Gui::PropertyEditor::PropertyItem);

QWidget* PropertySeparatorItem::createEditor(QWidget* parent, const QObject* receiver, const char* method) const
{
    return 0;
}

// --------------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyIntegerItem, Gui::PropertyEditor::PropertyItem);

PropertyIntegerItem::PropertyIntegerItem()
{
}

QVariant PropertyIntegerItem::value(const App::Property* prop) const
{
    assert(prop && prop->getTypeId().isDerivedFrom(App::PropertyInteger::getClassTypeId()));

    int value = (int)static_cast<const App::PropertyInteger*>(prop)->getValue();
    return QVariant(value);
}

void PropertyIntegerItem::setValue(const QVariant& value)
{
    if (!value.canConvert(QVariant::Int))
        return;
    int val = value.toInt();
    QString data = QString::fromAscii("%1").arg(val);
    setPropertyValue(data);
}

QWidget* PropertyIntegerItem::createEditor(QWidget* parent, const QObject* receiver, const char* method) const
{
    QSpinBox *sb = new QSpinBox(parent);
    sb->setFrame(false);
    QObject::connect(sb, SIGNAL(valueChanged(int)), receiver, method);
    return sb;
}

void PropertyIntegerItem::setEditorData(QWidget *editor, const QVariant& data) const
{
    QSpinBox *sb = qobject_cast<QSpinBox*>(editor);
    sb->setRange(INT_MIN, INT_MAX);
    sb->setValue(data.toInt());
}

QVariant PropertyIntegerItem::editorData(QWidget *editor) const
{
    QSpinBox *sb = qobject_cast<QSpinBox*>(editor);
    return QVariant(sb->value());
}

// --------------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyIntegerConstraintItem, Gui::PropertyEditor::PropertyItem);

PropertyIntegerConstraintItem::PropertyIntegerConstraintItem()
{
}

QVariant PropertyIntegerConstraintItem::value(const App::Property* prop) const
{
    assert(prop && prop->getTypeId().isDerivedFrom(App::PropertyIntegerConstraint::getClassTypeId()));

    int value = (int)static_cast<const App::PropertyIntegerConstraint*>(prop)->getValue();
    return QVariant(value);
}

void PropertyIntegerConstraintItem::setValue(const QVariant& value)
{
    if (!value.canConvert(QVariant::Int))
        return;
    int val = value.toInt();
    QString data = QString::fromAscii("%1").arg(val);
    setPropertyValue(data);
}

QWidget* PropertyIntegerConstraintItem::createEditor(QWidget* parent, const QObject* receiver, const char* method) const
{
    QSpinBox *sb = new QSpinBox(parent);
    sb->setFrame(false);
    QObject::connect(sb, SIGNAL(valueChanged(int)), receiver, method);
    return sb;
}

void PropertyIntegerConstraintItem::setEditorData(QWidget *editor, const QVariant& /*data*/) const
{
    const std::vector<App::Property*>& items = getPropertyData();
    App::PropertyIntegerConstraint* prop = (App::PropertyIntegerConstraint*)items[0];

    const App::PropertyIntegerConstraint::Constraints* c = 
        ((App::PropertyIntegerConstraint*)prop)->getConstraints();
    QSpinBox *sb = qobject_cast<QSpinBox*>(editor);
    if (c) {
        sb->setMinimum(c->LowerBound);
        sb->setMaximum(c->UpperBound);
        sb->setSingleStep(c->StepSize);
    }
    else {
        sb->setMinimum(INT_MIN);
        sb->setMaximum(INT_MAX);
    }
    sb->setValue(prop->getValue());
}

QVariant PropertyIntegerConstraintItem::editorData(QWidget *editor) const
{
    QSpinBox *sb = qobject_cast<QSpinBox*>(editor);
    return QVariant(sb->value());
}

// --------------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyFloatItem, Gui::PropertyEditor::PropertyItem);

PropertyFloatItem::PropertyFloatItem()
{
}

QVariant PropertyFloatItem::toString(const QVariant& prop) const
{
    double value = prop.toDouble();
    QString data = QString::fromAscii("%1").arg(value,0,'f',2);
    const std::vector<App::Property*>& props = getPropertyData();
    if (!props.empty()) {
        if (props.front()->getTypeId().isDerivedFrom(App::PropertyAngle::getClassTypeId()))
            data += QString::fromUtf8(" \xc2\xb0");
        else if (props.front()->getTypeId().isDerivedFrom(App::PropertyDistance::getClassTypeId()))
            data += QLatin1String(" mm");
        else if (props.front()->getTypeId().isDerivedFrom(App::PropertyLength::getClassTypeId()))
            data += QLatin1String(" mm");
    }

    return QVariant(data);
}

QVariant PropertyFloatItem::value(const App::Property* prop) const
{
    assert(prop && prop->getTypeId().isDerivedFrom(App::PropertyFloat::getClassTypeId()));

    double value = static_cast<const App::PropertyFloat*>(prop)->getValue();
    return QVariant(value);
}

void PropertyFloatItem::setValue(const QVariant& value)
{
    if (!value.canConvert(QVariant::Double))
        return;
    double val = value.toDouble();
    QString data = QString::fromAscii("%1").arg(val,0,'f',2);
    setPropertyValue(data);
}

QWidget* PropertyFloatItem::createEditor(QWidget* parent, const QObject* receiver, const char* method) const
{
    QDoubleSpinBox *sb = new QDoubleSpinBox(parent);
    sb->setFrame(false);
    QObject::connect(sb, SIGNAL(valueChanged(double)), receiver, method);
    return sb;
}

void PropertyFloatItem::setEditorData(QWidget *editor, const QVariant& data) const
{
    QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox*>(editor);
    sb->setRange((double)INT_MIN, (double)INT_MAX);
    sb->setValue(data.toDouble());
    const std::vector<App::Property*>& prop = getPropertyData();
    if (prop.empty())
        return;
    else if (prop.front()->getTypeId().isDerivedFrom(App::PropertyAngle::getClassTypeId()))
        sb->setSuffix(QString::fromUtf8(" \xc2\xb0"));
    else if (prop.front()->getTypeId().isDerivedFrom(App::PropertyDistance::getClassTypeId()))
        sb->setSuffix(QLatin1String(" mm"));
    else if (prop.front()->getTypeId().isDerivedFrom(App::PropertyLength::getClassTypeId())) {
        sb->setMinimum(0.0);
        sb->setSuffix(QLatin1String(" mm"));
    }
}

QVariant PropertyFloatItem::editorData(QWidget *editor) const
{
    QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox*>(editor);
    return QVariant(sb->value());
}

// --------------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyFloatConstraintItem, Gui::PropertyEditor::PropertyItem);

PropertyFloatConstraintItem::PropertyFloatConstraintItem()
{
}

QVariant PropertyFloatConstraintItem::toString(const QVariant& prop) const
{
    double value = prop.toDouble();
    QString data = QString::fromAscii("%1").arg(value,0,'f',2);
    return QVariant(data);
}

QVariant PropertyFloatConstraintItem::value(const App::Property* prop) const
{
    assert(prop && prop->getTypeId().isDerivedFrom(App::PropertyFloatConstraint::getClassTypeId()));

    double value = static_cast<const App::PropertyFloatConstraint*>(prop)->getValue();
    return QVariant(value);
}

void PropertyFloatConstraintItem::setValue(const QVariant& value)
{
    if (!value.canConvert(QVariant::Double))
        return;
    double val = value.toDouble();
    QString data = QString::fromAscii("%1").arg(val,0,'f',2);
    setPropertyValue(data);
}

QWidget* PropertyFloatConstraintItem::createEditor(QWidget* parent, const QObject* receiver, const char* method) const
{
    QDoubleSpinBox *sb = new QDoubleSpinBox(parent);
    sb->setFrame(false);
    QObject::connect(sb, SIGNAL(valueChanged(double)), receiver, method);
    return sb;
}

void PropertyFloatConstraintItem::setEditorData(QWidget *editor, const QVariant& /*data*/) const
{
    const std::vector<App::Property*>& items = getPropertyData();
    App::PropertyFloatConstraint* prop = (App::PropertyFloatConstraint*)items[0];

    const App::PropertyFloatConstraint::Constraints* c = ((App::PropertyFloatConstraint*)prop)->getConstraints();
    QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox*>(editor);
    if (c) {
        sb->setMinimum(c->LowerBound);
        sb->setMaximum(c->UpperBound);
        sb->setSingleStep(c->StepSize);
    }
    else {
        sb->setMinimum((double)INT_MIN);
        sb->setMaximum((double)INT_MAX);
        sb->setSingleStep(0.1);
    }
    sb->setValue(prop->getValue());
}

QVariant PropertyFloatConstraintItem::editorData(QWidget *editor) const
{
    QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox*>(editor);
    return QVariant(sb->value());
}

// --------------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyBoolItem, Gui::PropertyEditor::PropertyItem);

PropertyBoolItem::PropertyBoolItem()
{
}

QVariant PropertyBoolItem::value(const App::Property* prop) const
{
    assert(prop && prop->getTypeId().isDerivedFrom(App::PropertyBool::getClassTypeId()));
    
    bool value = ((App::PropertyBool*)prop)->getValue();
    return QVariant(value);
}

void PropertyBoolItem::setValue(const QVariant& value)
{
    if (!value.canConvert(QVariant::Bool))
        return;
    bool val = value.toBool();
    QString data = (val ? QLatin1String("True") : QLatin1String("False"));
    setPropertyValue(data);
}

QWidget* PropertyBoolItem::createEditor(QWidget* parent, const QObject* receiver, const char* method) const
{
    QComboBox *cb = new QComboBox(parent);
    cb->setFrame(false);
    cb->addItem(QLatin1String("false"));
    cb->addItem(QLatin1String("true"));
    QObject::connect(cb, SIGNAL(activated(int)), receiver, method);
    return cb;
}

void PropertyBoolItem::setEditorData(QWidget *editor, const QVariant& data) const
{
    QComboBox *cb = qobject_cast<QComboBox*>(editor);
    cb->setCurrentIndex(cb->findText(data.toString()));
}

QVariant PropertyBoolItem::editorData(QWidget *editor) const
{
    QComboBox *cb = qobject_cast<QComboBox*>(editor);
    return QVariant(cb->currentText());
}

// ---------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyVectorItem, Gui::PropertyEditor::PropertyItem);

PropertyVectorItem::PropertyVectorItem()
{
    m_x = static_cast<PropertyFloatItem*>(PropertyFloatItem::create());
    m_x->setParent(this);
    m_x->setPropertyName(QLatin1String("x"));
    this->appendChild(m_x);
    m_y = static_cast<PropertyFloatItem*>(PropertyFloatItem::create());
    m_y->setParent(this);
    m_y->setPropertyName(QLatin1String("y"));
    this->appendChild(m_y);
    m_z = static_cast<PropertyFloatItem*>(PropertyFloatItem::create());
    m_z->setParent(this);
    m_z->setPropertyName(QLatin1String("z"));
    this->appendChild(m_z);
}

QVariant PropertyVectorItem::toString(const QVariant& prop) const
{
    const Base::Vector3f& value = prop.value<Base::Vector3f>();
    QString data = QString::fromAscii("[%1, %2, %3]")
        .arg(value.x,0,'f',2)
        .arg(value.y,0,'f',2)
        .arg(value.z,0,'f',2);
    return QVariant(data);
}

QVariant PropertyVectorItem::value(const App::Property* prop) const
{
    assert(prop && prop->getTypeId().isDerivedFrom(App::PropertyVector::getClassTypeId()));

    const Base::Vector3f& value = static_cast<const App::PropertyVector*>(prop)->getValue();
    return QVariant::fromValue<Base::Vector3f>(value);
}

void PropertyVectorItem::setValue(const QVariant& value)
{
    if (!value.canConvert<Base::Vector3f>())
        return;
    const Base::Vector3f& val = value.value<Base::Vector3f>();
    QString data = QString::fromAscii("(%1, %2, %3)")
                    .arg(val.x,0,'f',2)
                    .arg(val.y,0,'f',2)
                    .arg(val.z,0,'f',2);
    setPropertyValue(data);
}

QWidget* PropertyVectorItem::createEditor(QWidget* parent, const QObject* /*receiver*/, const char* /*method*/) const
{
    QLineEdit *le = new QLineEdit(parent);
    le->setFrame(false);
    le->setReadOnly(true);
    return le;
}

void PropertyVectorItem::setEditorData(QWidget *editor, const QVariant& data) const
{
    QLineEdit* le = qobject_cast<QLineEdit*>(editor);
    const Base::Vector3f& value = data.value<Base::Vector3f>();
    QString text = QString::fromAscii("[%1, %2, %3]")
        .arg(value.x,0,'f',2)
        .arg(value.y,0,'f',2)
        .arg(value.z,0,'f',2);
    le->setText(text);
}

QVariant PropertyVectorItem::editorData(QWidget *editor) const
{
    QLineEdit *le = qobject_cast<QLineEdit*>(editor);
    return QVariant(le->text());
}

double PropertyVectorItem::x() const
{
    return data(1,Qt::EditRole).value<Base::Vector3f>().x;
}

void PropertyVectorItem::setX(double x)
{
    setValue(QVariant::fromValue(Base::Vector3f(x, y(), z())));
}

double PropertyVectorItem::y() const
{
    return data(1,Qt::EditRole).value<Base::Vector3f>().y;
}

void PropertyVectorItem::setY(double y)
{
    setValue(QVariant::fromValue(Base::Vector3f(x(), y, z())));
}

double PropertyVectorItem::z() const
{
    return data(1,Qt::EditRole).value<Base::Vector3f>().z;
}

void PropertyVectorItem::setZ(double z)
{
    setValue(QVariant::fromValue(Base::Vector3f(x(), y(), z)));
}

// --------------------------------------------------------------------

namespace Gui { namespace PropertyEditor {
class PlacementEditor : public Gui::LabelButton
{
public:
    PlacementEditor(QWidget * parent = 0) : LabelButton(parent)
    {
    }
    ~PlacementEditor()
    {
    }
protected:
    void browse()
    {
        Gui::Dialog::Placement dlg(this);
        dlg.setPlacement(value().value<Base::Placement>());
        connect(&dlg, SIGNAL(placementChanged(const QVariant &)),
                this, SLOT(setValue(const QVariant&)));
        dlg.exec();
    }
    void showValue(const QVariant& d)
    {
        const Base::Placement& p = d.value<Base::Placement>();
        double angle;
        Base::Vector3d dir, pos;
        p.getRotation().getValue(dir, angle);
        pos = p.getPosition();
        QString data = QString::fromAscii("[(%1,%2,%3);%4;(%5,%6,%7)]")
                        .arg(dir.x,0,'f',2)
                        .arg(dir.y,0,'f',2)
                        .arg(dir.z,0,'f',2)
                        .arg(angle,0,'f',2)
                        .arg(pos.x,0,'f',2)
                        .arg(pos.y,0,'f',2)
                        .arg(pos.z,0,'f',2);
        getLabel()->setText(data);
    }
};
}
}

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyPlacementItem, Gui::PropertyEditor::PropertyItem);

PropertyPlacementItem::PropertyPlacementItem()
{
    m_d = static_cast<PropertyStringItem*>(PropertyStringItem::create());
    m_d->setParent(this);
    m_d->setPropertyName(QLatin1String("Axis"));
    m_d->setReadOnly(true);
    this->appendChild(m_d);
    m_a = static_cast<PropertyStringItem*>(PropertyStringItem::create());
    m_a->setParent(this);
    m_a->setPropertyName(QLatin1String("Angle"));
    m_a->setReadOnly(true);
    this->appendChild(m_a);
    m_p = static_cast<PropertyStringItem*>(PropertyStringItem::create());
    m_p->setParent(this);
    m_p->setPropertyName(QLatin1String("Position"));
    m_p->setReadOnly(true);
    this->appendChild(m_p);
}

QString PropertyPlacementItem::getAxis() const
{
    QVariant value = data(1, Qt::EditRole);
    if (!value.canConvert<Base::Placement>())
        return QString();
    const Base::Placement& val = value.value<Base::Placement>();
    double angle;
    Base::Vector3d dir;
    val.getRotation().getValue(dir, angle);

    QString text = QString::fromAscii("[%1, %2, %3]")
        .arg(dir.x,0,'f',2)
        .arg(dir.y,0,'f',2)
        .arg(dir.z,0,'f',2);
    return text;
}

QString PropertyPlacementItem::getAngle() const
{
    QVariant value = data(1, Qt::EditRole);
    if (!value.canConvert<Base::Placement>())
        return QString();
    const Base::Placement& val = value.value<Base::Placement>();
    double angle;
    Base::Vector3d dir;
    val.getRotation().getValue(dir, angle);
    angle = angle*180.0/D_PI;

    QString text = QString::fromUtf8("%1 \xc2\xb0").arg(angle,0,'f',2);
    return text;
}

QString PropertyPlacementItem::getPosition() const
{
    QVariant value = data(1, Qt::EditRole);
    if (!value.canConvert<Base::Placement>())
        return QString();
    const Base::Placement& val = value.value<Base::Placement>();

    QString text = QString::fromAscii("[%1, %2, %3]")
        .arg(val.getPosition().x,0,'f',2)
        .arg(val.getPosition().y,0,'f',2)
        .arg(val.getPosition().z,0,'f',2);
    return text;
}

QVariant PropertyPlacementItem::value(const App::Property* prop) const
{
    assert(prop && prop->getTypeId().isDerivedFrom(App::PropertyPlacement::getClassTypeId()));

    const Base::Placement& value = static_cast<const App::PropertyPlacement*>(prop)->getValue();
    return QVariant::fromValue<Base::Placement>(value);
}

QVariant PropertyPlacementItem::toolTip(const App::Property* prop) const
{
    assert(prop && prop->getTypeId().isDerivedFrom(App::PropertyPlacement::getClassTypeId()));

    const Base::Placement& p = static_cast<const App::PropertyPlacement*>(prop)->getValue();
    double angle;
    Base::Vector3d dir, pos;
    p.getRotation().getValue(dir, angle);
    pos = p.getPosition();
    QString data = QString::fromAscii("Axis: (%1,%2,%3)\n"
                                      "Angle: %4\n"
                                      "Move: (%5,%6,%7)")
                    .arg(dir.x,0,'f',2)
                    .arg(dir.y,0,'f',2)
                    .arg(dir.z,0,'f',2)
                    .arg(angle,0,'f',2)
                    .arg(pos.x,0,'f',2)
                    .arg(pos.y,0,'f',2)
                    .arg(pos.z,0,'f',2);
    return QVariant(data);
}

QVariant PropertyPlacementItem::toString(const QVariant& prop) const
{
    const Base::Placement& p = prop.value<Base::Placement>();
    double angle;
    Base::Vector3d dir, pos;
    p.getRotation().getValue(dir, angle);
    pos = p.getPosition();
    QString data = QString::fromAscii("[(%1,%2,%3);%4;(%5,%6,%7)]")
                    .arg(dir.x,0,'f',2)
                    .arg(dir.y,0,'f',2)
                    .arg(dir.z,0,'f',2)
                    .arg(angle,0,'f',2)
                    .arg(pos.x,0,'f',2)
                    .arg(pos.y,0,'f',2)
                    .arg(pos.z,0,'f',2);
    return QVariant(data);
}

void PropertyPlacementItem::setValue(const QVariant& value)
{
    if (!value.canConvert<Base::Placement>())
        return;
    const Base::Placement& val = value.value<Base::Placement>();
    Base::Vector3d pos = val.getPosition();
    const Base::Rotation& rt = val.getRotation();
    QString data = QString::fromAscii("App.Placement("
                                      "App.Vector(%1,%2,%3),"
                                      "App.Rotation(%4,%5,%6,%7))")
                    .arg(pos.x,0,'g',6)
                    .arg(pos.y,0,'g',6)
                    .arg(pos.z,0,'g',6)
                    .arg(rt[0],0,'g',6)
                    .arg(rt[1],0,'g',6)
                    .arg(rt[2],0,'g',6)
                    .arg(rt[3],0,'g',6);
    setPropertyValue(data);
}

QWidget* PropertyPlacementItem::createEditor(QWidget* parent, const QObject* receiver, const char* method) const
{
    PlacementEditor *pe = new PlacementEditor(parent);
    QObject::connect(pe, SIGNAL(valueChanged(const QVariant &)), receiver, method);
    return pe;
}

void PropertyPlacementItem::setEditorData(QWidget *editor, const QVariant& data) const
{
    Gui::LabelButton *pe = qobject_cast<Gui::LabelButton*>(editor);
    pe->setValue(data);
}

QVariant PropertyPlacementItem::editorData(QWidget *editor) const
{
    Gui::LabelButton *pe = qobject_cast<Gui::LabelButton*>(editor);
    return pe->value();
}

// ---------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyEnumItem, Gui::PropertyEditor::PropertyItem);

PropertyEnumItem::PropertyEnumItem()
{
}

QVariant PropertyEnumItem::value(const App::Property* prop) const
{
    assert(prop && prop->getTypeId().isDerivedFrom(App::PropertyEnumeration::getClassTypeId()));

    const App::PropertyEnumeration* prop_enum = static_cast<const App::PropertyEnumeration*>(prop);
    if (prop_enum->getEnums() == 0) {
        return QVariant(QString());
    }
    else {
        const std::vector<std::string>& value = prop_enum->getEnumVector();
        long currentItem = prop_enum->getValue();
        return QVariant(QString::fromUtf8(value[currentItem].c_str()));
    }
}

void PropertyEnumItem::setValue(const QVariant& value)
{
    if (!value.canConvert(QVariant::StringList))
        return;
    QStringList items = value.toStringList();
    if (!items.isEmpty()) {
        QString val = items.front();
        QString data = QString::fromAscii("\"%1\"").arg(val);
        setPropertyValue(data);
    }
}

QWidget* PropertyEnumItem::createEditor(QWidget* parent, const QObject* receiver, const char* method) const
{
    QComboBox *cb = new QComboBox(parent);
    cb->setFrame(false);
    QObject::connect(cb, SIGNAL(activated(int)), receiver, method);
    return cb;
}

void PropertyEnumItem::setEditorData(QWidget *editor, const QVariant& data) const
{
    const std::vector<App::Property*>& items = getPropertyData();

    QStringList commonModes, modes;
    for (std::vector<App::Property*>::const_iterator it = items.begin(); it != items.end(); ++it) {
        if ((*it)->getTypeId() == App::PropertyEnumeration::getClassTypeId()) {
            App::PropertyEnumeration* prop = static_cast<App::PropertyEnumeration*>(*it);
            if (prop->getEnums() == 0) {
                commonModes.clear();
                break;
            }
            const std::vector<std::string>& value = prop->getEnumVector();
            if (it == items.begin()) {
                for (std::vector<std::string>::const_iterator jt = value.begin(); jt != value.end(); ++jt)
                    commonModes << QLatin1String(jt->c_str());
            }
            else {
                for (std::vector<std::string>::const_iterator jt = value.begin(); jt != value.end(); ++jt) {
                    if (commonModes.contains(QLatin1String(jt->c_str())))
                        modes << QLatin1String(jt->c_str());
                }

                commonModes = modes;
                modes.clear();
            }
        }
    }

    QComboBox *cb = qobject_cast<QComboBox*>(editor);
    if (!commonModes.isEmpty()) {
        cb->addItems(commonModes);
        cb->setCurrentIndex(cb->findText(data.toString()));
    }
}

QVariant PropertyEnumItem::editorData(QWidget *editor) const
{
    QComboBox *cb = qobject_cast<QComboBox*>(editor);
    return QVariant(cb->currentText());
}

// ---------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyStringListItem, Gui::PropertyEditor::PropertyItem);

PropertyStringListItem::PropertyStringListItem()
{
}

QWidget* PropertyStringListItem::createEditor(QWidget* parent, const QObject* /*receiver*/, const char* /*method*/) const
{
    QComboBox *cb = new QComboBox(parent);
    cb->setFrame(false);
    return cb;
}

void PropertyStringListItem::setEditorData(QWidget *editor, const QVariant& data) const
{
    QComboBox *cb = qobject_cast<QComboBox*>(editor);
    cb->setEditable(true);
    cb->insertItems(0, data.toStringList());
    cb->setCurrentIndex(0);
}

QVariant PropertyStringListItem::editorData(QWidget *editor) const
{
    QComboBox *cb = qobject_cast<QComboBox*>(editor);
    QStringList list;
    for (int i=0; i<cb->count(); i++)
        list << cb->itemText(i);
    return QVariant(list);
}

QVariant PropertyStringListItem::toString(const QVariant& prop) const
{
    QStringList list = prop.toStringList();
    QString text = QString::fromAscii("[%1]").arg(list.join(QLatin1String(",")));

    return QVariant(text);
}

QVariant PropertyStringListItem::value(const App::Property* prop) const
{
    assert(prop && prop->getTypeId().isDerivedFrom(App::PropertyStringList::getClassTypeId()));

    QStringList list;
    const std::vector<std::string>& value = ((App::PropertyStringList*)prop)->getValues();
    for ( std::vector<std::string>::const_iterator jt = value.begin(); jt != value.end(); ++jt ) {
        list << QString::fromAscii(jt->c_str());
    }

    return QVariant(list);
}

void PropertyStringListItem::setValue(const QVariant& value)
{
    if (!value.canConvert(QVariant::StringList))
        return;
    QStringList values = value.toStringList();
    QString data = QString::fromAscii("[");
    for (QStringList::Iterator it = values.begin(); it != values.end(); ++it) {
        data += QString::fromAscii("\"%1\",").arg(*it);
    }
    data += QString::fromAscii("]");
    setPropertyValue(data);
}

// --------------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyColorItem, Gui::PropertyEditor::PropertyItem);

PropertyColorItem::PropertyColorItem()
{
}

QVariant PropertyColorItem::decoration(const App::Property* prop) const
{
    App::Color value = ((App::PropertyColor*)prop)->getValue();
    QColor color((int)(255.0f*value.r),(int)(255.0f*value.g),(int)(255.0f*value.b));

    int size = QApplication::style()->pixelMetric(QStyle::PM_ListViewIconSize);
    QPixmap p(size, size);
    p.fill(color);

    return QVariant(p);
}

QVariant PropertyColorItem::toString(const QVariant& prop) const
{
    QColor value = prop.value<QColor>();
    QString color = QString::fromAscii("[%1, %2, %3]")
        .arg(value.red()).arg(value.green()).arg(value.blue());
    return QVariant(color);
}

QVariant PropertyColorItem::value(const App::Property* prop) const
{
    assert(prop && prop->getTypeId().isDerivedFrom(App::PropertyColor::getClassTypeId()));

    App::Color value = ((App::PropertyColor*)prop)->getValue();
    return QVariant(QColor((int)(255.0f*value.r),(int)(255.0f*value.g),(int)(255.0f*value.b)));
}

void PropertyColorItem::setValue(const QVariant& value)
{
    if (!value.canConvert<QColor>())
        return;
    QColor col = value.value<QColor>();
    App::Color val;
    val.r = (float)col.red()/255.0f;
    val.g = (float)col.green()/255.0f;
    val.b = (float)col.blue()/255.0f;
    QString data = QString::fromAscii("(%1,%2,%3)")
                    .arg(val.r,0,'f',2)
                    .arg(val.g,0,'f',2)
                    .arg(val.b,0,'f',2);
    setPropertyValue(data);
}

QWidget* PropertyColorItem::createEditor(QWidget* parent, const QObject* receiver, const char* method) const
{
    Gui::ColorButton* cb = new Gui::ColorButton( parent );
    QObject::connect(cb, SIGNAL(changed()), receiver, method);
    return cb;
}

void PropertyColorItem::setEditorData(QWidget *editor, const QVariant& data) const
{
    Gui::ColorButton *cb = qobject_cast<Gui::ColorButton*>(editor);
    QColor color = data.value<QColor>();
    cb->setColor(color);
}

QVariant PropertyColorItem::editorData(QWidget *editor) const
{
    Gui::ColorButton *cb = qobject_cast<Gui::ColorButton*>(editor);
    QVariant var;
    var.setValue(cb->color());
    return var;
}

// --------------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyFileItem, Gui::PropertyEditor::PropertyItem);

PropertyFileItem::PropertyFileItem()
{
}

QVariant PropertyFileItem::value(const App::Property* prop) const
{
    assert(prop && prop->getTypeId().isDerivedFrom(App::PropertyFile::getClassTypeId()));

    std::string value = ((App::PropertyFile*)prop)->getValue();
    return QVariant(QString::fromUtf8(value.c_str()));
}

void PropertyFileItem::setValue(const QVariant& /*value*/)
{
}

QVariant PropertyFileItem::toolTip(const App::Property* prop) const
{
    return value(prop);
}

// --------------------------------------------------------------------

TYPESYSTEM_SOURCE(Gui::PropertyEditor::PropertyPathItem, Gui::PropertyEditor::PropertyItem);

PropertyPathItem::PropertyPathItem()
{
}

QVariant PropertyPathItem::value(const App::Property* /*prop*/) const
{
    return QVariant();
}

void PropertyPathItem::setValue(const QVariant& /*value*/)
{
}

// --------------------------------------------------------------------

PropertyItemEditorFactory::PropertyItemEditorFactory()
{
}

PropertyItemEditorFactory::~PropertyItemEditorFactory()
{
}

QWidget * PropertyItemEditorFactory::createEditor (QVariant::Type /*type*/, QWidget * /*parent*/) const
{
    // do not allow to create any editor widgets because we do that in subclasses of PropertyItem
    return 0;
}

QByteArray PropertyItemEditorFactory::valuePropertyName (QVariant::Type /*type*/) const
{
    // do not allow to set properties because we do that in subclasses of PropertyItem
    return "";
}

#include "moc_PropertyItem.cpp"

