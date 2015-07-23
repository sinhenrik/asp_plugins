#pragma once

#include "B5Plugin.h"
#include "PluginRevisions.h"

#include "PluginOptionList.h"
#include "PluginImageSettings.h"

#include <QString>
#include <QObject>
#include <QCheckBox>

#define PLUGINNAME	"Bez"				// The  plugin group/owner name for all of your plug-in's settings keep it short but unique
#define PLUGINGROUP	"ferd"

//	Define here the names and IDs of your controls. 
//	Note that the buttons don't need IDs

#define FP_BEZ_B5			    "bez_importb5_button"
#define FP_BEZ_RESET			"bez_reset_button"	
#define FP_BEZ_ABOUT			"bez_about_button"	

#define FP_BEZ_ENABLED			"bez_enabled"			// Boolean - is the plug-in enabled?
#define FP_BEZ_EARLY			"bez_Early"				// Run Just before LAB
#define FP_BEZ_HCL				"bez_HCL"				// Convert to B&W Simply
#define FP_BEZ_OPACITY			"bez_opacity"			// Opacity definition slider
#define FP_BEZ_SATURATION		"bez_saturation"		// Saturation definition slider
#define FP_BEZ_LIGHTEN			"bez_lighten"			// Lighten definition slider
#define FP_BEZ_LIGHTENGAMMA		"bez_lightengamma"		// Black definition slider
#define FP_BEZ_MIDI				"bez_midi"				// Black definition slider
#define FP_BEZ_BLAK				"bez_blak"				// Black level slider
#define FP_BEZ_SIGCONTRAST		"bez_sigcontrast"		// Lab Contrast Sigmoidal Contrast Slider
#define FP_BEZ_SIGMIDDLE		"bez_sigmiddle"		    // contrast amount
#define FP_BEZ_SHADOWS			"bez_shadows"			// shadows slider
#define FP_BEZ_LOWMIDS			"bez_lowmids"			// lowmids slider
#define FP_BEZ_GREYS			"bez_greys"			    // lowmids slider
#define FP_BEZ_HIGHMIDS			"bez_highmids"			// highmids slider
#define FP_BEZ_HIGHLIGHTS		"bez_highlights"		// highlights slider
#define FP_BEZ_SHADOWS4			"bez_shadows4"			// shadows slider
#define FP_BEZ_LOWMIDS4			"bez_lowmids4"			// lowmids slider
#define FP_BEZ_HIGHMIDS4		"bez_highmids4"		    // highmids slider
#define FP_BEZ_HIGHLIGHTS4		"bez_highlights4"		// highlights slider

#define ID_BEZ_ENABLED			100
#define ID_BEZ_EARLY			101
#define ID_BEZ_HCL				102
#define ID_BEZ_OPACITY			103
#define ID_BEZ_SATURATION		104
#define ID_BEZ_LIGHTEN			105
#define ID_BEZ_LIGHTENGAMMA		106
#define ID_BEZ_MIDI				107
#define ID_BEZ_BLAK				108
#define ID_BEZ_SIGCONTRAST		109
#define ID_BEZ_SIGMIDDLE		110
#define ID_BEZ_SHADOWS			111
#define ID_BEZ_LOWMIDS			112
#define ID_BEZ_GREYS			113
#define ID_BEZ_HIGHMIDS			114
#define ID_BEZ_HIGHLIGHTS		115
#define ID_BEZ_SHADOWS4			116
#define ID_BEZ_LOWMIDS4			117
#define ID_BEZ_HIGHMIDS4		118
#define ID_BEZ_HIGHLIGHTS4		119

// Legacy variables for B5 settings

#define FP_OLD_ENABLED			"ferd_bez_enabled"			// Boolean - is the plug-in enabled?
#define FP_OLD_RGB				"ferd_bez_RGB"				// Run in RGB mode just before curves
#define FP_OLD_BW				"ferd_bez_BW"				// Convert to B&W Simply
#define FP_OLD_LIGHTEN			"ferd_bez_lighten"			// Lighten definition slider
#define FP_OLD_LIGHTENGAMMA		"ferd_bez_lightengamma"		// Black definition slider
#define FP_OLD_MIDI				"ferd_bez_midi"				// Black definition slider
#define FP_OLD_BLAK				"ferd_bez_blak"				// Black level slider
#define FP_OLD_SIGCONTRAST		"ferd_bez_sigcontrast"		// Lab Contrast Sigmoidal Contrast Slider
#define FP_OLD_SIGMIDDLE		"ferd_bez_sigmiddle"		// contrast amount
#define FP_OLD_CONTRAST			"ferd_bez_contrast"			// contrast amount
#define FP_OLD_CENTRE			"ferd_bez_centre"			// contrast centre
#define FP_OLD_SHADOWS			"ferd_bez_shadows"			// shadows slider
#define FP_OLD_LOWMIDS			"ferd_bez_lowmids"			// lowmids slider
#define FP_OLD_GREYS			"ferd_bez_greys"			// lowmids slider
#define FP_OLD_HIGHMIDS			"ferd_bez_highmids"			// highmids slider
#define FP_OLD_HIGHLIGHTS		"ferd_bez_highlights"		// highlights slider
#define FP_OLD_SHADOWS4			"ferd_bez_shadows4"			// shadows slider
#define FP_OLD_LOWMIDS4			"ferd_bez_lowmids4"			// lowmids slider
#define FP_OLD_HIGHMIDS4		"ferd_bez_highmids4"		// highmids slider
#define FP_OLD_HIGHLIGHTS4		"ferd_bez_highlights4"		// highlights slider

#define ID_OLD_ENABLED			200
#define ID_OLD_RGB				201
#define ID_OLD_BW				202
#define ID_OLD_OPACITY			203
#define ID_OLD_SATURATION		204
#define ID_OLD_LIGHTEN			205
#define ID_OLD_LIGHTENGAMMA		206
#define ID_OLD_MIDI				207
#define ID_OLD_BLAK				208
#define ID_OLD_SIGCONTRAST		209
#define ID_OLD_SIGMIDDLE		210
#define ID_OLD_CONTRAST			211
#define ID_OLD_CENTRE			212
#define ID_OLD_SHADOWS			213
#define ID_OLD_LOWMIDS			214
#define ID_OLD_GREYS			215
#define ID_OLD_HIGHMIDS			216
#define ID_OLD_HIGHLIGHTS		217
#define ID_OLD_SHADOWS4			218
#define ID_OLD_LOWMIDS4			219
#define ID_OLD_HIGHMIDS4		220
#define ID_OLD_HIGHLIGHTS4		221

//	The Plugin class
class BezPlugin : public QObject, public B5Plugin
{
        Q_OBJECT

public:
	BezPlugin() : m_hub( NULL ), m_id( -1 ), m_groupId( -1 ), m_pWindow( NULL ) { ; }
	virtual int priority() { return 900; }
	virtual QString name() { return QString(PLUGINNAME); }
	virtual QString group() { return QString(PLUGINGROUP); }
	virtual bool init(PluginHub *hub, int id, int groupId, const QString &bundlePath);
	virtual bool registerFilters();
	virtual bool registerOptions();
	virtual bool finish();
	virtual PluginDependency *createDependency(const QString &name);
	virtual QList<QString> toolFiles();
	virtual QList<QWidget*> toolWidgets();
	virtual void toolWidgetCreated(QWidget *uiWidget);

private slots:
	void showAbout() const;
	void resetControls() const;
	void handleSettingsChange(const PluginImageSettings &options, const PluginImageSettings &changed, int currentLayer);
	void handleControlChange(const QString &optionName, int groupId, int layer, const PluginImageSettings &options, const PluginOptionList &current, PluginOptionList &changes);
	void handleHotnessChanged(const PluginImageSettings &options);
    void importB5() const;

private:
	PluginHub *m_hub;
	int m_id;
	int m_groupId;
	QWidget *m_pWindow;
};
