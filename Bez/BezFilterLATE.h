#pragma once

#include "PluginLayerFilter.h"
#include "PluginHub.h"

#define FILTERNAMELATE     "BezFilterLATE"

typedef unsigned short uint16; 

// This is a filter class
// Probably you don't have to change much here...

class BezFilterLATE : public PluginLayerFilter
{
public:
	BezFilterLATE(PluginHub *hub, int groupId) : m_hub(hub), m_groupId(groupId) { ; }

	virtual QString name() const { return QString(FILTERNAMELATE); }

	virtual bool needsToRunLayer(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginOptionList &layerOptions) const;
	virtual void runLayer(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginTile &tile, PluginOptionList &layerOptions, int layerPos) const;
	virtual bool needsOriginalImage() { return false; }

	virtual bool isSlow(const PluginImageSettings &, const PluginPipeSettings &) const { return false; }
	virtual bool isLargeRadius(const PluginImageSettings &, const PluginPipeSettings &) const { return false; }
	virtual bool isPixelSource(const PluginImageSettings &, const PluginPipeSettings &) const { return false; }
	virtual QTransform transform(const PluginImageSettings &options, const PluginPipeSettings &settings) const;
	virtual QSize size(const PluginImageSettings &options, const PluginPipeSettings &settings, const QSize &startSize) const;
	virtual QList<PluginDependency*> prerequisites(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginTile &tile) const;

private:
	PluginHub *m_hub;
	int m_groupId;
};
