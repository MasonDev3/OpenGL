#ifndef MATERIALS_H
#define MATERIALS_H

#include <glm/glm.hpp>

/*
	Materials are based on the table from 
	http://devernay.free.fr/cours/opengl/materials.html
	and require that the light ambient, diffuse, and specular
	values remain at glm::vec3(1.0f) to get true color
*/

// Emerald Material
//---------------------
#define EMERALD_AMBIENT		glm::vec3(0.0215, 0.1745, 0.0215)
#define EMERALD_DIFFUSE		glm::vec3(0.07568, 0.61424, 0.07568)
#define EMERALD_SPECULAR	glm::vec3(0.633, 0.727811, 0.633)
constexpr auto EMERALD_SHINE = 0.6f;

// Jade Material
//---------------------------
#define JADE_AMBIENT		glm::vec3(0.135, 0.2225, 0.1575)
#define JADE_DIFFUSE		glm::vec3(0.54, 0.89, 0.63)
#define JADE_SPECULAR		glm::vec3(0.316228)
constexpr auto JADE_SHINE = 0.1f;

// Obsidian Material
//--------------------------
#define OBSIDIAN_AMBIENT	glm::vec3(0.05375, 0.05, 0.06625)
#define OBSIDIAN_DIFFUSE	glm::vec3(0.18275, 0.17, 0.22535)
#define OBSIDIAN_SPECULAR	glm::vec3(0.332741, 0.328634, 0.346435)
constexpr auto OBSIDIAN_SHINE = 0.3f;

// Pearl Material
//-------------------------
#define PEARL_AMBIENT		glm::vec3(0.25, 0.20725, 0.20725)
#define PEARL_DIFFUSE		glm::vec3(1.0, 0.829, 0.829)
#define PEARL_SPECULAR		glm::vec3(0.296648)
constexpr auto PEARL_SHINE = 0.088f;

// Ruby Material
//-------------------------
#define RUBY_AMBIENT		glm::vec3(0.1745, 0.01175, 0.01175)
#define RUBY_DIFFUSE		glm::vec3(0.61424, 0.04136, 0.04136)
#define RUBY_SPECULAR		glm::vec3(0.727811, 0.626959, 0.626959)
constexpr auto RUBY_SHINE = 0.6f;

// Turquoise Material
//-------------------------
#define TURQ_AMBIENT		glm::vec3(0.1, 0.18725, 0.1745)
#define TURQ_DIFFUSE		glm::vec3(0.396, 0.74151, 0.69102)
#define TURQ_SPECULAR		glm::vec3(0.297254, 0.30829, 0.306678)
constexpr auto TURQ_SHINE = 0.1f;

// Brass Material
//-------------------------
#define BRASS_AMBIENT		glm::vec3(0.329412, 0.223529, 0.027451)
#define BRASS_DIFFUSE		glm::vec3(0.780392, 0.568627, 0.113725)
#define BRASS_SPECULAR		glm::vec3(0.992157, 0.941176, 0.807843)
constexpr auto BRASS_SHINE = 0.21794872f;


#endif