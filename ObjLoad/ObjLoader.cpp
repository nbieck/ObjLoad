//author: Niklas Bieck

#include "ObjLoader.h"

namespace
{
	namespace OLT = ObjLoadTypes;
}

void ObjLoader::Load(OLT::String file_path, OLT::Container<Vertex>& out_vertices, OLT::Container<Index>& out_indices)
{
	OLT::InFile input(file_path);

	if (input.is_open())
	{
		//first, reset all internal persistent storage to start new work (just to be safe)
		positions_.clear();
		uvs_.clear();
		normals_.clear();
		//we will be moving out of this, so just create a new one to be safe
		indices_ = OLT::Container<Index>();
		helper_ = LoadHelper();

		//we go through the file line by line and then parse each line on its own

		//for lines
			//for token at start of line
				//choose appropriate action

		//last step: get results
		out_vertices = helper_.GetVertexList();
		out_indices = std::move(indices_);
	}
}

bool ObjLoader::Vertex::operator<(const Vertex & rhs)
{
	//we simply need to determine some kind of strong ordering,
	//so we order by pos -> uv -> normal
	//and by x -> y -> z within those
	auto pos_neq = position.cwiseNotEqual(rhs.position);

	if (pos_neq.x())
		return position.x() < rhs.position.x();
	if (pos_neq.y())
		return position.y() < rhs.position.y();
	if (pos_neq.z())
		return position.z() < rhs.position.z();

	auto uv_neq = tex_coord.cwiseNotEqual(rhs.tex_coord);

	if (uv_neq.x())
		return tex_coord.x() < rhs.tex_coord.x();
	if (uv_neq.y())
		return tex_coord.y() < rhs.tex_coord.y();

	auto norm_neq = normal.cwiseNotEqual(rhs.normal);

	if (norm_neq.x())
		return normal.x() < rhs.normal.x();
	if (norm_neq.y())
		return normal.y() < rhs.normal.y();
	if (norm_neq.z())
		return normal.z() < rhs.normal.z();

	return false;
}

OLT::Container<ObjLoader::Vertex>&& ObjLoader::LoadHelper::GetVertexList()
{
	return std::move(vertices_);
}

ObjLoader::Index ObjLoader::LoadHelper::GetIndexByVertex(const Vertex & v)
{
	//if we have this vertex before, returns its index
	//otherwise, add it to the vertices and add it to our lookup
	auto loc = indices_by_vertex_.find(v);
	if (loc != indices_by_vertex_.end())
		return loc->second;

	vertices_.push_back(v);
	indices_by_vertex_[v] = next_index_;

	return next_index_++;
}
