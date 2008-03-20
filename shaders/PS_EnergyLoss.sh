//*****************************************************************************
//**
//**   File:               PS_EnergyLoss.sh
//**   Author:             $Author: manuele.bonanno $
//**   Last Change:        $Date: 2008-03-16 19:42:46 +0000 (Sun, 16 Mar 2008) $
//**   Last Revision:      $Revision: 176 $
//**
//*****************************************************************************

[Shader]
Name = PS_EnergyLoss
Vertex = PS_EnergyLoss.vert
Fragment = PS_EnergyLoss.frag

[Attribute : GenAttrib1]
NameInShader = velocity

[Attribute : GenAttrib2]
NameInShader = offset

