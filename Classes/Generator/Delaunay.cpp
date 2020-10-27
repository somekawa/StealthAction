#include <cmath>
#include "MST.h"
#include "Geometry.h"
#include "Delaunay.h"

using namespace cocos2d;
using namespace std;

///参考資料
///https://note.com/pal_rex/n/n66fcc3d9143f
///http://tercel-sakuragaoka.blogspot.com/2011/06/processingdelaunay_3958.html


Delaunay::Delaunay()
{

}

Delaunay::~Delaunay()
{
}

const Triangle_Status& Delaunay::Create_Triangle(cocos2d::Vec2 pos, cocos2d::Vec2 size)
{
	triangle.radius = lpGeometry.Radius_Calculator(size);
	triangle.vertex[0] = Vec2(pos.x, pos.y + 2 * triangle.radius);
	triangle.vertex[1] = Vec2(pos.x - triangle.radius * std::sqrtf(3), pos.y - triangle.radius);
	triangle.vertex[2] = Vec2(pos.x + triangle.radius * std::sqrtf(3), pos.y - triangle.radius);

	//外接円の中心と半径
	Circumscribed_circle_Center(triangle);
	firstTriangle = triangle;

	triangle_data.emplace_back(triangle);
	return triangle;
}

Triangle_Data Delaunay::Check_Duplicative(Triangle_Data& data,Triangle_Data* duplicative_data,Triangle_Status status, cocos2d::Vec2 point)
{
	for (auto n = 0; n < 3; ++n)
	{
		triangle.vertex[0] = point;
		triangle.vertex[1] = status.vertex[(n % 3)];
		triangle.vertex[2] = status.vertex[(n + 1) % 3];
		//外接円の中心と半径
		triangle = Circumscribed_circle_Center(triangle);
		//新規に分割した三角形の中で重複している三角形があれば重複している用のdetaに保存し,
		//プッシュしない

		bool existsInNewTriangleList = false;
		for (auto iter = data.begin();
			iter != data.end(); iter++)
		{

			if (*iter == triangle) {
				existsInNewTriangleList = true;
				bool existsInDuplicativeTriangleList = false;

				for (auto iter2 = duplicative_data->begin();
					iter2 != duplicative_data->end(); iter2++)
				{
					if (*iter2 == triangle)
					{
						existsInDuplicativeTriangleList = true;
						break;
					}

				}
				if (!existsInDuplicativeTriangleList)
				{
					duplicative_data->push_back(triangle);
				}
				break;
			}
		}
		if (!existsInNewTriangleList) data.push_back(triangle);
	}
	return data;
}

const void Delaunay::Subdivision_Triangle(cocos2d::Vec2 point)
{
	triangle_data.reserve(100/*triangle_data.size() + 3*/);
	std::vector<Triangle_Data::iterator> erase_itr;
	Triangle_Data new_triangle_data;	//一時保存用
	Triangle_Data duplicative_triangle_data;	//一時保存用
	//全探査した後外接円内であれば分割する
	for (auto itr = triangle_data.begin(); itr < triangle_data.end();)
	{
		Triangle_Status status = *itr;
		auto X = std::abs(point.x - status.center.x);
		auto Y = std::abs(point.y - status.center.y);
		auto dist = X * X + Y * Y;
		auto A = std::floorf(status.radius * status.radius);
		auto B = std::floorf(dist);
		//status.radius >= std::abs(point.x - status.center.x) && status.radius >= std::abs(point.y - status.center.y)
		//外接円の中にpointが追加されたか
		if (A > B)
		{
			Check_Duplicative(new_triangle_data, &duplicative_triangle_data,status, point);
			itr = triangle_data.erase(itr);
		}
		else
		{
			itr++;
		}
	}
	auto a = 0;
	bool findFlag = false;
	//すべて分割したのち後重複していればその三角形を修正し削除する
	for (auto itr = new_triangle_data.begin();
		itr != new_triangle_data.end(); ++itr)
	{
		bool exists = false;
		for (auto itr2 = duplicative_triangle_data.begin();
			itr2 != duplicative_triangle_data.end(); ++itr2)
		{
			if (*itr == *itr2)
			{
				exists = true;
				break;
			}
		}
		if (!exists) triangle_data.emplace_back(*itr);
	}
}

const std::vector<Edge_Status> Delaunay::Triangle_To_Edge(void)
{
	std::vector<Edge_Status> edge_data;
	for (auto data : triangle_data)
	{
		for (int n = 0; n < triangle.vertex.size(); ++n)
		{
			auto distance = lpGeometry.Distance_Calculator(triangle.vertex[n], triangle.vertex[(n + 1) % 3]);
			edge_data.emplace_back(Edge_Status{ distance,Edge_List{data.vertex[n], data.vertex[(n + 1) % 3]} });
		}
	}

	for (auto itr = edge_data.begin(); itr < edge_data.end();++itr)
	{
		for (auto itr2 = edge_data.begin(); itr2 < edge_data.end();)
		{
			if (itr != itr2 && (itr._Ptr->pair_vertex[0] == itr2._Ptr->pair_vertex[0] && itr._Ptr->pair_vertex[1] == itr2._Ptr->pair_vertex[1])
				|| (itr._Ptr->pair_vertex[0] == itr2._Ptr->pair_vertex[1] && itr._Ptr->pair_vertex[1] == itr2._Ptr->pair_vertex[0]))
			{
				itr2 = edge_data.erase(itr2);
			}
			else
			{
				++itr2;
			}
		}
	}return edge_data;
}

void Delaunay::FinishDelaunay()
{
	// 初期頂点とつながっている三角形の削除
	for (auto itr = triangle_data.begin(); itr != triangle_data.end(); )
	{
		if (ORCheck(*itr, firstTriangle))
		{
			itr = triangle_data.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

const Triangle_Status Delaunay::Circumscribed_circle_Center(Triangle_Status triangle)
{
	//頂点算出
	//辺の長さ算出
	// (x1 - x)^2 + (y1 - y)^2 = (x2 - x)^2 + (y2 - y)^2 = (x3 - x)^2 + (y3 - y)^2
	// ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	lpGeometry.Distance_Calculator(triangle.vertex[2], triangle.vertex[1]);
	auto a = std::abs(std::sqrt(lpGeometry.Distance_Calculator(triangle.vertex[2], triangle.vertex[1])));
	auto b = std::abs(std::sqrt(lpGeometry.Distance_Calculator(triangle.vertex[0], triangle.vertex[2])));
	auto c = std::abs(std::sqrt(lpGeometry.Distance_Calculator(triangle.vertex[1], triangle.vertex[0])));

	auto aa = a * a;
	auto bb = b * b;
	auto cc = c * c;

	auto A = aa * (bb + cc - aa);
	auto B = bb * (cc + aa - bb);
	auto C = cc * (aa + bb - cc);

	auto ABC = A + B + C;

	triangle.center = ((A * triangle.vertex[0]) + (B * triangle.vertex[1]) + (C * triangle.vertex[2])) / ABC;

	triangle.radius = (a * b * c) / std::sqrt((a + b + c) * (b + c - a) * (a - b + c) * (a + b - c));

	//// c = 2 { (x2 - x1)(y3 - y1) - (y2 - y1)(x3 - x1) }
	//auto c = 2 * ((triangle.vertex[1].x - triangle.vertex[0].x) * (triangle.vertex[2].y - triangle.vertex[0].y)
	//	- (triangle.vertex[1].y - triangle.vertex[0].y) * (triangle.vertex[2].x - triangle.vertex[0].x));

	//// x = { (y3 - y1)(x2^2 - x1^2 + y2^2 - y1^2)
	////     + (y1 - y2)(x3^2 - x1^2 + y3^2 - y1^2) } / c 
	//auto tmp1 = ((triangle.vertex[2].y - triangle.vertex[0].y) * (pow_vertex1_x - pow_vertex_x + pow_vertex1_y - pow_vertex_y)
	//	+ (triangle.vertex[0].y - triangle.vertex[1].y) * (pow_vertex2_x - pow_vertex_x + pow_vertex2_y - pow_vertex_y));
	//auto x =  tmp1/ c;

	//// y = { (x1 - x3)(x2^2 - x1^2 + y2^2 - y1^2)
	////	   + (x2 - x1)(x3^2 - x1^2 + y3^2 - y1^2) } / c
	//auto y = ((triangle.vertex[0].x - triangle.vertex[2].x) * (pow_vertex1_x - pow_vertex_x + pow_vertex1_y - pow_vertex_y)
	//	+ (triangle.vertex[1].y - triangle.vertex[0].y) * (pow_vertex2_x - pow_vertex_x + pow_vertex2_y - pow_vertex_y)) / c;
	return triangle;
}

const Triangle_Data Delaunay::Get_Triangle_Data(void)
{
	return triangle_data;
}

bool operator==(const Triangle_Status& t, const Triangle_Status& t2)
{
	return(t.vertex[0] == t2.vertex[0] && t.vertex[1] == t2.vertex[1] && t.vertex[2] == t2.vertex[2] ||
		t.vertex[0] == t2.vertex[1] && t.vertex[1] == t2.vertex[2] && t.vertex[2] == t2.vertex[0] ||
		t.vertex[0] == t2.vertex[2] && t.vertex[1] == t2.vertex[0] && t.vertex[2] == t2.vertex[1] ||

		t.vertex[0] == t2.vertex[2] && t.vertex[1] == t2.vertex[1] && t.vertex[2] == t2.vertex[0] ||
		t.vertex[0] == t2.vertex[1] && t.vertex[1] == t2.vertex[0] && t.vertex[2] == t2.vertex[2] ||
		t.vertex[0] == t2.vertex[0] && t.vertex[1] == t2.vertex[2] && t.vertex[2] == t2.vertex[1]);
}

bool ORCheck(const Triangle_Status& t, const Triangle_Status& t2)
{
	return(t.vertex[0] == t2.vertex[0] || t.vertex[1] == t2.vertex[0] || t.vertex[2] == t2.vertex[0] ||
		   t.vertex[0] == t2.vertex[1] || t.vertex[1] == t2.vertex[1] || t.vertex[2] == t2.vertex[1] ||
		   t.vertex[0] == t2.vertex[2] || t.vertex[1] == t2.vertex[2] || t.vertex[2] == t2.vertex[2]);
}
