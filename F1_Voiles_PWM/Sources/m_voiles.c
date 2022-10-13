#include "m_voiles.h"

// Renvoie l'angle souhaité pour les voiles en fonction de l'angle de la girouette
// angle_girouette : angle entre -360 et 360 degrés
int angle_voiles(int angle_girouette)
{
	int t;
	// Recalage de l'angle girouette entre 0 et 180 degrés
	if (angle_girouette > 180)
	{
		angle_girouette -= 360;
	}
	else if (angle_girouette < -180)
	{
		angle_girouette += 360;
	}
	if (angle_girouette < 0) angle_girouette = -angle_girouette;
	
	// Calcul de l'angle des voiles en fonction de la girouette
	if (angle_girouette < 45 && angle_girouette > -45)
	{
		t = 0;
	}
	else if (angle_girouette >= 45)
	{
		t = (angle_girouette-45)*90/135;
	}
	return t;
}



