//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "triangle.h"
#include "triangle_mesh.h"
#include "bvh.h"

int main() {
    auto start = std::chrono::system_clock::now();
    std::clog << "Creating Scene.\n" << std::flush; 

    hittable_list world;

    // ground
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    // for (int a = -11; a < 11; a++) {
    //     for (int b = -11; b < 11; b++) {
    //         auto choose_mat = random_double();
    //         point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

    //         if ((center - point3(4, 0.2, 0)).length() > 0.9) {
    //             shared_ptr<material> sphere_material;

    //             if (choose_mat < 0.8) {
    //                 // diffuse
    //                 auto albedo = color::random() * color::random();
    //                 sphere_material = make_shared<lambertian>(albedo);
    //                 world.add(make_shared<sphere>(center, 0.2, sphere_material));
    //             } else if (choose_mat < 0.95) {
    //                 // metal
    //                 auto albedo = color::random(0.5, 1);
    //                 auto fuzz = random_double(0, 0.5);
    //                 sphere_material = make_shared<metal>(albedo, fuzz);
    //                 world.add(make_shared<sphere>(center, 0.2, sphere_material));
    //             } else {
    //                 // glass
    //                 sphere_material = make_shared<dielectric>(1.5);
    //                 world.add(make_shared<sphere>(center, 0.2, sphere_material));
    //             }
    //         }
    //     }
    // }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    // triangle
    // auto triangle_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    // world.add(make_shared<triangle>(point3(0, 0, 0), point3(1, 0, 0), point3(0, 1, 0), vec3(0, 0, 1), triangle_material));

    // triangle_mesh: school_badge
    char badge_file[128] = "/home/ronchuxia/Research/raytracing/src/InOneWeekend/school_badge.STL";
    auto badge_material = make_shared<lambertian>(color(0.7, 0.6, 0.5));
    triangle_mesh badge_mesh = triangle_mesh(badge_file, badge_material);
    // std::clog << badge_mesh.bounding_box().x.min << " " << badge_mesh.bounding_box().x.max << std::flush;
    shared_ptr<hittable> badge_mesh_bvh = make_shared<bvh_node>(badge_mesh);
    badge_mesh_bvh = make_shared<uniform_scale>(badge_mesh_bvh, 0.15);
    badge_mesh_bvh = make_shared<translate>(badge_mesh_bvh, vec3(-2, 0, -8));
    world.add(badge_mesh_bvh);

    // triangle_mesh: school_badge2
    char badge_file2[128] = "/home/ronchuxia/Research/raytracing/src/InOneWeekend/school_badge2.STL";
    auto badge_material2 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.5);
    triangle_mesh badge_mesh2 = triangle_mesh(badge_file2, badge_material2);
    // std::clog << badge_mesh2.bounding_box().x.min << " " << badge_mesh2.bounding_box().x.max << std::flush;
    shared_ptr<hittable> badge_mesh_bvh2 = make_shared<bvh_node>(badge_mesh2);
    badge_mesh_bvh2 = make_shared<uniform_scale>(badge_mesh_bvh2, 0.033);
    badge_mesh_bvh2 = make_shared<translate>(badge_mesh_bvh2, vec3(-4, 0, -5));
    world.add(badge_mesh_bvh2);

    // diffuse_light
    auto diffuse_light_material = make_shared<diffuse_light>(color(10, 10, 10));
    world.add(make_shared<sphere>(point3(0, 30, -30), 10.0, diffuse_light_material));

    world = hittable_list(make_shared<bvh_node>(world));
    
    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 10;
    cam.max_depth         = 20;

    cam.vfov     = 20;
    // cam.lookfrom = point3(13, 10, 20);
    // cam.lookat   = point3(-12, -5, -20);
    cam.lookfrom = point3(0, 0, 20);
    cam.lookat   = point3(0, 1, 10);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.1;
    cam.focus_dist    = 100.0;

    std::clog << "Rendering.\n" << std::flush;

    cam.render(world);

    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::clog << "Completed. Total time: " << duration.count() << "s.\n" << std::flush;
}
