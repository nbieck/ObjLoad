//author: Niklas Bieck

//a simple class that extracts the mesh data in the form of a list of vertices and indices from a .obj file

#pragma once

#include <vector>
#include <fstream>
#include <string>

//to easily replace stl types if necessary
namespace ObjLoadTypes
{
	using InFile = std::ifstream;

	template <typename T>
	using Container = std::vector<T>;

	using String = std::string;

	struct Vec3
	{
		float x, y, z;
	};

	struct Vec2
	{
		float x, y;
	};

	struct Vertex
	{
		Vec3 position;
		Vec2 tex_coord;
		Vec3 normal;
	};

	using Index = unsigned;
}

class ObjLoader
{
public:

	//takes the given file and reads the contained mesh data.
	// the results are returned in the two out_* containers.
	//no checking is done that the file is actually a valid .obj, this is simply assumed
	void Load(ObjLoadTypes::String file_path, 
			  ObjLoadTypes::Container<ObjLoadTypes::Vertex>& out_vertices, 
			  ObjLoadTypes::Container<ObjLoadTypes::Index>& out_indices);

private:

};