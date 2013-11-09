/******************************************************************************
* guacamole - delicious VR                                                   *
*                                                                            *
* Copyright: (c) 2011-2013 Bauhaus-Universität Weimar                        *
* Contact:   felix.lauer@uni-weimar.de / simon.schneegans@uni-weimar.de      *
*                                                                            *
* This program is free software: you can redistribute it and/or modify it    *
* under the terms of the GNU General Public License as published by the Free *
* Software Foundation, either version 3 of the License, or (at your option)  *
* any later version.                                                         *
*                                                                            *
* This program is distributed in the hope that it will be useful, but        *
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
* or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   *
* for more details.                                                          *
*                                                                            *
* You should have received a copy of the GNU General Public License along    *
* with this program. If not, see <http://www.gnu.org/licenses/>.             *
*                                                                            *
******************************************************************************/

#ifndef GUA_VOLUME_HPP
#define GUA_VOLUME_HPP

// guacamole headers
#include <gua/platform.hpp>
#include <gua/renderer/Geometry.hpp>

// external headers
#include <scm/gl_core.h>
#include <scm/gl_util/data/volume/volume_loader.h>

#include <scm/gl_core/gl_core_fwd.h>
#include <scm/gl_core/data_types.h>
#include <scm/gl_core/state_objects.h>

#include <scm/gl_util/primitives/primitives_fwd.h>
#include <scm/gl_util/primitives/geometry.h>

#include <scm/core/platform/platform.h>
#include <scm/core/utilities/platform_warning_disable.h>

#include <mutex>
#include <thread>

#include <vector>

//struct aiMesh;


namespace gua {

	struct RenderContext;

	/**
	* Stores geometry data.
	*
	* A volume can be loaded from an Assimp volume and the draw onto multiple
	* contexts.
	* Do not use this class directly, it is just used by the Geometry class to
	* store the individual meshes of a file.
	*/
	class Volume : public Geometry {
	public:

		/**
		* Default constructor.
		*
		* Creates a new and empty Volume.
		*/
		Volume();

		/**
		* Constructor from an Assimp volume.
		*
		* Initializes the volume from a given file path
		*
		* \param volume             The Assimp volume to load the data from.
		*/
		Volume(std::string const& file_name);

		/**
		* Draws the Volume.
		*
		* Draws the Volume to the given context.
		*
		* \param context          The RenderContext to draw onto.
		*/
		void draw(RenderContext const& context) const;

		void ray_test(Ray const& ray, PickResult::Options options,
			Node* owner, std::set<PickResult>& hits);

	private:
		void upload_to(RenderContext const& context) const;

		////Volume files
		//mutable std::vector<std::string>
		//	_volume_file_pathes;
		// Volume File path
		std::string _volume_file_path;

		//Volume boxes for each volume
		mutable std::vector<scm::gl::box_volume_geometry_ptr> 
			_volume_boxes_ptr;

		//Texture3D for volume data for each volume
		mutable std::vector<scm::gl::texture_3d_ptr>
			_volume_texture_ptr;

		mutable std::vector<scm::gl::texture_2d_ptr>
			_transfer_texture_ptr;

		mutable std::vector<scm::gl::sampler_state_ptr> _sstate;

#if GUA_COMPILER == GUA_COMPILER_MSVC&& SCM_COMPILER_VER <= 1700
		mutable boost::mutex upload_mutex_;
#else
		mutable std::mutex upload_mutex_;
#endif

	public:

	};

}

#endif  // GUA_VOLUME_HPP