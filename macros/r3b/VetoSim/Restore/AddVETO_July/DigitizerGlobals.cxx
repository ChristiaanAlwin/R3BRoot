    // NOTE: This section is added by C. A. Douma to account for global transformations.
    // It can be used outside of the VetoSim-framework if the proper fneuland-quantities are set.
    
    // globals:
    /*
    fneuland_PaddlesPerSinglePlane;// NOTE: Int_t
    fneuland_NsinglePlanes;        // NOTE: Int_t
    fneuland_TotalPaddleLength;    // NOTE: Double_t cm!
    fneuland_TotalPaddleWidth;     // NOTE: Double_t cm!
    fneuland_TotalPaddleThickness; // NOTE: Double_t cm!
    fneuland_Global_Xpos;          // NOTE: Double_t cm!
    fneuland_Global_Ypos;          // NOTE: Double_t cm!
    fneuland_Global_Zpos;          // NOTE: Double_t cm!
    fneuland_Global_XRotation;     // NOTE: Double_t radians!
    fneuland_Global_YRotation;     // NOTE: Double_t radians!
    fneuland_Global_ZRotation;     // NOTE: Double_t radians!
    fneuland_UseUnformDist;        // NOTE: Bool_t
    fneuland_RandGen;              // NOTE: TRandom3-pointer
    */
    
    // Declare some variables:
    Int_t Paddle_index = 0;
    Double_t zcenter = 0;
    Double_t xcenter = 0.0;
    Double_t ycenter = 0.0;
    TVector3 pos;
    
    // Now loop over all paddles to re-fill the xpaddle, ypaddle and zpaddle arrays:
    for (Int_t kplane = 0; kplane<fneuland_NsinglePlanes; ++kplane)
    {
        for (Int_t kbar = 0; kbar<fneuland_PaddlesPerSinglePlane; ++kbar)
        {
            // define the paddle index. It needs to run from 0 to npaddles:
            // note that for NeuLAND, kplane rins from 0 to 29 and kbar
            // runs from 0 to 49. Hence the proper way to define the index is:
            Paddle_index = kbar + fneuland_PaddlesPerSinglePlane*kplane;
            
            // So now we start in the local coordinate frame, where the first plane
            // is horizontal. the center of the z-coordinate is:
            zcenter = -0.5*fneuland_TotalPaddleThickness*((Int_t) (fneuland_NsinglePlanes - 1));
            zcenter = zcenter + ((Int_t) kplane)*fneuland_TotalPaddleThickness;
            
            // For xcenter and ycenter of the bars we need to distinguish between
            // horizontal and vertical planes. The first plane has kplane = 0 and
            // is horizontal:
            if ((kplane%2)==0)
            {
                // horizontal plane: ycenter should differ and xcenter should
                // be all zeros:
                xcenter = 0.0;
                ycenter = -0.5*fneuland_TotalPaddleWidth*((Int_t) (fneuland_PaddlesPerSinglePlane - 1));
                ycenter = ycenter + ((Int_t) kbar)*fneuland_TotalPaddleWidth;
            }
            else
            {
                // vertical plane: just the opposite:
                ycenter = 0.0;
                xcenter = -0.5*fneuland_TotalPaddleWidth*((Int_t) (fneuland_PaddlesPerSinglePlane - 1));
                xcenter = xcenter + ((Int_t) kbar)*fneuland_TotalPaddleWidth;
            }
            
            // Now we need to add the uniform distribution if this is required:
            if (fneuland_UseUnformDist)
            {
                zcenter = zcenter + fneuland_RandGen->Uniform(fneuland_TotalPaddleThickness) - 0.5*fneuland_TotalPaddleThickness;
                
                if ((kplane%2)==0)
                {
                    xcenter = xcenter + fneuland_RandGen->Uniform(fneuland_TotalPaddleLength) - 0.5*fneuland_TotalPaddleLength;
                    ycenter = ycenter + fneuland_RandGen->Uniform(fneuland_TotalPaddleWidth) - 0.5*fneuland_TotalPaddleWidth;
                }
                else
                {
                    ycenter = ycenter + fneuland_RandGen->Uniform(fneuland_TotalPaddleLength) - 0.5*fneuland_TotalPaddleLength;
                    xcenter = xcenter + fneuland_RandGen->Uniform(fneuland_TotalPaddleWidth) - 0.5*fneuland_TotalPaddleWidth;
                }
            }
            
            // Now we have all the coordinates within the local frame. Now we must transform
            // them to the global frame. But we should only do that AFTER the Tdiff-position
            // is added. This is because otherwise we do not know anymore where to add it!
            
            // now we can re-fill the arrays:
            xpaddle[Paddle_index] = xcenter;
            ypaddle[Paddle_index] = ycenter;
            zpaddle[Paddle_index] = zcenter;
            
            // Done!
        }
    }

    // Now the arrays are re-filled with the exact positions that we want! All that the next
    // piece of code then does is to replace either x or y with the Tdiff-position. For the
    // rest we just copy the array-value into the histograms!