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
#include "LabCurvesFilterAfter.h"
#include "LabCurvesPlugin.h"

#include "PluginPipeSettings.h"
#include "PluginTile.h"
#include "PluginImage.h"
#include "PluginImageSettings.h"
#include "PluginOptionList.h"

#include "srgblut.h"

#include <QDebug>
#include <iostream>


using namespace std;


bool LabCurvesFilterAfter::needsToRunLayer(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginOptionList &layerOptions) const
{
    Q_UNUSED( options );
    Q_UNUSED( settings );

    bool okay;
    bool val = layerOptions.getBool(Enable, m_groupId, okay);
    return val;
}

bool LabCurvesFilterAfter::isSlow(const PluginImageSettings &options, const PluginPipeSettings &settings) const
{
    Q_UNUSED( options );
    Q_UNUSED( settings );

    return false;
}

bool LabCurvesFilterAfter::isLargeRadius(const PluginImageSettings &options, const PluginPipeSettings &settings) const
{
    Q_UNUSED( options );
    Q_UNUSED( settings );

    return false;	//	We only use the current tile.
}

bool LabCurvesFilterAfter::isPixelSource(const PluginImageSettings &options, const PluginPipeSettings &settings) const
{
    Q_UNUSED( options );
    Q_UNUSED( settings );

    return false;	//	We don't generate pixels from scratch.
}

QTransform LabCurvesFilterAfter::transform(const PluginImageSettings &options, const PluginPipeSettings &settings) const
{
    Q_UNUSED( options );
    Q_UNUSED( settings );

    return QTransform();	//	We don't warp the image so return an identity transform.
}

QSize LabCurvesFilterAfter::size(const PluginImageSettings &options, const PluginPipeSettings &settings, const QSize &startSize) const
{
    Q_UNUSED( options );
    Q_UNUSED( settings );

    return startSize;	//	We don't change the size of the image.
}



// Prerequisites
QList<PluginDependency*> LabCurvesFilterAfter::prerequisites(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginTile &tile) const
{
    Q_UNUSED( options );
    Q_UNUSED( settings );
    Q_UNUSED( tile );

    return QList<PluginDependency*>();	//	Empty list - we don't depend on any other tiles.
}


void LabCurvesFilterAfter::runLayer(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginTile &tile, PluginOptionList &layerOptions, int layerPos) const
{
    Q_UNUSED( options );
    Q_UNUSED( layerOptions );

    // Read parameter values
    bool okay;
    float amp = 1.0 - layerOptions.getDouble(Scale, m_groupId, okay)/100.0;
    

	PluginImage *	pSrcImg = tile.image();
	PluginImage *	pDstImg;

	//	Layer 0 output should always go into tile.image(), otherwise output should go into tile.tmpImage()
	if ( layerPos == 0 )
		pDstImg = tile.image();
	else
		pDstImg = tile.tmpImage();

	int rw = pSrcImg->rowWords();	//	number of unsigned shorts in a row
	int pw = pSrcImg->planeWords();	//	increment to get to the next color component in the plane
	int np = pSrcImg->nextPixel();	//	increment for the next pixel within the same color plane - if the pointer is
									//  pointing at a green pixel this increment will point to the next green pixel

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
			// RGB input values are Lab values mapped to [0,65535]
			uint16 R = *(pIn);
			uint16 G = *(pIn + pw);
			uint16 B = *(pIn + 2*pw);

			// Convert to SRGB to compensate for the inverse SRGB
			// conversion that is done in the LabCurves filter
			R = srgblut[R];
			G = srgblut[G];
			B = srgblut[B];

			// Map data to [0,100] for L, and amp*[-192,191] for a and b
			// We use the amp*[-192,191] interval to be sure there is no color clipping when amp=1
			float L, a, b;
			L=INT16_TO_FLOAT(R)*100.0; // L in [0,100]
			a=amp*(-192.0+383.0*INT16_TO_FLOAT(G));
			b=amp*(-192.0+383.0*INT16_TO_FLOAT(B));            

			// Convert Lab to RGB
			lab2rgb(&L,&a,&b);

			// RGB in [0,65535]
			R = FLOAT_TO_INT16(L);
			G = FLOAT_TO_INT16(a);
			B = FLOAT_TO_INT16(b);

			// Ouput RGB values
			*(pOut) 		= R;
			*(pOut + pw) 	= G;
			*(pOut + 2*pw) 	= B;

			// Move to the next pixel (in the color plane)
			pOut += np;
			pIn += np;
		}
	}    
}
