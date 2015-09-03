//author: Niklas Bieck

//a simple class that extracts the mesh data in the form of a list of vertices and indices from a .obj file

#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <map>

#include <Eigen\Dense>

//to allow for easy replacing of used types
namespace ObjLoadTypes
{
	using InFile = std::ifstream;

	template <typename T>
	using Container = std::vector<T>;

	template <typename Key, typename Val>
	using AssocContainer = std::map<Key, Val>;

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

		//needed to determine duplicate vertices in the data
		bool operator<(const Vertex& rhs);
	};

	using Index = unsigned;

	//takes the given file and reads the contained mesh data.
	// the results are returned in the two out_* containers.
	//no checking is done that the file is actually a valid .obj, this is simply assumed
	//if there is an error loading the file, we silently fail
	void Load(ObjLoadTypes::String file_path, 
			  ObjLoadTypes::Container<Vertex>& out_vertices, 
			  ObjLoadTypes::Container<Index>& out_indices);

private:

	//.obj specifies different pieces of vertex data separately and can index them separately.
	//these serve as intermediate storage
	ObjLoadTypes::Container<ObjLoadTypes::Vec3> positions_;
	ObjLoadTypes::Container<ObjLoadTypes::Vec2> uvs_;
	ObjLoadTypes::Container<ObjLoadTypes::Vec3> normals_;

	//this class will help us get the correct indices for our actual complete vertices
	//it also provides us with the full list of unique vertices at the end
	class LoadHelper
	{
	public:

		ObjLoadTypes::Container<Vertex>&& GetVertexList();

		Index GetIndexByVertex(const Vertex& v);

	private:

		//we store the index of each unique vertex here
		ObjLoadTypes::AssocContainer<Vertex, Index> indices_by_vertex_;
		//the actual list of unique vertices
		ObjLoadTypes::Container<Vertex> vertices_;

		//the index for the next unknown vertex
		Index next_index_;

	} helper_;

	//these are the actual indices for our mesh that go together with the vertices returned by LoadHelper
	ObjLoadTypes::Container<Index> indices_;
};