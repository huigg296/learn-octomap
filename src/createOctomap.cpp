#include <octomap/octomap.h>
#include <octomap/OcTree.h>

// reference: https://github.com/OctoMap/octomap/blob/devel/octomap/src/simple_example.cpp

void print_query_info(octomap::point3d query, octomap::OcTreeNode* node)
{
	if(node != NULL)
	{
		std::cout << "occupancy probability at" << query << ":\t " 
		<< node->getOccupancy() << std::endl;
	}
	else
	{
		std::cout << "occupancy probability at" << query << 
		":\t is unknown" << std::endl;

	}
}


int main(int argc, char const *argv[])
{
	std::cout << std::endl;
	std::cout << "generating example map" << std::endl;

	// 创建一个Octree对象，分辨率为0.1米
	octomap::OcTree tree(0.1);

	// 向树中插入一些占据的测量数据
	for(int x=-20; x<20; x++)
	{
		for(int y=-20; y<20; y++)
		{
			for(int z=-20; z<20; z++)
			{
				octomap::point3d endpoint((float) x*0.05f, (float) y*0.05f, (float) z*0.05f); // 计算占据点的坐标
				tree.updateNode(endpoint, true); // true 表示将该node标记为占据
			}
		}
	}

	// 向树中插入一些空闲的测量数据
	for(int x=-30; x<30; x++)
	{
		for(int y=-30; y<30; y++)
		{
			for(int z=-30; z<30; z++)
			{
				octomap::point3d endpoint((float) x*0.02f-1.0f, (float)y*0.02f-1.0f, (float) z*0.02f-1.0f);
				tree.updateNode(endpoint, false); // false 表示将该node标记为空闲
			}
		}
	}


	// tree.updateInnerOccupancy();
	// tree.prune();

	std::cout << std::endl;
	std::cout << "performing some queries:" << std::endl;

	octomap::point3d query(0.0, 0.0, 0.0);
	octomap::OcTreeNode* result = tree.search(query);
	print_query_info(query, result);

	query = octomap::point3d(-1.0, -1.0, -1.0);
	result = tree.search(query);
	print_query_info(query, result);

	query = octomap::point3d(1.0, 1.0, 1.0);
	result = tree.search(query);
	print_query_info(query, result);

	std::cout << std::endl;
	tree.writeBinary("../map/simple_tree.bt");
	std::cout << "wrote example file simple_tree.bt" << std::endl;
	std::cout << "now you can use octovis to visualize: octovis simple_tree.bt" << std::endl;
	std::cout << "Hint: hit 'F'-key in viewer to see the freespace" << std::endl;

	return 0;
}