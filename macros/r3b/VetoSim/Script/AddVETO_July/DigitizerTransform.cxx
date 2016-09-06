// ...
// Now transform to the global LAB-frame:
pos.SetX(xx);
pos.SetY(yy);
pos.SetZ(zz);
pos.RotateX(fneuland_Global_XRotation);
pos.RotateY(fneuland_Global_YRotation);
pos.RotateZ(fneuland_Global_ZRotation);
xx = pos.X() + fneuland_Global_Xpos;
yy = pos.Y() + fneuland_Global_Ypos;
zz = pos.Z() + fneuland_Global_Zpos;