//author: Niklas Bieck

#include "ObjLoader.h"

namespace
{
	namespace OLT = ObjLoadTypes;
}

void ObjLoader::Load(OLT::String file_path, OLT::Container<Vertex>& out_vertices, OLT::Container<Index>& out_indices)
{
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
