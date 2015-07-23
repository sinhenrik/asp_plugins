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

#ifndef LABCURVES_FILTER_AFTER_H
#define LABCURVES_FILTER_AFTER_H

#include "PluginLayerFilter.h"
#include "PluginHub.h"

#define FILTER_NAME_AFTER    "LabCurvesFilterAfter"


// Clipping for floating point values (force x to be in [y,z])
#define CLIPF(x,y,z)  ( (x) <= (y) ? (y) : ( (x) >= (z) ? (z) : (x) ) )

// Convert 16 bits integer values to float, and float values to 16 bits integers
#define INT16_TO_FLOAT(i) ( (i) / 65535.0)
#define FLOAT_TO_INT16(f) ( CLIPF( (f) * 65535.0, 0, 65535) )


// RGB <-> LAB conversion (from Jacques Desmis work)
#define FORC(cnt) for (c=0; c < cnt; c++)
#define FORC3 FORC(3)

// Input data are in [0,100] (L channel), and approximately in [-187,145] (a channel), [-172,172] (b channel)
// Output data r, g, b are in [0,1]
static inline void lab2rgb(float *l, float *a, float *b)
{
    int c;
    float L;
    float xyz[3],f[3];
    static float ep=216.0/24389.0;
    static float ka=24389.0/27.0;
    static float d50_white[3]= {0.964220,1,0.825211};
    static double xyz_rgb[3][3] =
    {
        { 1.3459434662, -0.5445988425, 0.0000000000 },
        { -0.2556075408, 1.5081672430, 0.0000000000 },
        { -0.0511117722, 0.0205351405, 1.2118106377 }
    }; // From Jacques Desmis

    // Convert L*a*b* to XYZ
    L=(*l);
    f[1]=(L+16.0)/116.0;  // fy
    f[0]=f[1]+(*a)/500.0; // fx
    f[2]=f[1]-(*b)/200.0; // fz

    xyz[0]=d50_white[0]*(f[0]*f[0]*f[0]>ep?f[0]*f[0]*f[0]:(116.0*f[0]-16.0)/ka);
    xyz[1]=d50_white[1]*(L>ka*ep?f[1]*f[1]*f[1]:L/ka);
    xyz[2]=d50_white[2]*(f[2]*f[2]*f[2]>ep?f[2]*f[2]*f[2]:(116.0*f[2]-16.0)/ka);

    // Convert XYZ to RGB
    (*l)=(*a)=(*b)=0;
    FORC3
    {
        (*l)+=xyz[c]*xyz_rgb[c][0];
        (*a)+=xyz[c]*xyz_rgb[c][1];
        (*b)+=xyz[c]*xyz_rgb[c][2];
    }
}



class LabCurvesFilterAfter : public PluginLayerFilter
{
public:

    // Constructor
    LabCurvesFilterAfter(PluginHub *hub, int groupId) : m_hub(hub), m_groupId(groupId)
    {
        ;
    }

    //	The option IDs must be unique for each plugin group
    enum OptionNames { Enable=1, Scale=2 };

    // Filter name
    virtual QString name() const
    {
        return QString(FILTER_NAME_AFTER);
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
