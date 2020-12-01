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

const Triangle_Status& Delaunay::CreateTriangle(cocos2d::Vec2 pos, cocos2d::Vec2 size)
{
	triangle_.radius = lpGeometry.Radius_Calculator(size);
	triangle_.vertex[0] = Vec2(pos.x, pos.y + 2 * triangle_.radius);
	triangle_.vertex[1] = Vec2(pos.x - triangle_.radius * std::sqrtf(3), pos.y - triangle_.radius);
	triangle_.vertex[2] = Vec2(pos.x + triangle_.radius * std::sqrtf(3), pos.y - triangle_.radius);

	//外接円の中心と半径
	CircumscribedCircleCenter(triangle_);
	firstTriangle_ = triangle_;

	triangleData_.emplace_back(triangle_);
	return triangle_;
}

Triangle_Data Delaunay::CheckDuplicative(Triangle_Data& data,Triangle_Data* duplicative_data,Triangle_Status status, cocos2d::Vec2 point)
{
	for (auto n = 0; n < 3; ++n)
	{
		triangle_.vertex[0] = point;
		triangle_.vertex[1] = status.vertex[(n % 3)];
		triangle_.vertex[2] = status.vertex[(n + 1) % 3];
		//外接円の中心と半径
		triangle_ = CircumscribedCircleCenter(triangle_);
		//新規に分割した三角形の中で重複している三角形があれば重複している用のdetaに保存し,
		//プッシュしない

		bool existsInNewTriangleList = false;
		for (auto iter = data.begin();
			iter != data.end(); iter++)
		{

			if (*iter == triangle_) {
				existsInNewTriangleList = true;
				bool existsInDuplicativeTriangleList = false;

				for (auto iter2 = duplicative_data->begin();
					iter2 != duplicative_data->end(); iter2++)
				{
					if (*iter2 == triangle_)
					{
						existsInDuplicativeTriangleList = true;
						break;
					}

				}
				if (!existsInDuplicativeTriangleList)
				{
					duplicative_data->push_back(triangle_);
				}
				break;
			}
		}
		if (!existsInNewTriangleList) data.push_back(triangle_);
	}
	return data;
}

const void Delaunay::SubdivisionTriangle(cocos2d::Vec2 point)
{
	triangleData_.reserve(100/*triangle_data.size() + 3*/);
	std::vector<Triangle_Data::iterator> erase_itr;
	Triangle_Data new_triangle_data;	//一時保存用
	Triangle_Data duplicative_triangle_data;	//一時保存用
	//全探査した後外接円内であれば分割する
	for (auto itr = triangleData_.begin(); itr < triangleData_.end();)
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
			CheckDuplicative(new_triangle_data, &duplicative_triangle_data,status, point);
			itr = triangleData_.erase(itr);
		}
		else
		{
			itr++;
		}
	}
	DuplicateTriangle(new_triangle_data, duplicative_triangle_data);
}

const std::vector<Edge_Status> Delaunay::TriangletoEdge(void)
{
	std::vector<Edge_Status> edge_data;
	for (auto data : triangleData_)
	{
		for (int n = 0; n < triangle_.vertex.size(); ++n)
		{
			auto distance = std::sqrtf(lpGeometry.Distance_Calculator(triangle_.vertex[n], triangle_.vertex[(n + 1) % 3]));
			edge_data.emplace_back(Edge_Status{ distance,Edge_List{data.vertex[n], data.vertex[(n + 1) % 3] }, false });
		}
	}

	for (auto itr = edge_data.begin(); itr < edge_data.end();++itr)
	{
		for (auto itr2 = edge_data.begin(); itr2 < edge_data.end();)
		{
			if (itr != itr2 && ((*itr).pair_vertex[0] == (*itr2).pair_vertex[0] && (*itr).pair_vertex[1] == (*itr2).pair_vertex[1])
				|| ((*itr).pair_vertex[0] == (*itr2).pair_vertex[1] && (*itr).pair_vertex[1] == (*itr2).pair_vertex[0]))
			{
				itr2 = edge_data.erase(itr2);
			}
			else
			{
				++itr2;
			}
		}
	}
	return edge_data;
}

void Delaunay::FinishDelaunay()
{
	// 初期頂点とつながっている三角形の削除
	for (auto itr = triangleData_.begin(); itr != triangleData_.end(); )
	{
		if (ORCheck(*itr, firstTriangle_))
		{
			itr = triangleData_.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void Delaunay::DuplicateTriangle(Triangle_Data data,Triangle_Data duplicative_data)
{
	for (auto itr = data.begin();
		itr != data.end(); ++itr)
	{
		bool exists = false;
		for (auto itr2 = duplicative_data.begin();
			itr2 != duplicative_data.end(); ++itr2)
		{
			if (*itr == *itr2)
			{
				exists = true;
				break;
			}
		}
		if (!exists) triangleData_.emplace_back(*itr);
	}
}

const Triangle_Status Delaunay::CircumscribedCircleCenter(Triangle_Status triangle)
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

const Triangle_Data Delaunay::GetTriangleData(void)
{
	return triangleData_;
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
