#pragma once
#include "Material.h"
#include "VertexArray.h"
#include <memory>
#include <iostream>

/// <summary>
/// Struct containing mesh information and the material relevant to rendering
/// </summary>
struct Mesh
{
	std::shared_ptr<VertexArray> vao;
	std::shared_ptr<Material> material;
};