/*
 * by Dieter Steiner <spoilerhead@gmail.com>
 * (C) 2010
 * licensed: GPL v2+
 */
#ifndef SPHFILTERLLORENS_H
#define SPHFILTERLLORENS_H

#include "sphLayerFilter.h"

class sphLLorensFilter : public sphLayerFilter
{
public:
    sphLLorensFilter(PluginHub *hub, int groupId) :sphLayerFilter(hub,groupId,RGB2HCL,GAMMA_ALL){ } ;
	//virtual ~SharpenFilter() { ; }

	enum OptionNames {
	                EnableLL=30,LLStrength=31,LLIter=32,MCStrength=33,
	                };
	virtual QString name() const { return QString("WaveletSharpen2_LLorens_Filter"); }

	virtual bool needsToRunLayer(const ImageSettings &options, const PipeSettings  &settings, PluginOptionList  &layerOptions) const;
	virtual bool isSlow(const ImageSettings &options, const PipeSettings  &settings) const;
	virtual bool needsOriginalImage  ();

	virtual int additionalRadius(PluginOptionList  *layerOptions,float zoomLevel) const;
	virtual void ProcessBuffer(float *fimg[3],int width,int height,float zoomLevel,const ImageSettings &options, const PipeSettings  &settings, PluginTile &tile, PluginOptionList  &layerOptions, int layerPos) const;
};


#endif
