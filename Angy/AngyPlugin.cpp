#include "AngyPlugin.h"
#include "FilterAngy.h"


#include "PluginHub.h"
#include "PluginDependency.h"

#include <QDebug>
#include <iostream>


#include <QWidget>
#include <QPushButton>
#include <QSettings>


#define PLUGIN_NAME_HR "Angy"

#define DEFAULT_BLEACH false
#define DEFAULT_SCREEN 80
#define DEFAULT_TOTAL 100
#define DEFAULT_FLATTEN 0
#define DEFAULT_BRIGHTNESS 0
#define DEFAULT_RECOVERY 0
#define DEFAULT_DEGAMMA 100

extern "C" BIBBLE_API B5Plugin *b5plugin() { return new AngyPlugin; }


bool AngyPlugin::init(PluginHub *hub, int id, int groupId, const QString &bundlePath)
{
    Q_UNUSED(bundlePath);
    qDebug() << "Initialising "<<PLUGIN_NAME_HR<<" filter";
    
    if(hub == NULL) return false;
	m_hub = hub;
	m_id = id;
	m_groupId = groupId;
	
	// Store our group ID and group name (andrewj)
//    QSettings oSettings("Bibble and AfterShot Plugins", "PluginGroups");
//    oSettings.setValue(group(), groupId);
	return true;
}

bool AngyPlugin::registerFilters()
{
    AngyFilter *angy = new AngyFilter(m_hub, m_groupId);
    
    if (m_hub->addFilter(angy, PluginHub::Before, QString("Curves"))) {
        qDebug() << "Successfully registered "<<angy->name()<<" filter after Curves";
	} else {
    	qDebug() << "Failed to register "<<angy->name()<<" filter";
	    return false;
	}

	return true;
}

#define SNONE "None"
bool AngyPlugin::registerOptions()
{
	m_hub->addBoolOption  (AngyFilter::Enable   ,   "AngyOn" ,      "Enabled"           , tr("Enable Plugin"), "AngyFilter", false, false, 0);
	m_hub->addBoolOption  (AngyFilter::Bleach   ,   "AngyBleach" ,  "Bleach"            , tr("Bleach Bypass"), "AngyFilter", DEFAULT_BLEACH, false, 0);
	
	m_hub->addIntOption   (AngyFilter::Screen   ,   "AngyOpa" ,     "Screen"            , tr("Screen effect"), "AngyFilter", DEFAULT_SCREEN, 0, 0);
	m_hub->addIntOption   (AngyFilter::Total    ,   "AngyStrength", "Total"             , tr("Total Effect Strength"), "AngyFilter", DEFAULT_TOTAL, 0, 0);
	m_hub->addIntOption   (AngyFilter::Flatten  ,   "AngyEdge" ,    "Flatten"           , tr("Flattening effect, reduces contrast"), "AngyFilter", DEFAULT_FLATTEN, 0, 0);
	m_hub->addIntOption   (AngyFilter::Brightness,  "AngyBrightness","Brightness"       , tr("Brightness"), "AngyFilter", DEFAULT_BRIGHTNESS, 0, 0);
	m_hub->addIntOption   (AngyFilter::Recovery ,   "AngyRecovery", "Regovery"          , tr("Recovers shadows and highlights"), "AngyFilter", DEFAULT_RECOVERY, 0, 0);
	m_hub->addIntOption   (AngyFilter::Degamma  ,   "AngyDegamma",  "Degamma"           , tr("Modifies working space gamma value"), "AngyFilter", DEFAULT_DEGAMMA, 0, 0);

	return true;
}

bool AngyPlugin::finish()
{
	connect(m_hub, SIGNAL(settingsChanged(const PluginImageSettings &, const PluginImageSettings &, int)), this, SLOT(handleSettingsChange(const PluginImageSettings &, const PluginImageSettings &, int)));
	connect(m_hub, SIGNAL(controlChanged(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)), this, SLOT(handleControlChange(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)));
   	connect(m_hub, SIGNAL(hotnessChanged(const PluginImageSettings &)), this, SLOT(handleHotnessChanged(const PluginImageSettings &)));
	return true;
}

PluginDependency *AngyPlugin::createDependency(const QString &name)
{
	Q_UNUSED(name);
	return NULL;
}

QList<QString> AngyPlugin::toolFiles()
{
	return QList<QString>();
}

QList<QWidget*> AngyPlugin::toolWidgets()
{
	return QList<QWidget*>();
}

void AngyPlugin::toolWidgetCreated(QWidget *uiWidget)
{
    //Q_UNUSED(uiWidget);
    if(uiWidget == NULL) return; //protection
    
    
    if (QPushButton *rbtn = uiWidget->findChild<QPushButton*>("AngyReset_btn")) {
			connect(rbtn, SIGNAL(clicked()), this, SLOT(reset()));
		}

}

//========================================================================================================
void AngyPlugin::handleSettingsChange(const PluginImageSettings &options, const PluginImageSettings &changed, int currentLayer)
{

    Q_UNUSED(options);
    Q_UNUSED(changed);
    Q_UNUSED(currentLayer);
}

void AngyPlugin::handleControlChange(const QString &optionName, int groupId, int layer, const PluginImageSettings &options, const PluginOptionList &current, PluginOptionList &changes)
{

    Q_UNUSED(optionName);
    Q_UNUSED(groupId);
    Q_UNUSED(current);
    //   v-- You need to add this if you're going to use the options --v
    //   some changes do not have options - check for NULL
    //    qDebug()<<"handlecontrolchange_prenull";
    if (options.options(layer) == NULL) return;
    //bool okay;
    
    //qDebug()<<"handlecontrolchange";
	if(
	    changes.contains(AngyFilter::Bleach, m_groupId)
	  ||changes.contains(AngyFilter::Screen, m_groupId)
	  ||changes.contains(AngyFilter::Total, m_groupId)
	  ||changes.contains(AngyFilter::Flatten, m_groupId)
	  ||changes.contains(AngyFilter::Brightness, m_groupId)
 	  ||changes.contains(AngyFilter::Recovery, m_groupId)
	  ||changes.contains(AngyFilter::Degamma, m_groupId)
	) {
        changes.setBool(AngyFilter::Enable, m_groupId, true);
	}

}

//========================================================================================================
void AngyPlugin::handleHotnessChanged(const PluginImageSettings &options)
{

    Q_UNUSED(options);
}

void AngyPlugin::reset() {
    if (PluginOptionList *options = m_hub->beginSettingsChange("Reset Angy")) {
		options->setInt(AngyFilter::Bleach, m_groupId,      DEFAULT_BLEACH);
		options->setInt(AngyFilter::Screen, m_groupId,      DEFAULT_SCREEN);
		options->setInt(AngyFilter::Total, m_groupId,       DEFAULT_TOTAL);
		options->setInt(AngyFilter::Flatten, m_groupId,     DEFAULT_FLATTEN);
		options->setInt(AngyFilter::Brightness, m_groupId,  DEFAULT_BRIGHTNESS);
		options->setInt(AngyFilter::Recovery, m_groupId,    DEFAULT_RECOVERY);
		options->setInt(AngyFilter::Degamma, m_groupId,     DEFAULT_DEGAMMA);
		m_hub->endSettingChange();
	}
}
