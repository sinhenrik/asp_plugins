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
#include "LabToolsFilter.h"
#include "LabToolsPlugin.h"

// Bibble includes
#include "PluginHub.h"


#include <QDebug>
#include <QMessageBox>
#include <QSettings>


// Default values of the controls
#define CONTRAST_DEFAULT 0.0
#define LIGHTNESS_DEFAULT 0.0
#define SATURATION_DEFAULT 0.0


extern "C" BIBBLE_API BaseB5Plugin *b5plugin()
{
    return new LabToolsPlugin;
}

// Init function
bool LabToolsPlugin::init(PluginHub *hub, int id, int groupId, const QString &)
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
bool LabToolsPlugin::registerFilters()
{
    LabToolsFilter *filter = new LabToolsFilter(m_hub, m_groupId);

    // Filter is placed just before Curves
    if (m_hub->addFilter(filter, PluginHub::Before, QString("Curves")))
    {
        qDebug() << "Successfully registered "<<PLUGIN_NAME<<" filter";
        return true;
    }
    qDebug() << "Failed to register "<<PLUGIN_NAME<<" filter";
    return false;
}

//	Registering the options. It does the same thing as the B5 version, only the functions have different syntax
bool LabToolsPlugin::registerOptions()
{
    m_hub->addBoolOption( LabToolsFilter::Enable,"enable",tr("Enable plugin"), tr("Enable LabTools plugin"), FILTER_NAME, false, false, 0);
    m_hub->addDoubleOption( LabToolsFilter::Contrast,"contrast",tr("Contrast"),tr("Contrast level"), FILTER_NAME, CONTRAST_DEFAULT, 0, 0);
    m_hub->addDoubleOption( LabToolsFilter::Lightness,"lightness",tr("Lightness"),tr("Ligtness level"), FILTER_NAME, LIGHTNESS_DEFAULT, 0, 0);
    m_hub->addDoubleOption( LabToolsFilter::Saturation,"saturation",tr("Saturation"),tr("Saturation level"), FILTER_NAME, SATURATION_DEFAULT, 0, 0);

    return true;
}

bool LabToolsPlugin::finish()
{
    // Connect to controlChanged so we can do some settings dependency logic
    connect(m_hub,
            SIGNAL(controlChanged(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)),
            this,
            SLOT(handleControlChange(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)));

    return true;
}

PluginDependency *LabToolsPlugin::createDependency(const QString &name)
{
    Q_UNUSED(name);
    return NULL;
}

QList<QString> LabToolsPlugin::toolFiles()
{
    return QList<QString>();
}

QList<QWidget*> LabToolsPlugin::toolWidgets()
{
    return QList<QWidget*>();
}

void LabToolsPlugin::toolWidgetCreated(QWidget *uiWidget)
{
    // Connect the about button to its slot
    aboutButton = uiWidget->findChild<QPushButton*>("about_button");
    connect(aboutButton, SIGNAL(clicked()), this, SLOT(showAbout()));

    // Connect the reset button to its slot
    resetButton = uiWidget->findChild<QPushButton*>("reset_button");
    connect(resetButton, SIGNAL(clicked( bool )), this, SLOT(resetClicked(bool)));

    // Connect the enable button to its slot
    enableCheckBox = uiWidget->findChild<QCheckBox*>("enable_checkbox");

    // Connect the contrast edit button to its slot
    contrastEdit= uiWidget->findChild<QDoubleSpinBox*>("contrast_edit");

    // Connect the lightness edit button to its slot
    lightnessEdit= uiWidget->findChild<QDoubleSpinBox*>("lightness_edit");

    // Connect the saturation edit button to its slot
    saturationEdit= uiWidget->findChild<QDoubleSpinBox*>("saturation_edit");
}

// Enable the filter when the values change
void LabToolsPlugin::handleControlChange(const QString &optionName, int groupId, int layer, const PluginImageSettings &options, const PluginOptionList &current, PluginOptionList &changes)
{
    Q_UNUSED( current );

    // Check if one of the slider settings was changed
    if (groupId == m_groupId && ( optionName == "contrast" || optionName == "lightness" || optionName == "saturation"))
    {
        // A value did change - Set "enable" to true if it isn't
        bool okay;
        bool enable = options.options(layer)->getBool(LabToolsFilter::Enable, m_groupId, okay);
        if (enable == false)
            changes.setBool(LabToolsFilter::Enable, m_groupId, "true");
    }
}


// Display an info message in a window
void LabToolsPlugin::showAbout()
{
    QMessageBox::about(NULL,tr("LabTools Plugin"),
                       tr("LabTools Plugin version 0.6\n\
==================\n\n\
This plugin allows to increase or decrease the image contrast and lightness by applying an S-shape curve to the Lab L channel.\n\
\n\
It also allows to increase or decrease the saturation level by modifying the slope of the Lab a and b channels.\n\
\n\
The plugin effect is controlled using three parameters:\n\
- Contrast: increase or decrease the image contrast\n\
- Lightness: increase or decrease the image lightness\n\
- Saturation: increase or decrease the image saturation\n\
\n\
Note that, due to the nonlinear transformation used, the lightness effect depends on the contrast level value.\n\
\n\
LabTools 0.6\n\
Copyright (C) 2012 Roland Baudin\n\
http://roland65.free.fr/asp"));
}


// Reset plugin
void LabToolsPlugin::resetClicked(bool)
{
    disconnect(m_hub, SIGNAL(controlChanged(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)), this, SLOT(handleControlChange(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)));
    enableCheckBox->setChecked(false);
    contrastEdit->setValue(CONTRAST_DEFAULT);
    lightnessEdit->setValue(LIGHTNESS_DEFAULT);
    saturationEdit->setValue(SATURATION_DEFAULT);
    connect(m_hub, SIGNAL(controlChanged(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)), this, SLOT(handleControlChange(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)));
}
