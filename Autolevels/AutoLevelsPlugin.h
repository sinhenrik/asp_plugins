/*
 * (C) 2012, Roland Baudin <roland65@free.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef AUTOLEVELS_PLUGIN_H
#define AUTOLEVELS_PLUGIN_H

#include "B5Plugin.h"
#include <QObject>
#include <QString>
#include <QPushButton>
#include <QCheckBox>
#include <QDoubleSpinBox>

#include "PluginImageSettings.h"
#include "PluginOptionList.h"

#define PLUGIN_NAME		"AutoLevels"
#define PLUGIN_GROUP	"AutoLevels"


class AutoLevelsPlugin : public QObject, public B5Plugin  // QObject class needed because slots are used
{
    Q_OBJECT  // Needed because slots are used
public:

    // Constructor
    AutoLevelsPlugin()
    {
        ;
    }
    virtual int priority()
    {
        return 100;
    }

    // Plugin name
    virtual QString name()
    {
        return QString(PLUGIN_NAME);
    }

    // Plugin group (unique to several plugins of the same group)
    virtual QString group()
    {
        return QString(PLUGIN_GROUP);
    }
    virtual bool init(PluginHub *hub, int id, int groupId, const QString &bundlePath);
    virtual bool registerFilters();
    virtual bool registerOptions();
    virtual bool finish();
    virtual PluginDependency *createDependency(const QString &name);
    virtual QList<QString> toolFiles();
    virtual QList<QWidget*> toolWidgets();
    virtual void toolWidgetCreated(QWidget *uiWidget);

// Slots
private slots:
    void handleControlChange(const QString &optionName, int groupId, int layer, const PluginImageSettings &options, const PluginOptionList &current, PluginOptionList &changes);
    void showAbout();
    void resetClicked(bool);

private:
    PluginHub *m_hub;
    int m_id;
    int m_groupId;
    QPushButton* resetButton;
    QPushButton* aboutButton;
    QCheckBox* enableCheckBox;
    QDoubleSpinBox* minlevelEdit;
    QDoubleSpinBox* maxlevelEdit;
};

#endif
