/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://ww.OsgChina.org   
**********************************************************/

#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Image>
#include <osg/TexGen>
#include <osg/Texture1D>
#include <osg/TexEnv>
#include <osg/StateSet>
#include <osg/TextureRectangle>
#include <osg/TexMat>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

//创建一个四边形节点
osg::Node* createNode()
{
	osg::Geode* geode = new osg::Geode();

	osg::Geometry* geom = new osg::Geometry();

	//设置顶点
	osg::Vec3Array* vc= new osg::Vec3Array();
	vc->push_back(osg::Vec3(0.0f,0.0f,0.0f));
	vc->push_back(osg::Vec3(1.0f,0.0f,0.0f));
	vc->push_back(osg::Vec3(1.0f,0.0f,1.0f));
	vc->push_back(osg::Vec3(0.0f,0.0f,1.0f));

	geom->setVertexArray(vc);

	//设置纹理坐标
	osg::Vec2Array* vt= new osg::Vec2Array();
	vt->push_back(osg::Vec2(0.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,1.0f));
	vt->push_back(osg::Vec2(0.0f,1.0f));

	geom->setTexCoordArray(0,vt);

	//设置法线
	osg::Vec3Array* nc = new osg::Vec3Array();
	nc->push_back(osg::Vec3(0.0f,-1.0f,0.0f));

	geom->setNormalArray(nc);
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//添加图元
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	//绘制
	geode->addDrawable(geom);

	return (osg::Node*)(geode);
}

//创建二维纹理状态对象
osg::StateSet* createTexture2DState(osg::Image* image)
{
	//创建状态集对象
	osg::StateSet* stateset = new osg::StateSet();

	//创建二维纹理对象
	osg::TextureRectangle* texture = new osg::TextureRectangle();
	texture->setDataVariance(osg::Object::DYNAMIC);
	//设置贴图
	texture->setImage(image);

	//设置纹理矩阵，并设置为根据矩阵纹理(TextureRectangle)的大小自动缩放
	//从而允许应用一个矩形纹理到一个纹理坐标不在0-1上
	osg::TexMat* texmat = new osg::TexMat;
	texmat->setScaleByTextureRectangleSize(true);

	//启用纹理及纹理矩阵
	stateset->setTextureAttributeAndModes(0,texmat,osg::StateAttribute::ON);
	stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);

	//关闭光照
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	return stateset ;
}

int main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	osg::Group* root = new osg::Group();

	//读取贴图文件
	osg::Image* image = new osg::Image();
	image = osgDB::readImageFile("E:/SourceCode/OSG/OSGData/Robot/road.jpg");

	osg::Node* node = new osg::Node();
	node = createNode();

	//创建状态集对象
	osg::StateSet* stateset = new osg::StateSet();
	stateset = createTexture2DState(image);

	//使用二维纹理
	node->setStateSet(stateset);

	root->addChild(node);

	//优化场景数据
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root) ;

	viewer->setSceneData(root);

	viewer->realize();

	viewer->run();

	return 0 ;
}