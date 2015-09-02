//author: Niklas Bieck
//just a basic testing main for the .obj loading

#include <vector>
#include <iostream>

#include "ObjLoader.h"

int main(void)
{
	ObjLoader load;

	std::vector<ObjLoader::Vertex> vertices;
	std::vector<ObjLoader::Index> indices;

	load.Load("box.obj", vertices, indices);

	std::cout << "Vertices:" << std::endl;
	for (auto v : vertices)
	{
		std::cout << "Pos: (" << v.position.x() << "," << v.position.y() << "," << v.position.z() << ")";
		std::cout << "\tUV: (" << v.tex_coord.x() << "," << v.tex_coord.y() << ")";
		std::cout << "\tNorm: (" << v.normal.x() << "," << v.normal.y() << "," << v.normal.z() << ")" << std::endl;
	}

	std::cout << "Indices:" << std::endl;
	for (auto i : indices)
	{
		std::cout << i << std::endl;
	}

	system("PAUSE");
}