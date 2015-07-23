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

#ifndef LABCURVES_FILTER_BEFORE_H
#define LABCURVES_FILTER_BEFORE_H

#include "PluginLayerFilter.h"
#include "PluginHub.h"

#define FILTER_NAME_BEFORE    "LabCurvesFilterBefore"


// Clipping for floating point values (force x to be in [y,z])
#define CLIPF(x,y,z)  ( (x) <= (y) ? (y) : ( (x) >= (z) ? (z) : (x) ) )

// Convert 16 bits integer values to float, and float values to 16 bits integers
#define INT16_TO_FLOAT(i) ( (i) / 65535.0)
#define FLOAT_TO_INT16(f) ( CLIPF( (f) * 65535.0, 0, 65535) )


// Fast cubic root
// Polynomial rational fraction approximation
// Only valid for x in [0.008856,1.0]
static inline float fastcbrt(float x)
{
	float p, q;
	
	p = 3.409863594e-6+x*(2.45173547878e-3+x*(1.76492229565065e-1+x*(2.317873405508336+x*(5.913913851608542+x*2.116767478179906))));
	q = 4.3398904568e-5+x*(1.2547674891283e-2+x*(4.98306523961242e-1+x*(3.821346668873898+x*(5.409646088113729+x*7.85621180224256e-1))));
	
	return (p/q);
}


// RGB <-> LAB conversion (from Jacques Desmis work)
#define FORC(cnt) for (c=0; c < cnt; c++)
#define FORC3 FORC(3)

// Input data r, g, b must be in [0,1]
// Output data are in [0,100] (L channel), and approximately in [-187,145] (a channel), [-172,172] (b channel)
static inline void rgb2lab(float *r, float *g, float *b)
{
    int c;
    float t, xyz[3];

    static float ep=216.0/24389.0;
    static float ka=24389.0/27.0;

    static float d50_white[3]= {0.964220,1,0.825211};
    static const float rgb_xyz[3][3] =
    {
        { 0.7976748465, 0.1351917082, 0.0313534088 },
        { 0.2880402025, 0.7118741325, 0.0000856651 },
        { 0.0000000000, 0.0000000000, 0.8252114389 }
    }; // From Jacques Desmis

    // Convert RGB to XYZ
    xyz[0]=xyz[1]=xyz[2]=0;
    FORC3
    {
        xyz[c]+=rgb_xyz[c][0]* (*r);
        xyz[c]+=rgb_xyz[c][1]* (*g);
        xyz[c]+=rgb_xyz[c][2]* (*b);
    }

    // Convert XYZ to L*a*b*
    FORC3
    {
		t=xyz[c]/d50_white[c];
    	xyz[c]=t>ep?fastcbrt(t):(ka*t+16)/116.0;
	}

    (*r)=116.0*xyz[1]-16.0;
    (*g)=500.0*(xyz[0]-xyz[1]);;
    (*b)=200.0*(xyz[1]-xyz[2]);
}




class LabCurvesFilterBefore : public PluginLayerFilter
{
public:

    // Constructor
    LabCurvesFilterBefore(PluginHub *hub, int groupId) : m_hub(hub), m_groupId(groupId)
    {
        ;
    }

    //	The option IDs must be unique for each plugin group
    enum OptionNames { Enable=1, Scale=2 };

    // Filter name
    virtual QString name() const
    {
        return QString(FILTER_NAME_BEFORE);
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
