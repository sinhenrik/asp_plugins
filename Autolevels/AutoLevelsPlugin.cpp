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

// Plugin includes
#include "AutoLevelsData.h"
#include "AutoLevelsFilter.h"
#include "AutoLevelsPlugin.h"

// Bibble includes
#include "PluginHub.h"
#include "PluginDependency.h"


#include <QDebug>
#include <QMessageBox>
#include <QSettings>


// Default values of the controls
#define MINLEVEL_DEFAULT 0.01
#define MAXLEVEL_DEFAULT 0.01




extern "C" BIBBLE_API BaseB5Plugin *b5plugin()
{
    return new AutoLevelsPlugin;
}

// Init function
bool AutoLevelsPlugin::init(PluginHub *hub, int id, int groupId, const QString &)
{
    m_hub = hub;
    m_id = id;
    m_groupId = groupId;

	// Store our group ID and group name (andrewj)
    QSettings oSettings("Bibble and AfterShot Plugins", "PluginGroups");
    oSettings.setValue(group(), groupId);

    return true;
}

// Registering the filters
bool AutoLevelsPlugin::registerFilters()
{
    AutoLevelsFilter *filter = new AutoLevelsFilter(m_hub, m_groupId);

    // Filter is placed just after CameraRgbToWorkingspace_Raw
    if (m_hub->addFilter(filter, PluginHub::After, QString("CameraRgbToWorkingspace_Raw")))
    {
        qDebug() << "Successfully registered "<<PLUGIN_NAME<<" filter";
        return true;
    }
    qDebug() << "Failed to register "<<PLUGIN_NAME<<" filter";
    return false;
}

//	Registering the options. It does the same thing as the B5 version, only the functions have different syntax
bool AutoLevelsPlugin::registerOptions()
{
    m_hub->addBoolOption( AutoLevelsFilter::Enable,"enable",tr("Enable plugin"), tr("Enable AutoLevels plugin"), FILTER_NAME, false, false, 0);
    m_hub->addDoubleOption( AutoLevelsFilter::MinLevel,"minlevel",tr("Min level"),tr("Percentage of the image that will become pure black"), FILTER_NAME, MINLEVEL_DEFAULT, 0, 0);
    m_hub->addDoubleOption( AutoLevelsFilter::MaxLevel,"maxlevel",tr("Max level"),tr("Percentage of the image that will become pure white"), FILTER_NAME, MAXLEVEL_DEFAULT, 0, 0);

    return true;
}

bool AutoLevelsPlugin::finish()
{
    // Connect to controlChanged so we can do some settings dependency logic
    connect(m_hub,
            SIGNAL(controlChanged(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)),
            this,
            SLOT(handleControlChange(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)));

    return true;
}

PluginDependency *AutoLevelsPlugin::createDependency(const QString &name)
{
    if (name == PLUGINDATA_NAME)
        return new AutoLevelsData(m_hub,m_groupId);
    return NULL;
}

QList<QString> AutoLevelsPlugin::toolFiles()
{
    return QList<QString>();
}

QList<QWidget*> AutoLevelsPlugin::toolWidgets()
{
    return QList<QWidget*>();
}

void AutoLevelsPlugin::toolWidgetCreated(QWidget *uiWidget)
{
    // Connect the about button to its slot
    aboutButton = uiWidget->findChild<QPushButton*>("about_button");
    connect(aboutButton, SIGNAL(clicked()), this, SLOT(showAbout()));

    // Connect the reset button to its slot
    resetButton = uiWidget->findChild<QPushButton*>("reset_button");
    connect(resetButton, SIGNAL(clicked( bool )), this, SLOT(resetClicked(bool)));

    // Connect the enable button to its slot
    enableCheckBox = uiWidget->findChild<QCheckBox*>("enable_checkbox");

    // Connect the min level edit button to its slot
    minlevelEdit= uiWidget->findChild<QDoubleSpinBox*>("minlevel_edit");

    // Connect the max level edit button to its slot
    maxlevelEdit= uiWidget->findChild<QDoubleSpinBox*>("maxlevel_edit");

}

// Enable the filter when the values change
void AutoLevelsPlugin::handleControlChange(const QString &optionName, int groupId, int layer, const PluginImageSettings &options, const PluginOptionList &current, PluginOptionList &changes)
{
    Q_UNUSED( current );

    // Check if one of the slider settings was changed
    if (groupId == m_groupId && ( optionName == "minlevel" || optionName == "maxlevel"))
    {
        // A value did change - Set "enable" to true if it isn't
        bool okay;
        bool enable = options.options(layer)->getBool(AutoLevelsFilter::Enable, m_groupId, okay);
        if (enable == false)
            changes.setBool(AutoLevelsFilter::Enable, m_groupId, "true");
    }
}


// Display an info message in a window
void AutoLevelsPlugin::showAbout()
{
    QMessageBox::about(NULL,tr("AutoLevels Plugin"),
                       tr("AutoLevels Plugin version 0.3\n\
=====================\n\n\
This simple plugin implements the auto levels function.\n\n\
The image histogram is stretched so that a percentage (Black control) of the image is pure black and a percentage (White control) of the image is pure white.\n\n\
This is the same function as the builtin AutoLevel, but placed later in the filter stack.\n\n\
AutoLevels 0.3\n\
Copyright (C) 2012 Roland Baudin\n\
http://roland65.free.fr"));
}


// Reset plugin
void AutoLevelsPlugin::resetClicked(bool)
{
    disconnect(m_hub, SIGNAL(controlChanged(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)), this, SLOT(handleControlChange(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)));
    enableCheckBox->setChecked(false);
    minlevelEdit->setValue(MINLEVEL_DEFAULT);
    maxlevelEdit->setValue(MAXLEVEL_DEFAULT);
    connect(m_hub, SIGNAL(controlChanged(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)), this, SLOT(handleControlChange(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)));
}
