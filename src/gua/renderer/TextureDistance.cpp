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

// class header
#include <gua/renderer/TextureDistance.hpp>

// guacamole headers
#include <gua/platform.hpp>
#include <gua/utils/Logger.hpp>
#include <gua/math/math.hpp>

// external headers
#include <scm/gl_util/data/imaging/texture_loader.h>
#include <iostream>

namespace gua {
 
TextureDistance::TextureDistance(unsigned width,
                 unsigned height,
                 scm::gl::data_format color_format,
                 unsigned mipmap_layers,
                 scm::gl::sampler_state_desc const& state_descripton)
  : Texture2D(width, height, color_format, mipmap_layers, state_descripton){

  int pixel_size = height_ * width_ * 6; 
  int byte_size = pixel_size * sizeof(uint16_t); 
  texture_data_ = (uint16_t*)malloc(byte_size);
  world_depth_data_ = std::vector<float>(pixel_size, -1.0f);
}


void TextureDistance::download_data(RenderContext const& ctx, float near_clip, float far_clip){
  ctx.render_context->retrieve_texture_data(get_buffer(ctx), 0, texture_data_);
  unsigned size = height_*width_;
  for (int texel = 0; texel < size; ++texel){
    if (texture_data_[texel] == 65535){
      world_depth_data_[texel] = -1.0;
    }else{
      float z_n = (float)texture_data_[texel] / 65535.0;
      world_depth_data_[texel] = 2.0 * near_clip * far_clip / (far_clip + near_clip - z_n * (far_clip - near_clip));
    }
  } 
}

std::vector<float> const& TextureDistance::get_data(){
  // ASCII OUTPUT
  // for (int side = 0; side < 6; side++){
  //   std::cout << "SIDE: " << side <<  std::endl;
  //   for (int i = 0; i<height_; i++){
  //     for (int j = 0; j<height_; j++){
  //       if (world_depth_data_[i*height_*6 + j + side * height_] == -1.0f)
  //       {
  //         std::cout << "..";
  //       }
  //       else
  //       {
  //         std::cout << "##";
  //       }
  //     }
  //     std::cout << std::endl;
  //   }
  // }
  // std::cout << "============================" << std::endl;
  return world_depth_data_;
}

}