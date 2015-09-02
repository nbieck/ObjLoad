//author: Niklas Bieck

//a simple class that extracts the mesh data in the form of a list of vertices and indices from a .obj file

#pragma once

#include <vector>
#include <fstream>
#include <string>

#include <Eigen\Dense>

//to allow for easy replacing of used types
namespace ObjLoadTypes
{
	using InFile = std::ifstream;

	template <typename T>
	using Container = std::vector<T>;

	using String = std::string;

	using Vec3 = Eigen::Vector3f;

	using Vec2 = Eigen::Vector2f;
}

class ObjLoader
{
public:

	struct Vertex
	{
		ObjLoadTypes::Vec3 position;
		ObjLoadTypes::Vec2 tex_coord;
		ObjLoadTypes::Vec3 normal;
	};

	using Index = unsigned;

	//takes the given file and reads the contained mesh data.
	// the results are returned in the two out_* containers.
	//no checking is done that the file is actually a valid .obj, this is simply assumed
	void Load(ObjLoadTypes::String file_path, 
			  ObjLoadTypes::Container<Vertex>& out_vertices, 
			  ObjLoadTypes::Container<Index>& out_indices);

private:

};