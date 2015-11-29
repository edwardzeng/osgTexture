/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/

#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Camera>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/TexGen>
#include <osg/TexEnv>
#include <osg/NodeVisitor>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

#include <iostream>

//纹理坐标生成器，继承自NodeVisitor
class TexCoordGenerator: public osg::NodeVisitor 
{
public:
	//遍历所有的子节点
	TexCoordGenerator(): NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{
		//
	}

	void apply(osg::Geode& geode)
	{
		//通过包围盒来确定合适的比例
		const osg::BoundingSphere &bsphere = geode.getBound();

		float scale = 10;

		if (bsphere.radius() != 0) 
		{
			scale = 5 / bsphere.radius();
		}

		//遍历所有几何体，并设置纹理坐标
		for (unsigned i=0; i<geode.getNumDrawables(); ++i) 
		{
			osg::Geometry* geo = dynamic_cast<osg::Geometry* >(geode.getDrawable(i));

			if (geo) 
			{
				osg::Vec2Array* tc = generate_coords(geo->getVertexArray(), geo->getNormalArray(), scale);

				geo->setTexCoordArray(0, tc);
			}
		}

		NodeVisitor::apply(geode);
	}

protected:

	//计算纹理坐标
	osg::Vec2Array* generate_coords(osg::Array* vx, osg::Array* nx, float scale)
	{
		osg::Vec2Array* v2a = dynamic_cast<osg::Vec2Array*>(vx);
		osg::Vec3Array* v3a = dynamic_cast<osg::Vec3Array*>(vx);
		osg::Vec4Array* v4a = dynamic_cast<osg::Vec4Array*>(vx);
		osg::Vec2Array* n2a = dynamic_cast<osg::Vec2Array*>(nx);
		osg::Vec3Array* n3a = dynamic_cast<osg::Vec3Array*>(nx);
		osg::Vec4Array* n4a = dynamic_cast<osg::Vec4Array*>(nx);

		osg::ref_ptr<osg::Vec2Array> tc = new osg::Vec2Array;
		for (unsigned i=0; i<vx->getNumElements(); ++i) {

			osg::Vec3 P;
			if (v2a) P.set((*v2a)[i].x(), (*v2a)[i].y(), 0);
			if (v3a) P.set((*v3a)[i].x(), (*v3a)[i].y(), (*v3a)[i].z());
			if (v4a) P.set((*v4a)[i].x(), (*v4a)[i].y(), (*v4a)[i].z());

			osg::Vec3 N(0, 0, 1);
			if (n2a) N.set((*n2a)[i].x(), (*n2a)[i].y(), 0);
			if (n3a) N.set((*n3a)[i].x(), (*n3a)[i].y(), (*n3a)[i].z());
			if (n4a) N.set((*n4a)[i].x(), (*n4a)[i].y(), (*n4a)[i].z());

			int axis = 0;
			if (N.y() > N.x() && N.y() > N.z()) axis = 1;
			if (-N.y() > N.x() && -N.y() > N.z()) axis = 1;
			if (N.z() > N.x() && N.z() > N.y()) axis = 2;
			if (-N.z() > N.x() && -N.z() > N.y()) axis = 2;

			osg::Vec2 uv;

			switch (axis) {
				case 0: uv.set(P.y(), P.z()); break;
				case 1: uv.set(P.x(), P.z()); break;
				case 2: uv.set(P.x(), P.y()); break;
				default: ;
			}

			tc->push_back(uv * scale);
		}
		return tc.release();
	}

};

//创建二维纹理状态对象
osg::StateSet* createTexture2DState(osg::Image* image)
{
	//创建状态集对象
	osg::StateSet* stateset = new osg::StateSet();

	//创建二维纹理对象
	osg::Texture2D* texture = new osg::Texture2D();
	texture->setDataVariance(osg::Object::DYNAMIC);
	//设置贴图
	texture->setImage(image);

	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);

	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

	stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);

	return stateset ;
}

int main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	osg::Group* root = new osg::Group();

	//读取贴图文件
	osg::Image* image = new osg::Image();
	image = osgDB::readImageFile("E:/SourceCode/OSG/OSGData/Robot/right/road2.jpg");

	osg::Node* node = new osg::Node();
	node = osgDB::readNodeFile("E:/SourceCode/OSG/OSGData/Robot/right/car.ive");

	//计算纹理坐标
	TexCoordGenerator tcg ;

	node->accept(tcg);

	//创建状态集对象
	osg::StateSet* stateset = new osg::StateSet();
	stateset = createTexture2DState(image);

	//使用二维纹理
	node->setStateSet(stateset);

	root->addChild(node);
	viewer->addEventHandler( new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()) );
	viewer->addEventHandler(new osgViewer::StatsHandler);
	//优化场景数据
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root) ;

	viewer->setSceneData(root);

	viewer->realize();

	viewer->run();

	return 0 ;
}