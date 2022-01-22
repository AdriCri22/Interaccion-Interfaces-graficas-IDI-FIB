#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;
uniform vec3 posFocus1;
uniform vec3 posFocus2;
uniform vec3 posFocus3;
uniform vec3 colorFocus1;
uniform vec3 colorFocus3;
uniform vec3 colorFocus2;

out vec4 FragColor;

vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Lambert (vec3 NormSCO, vec3 L, vec3 colorFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colorRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colorRes = colorFocus * fmatdiff * dot (L, NormSCO);
  return (colorRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colorFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colorRes = Lambert(NormSCO, L, colorFocus);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colorRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colorRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colorRes + fmatspec * colorFocus * shine);
}

void main()
{    
    vec3 normal = normalize(fnormal);

    vec3 posFocusSCO = (vec4(posFocus1, 1.0)).xyz;
    vec3 L1 = normalize(posFocusSCO - fvertex);
    
    vec3 posFocusSCA = (View * vec4(posFocus2, 1.0)).xyz;
    vec3 L2 = normalize(posFocusSCA - fvertex);
    
    vec3 posFocusSCM = (View * TG * vec4(posFocus3, 1.0)).xyz;
    vec3 L3 = normalize(posFocusSCM - fvertex);
    
    vec3 fcolorSCO = Phong(normal, L1, fvertex, colorFocus1);
    vec3 fcolorSCA = Phong(normal, L2, fvertex, colorFocus2);
    vec3 fcolorSCM = Phong(normal, L3, fvertex, colorFocus3);
    vec3 fcolor = Ambient() + fcolorSCO + fcolorSCA + fcolorSCM;

    FragColor = vec4(fcolor, 1);
}
