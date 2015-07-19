#include "SharpenPlugin.h"

#include "FilterSharpen.h"
#include "FilterUSM.h"
#include "FilterLLorens.h"
#include "PluginHub.h"
#include "PluginDependency.h"

#include "ToolData.h"

#include <QDebug>
#include <iostream>
#include <QSettings>

#define PLUGIN_NAME_HR "Wavelet Sharpen 3"

extern "C" BIBBLE_API BaseB5Plugin *b5plugin() { return new SharpenPlugin; }


bool SharpenPlugin::init(PluginHub *hub, int id, int groupId, const QString &bundlePath)
{
    Q_UNUSED(bundlePath);
    qDebug() << "Initialising "<<PLUGIN_NAME_HR<<" filter";
	m_hub = hub;
	m_id = id;
	m_groupId = groupId;
	
	// Store our group ID and group name (andrewj)
    QSettings oSettings("Bibble and AfterShot Plugins", "PluginGroups");
    oSettings.setValue(group(), groupId);
	return true;
}

bool SharpenPlugin::registerFilters()
{
    sphLLorensFilter *llorens = new sphLLorensFilter(m_hub, m_groupId);
	//if (m_hub->addFilter(llorens, PluginHub::After, QString("RgbToLab"))) {
		if (m_hub->addFilter(llorens, PluginHub::Before, QString("Sharpen"))) {
        qDebug() << "Successfully registered "<<llorens->name()<<" filter";
	} else {
    	qDebug() << "Failed to register "<<llorens->name()<<" filter";
	    return false;
	}


    sphUSMFilter *usm = new sphUSMFilter(m_hub, m_groupId);
	if (m_hub->addFilter(usm, PluginHub::Before, QString("Sharpen"))) {
        qDebug() << "Successfully registered "<<usm->name()<<" filter";
	} else {
    	qDebug() << "Failed to register "<<usm->name()<<" filter";
	    return false;
	}
	
	
    sphWaveletSharpenFilter *sharpener = new sphWaveletSharpenFilter(m_hub, m_groupId);
	if (m_hub->addFilter(sharpener, PluginHub::After, usm->name())) {
        qDebug() << "Successfully registered "<<sharpener->name()<<" filter";
	} else {
    	qDebug() << "Failed to register "<<sharpener->name()<<" filter";
	    return false;
	}
	
	qDebug() << "All "<<PLUGIN_NAME_HR<<" Filters enabled";
	return true;
}

bool SharpenPlugin::registerOptions()
{

    m_hub->addBoolOption  (sphUSMFilter::EnableUSM, "bSphWaveletUsmon"            , "Usm Enabled"  , tr("Enables Usm"), "WaveletSharpen2_USM_Filter", false, false, 0);
    m_hub->addIntOption   (sphUSMFilter::RadiusUSM, "bSphWaveletUsmRadius"        , "Usm Radius"   , tr("Increases the Radius of the Blur filter"), "WaveletSharpen2_USM_Filter", 50, 0, 0);
    m_hub->addIntOption   (sphUSMFilter::AmountUSM, "bSphWaveletUsmAmount"        , "Usm Amount"   , tr("Amount"), "WaveletSharpen2_USM_Filter", 100, 0, 0);
    m_hub->addIntOption   (sphUSMFilter::ThresholdUSM, "bSphWaveletUsmThreshold"  , "Usm Threshold", tr("Define Minimum Edge for sharpening"), "WaveletSharpen2_USM_Filter", 0, 0, 0);
    m_hub->addBoolOption  (sphUSMFilter::USMasClarity, "bSphWaveletUsmClarity"    , "Clarity Mode" , tr("Makes usm work as clarity control"), "WaveletSharpen2_USM_Filter", false, false, 0);
    
    
    m_hub->addBoolOption  (sphLLorensFilter::EnableLL, "bSphWaveletLLenable"            , "Gradient Enabled"  , tr("Enables Gradient Sharpening"), "WaveletSharpen2_LLorens_Filter", false, false, 0);
    m_hub->addIntOption   (sphLLorensFilter::LLStrength, "bSphWaveletLLStrength"        , "Gradient Strength" , tr("Gradient Sharpening Strength"), "WaveletSharpen2_LLorens_Filter", 30, 0, 0);
    m_hub->addIntOption   (sphLLorensFilter::LLIter, "bSphWaveletLLiter"                , "Gradient Width"    , tr("Width of the Gradient (slow)"), "WaveletSharpen2_LLorens_Filter", 3, 1, 0);
    m_hub->addIntOption   (sphLLorensFilter::MCStrength, "bSphWaveletMCStrength"        , "Microcontrast"      , tr("Amount of Microcontrast to be added"), "WaveletSharpen2_LLorens_Filter", 0, 0, 0);
    
    

    m_hub->addBoolOption  (sphWaveletSharpenFilter::Enable,   "bSphWaveleton"              , "Sharpen 1 Enable",  tr("Enable Filter"), "WaveletSharpen2_Wavelet_Filter", false, false, 0);
    m_hub->addIntOption   (sphWaveletSharpenFilter::Amount,   "bSphWaveletAmount"          , "Sharpen 1 Amount",  tr("Sharpening Strength"), "WaveletSharpen2_Wavelet_Filter", 50, 0, 0);
    m_hub->addDoubleOption(sphWaveletSharpenFilter::Radius,   "bSphWaveletRadius"          , "Sharpen 1 Radius",  tr("Sharpening Radius"), "WaveletSharpen2_Wavelet_Filter", 0.5, 0, 0);
//	m_hub->addIntOption   (sphWaveletSharpenFilter::Blend,    "bSphWaveletBlend"           , "Sharpen 1 Blend",   "Blend", "WaveletSharpen2_Wavelet_Filter", 100, 0, 0);
    m_hub->addBoolOption  (sphWaveletSharpenFilter::DePepper, "bSphWaveletSharpenDePepper" , "Sharpen 1 DePepper",tr("Salt and Pepper noise removal (slow)"), "WaveletSharpen2_Wavelet_Filter", false, false, 0);
    m_hub->addIntOption  (sphWaveletSharpenFilter::Edges,    "bSphWaveletEdge"     , "Sharpen 1 Edge",    tr("Limit Sharpening to Edges"), "WaveletSharpen2_Wavelet_Filter", 100, 0, 0);
    m_hub->addBoolOption  (sphWaveletSharpenFilter::Clarity,   "bSphWaveletSharpenClarity" , "Sharpen 1 Clarity",  tr("Apply with clarity mask"), "WaveletSharpen2_Wavelet_Filter", false, false, 0);
	
	
	
	
    m_hub->addBoolOption  (sphWaveletSharpenFilter::Enable2,   "bSphWaveleton2"              , "Sharpen 2 Enable",  tr("Enable Filter"), "WaveletSharpen2_Wavelet_Filter", false, false, 0);
    m_hub->addIntOption   (sphWaveletSharpenFilter::Amount2,   "bSphWaveletAmount2"          , "Sharpen 2 Amount",  tr("Sharpening Strength"), "WaveletSharpen2_Wavelet_Filter", 50, 0, 0);
    m_hub->addDoubleOption(sphWaveletSharpenFilter::Radius2,   "bSphWaveletRadius2"          , "Sharpen 2 Radius",  tr("Sharpening Radius"), "WaveletSharpen2_Wavelet_Filter", 2.0, 0, 0);
//	m_hub->addIntOption   (sphWaveletSharpenFilter::Blend2,    "bSphWaveletBlend2"           , "Sharpen 2 Blend",   "Blend", "WaveletSharpen2_Wavelet_Filter", 100, 0, 0);
    m_hub->addBoolOption  (sphWaveletSharpenFilter::DePepper2, "bSphWaveletSharpenDePepper2" , "Sharpen 2 DePepper", tr("Salt and Pepper noise removal (slow)"), "WaveletSharpen2_Wavelet_Filter", false, false, 0);
    m_hub->addIntOption  (sphWaveletSharpenFilter::Edges2,    "bSphWaveletEdge2"     , "Sharpen 2 Edge",    tr("Limit Sharpening to Edges"), "WaveletSharpen2_Wavelet_Filter", 100, 0, 0);
    m_hub->addBoolOption  (sphWaveletSharpenFilter::Clarity2,   "bSphWaveletSharpenClarity2" , "Sharpen 2 Clarity",  tr("Apply with clarity mask"), "WaveletSharpen2_Wavelet_Filter", false, false, 0);
    
    /*m_hub->addIntOption   (SharpenFilter::EAWAlpha, "bSphWaveletEAWAlpha"        , "EAWAlpha", "Alpha", "WaveletSharpen2_Wavelet_Filter", 0, 0, 0);
    m_hub->addIntOption   (SharpenFilter::EAWMaxband, "bSphWaveletEAWMaxband"    , "EAWMaxband", "Maxband", "WaveletSharpen2_Wavelet_Filter", 10, 0, 0);
    m_hub->addIntOption   (SharpenFilter::EAWInband, "bSphWaveletEAWInband"     , "EAWInband", "Inband", "WaveletSharpen2_Wavelet_Filter", 5, 0, 0);
    m_hub->addIntOption   (SharpenFilter::EAWMode, "bSphWaveletEAWMode"          , "EAWMode", "Mode", "WaveletSharpen2_Wavelet_Filter", 0, 0, 0);    */
		


	return true;
}

bool SharpenPlugin::finish()
{
	connect(m_hub, SIGNAL(settingsChanged(const PluginImageSettings &, const PluginImageSettings &, int)), this, SLOT(handleSettingsChange(const PluginImageSettings &, const PluginImageSettings &, int)));
	connect(m_hub, SIGNAL(controlChanged(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)), this, SLOT(handleControlChange(const QString &, int, int , const PluginImageSettings &, const PluginOptionList &, PluginOptionList &)));
   	connect(m_hub, SIGNAL(hotnessChanged(const PluginImageSettings &)), this, SLOT(handleHotnessChanged(const PluginImageSettings &)));
	return true;
}

PluginDependency *SharpenPlugin::createDependency(const QString &dname)
{
    qDebug()<<PLUGIN_NAME_HR<<" Dependency requested";
    if (dname == "ToolData") {        //For asPluginupdate
        ToolData *toolData = new ToolData();
        if (toolData) {
            toolData->owner = this->name();
            toolData->group = this->group();
            toolData->ownerId = m_id;
            toolData->groupId = m_groupId;
            toolData->addEnabledId(m_hub->optionIdForName("bSphWaveleton", m_id),"WS1","Sharpen 1");
            toolData->addEnabledId(m_hub->optionIdForName("bSphWaveletUsmon", m_id),"USM","USM");
            toolData->addEnabledId(m_hub->optionIdForName("bSphWaveleton2", m_id),"WS2","Sharpen 2");
            toolData->addEnabledId(m_hub->optionIdForName("bSphWaveletLLenable", m_id),"Grad","Gradient Sharpen");
            return toolData;
        }
    }
   return NULL;
}

QList<QString> SharpenPlugin::toolFiles()
{
	return QList<QString>();
}

QList<QWidget*> SharpenPlugin::toolWidgets()
{
	return QList<QWidget*>();
}

void SharpenPlugin::toolWidgetCreated(QWidget *uiWidget)
{
    Q_UNUSED(uiWidget);
}

//========================================================================================================
void SharpenPlugin::handleSettingsChange(const PluginImageSettings &options, const PluginImageSettings &changed, int currentLayer)
{
    Q_UNUSED(options);
    Q_UNUSED(changed);
    Q_UNUSED(currentLayer);
    
    if(options.options(currentLayer) == NULL) return;
}

void SharpenPlugin::handleControlChange(const QString &optionName, int groupId, int layer, const PluginImageSettings &options, const PluginOptionList &current, PluginOptionList &changes)
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
    // USM settings changes   
	if(
        changes.contains(sphUSMFilter::RadiusUSM, m_groupId)
        || changes.contains(sphUSMFilter::AmountUSM, m_groupId)
        || changes.contains(sphUSMFilter::ThresholdUSM, m_groupId)
        || changes.contains(sphUSMFilter::USMasClarity, m_groupId)
	){
        changes.setBool(sphUSMFilter::EnableUSM, m_groupId, true);
	}
	
	// Sharpen1 settings changes   
	if(
        changes.contains(sphWaveletSharpenFilter::Amount, m_groupId)
        || changes.contains(sphWaveletSharpenFilter::Radius, m_groupId)
        || changes.contains(sphWaveletSharpenFilter::Clarity, m_groupId)
        || changes.contains(sphWaveletSharpenFilter::DePepper, m_groupId)
        || changes.contains(sphWaveletSharpenFilter::Edges, m_groupId)
	){
        changes.setBool(sphWaveletSharpenFilter::Enable, m_groupId, true);
	}
	
	// Sharpen2 settings changes   
	if(
        changes.contains(sphWaveletSharpenFilter::Amount2, m_groupId)
        || changes.contains(sphWaveletSharpenFilter::Radius2, m_groupId)
        || changes.contains(sphWaveletSharpenFilter::Clarity2, m_groupId)
        || changes.contains(sphWaveletSharpenFilter::DePepper2, m_groupId)
        || changes.contains(sphWaveletSharpenFilter::Edges2, m_groupId)
	){
        changes.setBool(sphWaveletSharpenFilter::Enable2, m_groupId, true);
	}
	
	// LLorens (Gradient) settings changes   
	if(
        changes.contains(sphLLorensFilter::LLStrength, m_groupId)
        || changes.contains(sphLLorensFilter::LLIter, m_groupId)
        || changes.contains(sphLLorensFilter::MCStrength, m_groupId)
	){
        changes.setBool(sphLLorensFilter::EnableLL, m_groupId, true);
	}
}

//========================================================================================================
void SharpenPlugin::handleHotnessChanged(const PluginImageSettings &options)
{
    Q_UNUSED(options);
}

