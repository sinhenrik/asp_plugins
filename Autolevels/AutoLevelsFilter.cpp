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
#include "AutoLevelsPlugin.h"

#include "PluginPipeSettings.h"
#include "PluginTile.h"
#include "PluginImage.h"
#include "PluginImageSettings.h"
#include "PluginOptionList.h"

#include <math.h>

#include <QDebug>
#include <iostream>

using namespace std;


bool AutoLevelsFilter::needsToRunLayer(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginOptionList &layerOptions) const
{
    Q_UNUSED( options );
    Q_UNUSED( settings );

    bool okay;
    bool val = layerOptions.getBool(Enable, m_groupId, okay);
    return val;
}

bool AutoLevelsFilter::isSlow(const PluginImageSettings &options, const PluginPipeSettings &settings) const
{
    Q_UNUSED( options );
    Q_UNUSED( settings );

    return false;
}

bool AutoLevelsFilter::isLargeRadius(const PluginImageSettings &options, const PluginPipeSettings &settings) const
{
    Q_UNUSED( options );
    Q_UNUSED( settings );

    return false;	//	We only use the current tile.
}

bool AutoLevelsFilter::isPixelSource(const PluginImageSettings &options, const PluginPipeSettings &settings) const
{
    Q_UNUSED( options );
    Q_UNUSED( settings );

    return false;	//	We don't generate pixels from scratch.
}

QTransform AutoLevelsFilter::transform(const PluginImageSettings &options, const PluginPipeSettings &settings) const
{
    Q_UNUSED( options );
    Q_UNUSED( settings );

    return QTransform();	//	We don't warp the image so return an identity transform.
}

QSize AutoLevelsFilter::size(const PluginImageSettings &options, const PluginPipeSettings &settings, const QSize &startSize) const
{
    Q_UNUSED( options );
    Q_UNUSED( settings );

    return startSize;	//	We don't change the size of the image.
}



// Prerequisites
QList<PluginDependency*> AutoLevelsFilter::prerequisites(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginTile &tile) const
{
    Q_UNUSED( options );

    int srcStep = m_hub->step(this) - 1;
    QSize size = settings.size(srcStep);
    QRect srcRect = QRect(QPoint(tile.x(), tile.y()), settings.tileSize());

    QList<PluginDependency*> deps = settings.tilesIn(srcRect, srcStep);
    deps.append(settings.dependency(PLUGIN_NAME":"PLUGINDATA_NAME));

    return deps;
}


void AutoLevelsFilter::runLayer(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginTile &tile, PluginOptionList &layerOptions, int layerPos) const
{
    Q_UNUSED( options );
    Q_UNUSED( layerOptions );

    PluginImage *	pSrcImg = tile.image();
    PluginImage *	pDstImg;

    // Get PluginData object
    AutoLevelsData* data = (AutoLevelsData*)settings.dependency("AutoLevels:AutoLevelsData");

    // Get histogram transformation
    float* histlut = data->LUT();

    //	Layer 0 output should always go into tile.image(), otherwise output should go into tile.tmpImage()
    if ( layerPos == 0 )
        pDstImg = tile.image();
    else
        pDstImg = tile.tmpImage();

    int rw = pSrcImg->rowWords();	//	number of unsigned shorts in a row
    int pw = pSrcImg->planeWords();	//	increment to get to the next color component in the plane
    int np = pSrcImg->nextPixel();	//	increment for the next pixel within the same color plane - if the pointer is
    //	pointing at a green pixel this incremet will point to the next green pixel

    int tileWidth = settings.paddedWidth();        // Tile Dimensions
    int tileHeight = settings.paddedHeight();

    uint16 *pIn = NULL;
    uint16 *pOut = NULL;


    // Tile loop
    for ( int y = 0; y < tileHeight; y++ )
    {
        //	image-data16() points to the very first unpadded pixel
        //	add to that (rw * y) to point us at the first unpadded pixel for line 'y'
        pIn = pSrcImg->data16() + rw * y;
        pOut = pDstImg->data16() + rw * y;

        for ( int x = 0; x < tileWidth ; x++ )
        {
            // RGB values
            uint16 r = *(pIn);
            uint16 g = *(pIn + pw);
            uint16 b = *(pIn + 2*pw);

            // Histogram transformation
            *(pOut) 		= histlut[r];
            *(pOut + pw) 	= histlut[g];
            *(pOut + 2*pw) 	= histlut[b];

            // Move to the next pixel (in the color plane)
            pOut += np;
            pIn += np;
        }
    }
}
