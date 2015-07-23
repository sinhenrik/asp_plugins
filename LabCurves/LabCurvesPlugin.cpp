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
#include "LabCurvesFilterBefore.h"
#include "LabCurvesFilterAfter.h"
#include "LabCurvesPlugin.h"

// Bibble includes
#include "PluginHub.h"


#include <QDebug>
#include <QMessageBox>
#include <QSettings>


// Default values of the controls
#define SCALE_DEFAULT 0.0


extern "C" BIBBLE_API BaseB5Plugin *b5plugin()
{
    return new LabCurvesPlugin;
}

// Init function
bool LabCurvesPlugin::init(PluginHub *hub, int id, int groupId, const QString &)
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
bool LabCurvesPlugin::registerFilters()
{
    LabCurvesFilterBefore *filter1 = new LabCurvesFilterBefore(m_hub, m_groupId);
    LabCurvesFilterAfter *filter2 = new LabCurvesFilterAfter(m_hub, m_groupId);

    // Filters are placed before and after Curves
    if ( m_hub->addFilter(filter1, PluginHub::Before, QString("Curves")) &&  m_hub->addFilter(filter2, PluginHub::After, QString("Curves"))  )
    {
        qDebug() << "Successfully registered LabCurvesFilterBefore and LabCurvesFilterAfter";
        return true;
    }
    qDebug() << "Failed to register "<<PLUGIN_NAME<<" filters";
    return false;
}


//	Registering the options. It does the same thing as the B5 version, only the functions have different syntax
bool LabCurvesPlugin::registerOptions()
{
    m_hub->addBoolOption( LabCurvesFilterBefore::Enable,"enable",tr("Enable plugin"), tr("Enable LabCurves plugin"), FILTER_NAME_BEFORE, false, false, 0);
    m_hub->addDoubleOption( LabCurvesFilterBefore::Scale,"scale",tr("a,b scale"),tr("Scale factor for a and b channels"), FILTER_NAME_BEFORE, SCALE_DEFAULT, 0, 0);

    return true;
}

bool LabCurvesPlugin::finish()
{
    // Connect to controlChanged so we can do some settings dependency logic
    connect(m_hub,
            SIGNAL(controlChanged(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)),
            this,
            SLOT(handleControlChange(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)));

    return true;
}

PluginDependency *LabCurvesPlugin::createDependency(const QString &name)
{
    Q_UNUSED(name);
    return NULL;
}

QList<QString> LabCurvesPlugin::toolFiles()
{
    return QList<QString>();
}

QList<QWidget*> LabCurvesPlugin::toolWidgets()
{
    return QList<QWidget*>();
}

void LabCurvesPlugin::toolWidgetCreated(QWidget *uiWidget)
{
    // Connect the about button to its slot
    aboutButton = uiWidget->findChild<QPushButton*>("about_button");
    connect(aboutButton, SIGNAL(clicked()), this, SLOT(showAbout()));

    // Connect the reset button to its slot
    resetButton = uiWidget->findChild<QPushButton*>("reset_button");
    connect(resetButton, SIGNAL(clicked( bool )), this, SLOT(resetClicked(bool)));

    // Connect the enable button to its slot
    enableCheckBox = uiWidget->findChild<QCheckBox*>("enable_checkbox");

    // Connect the scale edit button to its slot
    scaleEdit= uiWidget->findChild<QDoubleSpinBox*>("scale_edit");
}

// Enable the filter when the values change
void LabCurvesPlugin::handleControlChange(const QString &optionName, int groupId, int layer, const PluginImageSettings &options, const PluginOptionList &current, PluginOptionList &changes)
{
    Q_UNUSED( current );

    // Check if one of the slider settings was changed
    if (groupId == m_groupId && ( optionName == "scale"))
    {
        // A value did change - Set "enable" to true if it isn't
        bool okay;
        bool enable = options.options(layer)->getBool(LabCurvesFilterBefore::Enable, m_groupId, okay);
        if (enable == false)
            changes.setBool(LabCurvesFilterBefore::Enable, m_groupId, "true");
    }
}


// Display an info message in a window
void LabCurvesPlugin::showAbout()
{
    QMessageBox::about(NULL,tr("LabCurves Plugin"),
                       tr("LabCurves Plugin version 0.1\n\
=====================\n\n\
This plugin allows to modify curves in Lab space. When the plugin is enabled, \
the RGB curves that are displayed in the Curves tool become indeed Lab curves.\n\
\n\
Thus, red curve becomes L, green curve becomes a and blue curve becomes b. \
Then, adjusting the curves with the mouse allows to fully correct the image in Lab space.\n\
\n\
Because the a and b histograms are very narrow, a scale factor allows to expand the histograms \
of a and b, and makes curve manipulation of these channels easier, but at the price of color clipping.\n\
\n\
This plugin is based on the former LabCurves plugin, created for Bibble by Thomas Baruchel.\n\
\n\
LabCurves 0.1\n\
Copyright (C) 2012 Roland Baudin\n\
http://roland65.free.fr/asp"));
}


// Reset plugin
void LabCurvesPlugin::resetClicked(bool)
{
    disconnect(m_hub, SIGNAL(controlChanged(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)), this, SLOT(handleControlChange(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)));
    enableCheckBox->setChecked(false);
    scaleEdit->setValue(SCALE_DEFAULT);
    connect(m_hub, SIGNAL(controlChanged(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)), this, SLOT(handleControlChange(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)));
}
