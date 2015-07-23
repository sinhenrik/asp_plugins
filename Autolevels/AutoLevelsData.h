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

#ifndef AUTOLEVELS_DATA_H
#define AUTOLEVELS_DATA_H

#include "PluginData.h"
#include "PluginOptionList.h"


#define PLUGINDATA_NAME "AutoLevelsData"

// Clipping for floating point values (force x to be in [y,z])
#define CLIPF(x,y,z)  ( (x) <= (y) ? (y) : ( (x) >= (z) ? (z) : (x) ) )


class AutoLevelsData : public PluginData
{
public:

    // Constructor
    AutoLevelsData(PluginHub *hub, int groupId): m_hub(hub), m_groupId(groupId)
    {
    }

    virtual QList<PluginDependency*> prerequisites(const PluginImageSettings &options, const PluginPipeSettings &settings) const;
    virtual Status run(const PluginImageSettings &options, const PluginPipeSettings &settings);
    virtual bool requiresDisk() const;
    virtual QString filterName() const;

    // Lookup table for the histogram transformation
    float* LUT()
    {
        return histlut;
    }


private:
    PluginHub *m_hub;
    float histlut[65536];
    int m_groupId;
};

#endif
