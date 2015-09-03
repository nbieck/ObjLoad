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
		while (!input.eof())
		{
			OLT::String line;

			std::getline(input, line);

			//use this to make use of stream operations to get the individual tokens from the line
			OLT::StringStream linestream(line);
			OLT::String token;

			linestream >> token;
			if (token.compare("v") == 0)
				ReadPosition(linestream);
			else if (token.compare("vt") == 0)
				;//uv
			else if (token.compare("vn") == 0)
				;//normal
			else if (token.compare("f") == 0)
				ReadFace(linestream);
			//else -> ignore line
		}

		//last step: get results
		out_vertices = helper_.GetVertexList();
		out_indices = std::move(indices_);
	}
}

void ObjLoader::ReadPosition(OLT::StringStream & line)
{
	OLT::Vec3 temp_pos;
	line >> temp_pos.x() >> temp_pos.y() >> temp_pos.z();
	positions_.push_back(temp_pos);
}

void ObjLoader::ReadFace(OLT::StringStream & line)
{
	//faces can be specified in a multitude of formats:
	// (legend)
	// i -> position index
	// t -> texture index
	// n -> normal index
	// (formats)
	// 1. f i i i
	// 2. f i/t i/t i/t
	// 3. f i/t/n i/t/n i/t/n
	// 4. f i//n i//n i//n
	// we need to determine which we are dealing with
	// indices can also be positive or negative, where positive is absolute 
	// and negative is offset from the back
	// indices are one-based

	//for now, assume format one and only positive indices
	while (!line.eof())
	{
		OLT::String block;
		line >> block;

		//make sure that we can deal with trailing whitespace and the like
		if (block == "")
			break;

		int pos_idx = std::stoi(block);
		indices_.push_back(helper_.GetIndexByVertex(MakeVertex(pos_idx, 0, 0)));
	}
}

ObjLoader::Vertex ObjLoader::MakeVertex(int pos_idx, int uv_idx, int normal_idx)
{
	Vertex vtx;

	int num_pos = positions_.size();
	if (pos_idx > 0 && pos_idx <= num_pos)
		vtx.position = positions_[pos_idx - 1];
	else if (pos_idx < 0 && pos_idx >= -num_pos)
		vtx.position = positions_[num_pos - pos_idx];
	else
		vtx.position = OLT::Vec3(0, 0, 0);

	int num_uv = uvs_.size();
	if (uv_idx > 0 && uv_idx <= num_uv)
		vtx.tex_coord = uvs_[uv_idx - 1];
	else if (uv_idx < 0 && uv_idx >= -num_uv)
		vtx.tex_coord = uvs_[num_uv - uv_idx];
	else
		vtx.tex_coord = OLT::Vec2(0, 0);

	int num_normal = normals_.size();
	if (normal_idx > 0 && normal_idx <= num_normal)
		vtx.normal = normals_[normal_idx - 1];
	else if (normal_idx < 0 && normal_idx >= -num_normal)
		vtx.normal = normals_[num_normal - normal_idx];
	else
		vtx.normal = OLT::Vec3(0, 0, 0);

	return vtx;
}

bool ObjLoader::Vertex::operator<(const Vertex & rhs) const
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
