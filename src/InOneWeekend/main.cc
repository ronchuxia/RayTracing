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
#include "quad.h"

void spheres(){
    auto start = std::chrono::system_clock::now();
    std::clog << "Creating Scene.\n" << std::flush; 

    hittable_list world;

    // ground
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -51; a < 11; a++) {
        for (int b = -51; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    std::clog << "Number of spheres: " << world.objects.size() << "\n" << std::flush;

    // triangle
    // auto triangle_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    // world.add(make_shared<triangle>(point3(0, 0, 0), point3(1, 0, 0), point3(0, 1, 0), vec3(0, 0, 1), triangle_material));

    // triangle_mesh: school_badge
    // char badge_file[128] = "/home/ronchuxia/Research/raytracing/src/InOneWeekend/school_badge.STL";
    // auto badge_material = make_shared<lambertian>(color(0.7, 0.6, 0.5));
    // triangle_mesh badge_mesh = triangle_mesh(badge_file, badge_material);
    // shared_ptr<hittable> badge_mesh_bvh = make_shared<bvh_node>(badge_mesh);
    // badge_mesh_bvh = make_shared<uniform_scale>(badge_mesh_bvh, 0.12);
    // badge_mesh_bvh = make_shared<translate>(badge_mesh_bvh, vec3(-2, 0, -8));
    // world.add(badge_mesh_bvh);

    // triangle_mesh: school_badge2 with bvh
    char badge_file2[128] = "/home/ronchuxia/Research/raytracing/src/InOneWeekend/school_badge2.STL";
    auto badge_material2 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.5);
    triangle_mesh badge_mesh2 = triangle_mesh(badge_file2, badge_material2);
    shared_ptr<hittable> badge_mesh_bvh2 = make_shared<bvh_node>(badge_mesh2);
    badge_mesh_bvh2 = make_shared<uniform_scale>(badge_mesh_bvh2, 0.033);
    badge_mesh_bvh2 = make_shared<translate>(badge_mesh_bvh2, vec3(-3, 0, -5));
    world.add(badge_mesh_bvh2);

    // triangle_mesh: school_badge2 without bvh 
    // char badge_file2[128] = "/home/ronchuxia/Research/raytracing/src/InOneWeekend/school_badge2.STL";
    // auto badge_material2 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.5);
    // shared_ptr<hittable> badge_mesh2 = make_shared<triangle_mesh>(badge_file2, badge_material2);
    // badge_mesh2 = make_shared<uniform_scale>(badge_mesh2, 0.033);
    // badge_mesh2 = make_shared<translate>(badge_mesh2, vec3(-3, 0, -5));
    // world.add(badge_mesh2);

    // diffuse_light
    auto diffuse_light_material = make_shared<diffuse_light>(color(10, 10, 10));
    world.add(make_shared<sphere>(point3(0, 30, -30), 10.0, diffuse_light_material));

    world = hittable_list(make_shared<bvh_node>(world));
    
    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 4096;
    cam.max_depth         = 100;

    cam.vfov     = 20;
    cam.lookfrom = point3(13, 10, 20);
    cam.lookat   = point3(-12, -5, -20);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focus_dist    = 100.0;

    std::clog << "Rendering.\n" << std::flush;

    cam.render(world);

    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::clog << "Completed. Total time: " << duration.count() << "s.\n" << std::flush;
}

void cornell_box() {
    auto start = std::chrono::system_clock::now();
    std::clog << "Creating Scene.\n" << std::flush; 

    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(343+65, 554, 332+52.5), vec3(-130*2,0,0), vec3(0,0,-105*2), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    // triangle_mesh: school_badge
    char badge_file[128] = "/home/ronchuxia/Research/raytracing/src/InOneWeekend/school_badge.STL";
    auto badge_material = make_shared<lambertian>(color(0.7, 0.6, 0.5));
    triangle_mesh badge_mesh = triangle_mesh(badge_file, badge_material);
    shared_ptr<hittable> badge_mesh_bvh = make_shared<bvh_node>(badge_mesh);
    badge_mesh_bvh = make_shared<uniform_scale>(badge_mesh_bvh, 4);
    badge_mesh_bvh = make_shared<rotate_y>(badge_mesh_bvh, 195);
    badge_mesh_bvh = make_shared<translate>(badge_mesh_bvh, vec3(430, 330, 295));
    world.add(badge_mesh_bvh);

    // triangle_mesh: school_badge2
    char badge_file2[128] = "/home/ronchuxia/Research/raytracing/src/InOneWeekend/school_badge2.STL";
    auto badge_material2 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.5);
    triangle_mesh badge_mesh2 = triangle_mesh(badge_file2, badge_material2);
    shared_ptr<hittable> badge_mesh_bvh2 = make_shared<bvh_node>(badge_mesh2);
    badge_mesh_bvh2 = make_shared<uniform_scale>(badge_mesh_bvh2, 1);
    badge_mesh_bvh2 = make_shared<rotate_y>(badge_mesh_bvh2, -18);
    badge_mesh_bvh2 = make_shared<translate>(badge_mesh_bvh2, vec3(130, 165, 65));
    world.add(badge_mesh_bvh2);

    // sphere
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(330, 50, 60), 50.0, material1));

    auto material2 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(415, 50, 130), 50.0, material2));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 8192;
    cam.max_depth         = 100;

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    std::clog << "Rendering.\n" << std::flush;

    cam.render(world);

    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::clog << "Completed. Total time: " << duration.count() << "s.\n" << std::flush;
}

int main() {
    cornell_box();
    // spheres();
}