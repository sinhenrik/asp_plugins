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

#ifndef AUTOLEVELS_FILTER_H
#define AUTOLEVELS_FILTER_H


#include "PluginLayerFilter.h"
#include "PluginHub.h"

#define FILTER_NAME 	"AutoLevelsFilter"


class AutoLevelsFilter : public PluginLayerFilter
{
public:

    // Constructor
    AutoLevelsFilter(PluginHub *hub, int groupId) : m_hub(hub), m_groupId(groupId)
    {
        ;
    }

    //	The option IDs must be unique for each plugin group
    enum OptionNames { Enable=1, MinLevel=2, MaxLevel=3 };

    // Filter name
    virtual QString name() const
    {
        return QString(FILTER_NAME);
    }

    virtual bool needsToRunLayer(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginOptionList &layerOptions) const;
    virtual void runLayer(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginTile &tile, PluginOptionList &layerOptions, int layerPos) const;
    virtual bool needsOriginalImage()
    {
        return false;
    };

    virtual bool isSlow(const PluginImageSettings &options, const PluginPipeSettings &settings) const;
    virtual bool isLargeRadius(const PluginImageSettings &options, const PluginPipeSettings &settings) const;
    virtual bool isPixelSource(const PluginImageSettings &options, const PluginPipeSettings &settings) const;
    virtual QTransform transform(const PluginImageSettings &options, const PluginPipeSettings &settings) const;
    virtual QSize size(const PluginImageSettings &options, const PluginPipeSettings &settings, const QSize &startSize) const;
    virtual QList<PluginDependency*> prerequisites(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginTile &tile) const;

private:
    PluginHub *m_hub;
    int m_groupId;
};

#endif
