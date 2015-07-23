#include "BezPlugin.h"
#include "BezFilterEARLY.h"
#include "BezFilterLATE.h"
#include "PluginHub.h"
#include "PluginDependency.h"
#include "PluginOptionList.h"
#include "PluginImageSettings.h"
#include "cmath"
#include "../PluginTools/ToolData.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>

int midi_b5,blak_b5,contrast_b5,centre_b5,shadows_b5,lowmids_b5,greys_b5,highmids_b5,highlights_b5,shadows4_b5,lowmids4_b5,highmids4_b5,highlights4_b5;
double lighten_b5,lightengamma_b5,sigcontrast_b5,sigmiddle_b5;
bool enabled_b5, rgb_b5, bw_b5;

extern "C" BIBBLE_API BaseB5Plugin *b5plugin() { return new BezPlugin; }

bool BezPlugin::init(PluginHub *hub, int id, int groupId, const QString &)
{
	if(hub == NULL) return false;
	
	m_hub = hub;
	m_id = id;
	m_groupId = groupId;
	return true;
}

// Registering the filters
bool BezPlugin::registerFilters()
{
        //BezFilter is added after Negative Vignette in the processing pipe
        BezFilterEARLY *BezEARLY = new BezFilterEARLY(m_hub, m_groupId);
        BezFilterLATE *BezLATE = new BezFilterLATE(m_hub, m_groupId);
        if ( (m_hub->addFilter(BezEARLY, PluginHub::Before, QString("RgbToLab"))) && (m_hub->addFilter(BezLATE, PluginHub::Before, QString("Curves"))) ) {
		qDebug() << "Successfully registered "<<BezEARLY->name()<<" before RgbToLab and "<<BezLATE->name()<<" before Curves using priority " <<priority();
        return true;
	}
        qDebug() << "Failed to load the Bez filters";
	return false;
}

//	Registering the options. It does the same thing as the B5 version, only the functions have different syntax
//	If you want translateable strings, add the tr(...) around it.
bool BezPlugin::registerOptions()
{
        bool b = true;
		b &= m_hub->addBoolOption( ID_BEZ_ENABLED,FP_BEZ_ENABLED,tr("Enable"),tr("Enable plugin"), FILTERNAME, false, false, 0);
		b &= m_hub->addBoolOption( ID_BEZ_EARLY,FP_BEZ_EARLY,tr("Early Mode"),tr("Run Bez just before LAB rather than before Curves"), FILTERNAME, false, false, 0);
		b &= m_hub->addBoolOption( ID_BEZ_HCL,FP_BEZ_HCL,tr("HCL Mode"),tr("Operate Bez in HCL colour space instead of HSL"), FILTERNAME, false, false, 0);
		b &= m_hub->addIntOption( ID_BEZ_OPACITY,FP_BEZ_OPACITY,tr("Opacity"),tr("Adjust the opacity of the overall impact of Bez"), FILTERNAME, 100, 0, 0);
        b &= m_hub->addIntOption( ID_BEZ_SATURATION,FP_BEZ_SATURATION,tr("Saturation"),tr("Vary the saturation: 0 = BW, 100 = original"), FILTERNAME, 100, 0, 0);
		b &= m_hub->addIntOption( ID_BEZ_LIGHTEN,FP_BEZ_LIGHTEN,tr("Lighten Sigmoidal"),tr("Raise the lightness sigmoidally"), FILTERNAME, 0, 0, 0);
		b &= m_hub->addIntOption( ID_BEZ_LIGHTENGAMMA,FP_BEZ_LIGHTENGAMMA,tr("Lighten Gamma"),tr("Raise the lightness with gamma"), FILTERNAME, 0, 0, 0);
		b &= m_hub->addIntOption( ID_BEZ_MIDI,FP_BEZ_MIDI,tr("Midtones"),tr("Adjust the midtones or average brightness"), FILTERNAME, 50, 0, 0);
		b &= m_hub->addIntOption( ID_BEZ_SIGCONTRAST,FP_BEZ_SIGCONTRAST,tr("Sigmoidal Contrast"),tr("Increase or decrease contrast sigmoidally"), FILTERNAME, 0, 0, 0);
		b &= m_hub->addIntOption( ID_BEZ_SIGMIDDLE,FP_BEZ_SIGMIDDLE,tr("Sigmoidal Middle Grey"),tr("The centre point for sigmoidal contrast"), FILTERNAME, 50, 0, 0 );
		b &= m_hub->addIntOption( ID_BEZ_SHADOWS,FP_BEZ_SHADOWS,tr("5 Tone Shadows"),tr("Set the 5 Tone Curve shadows point"), FILTERNAME, 10, 0, 0);
		b &= m_hub->addIntOption( ID_BEZ_LOWMIDS,FP_BEZ_LOWMIDS,tr("5 Tone Low Midtones"),tr("Set the 5 Tone Curve low midtones point"), FILTERNAME, 30, 0, 0);
		b &= m_hub->addIntOption( ID_BEZ_GREYS,FP_BEZ_GREYS,tr("5 Tone Grey tones"),tr("Set the 5 Tone Curve grey tones point"), FILTERNAME, 50, 0, 0);
		b &= m_hub->addIntOption( ID_BEZ_HIGHMIDS,FP_BEZ_HIGHMIDS,tr("5 Tone High Midtones"),tr("Set the 5 Tone Curve high midtones point"), FILTERNAME, 70, 0, 0);
		b &= m_hub->addIntOption( ID_BEZ_HIGHLIGHTS,FP_BEZ_HIGHLIGHTS,tr("5 Tone Highlights"),tr("Set the 5 Tone Curve highlights point"), FILTERNAME, 90, 0, 0);
		b &= m_hub->addIntOption( ID_BEZ_SHADOWS4,FP_BEZ_SHADOWS4,tr("4 Tone Shadows"),tr("Set the 4 Tone Curve shadows point"), FILTERNAME, 20, 0, 0);
		b &= m_hub->addIntOption( ID_BEZ_LOWMIDS4,FP_BEZ_LOWMIDS4,tr("4 Tone Low Midtones"),tr("Set the 4 Tone Curve low midtones point"), FILTERNAME, 40, 0, 0);
		b &= m_hub->addIntOption( ID_BEZ_HIGHMIDS4,FP_BEZ_HIGHMIDS4,tr("4 Tone High Midtones"),tr("Set the 4 Tone Curve high midtones point"), FILTERNAME, 60, 0, 0);
		b &= m_hub->addIntOption( ID_BEZ_HIGHLIGHTS4,FP_BEZ_HIGHLIGHTS4,tr("4 Tone Highlights"),tr("Set the 4 Tone Curve highlights point"), FILTERNAME, 80, 0, 0);

		// Legacy B5 parameters
		b &= m_hub->addBoolOption( ID_OLD_ENABLED,FP_OLD_ENABLED,tr("Old Enable"),tr("Old Enable plugin"), FILTERNAME, false, false, PluginHub::POPTION_NO_DISPLAY );  //PluginHub::POPTION_DOESNOTPERSIST = 8,  + PluginHub::POPTION_NO_DISPLAY = 16384 }
		b &= m_hub->addBoolOption( ID_OLD_RGB,FP_OLD_RGB,tr("Old RGB Mode"),tr("Old Run in RGB mode just before curves"), FILTERNAME, false, false, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addBoolOption( ID_OLD_BW,FP_OLD_BW,tr("Old B and W Mode"),tr("Old Convert to B and W using luminosity"), FILTERNAME, false, false, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addDoubleOption( ID_OLD_LIGHTEN,FP_OLD_LIGHTEN,tr("Old Lighten_Sig"),tr("Old Raise the lightness sigmoidally"), FILTERNAME, 0.0, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addDoubleOption( ID_OLD_LIGHTENGAMMA,FP_OLD_LIGHTENGAMMA,tr("Lighten_gamma"),tr("Raise the lightness with gamma"), FILTERNAME, 0.0, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addIntOption( ID_OLD_MIDI,FP_OLD_MIDI,tr("Old Midtones"),tr("Old Adjust the midtones"), FILTERNAME, 50, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addIntOption( ID_OLD_BLAK,FP_OLD_BLAK,tr("Old Blacks"),tr("Old Set the black clipping"), FILTERNAME, 0, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addDoubleOption( ID_OLD_SIGCONTRAST,FP_OLD_SIGCONTRAST,tr("Old Sigmoidal Contrast B5"),tr("Old The B5 Sigmoidal Contrast"), FILTERNAME, 0.0, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addDoubleOption( ID_OLD_SIGMIDDLE,FP_OLD_SIGMIDDLE,tr("Old Sigmoidal Middle Grey B5"),tr("Old The B5 Sigmoidal contrast centre"), FILTERNAME, 0.5, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addIntOption( ID_OLD_CONTRAST,FP_OLD_CONTRAST,tr("Old Contrast"),tr("Old Set the Bez contrast level"), FILTERNAME, 0, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addIntOption( ID_OLD_CENTRE,FP_OLD_CENTRE,tr("Old Centre"),tr("Old Set the Bez contrast centre"), FILTERNAME, 0, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addIntOption( ID_OLD_SHADOWS,FP_OLD_SHADOWS,tr("Old Shadows"),tr("Old Set the shadows curve 5 point"), FILTERNAME, 10, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addIntOption( ID_OLD_LOWMIDS,FP_OLD_LOWMIDS,tr("Old Low Midtones"),tr("Old Set the low midtones curve 5 point"), FILTERNAME, 30, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addIntOption( ID_OLD_GREYS,FP_OLD_GREYS,tr("Old Grey tones"),tr("Old Set the grey tones curve 5point"), FILTERNAME, 50, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addIntOption( ID_OLD_HIGHMIDS,FP_OLD_HIGHMIDS,tr("Old High Midtones"),tr("Old Set the high midtones curve 5 point"), FILTERNAME, 70, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addIntOption( ID_OLD_HIGHLIGHTS,FP_OLD_HIGHLIGHTS,tr("Old Highlights"),tr("Old Set the highlights curve 5 point"), FILTERNAME, 90, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addIntOption( ID_OLD_SHADOWS4,FP_OLD_SHADOWS4,tr("Old Shadows 4 zone"),tr("Old Set the shadows curve point 4 zone"), FILTERNAME, 20, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addIntOption( ID_OLD_LOWMIDS4,FP_OLD_LOWMIDS4,tr("Old Low Midtones 4 zone"),tr("Old Set the low midtones curve point 4 zone"), FILTERNAME, 40, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addIntOption( ID_OLD_HIGHMIDS4,FP_OLD_HIGHMIDS4,tr("Old High Midtones 4 zone"),tr("Old Set the high midtones curve point 4 zone"), FILTERNAME, 60, 0, PluginHub::POPTION_NO_DISPLAY );
		b &= m_hub->addIntOption( ID_OLD_HIGHLIGHTS4,FP_OLD_HIGHLIGHTS4,tr("Old Highlights 4 zone"),tr("Old Set the highlights curve point 4 zone"), FILTERNAME, 80, 0, PluginHub::POPTION_NO_DISPLAY );

        if ( b == false ) qDebug() << "BezPlugin: Unable to register options.";

        return b;
}


//-----------------------------------------------------------------------------
//	Handle settings change: if a control depends on an other (e.g. changing something enables the plugin)
void BezPlugin::handleSettingsChange(const PluginImageSettings &options, const PluginImageSettings &changed, int currentLayer)
{

    // Update our controls when settings are changed on the main layer
	if ((options.options(0) == NULL) || (changed.options(0) == NULL) || (currentLayer != 0))
		return;					// Only care about the main layer

}
//	Handle control change: if a control depends on an other (e.g. changing something enables the plugin)
void BezPlugin::handleControlChange( const QString &optionName, int groupId, int layer, const PluginImageSettings &options, const PluginOptionList &current, PluginOptionList &changes )
{

    //	some changes do not have options - check for NULL
    if ( options.options( layer ) == NULL )return;
    
    //	ignore changes that aren't for us
    if ( groupId != m_groupId ) return;
    
    bool bOk;
	    // if the plugin isn't enabled and a control is changed, enable the plugin
    if(!current.getBool(ID_BEZ_ENABLED,m_groupId,bOk)){

		if((optionName==FP_BEZ_EARLY)||(optionName==FP_BEZ_HCL)||(optionName==FP_BEZ_OPACITY)||(optionName==FP_BEZ_SATURATION)||(optionName==FP_BEZ_LIGHTEN)||(optionName==FP_BEZ_LIGHTENGAMMA)||(optionName==FP_BEZ_MIDI)||(optionName==FP_BEZ_SIGCONTRAST)||(optionName==FP_BEZ_SIGMIDDLE)||(optionName==FP_BEZ_SHADOWS)||(optionName==FP_BEZ_LOWMIDS)||(optionName==FP_BEZ_GREYS)||(optionName==FP_BEZ_HIGHMIDS)||(optionName==FP_BEZ_HIGHLIGHTS)||(optionName==FP_BEZ_SHADOWS4)||(optionName==FP_BEZ_LOWMIDS4)||(optionName==FP_BEZ_HIGHMIDS4)||(optionName==FP_BEZ_HIGHLIGHTS4))
		{
			changes.setBool(ID_BEZ_ENABLED,m_groupId,true);
		}
    }
}

//  Handle hotness change:  when we arrive on a new image
void BezPlugin::handleHotnessChanged(const PluginImageSettings &options)
{

    // Read B5 settings to prepare for import button press
    bool bOk;
    enabled_b5 = options.options(0)->getBool( ID_OLD_ENABLED, m_groupId, bOk );
    rgb_b5 = options.options(0)->getBool( ID_OLD_RGB, m_groupId, bOk );
    bw_b5 = options.options(0)->getBool( ID_OLD_BW, m_groupId, bOk );
    lighten_b5 = options.options(0)->getDouble( ID_OLD_LIGHTEN, m_groupId, bOk );
    lightengamma_b5 = options.options(0)->getDouble( ID_OLD_LIGHTENGAMMA, m_groupId, bOk );
    midi_b5 = options.options(0)->getInt( ID_OLD_MIDI, m_groupId, bOk );
    blak_b5 = options.options(0)->getInt( ID_OLD_BLAK, m_groupId, bOk );
    sigcontrast_b5 = options.options(0)->getDouble(ID_OLD_SIGCONTRAST, m_groupId, bOk );
    sigmiddle_b5 = options.options(0)->getDouble( ID_OLD_SIGMIDDLE, m_groupId, bOk );
    contrast_b5 = options.options(0)->getInt( ID_OLD_CONTRAST, m_groupId, bOk );
    centre_b5 = options.options(0)->getInt( ID_OLD_CENTRE, m_groupId, bOk );
    shadows_b5 = options.options(0)->getInt( ID_OLD_SHADOWS, m_groupId, bOk );
    lowmids_b5 = options.options(0)->getInt( ID_OLD_LOWMIDS, m_groupId, bOk );
    greys_b5 = options.options(0)->getInt( ID_OLD_GREYS, m_groupId, bOk );
    highmids_b5 = options.options(0)->getInt( ID_OLD_HIGHMIDS, m_groupId, bOk );
    highlights_b5 = options.options(0)->getInt( ID_OLD_HIGHLIGHTS, m_groupId, bOk );
    shadows4_b5 = options.options(0)->getInt( ID_OLD_SHADOWS4, m_groupId, bOk );
    lowmids4_b5 = options.options(0)->getInt( ID_OLD_LOWMIDS4, m_groupId, bOk );
    highmids4_b5 = options.options(0)->getInt( ID_OLD_HIGHMIDS4, m_groupId, bOk );
    highlights4_b5 = options.options(0)->getInt( ID_OLD_HIGHLIGHTS4, m_groupId, bOk );

}
//	Handle importB5
void BezPlugin::importB5() const
{
    if (PluginOptionList *options = m_hub->beginSettingsChange("B5 Import")) {

        //int midi_b5,blak_b5,shadows_b5,lowmids_b5,greys_b5,highmids_b5,highlights_b5,shadows4_b5,lowmids4_b5,highmids4_b5,highlights4_b5;
        //double lighten_b5,lightengamma_b5,sigcontrast_b5,sigmiddle_b5;
        //bool enable_b5, rgb_b5, bw_b5;

		options->setBool(ID_BEZ_ENABLED,m_groupId,enabled_b5);
		options->setBool(ID_BEZ_EARLY,m_groupId,!rgb_b5);
		if (bw_b5) options->setInt(ID_BEZ_SATURATION,m_groupId,0);
		options->setInt(ID_BEZ_LIGHTEN,m_groupId,(int)(lighten_b5*100.00f));
		options->setInt(ID_BEZ_LIGHTENGAMMA,m_groupId,int(lightengamma_b5*100.00f));
		options->setInt(ID_BEZ_MIDI,m_groupId,midi_b5);
		options->setInt(ID_BEZ_SIGCONTRAST,m_groupId,(int)(sigcontrast_b5*5.00f) );
		options->setInt(ID_BEZ_SIGMIDDLE,m_groupId,(int)(sigmiddle_b5*100.00f) );
		options->setInt(ID_BEZ_SHADOWS,m_groupId,shadows_b5);
		options->setInt(ID_BEZ_LOWMIDS,m_groupId,lowmids_b5);
		options->setInt(ID_BEZ_GREYS,m_groupId,greys_b5);
		options->setInt(ID_BEZ_HIGHMIDS,m_groupId,highmids_b5);
		options->setInt(ID_BEZ_HIGHLIGHTS,m_groupId,highlights_b5);
		options->setInt(ID_BEZ_SHADOWS4,m_groupId,shadows4_b5);
		options->setInt(ID_BEZ_LOWMIDS4,m_groupId,lowmids4_b5);
		options->setInt(ID_BEZ_HIGHMIDS4,m_groupId,highmids4_b5);
		options->setInt(ID_BEZ_HIGHLIGHTS4,m_groupId,highlights4_b5);

        m_hub->endSettingChange();
   }
}

//	About button
void BezPlugin::showAbout() const
{

    QMessageBox::about( m_pWindow, tr("About Bez"), tr("Bez Version 1.0.1 of 18 July 2012. Bezier curve-based tonal controls for AfterShotPro.\nWritten by Ferdinand Paris (F_P), based on a sample plugin by Barna Keresztes (kbarni) and colour conversion libraries by Dieter Steiner (spoilerhead).  The initial motivation was to provide more tonality controls, like the LR tone curve and brightness (mid-tone) tools, and it grew from there.\n\nYou can press the B5 BUTTON to import your B5 settings as a starting point.  The Bez RESET button won't wipe your B5 settings **but be warned** that a CTRL-R will!!!\n\nThe EARLY CHECKBOX will place Bez before the LAB stage in the pipe.  If this is unchecked then Bez will be placed just before the curves stage - like the former RGB option.\n\nThe OPACITY tool will reduce the overall effect of the plugin, but will not reduce the effect of the saturation slider if it is set to zero (on the assumption that you want a B&W conversion).  The SATURATION tool will change the saturation in HSL (or HCL) space without affecting luminosity.  All other sliders operate on the luminosity channel.  Therefore some of the dramatic increases in saturation that happened in the former RGB mode when changing tonality should not now occur, unless you overuse the saturation slider!\n\nThe LIGHTEN tools roughly simulate dragging the (192,192) point on the Aftershot Curves tool upwards. SIG LIGHTEN has been designed to have less impact in the shadows.  GAM LIGHTEN is more like dragging the curve point upward and so has more impact on the shadow regions. MIDTONES is intended to simulate dragging the (128,128) point upwards or downwards, so it is like brightness in LR.  The Blacks tool has gone since ASP has its own.\n\nSIG CONTRAST is from the Lab Contrast plugin by Thomas Baruchel, who implemented the concept based on http://www.cs.dartmouth.edu/~farid/tutorials/fip.pdf .  I have added to this by allowing for contrast reductions (using the inverse sigmoidal function, i.e. the logit function.)  The 5 TONE CURVE allows precise tonality changes which don't affect distant parts of the histogram, whereas the 4 TONE CURVE provides greater impact on non-adjacent zones.  Use the 4 zone version for large changes in tonality.  You can use both for a mix of wide and narrow changes.") ) ;

}

//	Reset button
void BezPlugin::resetControls() const
{
   if (PluginOptionList *options = m_hub->beginSettingsChange("Reset all controls")) {
			options->setBool(ID_BEZ_ENABLED,m_groupId,true);
			options->setBool(ID_BEZ_EARLY,m_groupId,false);
			options->setBool(ID_BEZ_HCL,m_groupId,false);
			options->setInt(ID_BEZ_OPACITY,m_groupId,100);
			options->setInt(ID_BEZ_SATURATION,m_groupId,100);
			options->setInt(ID_BEZ_LIGHTEN,m_groupId,0);
			options->setInt(ID_BEZ_LIGHTENGAMMA,m_groupId,0);
			options->setInt(ID_BEZ_MIDI,m_groupId,50);
			options->setInt(ID_BEZ_SIGCONTRAST,m_groupId,0);
			options->setInt(ID_BEZ_SIGMIDDLE,m_groupId,50);
			options->setInt(ID_BEZ_SHADOWS,m_groupId,10);
			options->setInt(ID_BEZ_LOWMIDS,m_groupId,30);
			options->setInt(ID_BEZ_GREYS,m_groupId,50);
			options->setInt(ID_BEZ_HIGHMIDS,m_groupId,70);
			options->setInt(ID_BEZ_HIGHLIGHTS,m_groupId,90);
			options->setInt(ID_BEZ_SHADOWS4,m_groupId,20);
			options->setInt(ID_BEZ_LOWMIDS4,m_groupId,40);
			options->setInt(ID_BEZ_HIGHMIDS4,m_groupId,60);
			options->setInt(ID_BEZ_HIGHLIGHTS4,m_groupId,80);

		m_hub->endSettingChange();
   }
} 

bool BezPlugin::finish()
{
	// Connect to settingsChanged so we can keep our custom controls in sync
	connect(m_hub, SIGNAL(settingsChanged(const PluginImageSettings &, const PluginImageSettings &, int)), this, SLOT(handleSettingsChange(const PluginImageSettings &, const PluginImageSettings &, int)));

    // Connect to controlChanged so we can do some settings dependency logic
    connect( m_hub, SIGNAL( controlChanged( const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &) ), SLOT( handleControlChange( const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList & ) ) );

	// Connect to hotnessChanged so we can setup our custom controls when the active version changes
	connect(m_hub, SIGNAL(hotnessChanged(const PluginImageSettings &)), this, SLOT(handleHotnessChanged(const PluginImageSettings &)));

    return true;
}

PluginDependency *BezPlugin::createDependency(const QString &depName)
{
	// Q_UNUSED(name);
    if (depName == "ToolData") {
        ToolData *toolData = new ToolData();
        if (toolData) {
            toolData->owner = name();
            toolData->group = group();
            toolData->ownerId = m_id;
            toolData->groupId = m_groupId;
            toolData->addEnabledId(m_hub->optionIdForName("bez_enabled",m_id), "On", "Enable Bez", "This indicates whether Bez is enabled");
            qDebug() << "asPluginManager: createDependency ToolData" <<
            toolData;
        return toolData;
        }
    }
	return NULL;
}

QList<QString> BezPlugin::toolFiles()
{
	return QList<QString>();
}

QList<QWidget*> BezPlugin::toolWidgets()
{
	return QList<QWidget*>();
}

//	Connect the About button to the showAbout() function
void BezPlugin::toolWidgetCreated(QWidget *uiWidget)
{
	if (uiWidget == NULL) return;
	if ( QPushButton * pimportB5 = uiWidget->findChild<QPushButton*>("bez_importb5_button"))
            connect( pimportB5, SIGNAL( clicked() ), SLOT( importB5() ) );
    else qDebug() << "Bez: Import B5 button problem.";
	if ( QPushButton * pReset = uiWidget->findChild<QPushButton*>("bez_reset_button"))
            connect( pReset, SIGNAL( clicked() ), SLOT( resetControls() ) );
    else qDebug() << "Bez: Reset button problem.";
    if ( QPushButton * pAbout = uiWidget->findChild<QPushButton*>("bez_about_button"))
        connect( pAbout, SIGNAL( clicked() ), SLOT( showAbout() ) );
    else qDebug() << "Bez: About button problem.";
        m_pWindow = uiWidget->window();
}
