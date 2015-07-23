#include "BezFilterLATE.h"
#include "PluginPipeSettings.h"
#include "PluginTile.h"
#include "PluginImage.h"
#include "PluginImageSettings.h"
#include "PluginOptionList.h"

#include "BezPlugin.h"
#include "bezLoop.h"
#include "cmath"
#include "colorspace.h"
#include "sqrtlut.h"

#include <QDebug>
#include <iostream>

using namespace std;

//	Check if the plugin needs to run
bool BezFilterLATE::needsToRunLayer(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginOptionList &layerOptions) const
{
	Q_UNUSED( options );
	Q_UNUSED( settings );
	
	bool okay;
        bool enabl = layerOptions.getBool(ID_BEZ_ENABLED, m_groupId, okay); //is the plugin enabled?
        bool val = layerOptions.getBool(ID_BEZ_EARLY, m_groupId, okay); //is the checkbox checked?
	return enabl && !val; //run if the plugin is enabled and the checkbox isn't checked
}

QTransform BezFilterLATE::transform(const PluginImageSettings &options, const PluginPipeSettings &settings) const
{
	Q_UNUSED( options );
	Q_UNUSED( settings );
	
	return QTransform();	//	We don't warp the image so return an identity transform.
}

QSize BezFilterLATE::size(const PluginImageSettings &options, const PluginPipeSettings &settings, const QSize &startSize) const
{
	Q_UNUSED( options );
	Q_UNUSED( settings );
	
	return startSize;	//	We don't change the size of the image.
}

QList<PluginDependency*> BezFilterLATE::prerequisites(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginTile &tile) const
{
	Q_UNUSED( options );
	Q_UNUSED( settings );
	Q_UNUSED( tile );
	
	return QList<PluginDependency*>();	//	Empty list - we don't depend on any other tiles.
}

//	******************************************************************************
//	The processing function
//	Basically very similar to the B5 version, the processing loop can almost be used directly.
//	******************************************************************************
void BezFilterLATE::runLayer(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginTile &tile, PluginOptionList &layerOptions, int layerPos) const
{
	Q_UNUSED( options );
	Q_UNUSED( settings );
	Q_UNUSED( layerOptions );
	
	PluginImage *	pSrcImg = tile.image();
	PluginImage *	pDstImg;
	
	//	Layer 0 output should always go into tile.image(), otherwise output should go into tile.tmpImage()
	if ( layerPos == 0 )
		pDstImg = tile.image();
	else
		pDstImg = tile.tmpImage();

	if((pSrcImg == NULL) || (pDstImg == NULL)) return; //NULLPointer check for safety
	
	//	Tile parameters
	int rw = pSrcImg->rowWords();	//	number of unsigned shorts in a row 
	int pw = pSrcImg->planeWords();	//	increment to get to the next color component in the plane
    int np = pSrcImg->nextPixel();	//	increment for the next pixel within the same color plane

	int tileWidth = settings.paddedWidth();        //Tile Dimensions
	int tileHeight = settings.paddedHeight();

	//	Reading the options
    bool bOk;
	int opacity,saturation,lighten,lightengamma,midi,sigcontrast,sigmiddle,shadows,lowmids,greys,highmids,highlights,shadows4,lowmids4,highmids4,highlights4;
	bool hcl;

	hcl = layerOptions.getBool( ID_BEZ_HCL, m_groupId, bOk );
	opacity = layerOptions.getInt( ID_BEZ_OPACITY, m_groupId, bOk );
	saturation = layerOptions.getInt( ID_BEZ_SATURATION, m_groupId, bOk );
	lighten = layerOptions.getInt( ID_BEZ_LIGHTEN, m_groupId, bOk );
	lightengamma = layerOptions.getInt( ID_BEZ_LIGHTENGAMMA, m_groupId, bOk );
	midi = layerOptions.getInt( ID_BEZ_MIDI, m_groupId, bOk );
	sigcontrast = layerOptions.getInt( ID_BEZ_SIGCONTRAST, m_groupId, bOk );
	sigmiddle = layerOptions.getInt( ID_BEZ_SIGMIDDLE, m_groupId, bOk );
	shadows = layerOptions.getInt( ID_BEZ_SHADOWS, m_groupId, bOk );
	lowmids = layerOptions.getInt( ID_BEZ_LOWMIDS, m_groupId, bOk );
	greys = layerOptions.getInt( ID_BEZ_GREYS, m_groupId, bOk );
	highmids = layerOptions.getInt( ID_BEZ_HIGHMIDS, m_groupId, bOk );
	highlights = layerOptions.getInt( ID_BEZ_HIGHLIGHTS, m_groupId, bOk );
	shadows4 = layerOptions.getInt( ID_BEZ_SHADOWS4, m_groupId, bOk );
	lowmids4 = layerOptions.getInt( ID_BEZ_LOWMIDS4, m_groupId, bOk );
	highmids4 = layerOptions.getInt( ID_BEZ_HIGHMIDS4, m_groupId, bOk );
	highlights4 = layerOptions.getInt( ID_BEZ_HIGHLIGHTS4, m_groupId, bOk );

    float t,opacity2,saturation2,lighten2,lightengamma2,midi2,sigcontrast2,sigmiddle2,shadows2,lowmids2,greys2,highmids2,highlights2,shadows42,lowmids42,highmids42,highlights42,newval,oldval;
    float x1;
    float beta,c1,c2,lbeta,lc1,lc2;
    float R, G, B, H, S, L;

	opacity2=((float)opacity)/100.00f;
	saturation2=((float)saturation)/100.00f;
	lighten2=((float)lighten)/100.00f;
	lightengamma2=((float)lightengamma)/100.00f;
	midi2=((float)midi)/100.00f;
	sigcontrast2 = ((float)sigcontrast)/5.00f;
    sigmiddle2 = ((float)sigmiddle)/100.00f;
	shadows2=((float)shadows)/100.00f;
	lowmids2=((float)lowmids)/100.00f;
	greys2=((float)greys)/100.00f;
	highmids2=((float)highmids)/100.00f;
	highlights2=((float)highlights)/100.00f;
	shadows42=((float)shadows4)/100.00f;
	lowmids42=((float)lowmids4)/100.00f;
	highmids42=((float)highmids4)/100.00f;
	highlights42=((float)highlights4)/100.00f;

	// First establish parameters for efficient calculation of the 5-zone curve
	float pascalt[11] = {1.00,10.00,45.00,120.00,210.00,252.00,210.00,120.00,45.00,10.00,1.00};
	float tee[11],omtee[11];
	tee[0] = 1.00f;
	omtee[0]= 1.00f;

	uint16 *pIn = NULL;
	uint16 *pOut = NULL;

	//	Processing loop
    for ( int y = 0; y < tileHeight; y++ )
	{
		pIn = pSrcImg->data16() + rw * y;
		pOut = pDstImg->data16() + rw * y;

		for ( int x = 0; x < tileWidth; x++ )
		{

			// ********START MY LOOP*******

			// These need to be reset for each pixel as they get redefined.
			R = G = B = H = S = L = 0.0f;
            x1 = 0.00f;
			t = 0.00f;
			beta = 0.00f;
			c1 = 0.00f;
			c2 = 0.00f;
			lbeta = 0.00f;
			lc1 = 0.00f;
			lc2 = 0.00f;
        	sigcontrast2 = ((float)sigcontrast)/5.00f;
			sigmiddle2 = ((float)sigmiddle)/100.00f;
        	lighten2=((float)lighten)/100.00f;
	        lightengamma2=((float)lightengamma)/100.00f;

            // Modifications for HSV approach - Note that LUT does 65535
            R = sqrtLUT[*pIn];
            G = sqrtLUT[*(pIn + pw)];
            B = sqrtLUT[*(pIn + 2*pw)];            
            //R = (float)*(pIn)/ 65535.00f; 
            //G = (float)*(pIn + pw)/ 65535.00f; 
            //B = (float)*(pIn + 2*pw)/ 65535.00f;
            if (hcl) {
                rgb2hcl (&R, &G, &B);
            } else {
                rgb2hsl (&R, &G, &B);
            }
            H = B;
            S = G;
            L = R;
            oldval = R;  
			newval = R;

            // Former LAB code
            //oldval = ((float)*(pIn)) / 65535.00f;  //Not Baruchel's 65280.0
			//newval = ((float)*(pIn)) / 65535.00f;

            // All the mathematical calculations are performed in this routine
            bezLoop( t, opacity2, saturation2, lighten2, lightengamma2, midi2, sigcontrast2, sigmiddle2, shadows2, lowmids2, greys2, highmids2, highlights2, shadows42, lowmids42, highmids42, highlights42, newval, oldval, x1, beta, c1, c2, lbeta, lc1, lc2, G, S, pascalt, tee, omtee);

            B = H;
            G = S;
            R = newval;

            // Return the values - HSV version
            if (hcl) {
                hcl2rgb (&R, &G, &B);
            } else {
                hsl2rgb (&R, &G, &B);
            }

            // Former LAB code
			//newval = newval * 65535.00f;
			//if(newval>65535.0)newval=65535.0f;
			//if(newval<0.0)newval=0.0f;

            R *= R;  //DE-Gamma correction for the ave of PhotophotoRGB & AdobeRGB
            G *= G;  //DE-Gamma correction for the ave of PhotophotoRGB & AdobeRGB
            B *= B;  //DE-Gamma correction for the ave of PhotophotoRGB & AdobeRGB
            R = R * 65535.00f;
            G = G * 65535.00f;
            B = B * 65535.00f;
            if(R>65535.0)R=65535.0;
            if(G>65535.0)G=65535.0;
            if(B>65535.0)B=65535.0;
            if(R<0.0)R=0.0;
            if(G<0.0)G=0.0;
            if(B<0.0)B=0.0;

			*(pOut)=(int)R;
			*(pOut+pw)=(int)G;
			*(pOut+pw+pw)=(int)B;

            //***********END MY LOOP*******

			pIn+=np;pOut+=np;
		}
	}
}