#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "rtweekend.h"
#include "hittable.h"
#include "triangle.h"
#include "aabb.h"
#include <vector>

class triangle_mesh : public hittable_list {
  private:
    shared_ptr<material> mat;
    int triangleNum;

  public:
    triangle_mesh(const char *fileName, shared_ptr<material> _material) : mat(_material) {
      FILE *file;
      file = fopen(fileName,"rb");

      // 文件名
      char modelName[128];
      fread(modelName, 80, 1, file);

      // 三角形个数
      fread(&triangleNum, 4, 1, file);
      std::clog << "New mesh created. Triangle number: " << triangleNum << "\n" << std::flush;

      float n[3];
      float v[3][3];
      char info[2];

      for (int i = 0; i < triangleNum; ++i){
        fread(n, 4, 3, file); // 法线
        fread(v, 4, 9, file); // 顶点
        fread(info, 2, 1, file); // 其他信息

        // for (int j = 0; j < 3; ++j){
        //   for (int k = 0; k < 3; ++k){
        //     v[j][k] /= 15;
        //   }
        // }

        // 添加三角形
        add(make_shared<triangle>(point3(v[0][0], v[0][1], v[0][2]), point3(v[1][0], v[1][1], v[1][2]), point3(v[2][0], v[2][1], v[2][2]), vec3(n[0], n[1], n[2]), mat));
      }   

	    fclose(file);
    }
};

#endif