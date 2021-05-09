#pragma once
#include "Material.h"
#include "VertexArray.h"
#include <memory>
#include <iostream>

//Struct containing mesh information and the material relevant to rendering
struct Mesh
{
	std::shared_ptr<VertexArray> m_vao;
	std::shared_ptr<Material> m_material;
};