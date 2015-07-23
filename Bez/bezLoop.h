#include "cmath"

static void bezLoop (float& t, float& opacity2, float& saturation2, float& lighten2, float& lightengamma2, float& midi2, float& sigcontrast2, float& sigmiddle2, float& shadows2, float& lowmids2, float& greys2, float& highmids2, float& highlights2, float& shadows42, float& lowmids42, float& highmids42, float& highlights42, float& newval, float& oldval, float& x1, float& beta, float& c1, float& c2, float& lbeta, float& lc1, float& lc2, float& G, float& S, float* pascalt, float* tee, float* omtee) {
	// Do the MIDTONE ADJUSTMENT - quadratic bezier
	if ( (midi2 > 0.5001) || (midi2 < 0.4999) ) 
	{
		x1 = 1.00f - midi2;
		if		(x1 == 0.5)  t=newval;	// first pass lower point - y1, x1
		else	t = (-2.00f* x1 + sqrt(4.00f * x1 * x1 + 4.00f * (1.00f - 2.00f * x1) * newval )) / (2.00f * (1.00f - 2.00f * x1));
			newval = 2.00f * (1.00f - t) * t * midi2 + t*t;
	}

	// do the SIGMOIDAL CONTRAST, borrowed from the Lab Contrast plugin of Thomas Baruchel.
	if (sigcontrast2>0.005)
	{
		sigmiddle2 = 1.00f - sigmiddle2;
		beta = sigcontrast2 * sigmiddle2;
		c1 = 1.00f / (1.00f + exp(beta));
		c2 = 1.00f / (1.00f + exp(beta-sigcontrast2)) - c1;
		newval = (1.00f / (1.00f + exp(beta - sigcontrast2 * newval)) - c1 ) / c2;
	}
    else if (sigcontrast2 < -0.005)     // This calculation is the exact inverse function of the Sig function above - i.e. the Logit function normalised the same way
    {
        //sigmiddle2 = 1.00f - sigmiddle2;
        sigcontrast2 = - sigcontrast2 / 2.00f;
		beta = sigcontrast2 * sigmiddle2;
		c1 = 1.00f / (1.00f + exp(beta));
		c2 = 1.00f / (1.00f + exp(beta-sigcontrast2)) - c1;
		newval = (beta + log( ( c1 + c2 * newval) / ( 1.00f - c1 - c2 * newval) ) ) / sigcontrast2;
    }

	// do the LIGHTEN ADJUSTMENT SIGMOIDALLY
	// this quartic bezier IS NOT USED DUE TO CLIPPING
	// newval = 4.00f*(1.00f-newval)*(1.00f-newval)*(1.00f-newval)*newval*(0.25f) + six*(1.00f-newval)*(1.00f-newval)*newval*newval*(0.50f) + (lighten2-(0.75f)*(1.20f) + + 4.00f*(1.00f-newval)*newval*newval*newval*lighten2 + newval*newval*newval*newval;
	// A Sigmoidal curve is used for lightness.  But rather than beta=contrast*midpoint, beta=1.
	// This gives an increasing midpoint, and simulates an inverted gamma adjustment 1-(1-newval2)^gamma but with less impact on the shadows.
	// alpha parameter for sigmoidal adjustment is lighten 2. Range is magnified from (0,1) to (2,10)
	if ( lighten2 > 0.001 ) 
	{
		lighten2 = lighten2 * 8.00f + 2.00f;
		lbeta = 1.00f;
		lc1 = 1.00f / (1.00f + exp(lbeta));
		lc2 = 1.00f / (1.00f + exp(lbeta - lighten2)) - lc1;
		if (lighten2 > 2.005)
		{
			newval =((1.00f / (1.00f + exp(lbeta-lighten2*newval))) - lc1) / lc2;
		}
	}

	// Gamma version of LIGHTEN ADJUSTMENT
	if ( lightengamma2 > 0.001 ) 
	{
		lightengamma2 = lightengamma2 * 2.5f + 1.00f;
		if (newval < 1.0) {
			newval = 1.00f - pow(( 1.00f - newval),lightengamma2);
		}
	}

	// Calculate the 5 zone TONE CURVE
	if ( (shadows2 > 0.101 ) || (shadows2 < 0.099 ) || (lowmids2 > 0.301 ) || (lowmids2 < 0.299 ) || (greys2 > 0.501 ) || (greys2 < 0.499 ) || (highmids2 > 0.701 ) || (highmids2 < 0.699 ) || (highlights2 > 0.901 ) || (highlights2 < 0.899 ) )
	{
		// Calculate powers of newval
		for (int n=1; n<11; n++) {
			tee[n]   = tee[n-1]   *  newval;
			omtee[n] = omtee[n-1] * (1.00f - newval);
		}
		// Tone Curve is Berstein Polynomial of degree 10
		newval = pascalt[1]*tee[1]*omtee[9]*shadows2 + pascalt[2]*tee[2]*omtee[8]*0.2f + pascalt[3]*tee[3]*omtee[7]*lowmids2 + pascalt[4]*tee[4]*omtee[6]*0.40f + pascalt[5]*tee[5]*omtee[5]*greys2 + pascalt[6]*tee[6]*omtee[4]*0.60f + pascalt[7]*tee[7]*omtee[3]*highmids2 + pascalt[8]*tee[8]*omtee[2]*0.80f + pascalt[9]*tee[9]*omtee[1]*highlights2 + pascalt[10]*tee[10]*(1.00f);
	}

	// Calculate the 4 zone TONE CURVE
	if ( (shadows42 > 0.201 ) || (shadows42 < 0.199 ) || (lowmids42 > 0.401 ) || (lowmids42 < 0.399 ) || (highmids42 > 0.601 ) || (highmids42 < 0.599 ) || (highlights42 > 0.801 ) || (highlights42 < 0.799 ) )
	{
		// Berstein polynomial of degree 5
		newval = 5.00f*(1.00f-newval)*(1.00f-newval)*(1.00f-newval)*(1.00f-newval)*newval*shadows42 + 10.00f*(1.00f-newval)*(1.00f-newval)*(1.00f-newval)*newval*newval*lowmids42 + 10.00f*(1.00f-newval)*(1.00f-newval)*newval*newval*newval*highmids42 + 5.00f*(1.00f-newval)*newval*newval*newval*newval*highlights42 + newval*newval*newval*newval*newval;
	}

	// Old LAB Version
    // Ensure the boundaries are not breached
    //newval = newval * 65535.00f;
	//if(newval>65535.0)newval=65535.0f;
	//if(newval<0.0)newval=0.0f;

	// Adjust the saturation
	if ( (saturation2 < 1.0) || (saturation2 > 1.0) )
	{
		S = S*saturation2;
	}

	// Opacity Adjustment
    newval = oldval + (newval-oldval) * opacity2;  
	if (saturation2 > 0.005) S = G + (S-G) * opacity2;  //Not applied to Sat if saturation2==0 as assume intentionally converted to B&W

    // Return the values - Former LAB code
	//*(pOut)=(int)newval;
	//if (bw)
	//{
	//	*(pOut+pw)=(int)32768;
	//	*(pOut+pw+pw)=(int)32768;
	//}
	//else 
	//{
	//	*(pOut+pw)=*(pIn+pw);
	//	*(pOut+pw+pw)=*(pIn+pw+pw);
	//}

    // Ensure the boundaries are not breached
	if(newval>1.0)newval=1.0f;
	if(newval<0.0)newval=0.0f;
	if(S > 1.0)S = 1.0f;
	if(S < 0.0)S = 0.0f;
	

}