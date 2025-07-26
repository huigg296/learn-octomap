#include <octomap/octomap.h>
#include <octomap/OcTree.h>
#include <chrono>

// reference: https://github.com/OctoMap/octomap/blob/devel/octomap/src/simple_example.cpp

void print_query_info(octomap::point3d query, octomap::OcTreeNode* node) {
  if (node != NULL) {
    std::cout << "occupancy probability at" << query << ":\t "
      << node->getOccupancy() << std::endl;
  }
  else {
    std::cout << "occupancy probability at" << query <<
      ":\t is unknown" << std::endl;
  }
}


int main(int argc, char const* argv[]) {
  constexpr float res = 0.1f; // 分辨率为0.1米
  octomap::OcTree tree(res);

  auto t0 = std::chrono::high_resolution_clock::now();
  // 向树中插入一些占据的测量数据，共64000个点
  for (int x = -20; x < 20; x++) {
    for (int y = -20; y < 20; y++) {
      for (int z = -20; z < 20; z++) {
        octomap::point3d endpoint((float)x * 0.05f, (float)y * 0.05f, (float)z * 0.05f); // 计算占据点的坐标
        tree.updateNode(endpoint, true);                                                 // true 表示将该node标记为占据
      }
    }
  }
  auto t1 = std::chrono::high_resolution_clock::now();

  // 向树中插入一些空闲的测量数据，共216000个点
  for (int x = -30; x < 30; x++) {
    for (int y = -30; y < 30; y++) {
      for (int z = -30; z < 30; z++) {
        octomap::point3d endpoint((float)x * 0.02f - 1.0f, (float)y * 0.02f - 1.0f, (float)z * 0.02f - 1.0f);
        tree.updateNode(endpoint, false); // false 表示将该node标记为空闲
      }
    }
  }
  auto t2 = std::chrono::high_resolution_clock::now();

  // tree.updateInnerOccupancy();
  // tree.prune();

  // 访问10m*10m*1m范围内的点
  constexpr float cube_size_x = 10.0f;
  constexpr float cube_size_y = 10.0f;
  constexpr float cube_size_z = 1.0f;
  constexpr float half_cube_size_x = cube_size_x / 2.0f;
  constexpr float half_cube_size_y = cube_size_y / 2.0f;
  constexpr float half_cube_size_z = cube_size_z / 2.0f;

  // 访问10m*10m*10m范围内的所有点，分辨率为0.1m
  for (float x = -half_cube_size_x; x <= half_cube_size_x; x += res) {
    for (float y = -half_cube_size_y; y <= half_cube_size_y; y += res) {
      for (float z = -half_cube_size_z; z <= half_cube_size_z; z += res) {
        octomap::point3d point(x, y, z);
        octomap::OcTreeNode* node = tree.search(point);
      }
    }
  }

  auto t3 = std::chrono::high_resolution_clock::now();


  // 使用grid_map访问10m*10m*10m范围内的所有点
  constexpr int grid_size = static_cast<int>(cube_size_x / res);
  float*** grid_map = new float**[grid_size];
  for (int i = 0; i < grid_size; ++i) {
    grid_map[i] = new float*[grid_size];
    for (int j = 0; j < grid_size; ++j) {
      grid_map[i][j] = new float[grid_size];
      for (int k = 0; k < grid_size; ++k) {
        octomap::point3d point(i, j, k);
        grid_map[i][j][k] = 0.0f; // 初始化为0
      }
    }
  }

  auto t4 = std::chrono::high_resolution_clock::now();


  // 输出运行时间
  std::cout << std::endl;
  auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
  auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
  auto duration4 = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
  std::cout << "Time taken to insert occupied nodes: " << duration1 << " ms" << std::endl;
  std::cout << "Time taken to insert free nodes: " << duration2 << " ms" << std::endl;
  std::cout << "Time taken to perform queries: " << duration3 << " ms" << std::endl;
  std::cout << "Time taken to create grid_map: " << duration4 << " ms" << std::endl;
  std::cout << std::endl;

  return 0;
}
