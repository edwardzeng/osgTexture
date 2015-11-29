/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/

#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Image>
#include <osg/TexGen>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/StateSet>
#include <osg/ShapeDrawable>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

//创建二维纹理属性
osg::StateSet* createTexture2DState()
{
	osg::Image* image = new osg::Image();
	image = osgDB::readImageFile("E:/SourceCode/OSG/OSGData/Robot/right/road2.jpg");

	//创建二维纹理
	osg::Texture2D* texture = new osg::Texture2D;
	//设置边界处理
	texture->setWrap(osg::Texture2D::WRAP_S,osg::Texture2D::REPEAT);
	texture->setWrap(osg::Texture2D::WRAP_T,osg::Texture2D::REPEAT);
	//设置滤波
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
	//设置贴图
	texture->setImage(image);

	//设置自动纹理坐标，并指定相关的平面
	osg::TexGen* texgen = new osg::TexGen;
	texgen->setMode(osg::TexGen::OBJECT_LINEAR);
	texgen->setPlane(osg::TexGen::S,osg::Plane(0.0f,0.0f,1.0f,0.0f));

	//创建属性集
	osg::StateSet* stateset = new osg::StateSet;

	//启用二维纹理
	stateset->setTextureAttribute(0,texture,osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

	//启用纹理坐标生成器
	stateset->setTextureAttribute(0,texgen,osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0,GL_TEXTURE_GEN_S,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0,GL_TEXTURE_GEN_T,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

	return stateset;
}

int main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	osg::Group* root = new osg::Group();

	//osg::Node* node = new osg::Node() ;
	//node = osgDB::readNodeFile("E:/SourceCode/OSG/OSGData/Robot/right/car.ive");
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("E:/SourceCode/OSG/OSGData/Robot/right/car.ive");

	osg::ref_ptr<osg::Geode> sphere=new osg::Geode;
	sphere->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0,0.0,0.0),120)));
	root->addChild(sphere);

	osg::BoundingSphere bs=node->getBound();
	float x=bs.center().x();
	float y=bs.center().y();
	float z=bs.center().z();

	//自动生成纹理坐标属性
	osg::StateSet* stateset = new osg::StateSet();
	stateset =createTexture2DState();

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