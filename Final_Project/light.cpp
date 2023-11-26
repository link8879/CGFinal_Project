#include "light.h"

void Light::setLight(Shader& shader)
{
	unsigned int lightPosLocation = glGetUniformLocation(shader.ID, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 2.0, 0.0);

	unsigned int lightColorLocation = glGetUniformLocation(shader.ID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
}
