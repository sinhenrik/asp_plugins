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

// Plugin includes
#include "AutoLevelsData.h"
#include "AutoLevelsFilter.h"

// Bibble includes
#include "PluginHub.h"
#include "PluginImageSettings.h"
#include "PluginPipeSettings.h"
#include "PluginTile.h"
#include "PluginImage.h"
#include "srgblut.h"

#include <QDebug>


// MIP level (i.e. zoom level) used to compute the image statistics
// MIP level = 0 => full image
// MIP level = 1 => image / 4
// MIP level = 2 => image / 16
// MIP level = 3 => image / 64
// MIP level = 4 => image / 256
#define USEMIP 4


QList<PluginDependency*> AutoLevelsData::prerequisites(const PluginImageSettings &options, const PluginPipeSettings &settings) const
{
    Q_UNUSED(options);

    // Choose the smallist mip level and return all tiles in it
    int srcMip = USEMIP;
    int srcStep = m_hub->step(FILTER_NAME) - 1;
    QRect srcRect = QRect(QPoint(0, 0), settings.size(srcStep, srcMip));

    return settings.tilesIn(srcRect, srcStep, srcMip);
}


// Compute the image histogram (256 levels) and the histogram transformation
// We use the SRGB transformation instead of a gamma curve because the histogram displayed in ASP seems to be in SRGB space
PluginData::Status AutoLevelsData::run(const PluginImageSettings &options, const PluginPipeSettings &settings)
{
    // Read the min level and max level percentages
    bool okay;
    PluginOptionList* opt = options.options(options.currentLayer());
    float minlevel = (float) opt->getDouble(AutoLevelsFilter::MinLevel, m_groupId, okay) /100.0;
    float maxlevel = (float) opt->getDouble(AutoLevelsFilter::MaxLevel, m_groupId, okay) /100.0;

    // Histogram array
    unsigned int hist[256]= {0};

    // Min and max levels obtained from the histogram
    quint16 Lmin, Lmax;

    // Walk all the tiles (at the mip level given by USEMIP)
    float count=0;
    int srcMip = USEMIP;
    int srcStep = m_hub->step(FILTER_NAME) - 1;
    QSize srcSize = settings.size(srcStep, srcMip);
    QSize tileSize = settings.tileSize();

    int cols = (srcSize.width() + tileSize.width() - 1) / tileSize.width();
    int rows = (srcSize.height() + tileSize.height() - 1) / tileSize.height();

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (PluginTile *tile = dynamic_cast<PluginTile*>(settings.tileAt(j, i, srcStep, srcMip)))
            {
                if (PluginImage *image = tile->image())
                {
                    int width = tile->activeSize().width();
                    int height = tile->activeSize().height();

                    // Walk over the 16 bit planer data
                    for (int y = 0; y < height; ++y)
                    {
                        quint16 *rScan = image->scanLine16(y);
                        quint16 *gScan = rScan + image->planeWords();
                        quint16 *bScan = gScan + image->planeWords();

                        for (int x = 0; x < width; ++x)
                        {
                            // RGB values in linear space
                            quint16 r = (*rScan++);
                            quint16 g = (*gScan++);
                            quint16 b = (*bScan++);

                            // Conversion to SRGB space
                            quint16 R = srgblut[r];
                            quint16 G = srgblut[g];
                            quint16 B = srgblut[b];

                            // Luminance value in [0,255]
                            float L = 0.3*R+0.59*G+0.11*B;
                            L=CLIPF(L,0.0,65535.0);
                            quint8 i = (quint8)(L/257.0);

                            // Add to the histogram
                            hist[i]++;
                        }
                    }
                    count += width * height;
                }
            }
        }
    }

    // Compute Lmin and Lmax according to the percentages values
    // We use the cumulative histogram to find Lmin and Lmax
    int i;
    float lcum=0.0;
    for (i=0; i<=255; i++)
    {
        lcum += hist[i];
        if ( lcum/count >= minlevel)
            break;
    }
    Lmin=257*(i--);

    float rcum=0.0;
    for (i=255; i>=0; i--)
    {
        rcum += hist[i];
        if ( rcum/count >= maxlevel)
            break;
    }
    Lmax=257*(i++);

    // Compute the histogram transformation as a lookup table
    // It is computed in linear space, so that in SRGB space the histogram looks good
    float K = 65535.0/(Lmax-Lmin);
    quint16 invLmin = invsrgblut[Lmin];
    quint16 invLmax = invsrgblut[Lmax];

    // Compute the histogram transform in linear space
    for (int i=0; i<=65535; i++)
    {
        if (i <= invLmin)
            histlut[i]=0;

        else if (i >= invLmax)
            histlut[i]=65535.0;

        else
        {
            float t = K*(srgblut[i]-Lmin);
            quint16 j = (quint16)(CLIPF(t,0.0,65535.0));
            histlut[i] = invsrgblut[j];
        }
    }

    return Complete;
}


bool AutoLevelsData::requiresDisk() const
{
    return false;
}


QString AutoLevelsData::filterName() const
{
    return QString(FILTER_NAME);
}
