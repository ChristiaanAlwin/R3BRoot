// rotate MC points in opposite direction:
TVector3 PosIn;
TVector3 PosOut;
        
PosIn.SetX(landPoint->GetXIn() - fneuland_Global_Xpos);
PosIn.SetY(landPoint->GetYIn() - fneuland_Global_Ypos);
PosIn.SetZ(landPoint->GetZIn() - fneuland_Global_Zpos);
        
PosOut.SetX(landPoint->GetXOut() - fneuland_Global_Xpos);
PosOut.SetY(landPoint->GetYOut() - fneuland_Global_Ypos);
PosOut.SetZ(landPoint->GetZOut() - fneuland_Global_Zpos);
        
PosIn.RotateZ(-1.0*fneuland_Global_ZRotation);
PosIn.RotateY(-1.0*fneuland_Global_YRotation);
PosIn.RotateX(-1.0*fneuland_Global_XRotation);
        
PosOut.RotateZ(-1.0*fneuland_Global_ZRotation);
PosOut.RotateY(-1.0*fneuland_Global_YRotation);
PosOut.RotateX(-1.0*fneuland_Global_XRotation);