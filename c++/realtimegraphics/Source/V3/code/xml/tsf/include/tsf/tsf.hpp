#pragma once
#ifndef __TSF__
#define __TSF__

#include <string>
#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include <hash_map>
#include <vector>
#include <iostream>
#include <boost/variant.hpp>
#include <boost/function.hpp>

#pragma warning( disable : 4290 ) // disable warning regarding c++ exceptions

namespace tsf
{
	struct colour
	{
		float r, g, b;

		colour() : r(0), g(0), b(0) {}
		colour( float R, float G, float B ) : r(R), g(G), b(B) {}
	};
	inline std::ostream& operator<< ( std::ostream& os, const colour& v ) { os << v.r << " " << v.g << " " << v.b; return os; }
	inline std::istream& operator>> ( std::istream& is, colour& v ) { is >> v.r >> v.g >> v.b; return is; }

	struct coordinate
	{
		float s, t;

		coordinate() : s(0), t(0) {}
		coordinate( float X, float Y ) : s(X), t(Y) {}
	};
	inline std::ostream& operator<< ( std::ostream& os, const coordinate& v ) { os << v.s << " " << v.t; return os; }
	inline std::istream& operator>> ( std::istream& is, coordinate& v ) { is >> v.s >> v.t; return is; }

	struct vector
	{
		float x, y, z;

		vector() : x(0), y(0), z(0) {}
		vector( float X, float Y, float Z ) : x(X), y(Y), z(Z) {}
	};
	inline std::ostream& operator<< ( std::ostream& os, const vector& v ) { os << v.x << " " << v.y << " " << v.z; return os; }
	inline std::istream& operator>> ( std::istream& is, vector& v ) { is >> v.x >> v.y >> v.z; return is; }

	struct point
	{
		float x, y, z;

		point() : x(0), y(0), z(0) {}
		point( float X, float Y, float Z ) : x(X), y(Y), z(Z) {}
	};
	inline std::ostream& operator<< ( std::ostream& os, const point& v ) { os << v.x << " " << v.y << " " << v.z; return os; }
	inline std::istream& operator>> ( std::istream& is, point& v ) { is >> v.x >> v.y >> v.z; return is; }

	struct hvector
	{
		float x, y, z, w;

		hvector() : x(0), y(0), z(0), w(1) {}
		hvector( float X, float Y, float Z, float W ) : x(X), y(Y), z(Z), w(W) {}
	};
	inline std::ostream& operator<< ( std::ostream& os, const hvector& v ) { os << v.x << " " << v.y << " " << v.z << " " << v.w; return os; }
	inline std::istream& operator>> ( std::istream& is, hvector& v ) { is >> v.x >> v.y >> v.z >> v.w; return is; }

	class matrix
	{
	public:
		float m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33;
	public:
		matrix() : m00(1), m10(0), m20(0), m30(0), m01(0), m11(1), m21(0), m31(0), m02(0), m12(0), m22(1), m32(0), m03(0), m13(0), m23(0), m33(1) {}
		matrix( float M00, float M01, float M02, float M03, float M10, float M11, float M12, float M13, float M20, float M21, float M22, float M23, float M30, float M31, float M32, float M33 ) : m00(M00), m10(M10), m20(M20), m30(M30), m01(M01), m11(M11), m21(M21), m31(M31), m02(M02), m12(M12), m22(M22), m32(M32), m03(M03), m13(M13), m23(M23), m33(M33) {}
	};
	inline std::ostream& operator<< ( std::ostream& os, const matrix& m ) { os << m.m00 << " " << m.m01 << " " << m.m02 << " " << m.m03 << " " << m.m10 << " " << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m20 << " " << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m30 << " " << m.m31 << " " << m.m32 << " " << m.m33; return os; }
	inline std::istream& operator>> ( std::istream& is, matrix& m ) { is >> m.m00 >> m.m01 >> m.m02 >> m.m03 >> m.m10 >> m.m11 >> m.m12 >> m.m13 >> m.m20 >> m.m21 >> m.m22 >> m.m23 >> m.m30 >> m.m31 >> m.m32 >> m.m33; return is; }

	enum axis { axis_X, axis_Y, axis_Z };

	struct  asset
	{
		float  units_to_meters;
		axis   up_axis;
	};

	struct camera
	{
		float  aspect_ratio;
		float  field_of_view;
		float  z_far;
		float  z_near;
	};
	typedef boost::shared_ptr< camera >  camera_ptr;

	typedef std::vector< coordinate >  coordinate_array;
	typedef std::vector< point >  point_array;
	typedef std::vector< vector >  vector_array;
	typedef std::vector< hvector >  hvector_array;
	typedef std::vector< float >  knot_array;
  typedef std::vector< float > float_array;
	typedef std::vector< int >  index_array;
  typedef std::vector< std::string > string_array;
  typedef std::vector< matrix > matrix_array;
	typedef std::list< index_array >  polygon_batch;
	typedef boost::shared_ptr< polygon_batch >  polygon_batch_ptr;
	typedef std::pair< std::string, polygon_batch_ptr >  polygon_batch_map_entry;
	typedef stdext::hash_map< std::string, polygon_batch_ptr >  polygon_batch_map;

	struct mesh
	{
		vector_array       normal_array;
		polygon_batch_map  polygons;
		point_array        position_array;
		vector_array       tangent_array;
		coordinate_array   texcoord_array;
	};
	typedef boost::shared_ptr< mesh >  mesh_ptr;

  struct skin
  {
		vector_array       normal_array;
		polygon_batch_map  polygons;
		point_array        position_array;
		vector_array       tangent_array;
		coordinate_array   texcoord_array;
    float_array        blend_array;
    index_array        bone_array;
    string_array       node_array;
    matrix_array       bind_array;
  };
  typedef boost::shared_ptr< skin >  skin_ptr;

	typedef std::pair< std::string, std::string > symbol_map_entry;
	typedef stdext::hash_map< std::string, std::string >  symbol_map;
	enum model_type { model_mesh, model_surface, model_skin };

	struct model
	{
		std::string  geometry;
		symbol_map   materials;
		model_type   type;
	};
	typedef boost::shared_ptr< model >  model_ptr;

	enum attachment_type { attachment_none, attachment_camera, attachment_light, attachment_model, attachment_graph };

	typedef boost::shared_ptr< struct node >  node_ptr;
	typedef std::list< node_ptr >  node_list;

	struct node
	{
    std::string      name;
		std::string      attachment;
		node_list        children;
		vector           rotate;
		vector           scale;
		vector           translate;
		attachment_type  type;
	};

	struct surface
	{
		hvector_array  point_array;
		int            ucount;
		knot_array     uknot;
		int            vcount;
		knot_array     vknot;
	};
	typedef boost::shared_ptr< surface >  surface_ptr;

	typedef boost::variant< int, float, colour, point, coordinate, vector, hvector, matrix, std::string > parameter;
	typedef boost::shared_ptr< parameter >  parameter_ptr;
	typedef std::pair< std::string, parameter_ptr >  parameter_map_entry;
	typedef stdext::hash_map< std::string, parameter_ptr >  parameter_map;

	template< typename T >
	const T*  parameter_get ( const std::string& str, const parameter_map& pm )
	{
		parameter_map::const_iterator it = pm.find( str );
		if ( it == pm.end() )
			return 0;
		return boost::get< T >( it->second.get() );
	}

	struct material
	{
		std::string    type;
		parameter_map  parameters;
	};
	typedef boost::shared_ptr< material >  material_ptr;

	struct light
	{
		std::string    type;
		parameter_map  parameters;
	};
	typedef boost::shared_ptr< light >  light_ptr;

	typedef std::pair< std::string, camera_ptr > camera_map_entry;
	typedef stdext::hash_map< std::string, camera_ptr >  camera_map;
	typedef std::pair< std::string, node_ptr > graph_map_entry;
	typedef stdext::hash_map< std::string, node_ptr >  graph_map;
	typedef std::pair< std::string, light_ptr > light_map_entry;
	typedef stdext::hash_map< std::string, light_ptr >  light_map;
	typedef std::pair< std::string, material_ptr > material_map_entry;
	typedef stdext::hash_map< std::string, material_ptr >  material_map;
	typedef std::pair< std::string, mesh_ptr > mesh_map_entry;
	typedef stdext::hash_map< std::string, mesh_ptr >  mesh_map;
	typedef std::pair< std::string, model_ptr > model_map_entry;
	typedef stdext::hash_map< std::string, model_ptr >  model_map;
	typedef std::pair< std::string, surface_ptr > surface_map_entry;
	typedef stdext::hash_map< std::string, surface_ptr >  surface_map;
  typedef std::pair< std::string, skin_ptr > skin_map_entry;
  typedef stdext::hash_map< std::string, skin_ptr > skin_map;

	struct scene
	{
		asset         asset_info;
		camera_map    cameras;
		graph_map     graphs;
		light_map     lights;
		material_map  materials;
		mesh_map      meshes;
    skin_map      skins;
		model_map     models;
		surface_map   surfaces;
		std::string   root_graph;
	};

	typedef boost::shared_ptr< scene >  scene_ptr;

	scene_ptr  read_scene_from_file ( const std::string& filename, bool disable_validation =false ) throw ( std::runtime_error );
	void       write_scene_to_file  ( const scene_ptr& scene, const std::string& filename, bool use_binary_mesh =false ) throw ( std::runtime_error );

	typedef node_list  path;
	typedef boost::shared_ptr< path >  path_ptr;
	typedef std::list< path_ptr >  path_list;

	int  find_if ( const node_ptr& node, const graph_map& library_graphs, boost::function< bool (const node_ptr&) > pred, path_list& paths );

	class node_is_a
	{
	public:
		node_is_a( attachment_type _type ) : type(_type) {}
	public:
		bool operator() ( const node_ptr& node ) { return node->type == type; }
	private:
		attachment_type type;
	};
}

#endif
